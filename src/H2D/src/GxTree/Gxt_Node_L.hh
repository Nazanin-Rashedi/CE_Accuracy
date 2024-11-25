#ifndef H2D_GXTREE_L_NODE_HH
#define H2D_GXTREE_L_NODE_HH

#include <iostream>
#include <iomanip>
#include <inttypes.h>
// #include <asm-ia64/gcc_intrin.h>

#include "infra/types.hh"
#include "infra/grid_tt.hh"

#include "GxtTypes.hh"

// Node types:
// L_Inner_A w/o BR: 3 child ptr, 2 separators with 4 bits each
// L_Inner_B w/o BR: 2 child ptr, 1 separator  with 4 bits
// L_Inner_C w/o BR: 1 child ptr, no separator
// L_Leaf_A  w/o BR: no child ptr, G^-{4,5} ( 80 bits)
// L_Leaf_B  w/o BR: no child ptr, G^-{5,4} (100 bits)
// L_Leaf_C   no BR: no child ptr, G^-{6,3} (108 bits)

// since there is little space in the three ptr plus br case,
// we have to split the htBits header into two header bytes
// hdr1 and hdr2. hdr2 is only present if less than three different
// child pointers exist.

// non-leaf case:

// bits for hdr1:
// l 3 b1 t t t t t
// l = 0: non-leaf node, l = 1: leaf node
// 3 = 0: has less than 3 child ptrs, 3 = 1: has three child ptrs
// b1 = 0: no br, b1 = 1: has br or b1 is part of b1 b2 which is the br code
// ttttt: total
// node types:
// l 3 t t t t t b1
// 0 1 t t t t t 1 : L_Inner_A with BR
// 0 1 t t t t t 0 : L_Inner_A without BR
// 0 0 t t t t t 1 : L_Inner_B,C with BR
// 0 0 t t t t t 0 : L_Inner_B,C without BR
// second hdr byte
// other node types have a second header byte
// 0 1 1 t t t t t s2 s2 s2 s2 s1 s1 s1 s1
// s2: 4 separator bits for separator 2
// s1: 4 separator bits for separator 1
// l 3 t o t a l b1
// 0 1 t t t t t 1  s2 s2 s2 s2 s1 s1 s1 s1 // L_Inner_A with BR
// 0 1 t t t t t 0  s2 s2 s2 s2 s1 s1 s1 s1 // L_Inner_A without BR
// 0 0 t t t t t b1 b2 L  M  S  s1 s1 s1 s1 // L_Inner_B 
// 0 0 t t t t t b1 b2 L  M  S  s1 s1 s1 s1 // L_Inner_B 
// 0 0 t t t t t b1 b2 L  M  S  0  0  0  0  // L_Inner_C 
// 0 0 t t t t t b1 b2 L  M  S  0  0  0  0  // L_Inner_C 
// L = 0: no L child, L = 1: has L child
// M = 0: no M child, M = 1: has M child
// S = 0: no S child, S = 1: has S child

// leaf node case:

// | --- hdr 1 --- | | --- hdr 2 --- |
// 1 0 0 0 0 0 0 b1   b2 t t t t t t t  L_Leaf_A
// 1 0 0 0 0 0 1 b1   b2 t t t t t t t  L_Leaf_B
// 1 0 0 0 0 1 0 0    0  t t t t t t t  L_Leaf_C (never has a BR)


// Alternative I
// layout for header:
// pp bb ttttt ssss kk
// pp: number of ptr
// bb: br kind
// ttttt: total (5 bits)
// ssss: separator, if present (only for node type L_Inner_B2)
// kk: leaf kind (00 = A, 01 = B, 10 = C), leafs only
// 

// Alternative II
// layout for header:
// l [pp|kk] bb tttttt ssss
// l: leaf node indicator (1: leaf, 0: inner node)
// l=0: pp number of child ptrs
// l=1: kk leaf kind (00 = A, 01 = B, 10 = C)
// bb: br kind
// tttttt: total (6 bits)
// ssss: separator, if present (only for node type L_Inner_B2)
// ssss: node kind addressed (L_Inner_A1, L_Inner_A2)
// 1+2+2+6+4=15

// both alternatives need an additional bit:
// if only one ptr is present and we use separator based addressing,
// as in case of Inner_B2 (only case for L nodes), then
// we must distinguish the cases whether the two pointers address
// L and M nodes or M and S nodes. This is done by an additional bit
// called _sepLMS.
// _sepLMS == 0: L,M  (GMLS pattern = 0110 = 0x6)
// _sepLMS == 1: M,S  (GMLS pattern = 0011 = 0x3)

// both alternatives use the following layout for the non-header part:
// in _arr[0]: br if present
// then follow the ptr
// then follows the grid

// determine values for noPtr and sepLMS
// note: addressing is indirect
//     GLMS   noPtr
// 6 = 0110    10
// 3 = 0011    10
// 5 = 0101    00

namespace H2D {

  class GxTreeItp;

  namespace Gxt {

/*
    enum L_Node_Kind {
      L_Inner_A_K  = 0, // 1 ptr
      L_Inner_B_K  = 1, // 2 ptr
      L_Inner_C_K  = 2, // 3 ptr
      L_Leaf_A_K   = 3, 
      L_Leaf_B_K   = 4,
      L_Leaf_C_K   = 5,
      L_NO_KIND_K  = 6
    };
*/


    struct Node_L_Generic {
      enum Node_Kind_Fine {
        Inner_A1 =  0, // with BR
        Inner_A2 =  1, // without BR
        Inner_B1 =  2, // with BR
        Inner_B2 =  3, // without BR
        Inner_C1 =  4, // with BR
        Inner_C2 =  5, // without BR
        Leaf_A1  =  6, // with BR
        Leaf_B1  =  7, // with BR
        Leaf_B2  =  8, // without BR
        Leaf_C1  =  9, // with BR
        Leaf_C2  = 10, // without BR
        NO_NODE_FINE = 11
      };

      static const char* _name[NO_NODE_FINE+1];

      typedef Grid_TT<4,5> Grid_I_A1;
      typedef Grid_TT<4,6> Grid_I_A2;
      typedef Grid_TT<4,4> Grid_I_B1;
      typedef Grid_TT<4,5> Grid_I_B2;
      typedef Grid_TT<4,3> Grid_I_C1;
      typedef Grid_TT<4,4> Grid_I_C2;
      typedef Grid_TT<4,5> Grid_L_A1;
      typedef Grid_TT<5,3> Grid_L_B1;
      typedef Grid_TT<5,4> Grid_L_B2;
      typedef Grid_TT<6,2> Grid_L_C1;
      typedef Grid_TT<6,3> Grid_L_C2;

      typedef q::Scale_S ScaleTotal;
      typedef q::Scale_S ScaleGrid;

      static const ScaleTotal* _scaleTotal;
      static const ScaleGrid*  _scaleGrid[NO_NODE_FINE];

      static inline const ScaleGrid* scaleGrid(const Node_Kind_Fine aNkf) {
                                       return _scaleGrid[aNkf];
                                     }


      static const uint32_t _gridOffset[NO_NODE_FINE];
      static const uint32_t _ptrOffset[Leaf_A1];
 

      struct hdr_t {
        uint16_t _isLeaf   : 1;
        uint16_t _noPtr    : 2; // is also leaf kind for leafs
        uint16_t _brkind   : 2;
        uint16_t _total    : 6;
        uint16_t _sepLMS   : 1;
        uint16_t _sep      : 4;
      };

      uint16_t  _arr[7];
      hdr_t     _hdr;

      Node_L_Generic();

      inline uint32_t noPtrBits() const { return _hdr._noPtr; }
      inline uint32_t noPtr() const { 
                        uint32_t lNoPtr = _hdr._noPtr;
                        if(0 == lNoPtr) {
                          lNoPtr = 2;
                        }
                        return lNoPtr;
                      }
      inline void     noPtr(const uint16_t x) { _hdr._noPtr = x; }

      inline uint32_t isLeaf() const { return (0 != _hdr._isLeaf); }
      inline void     isLeaf(const uint16_t x) { _hdr._isLeaf = x; }

      inline uint32_t leafKind() const { return _hdr._noPtr; }
      inline void     leafKind(const uint16_t x) { _hdr._noPtr = x; }

      inline uint32_t total() const { return _hdr._total; } // total undecomressed
      inline uint32_t totalDecompressed() const {
                        return _scaleTotal->decompress(total());
                      }
      inline void     total(const uint16_t x) { _hdr._total = x; }

      inline bool     hasBr() const { return (0 != _hdr._brkind); }

      inline uint32_t brKind() const { return _hdr._brkind; }
      inline void     brKind(const uint16_t& x) { _hdr._brkind = x; }

      inline uint32_t brCode() const { return ((uint32_t) (_arr[0])); }
      inline void     brCode(const uint16_t x) { _arr[0] = x; }

      inline uint32_t sep() const { return _hdr._sep; }
      inline void     sep(const uint16_t x) { _hdr._sep = x; }
      inline bool     sepLMS() const { return (0 == _hdr._sepLMS); }
      inline void     sepLMS(const uint16_t x) { _hdr._sepLMS = x; }

      inline node_type_t childNodeKind() const { // only for L_Inner_A1,2
                           return ((node_type_t) sep());
                         }
      inline void        childNodeKind(const node_type_t x) { // only for L_Inner_A1,2
                           _hdr._sep = x;
                         }

      // L_Node_Kind    determineNodeKind() const;

      Node_Kind_Fine determineNodeKindFine() const;

      inline uint32_t gridOffset(const Node_Kind_Fine x) const {
                        return (_gridOffset[x]);
                      }

      inline const uint8_t* grid(const Node_Kind_Fine x) const { 
                               return ((const uint8_t*) &(_arr[gridOffset(x)])); 
                            }
      inline       uint8_t* gridNonConst(const Node_Kind_Fine x) const { 
                               return ((uint8_t*) &(_arr[gridOffset(x)])); 
                            }

      inline uint32_t  getPtr(const Node_Kind_Fine x, const uint i) const {
                         return (_arr[_ptrOffset[x] + i]);
                       }
      inline void  setPtr(const Node_Kind_Fine x, 
                          const uint i, 
                          const uint16_t aPtr) {
                     _arr[_ptrOffset[x] + i] = aPtr;
                   }

      // only for Inner_B2:
      void getPointerAndSeparator(uint32_t*  aPtrOut, /* [4]*/
                                  uint32_t*  aSep,    /* [4]*/
                                  const Node_Kind_Fine aLnfk ) const;

      double estimate(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aTileRectangle,
                      const GxTreeItp&   aGxtItp,
                      const bool         aTrace, // for tracing
                      const uint         aNodeIdx, // for tracing
                      const uint         aLevel) const; // for tracing

      // delete?
      void extractBr(      rectangle_t& aBrOut,
                                  bool& aHasBrOut,
                        const uint32_t  aBrKind,
                     const rectangle_t& aTileRectangle) const;


      void print(std::ostream& os) const;
    };

  } // end namespace Gxt

} // end namespace H2D


#endif


