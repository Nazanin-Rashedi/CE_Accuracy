#ifndef H2D_GXTREE_M_NODE_HH
#define H2D_GXTREE_M_NODE_HH

#include <iostream>
#include <iomanip>
#include <inttypes.h>
// #include <asm-ia64/gcc_intrin.h>

#include "infra/types.hh"
#include "infra/grid_tt.hh"

#include "GxtTypes.hh"

//
// layout for M.Inner.A:
// - hdr: 1 (ein bit!, immer 1) 
// - total: 4 bit  (scale: Scale_S_4_2)
// - 1 ptr: 16 bit
// - 1 br : 16 bit
// - grid :  G^+_{3,2}: 27 bit
// in mode GLMx: only M-children  
// in mode GLMS: only S children
//
// M.Inner.A1 uses indirect addressing
// M.Inner.A2 uses   direct addressing
// M.Inner.B1 uses indirect addressing
//
// Header for all other node types:
// hdr consists of twelve (12) bit: 
// alStttttxxxx
// where
// a: distinguish M.Inner.A from all other node types 
// l: leaf indicator: l = 0: non-leaf, l = 1: leaf node
// S: scale bit: two different scales are possible for every node type
// ttttt: total (5 bits, scale: Scale_S_5_1)
// xxxx: depends:
// bbkk: brKind, kind
// bb: brKind for all nodes other than M.Inner.A
// kk: kind:
// -- kk: 00: only M node children in case of M.Inner.B
// -- kk: 01: only S node children in case of M.Inner.B
// for leaf nodes:
// -- kk: 00: M.Leaf.A
// -- kk: 01: M.Leaf.B
// -- kk: 10: M.Leaf.C

// a) M.Inner.B (1 ptr, no br, only all M or all S children):
// 100ttttt000x : M.Inner.B, 1 ptr, no br, G^+_{3,3}, scale 3.3, MUSTF 231
// 101ttttt000x : M.Inner.B, 1 ptr, no br, G^+_{3,3}, scale 3.2, MUSTF  88
// x = 0 all children are M nodes, x = 1 all children are S nodes
// b) M.Leaf.X:
// 110tttttbb00 : M.Leaf.A1      br, G^-_{3,4}, scale 4.2, MUSTF  497
// 111tttttbb00 : M.Leaf.A1      br, G^-_{3,4}, scale 4.1, MUSTF   97
// 110ttttt0000 : M.Leaf.A2   no br, G^-_{3,5}, scale 5.1, MUSTF 1964
// 111ttttt0000 : M.Leaf.A2   no br, G^-_{3,5}, scale 5.1, MUSTF 1964
// 110tttttbb01 : M.Leaf.B1      br, G^-_{4,2}, scale 2.2, MUSTF   84  ***
// 111tttttbb01 : M.Leaf.B1      br, G^-_{4,2}, scale 2.1, MUSTF   39
// 110ttttt0001 : M.Leaf.B2   no br, G^-_{4,3}, scale 3.2, MUSTF   88
// 111ttttt0001 : M.Leaf.B2   no br, G^-_{4,3}, scale 3.1, MUSTF   50
// 110ttttt0010 : M.Leaf.C2,  no br, G^-_{5,2}, scale 2.2, MUSTF   88  ***
// 111ttttt0010 : M.Leaf.C2,  no br, G^-_{5,2}, scale 2.1, MUSTF   39
// ***:
// scale 2.2 has a q-error of 2.0. thus, these combinations should
// be avoided during construction
// scale 2.1 has q-error 1.44
// scale 2.2 has q-error 2.0
// scale 3.2 has q-error 1.2
// scale 3.1 has q-error 1.1
// scale 4.2 has q-error 1.15
// scale 4.1 has q-error 1.10
// scale 5.1 has q-error 1.10

// the other non-header entries look as follows:
// for all (inner) nodes: _arr[0...] = [br? ptr? grid]

namespace H2D {

  class GxTreeItp;

  namespace Gxt {

    struct Node_M_Generic {

      enum Node_Kind_Fine {
        Inner_A1 = 0, // 1 ptr, BR (ptr 15 bits, bit 16: indicator M/S ptr)
        Inner_A2 = 1, // 1 ptr, no BR
        Inner_B2 = 2, // 2 ptr, no BR
        Leaf_A1  = 3, // with BR     // kind code 01
        Leaf_A2  = 4, // without BR  // kind code 00
        Leaf_B1  = 5, // with BR     // kind code 01
        Leaf_B2  = 6, // without BR  // kind code 01
        Leaf_C2  = 7, // without BR  // kind code 10
        NO_NODE_FINE = 8
      };

      static const char* _name[NO_NODE_FINE + 1];

      typedef Grid_TT<3,3> Grid_I_A1;
      typedef Grid_TT<3,4> Grid_I_A2;
      typedef Grid_TT<3,3> Grid_I_B2;
      typedef Grid_TT<3,4> Grid_L_A1;
      typedef Grid_TT<3,5> Grid_L_A2;
      typedef Grid_TT<4,2> Grid_L_B1;
      typedef Grid_TT<4,3> Grid_L_B2;
      typedef Grid_TT<5,2> Grid_L_C2;

      static const uint32_t _gridOffset[NO_NODE_FINE];
      static const uint32_t _ptrOffset[Leaf_A1];

      typedef q::Scale_S ScaleTotal;
      typedef q::Scale_S ScaleGrid;

      static const ScaleTotal* _scaleTotalS; // only for M.Inner.A1, M.Inner.B2
      static const ScaleGrid*  _scaleTotalX;

      // scale for grid entries/tiles: 
      // 2-dim:
      // a) with/without finer scale (only if needed, otherwise duplicated,
      //                              to be independent of the scale bit)
      // b) for each node type
      static const ScaleGrid*  _scaleGrid[2][NO_NODE_FINE]; 

      static const ScaleGrid*  scaleGrid(uint i, Node_Kind_Fine j) {
                                 return _scaleGrid[i][j];
                               }

      // header for M.Inner.A1, M.Inner.B2 (5 bits)
      // called small header (hdr_S_t)
      // node_type:
      // 00: M.Inner.A1
      // 01: M.Inner.A2
      // 10: M.Inner.B2
      // 11: M.Leaf.*

      struct hdr_S_t {
        uint16_t  _gridExtension     : 11; // do not use
        uint16_t  _total             :  3; // tBits
        uint16_t  _node_type         :  2; // see above
      };

      // header (12 bits) for all other node types (X is variable for anything but A)
      // called Xtra large header (hdr_X_t)
      struct hdr_X_t {
        uint16_t  _gridExtension     : 4; // do not use
        uint16_t  _brKind            : 2; // kind of bounding rectangle
        uint16_t  _total             : 5; // tBits
        uint16_t  _scaleBit          : 1; // use more precise scale?
        uint16_t  _leaf_kind         : 2; // kind of leaf node, kind of child ptr for M.Inner.A2
        uint16_t  _node_type         : 2; // see above
      };

      typedef union {
                hdr_S_t  _s; 
                hdr_X_t  _x; 
                uint16_t _num; // to set everything to 0
              }  hdr_ut;

      uint16_t _arr[3];
      hdr_ut   _hdr;
    

      Node_M_Generic();
      inline hdr_ut   hdr() const { return _hdr; }
      inline hdr_S_t  hdrS() const { return _hdr._s; }
      inline hdr_X_t  hdrX() const { return _hdr._x; }

      inline uint32_t totalS() const { return _hdr._s._total; }
      inline uint32_t totalX() const { return _hdr._x._total; }
  
      inline void     totalS(const uint32_t x) {
                        _hdr._s._total = (x & 0x7);
                      }

      inline void     totalX(const uint32_t x) {
                        _hdr._x._total = (x & 0x1F);
                      }

      inline double totalDecompressedS() {
                      return _scaleTotalS->decompressDouble(totalS());
                    }

      inline double totalDecompressedX() {
                      return _scaleTotalX->decompressDouble(totalX());
                    }

      // for tracing:
      inline uint32_t total() const {
                        if(hasSmallHeader()) {
                          return totalS();
                        }
                        return totalX();
                      }
      inline uint32_t totalDecompressed() const {
                        if(hasSmallHeader()) {
                          return _scaleTotalS->decompress(totalS());
                        } else {
                          return _scaleTotalX->decompress(totalX());
                        }
                      }

      inline uint32_t scaleBitX() const { return _hdr._x._scaleBit; }

      inline void     scaleBitX(const uint32_t x) {
                        _hdr._x._scaleBit = (x & 0x1);
                      }

      inline bool     hasSmallHeader() const { return (0 == (_hdr._s._node_type & 0x1)); }

      inline uint32_t nodeType() const { return _hdr._s._node_type; }
      inline bool     isInnerA1() const { return (0x0 == nodeType()); }
      inline bool     isInnerA2() const { return (0x1 == nodeType()); }
      inline bool     isInnerB2() const { return (0x2 == nodeType()); }

      inline void     setNodeType(const uint16_t x) { _hdr._s._node_type = x; }
      inline void     setInnerA1() { setNodeType(0x0); }
      inline void     setInnerA2() { setNodeType(0x1); }
      inline void     setInnerB2() { setNodeType(0x2); }

      inline bool     isLeaf() const { return (0x3 == nodeType()); }
      inline void     setLeaf() { setNodeType(0x3); }
     
      inline bool     hasBrLeaf() const { return (0 != _hdr._x._brKind); }

      inline void     brKind(const uint32_t x) { 
                        _hdr._x._brKind = x;
                      }

      inline bool     hasBr() const { 
                        if(0 == nodeType()) {
                          return true;
                        } else
                        if(isLeaf()) {
                          return hasBrLeaf();
                        }
                        // only M.Inner.B remains, has no BR
                        return false;
                      }

      // determine grid scale
      inline const ScaleGrid* scaleGrid(Node_Kind_Fine aNkf) const {
                                return _scaleGrid[scaleBitX()][aNkf];
                              }
      // leaf kind bits
      inline uint32_t leafKind() const { return _hdr._x._leaf_kind; }

      inline void leafKind(const uint32_t x) {
                    _hdr._x._leaf_kind = (x & 0x3);
                  }

      // only for M.Inner.A2:
      inline uint32_t ptrKindBit() const { return (_hdr._x._leaf_kind & 0x1); };
      inline void     ptrKindBit(const uint x) { _hdr._x._leaf_kind = x; };


      // the following only for non M.Inner.A nodes
      inline uint32_t brKindA1() const { return 0x1; } // only possibility for A !!!
      inline uint32_t brKindX() const { return _hdr._x._brKind; }
      inline void     brKindX(const uint32_t x) { _hdr._x._brKind = (x & 0x3); }
      inline uint32_t brCode() const { return ((uint32_t) (_arr[0])); }
      inline void     brCode(const uint16_t x) { _arr[0] = x; }

      // pointer  (child base idx)
      inline uint16_t getPtr(const Node_Kind_Fine aNkf) const {
                        return(_arr[_ptrOffset[aNkf]]);
                      }

      inline void     setPtr(const Node_Kind_Fine aNkf,
                             const uint16_t       aPtr) {
                        _arr[_ptrOffset[aNkf]] = aPtr;
                      }

      // pointer for M.Inner.B2 only (has two pointers)
      inline uint16_t getPtrToM() const { return _arr[0]; }
      inline uint16_t getPtrToS() const { return _arr[1]; }
      inline void     setPtrToM(const uint16_t aPtr) { _arr[0] = aPtr; }
      inline void     setPtrToS(const uint16_t aPtr) { _arr[1] = aPtr; }

      // if discrimination is needed
      inline bool     isPtrToNode_M(const uint16_t x) const { return (0 == (x & 0x8000)); }
      inline bool     isPtrToNode_S(const uint16_t x) const { return (0 != (x & 0x8000)); }
      inline uint16_t mkPtrToNode_M(const uint16_t x) const { return (x & ~0x8000); }
      inline uint16_t mkPtrToNode_S(const uint16_t x) const { return (x | 0x8000); }

      inline uint16_t thePtr(const uint16_t x) const { return (x & ~0x8000); }

      // grid access
      inline uint32_t gridOffset(const Node_Kind_Fine x) const {
                        return (_gridOffset[x]);
                      }

      inline const uint8_t* grid(const Node_Kind_Fine x) const { 
                              return (const uint8_t*) &(_arr[gridOffset(x)]); 
                            }

      inline       uint8_t* gridNonConst(const Node_Kind_Fine x) const {
                              return (uint8_t*) &(_arr[gridOffset(x)]); 
                            }

      // get node type
      Node_Kind_Fine     determineNodeKindFine() const;

      // extract bounding rectangle
      void extractBr(      rectangle_t& aBr, 
                           bool&        aHasBr, 
                     const rectangle_t& aTileRectangle) const;

      // estimate
      double estimate(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aTileRectangle,
                      const GxTreeItp&   aGxtItp,
                      const bool         aTrace, // for tracing
                      const uint         aNodeIdx, // for tracing
                      const uint         aLevel) const; // for tracing

      // print summary of node
      void print(std::ostream& os) const;

      // determine number of children
      uint noChildren() const;
    };



  } // end namespace Gxt


} // end namespace H2D


#endif

