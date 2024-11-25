#ifndef H2D_GXTREE_S_NODE_HH
#define H2D_GXTREE_S_NODE_HH

#include <iostream>
#include <iomanip>
#include <inttypes.h>

#include "infra/types.hh"
#include "infra/grid_tt.hh"

#include "GxtTypes.hh"

// Header structure:
// Ltttxy (6 bit)
// L: leaf indicator, 1 bit
// ttt: total, 3 bits
// x: only for leaf nodes, 1 bit
// y: only for leaf nodes S.L.A1, S.L.A1, 1 bit
// node kind fine
// S.Inner.A2  0ttt.. (x,y unused), no BR, 1 Ptr
// S.Leaf.A1   1ttt00 (with Br)
// S.Leaf.A2   1ttt01 (no Br)
// S.Leaf.B2   1ttt1. (y unused, no Br)

// S.Inner.A2  : no BR, direct addressing
// S.Leaf.A1   : BR
// S.Leaf.A2   : no BR
// S.Leaf.B2   : no BR

// can do better with only one more bit as needed:
// tBits: 3, hBits: 2 (always, indicating node kind fine)

namespace H2D {

  class GxTree;
  class GxTreeItp;

  namespace Gxt {

    struct Node_S_Generic {

      enum Node_Kind_Fine {
        Inner_A2 = 0, // without Br, 1 ptr (ptr 15 bits), direct addressing
        Leaf_A1  = 1, // with    BR
        Leaf_A2  = 2, // without BR
        Leaf_B2  = 3, // without BR 
        NO_NODE_FINE = 4
      };

      static const char* _name[NO_NODE_FINE + 1];

      typedef Grid_TT<2,3>  Grid_I_A2; // G^+_{2,2}, size: 2*2*3 = 12
      typedef Grid_TT<2,3>  Grid_L_A1; // G^-_{2,3}, size: 2*2*3 = 12
      typedef Grid_TT<2,4>  Grid_L_A2; // G^-_{2,4}, size: 2*2*4 = 16
      typedef Grid_TT<3,3>  Grid_L_B2; // G^-_{3,3}, size: 3*3*3 = 27

      typedef q::Scale_S ScaleTotal;
      typedef q::Scale_S ScaleGrid;

      static const ScaleTotal* _scaleTotal;
      static const ScaleGrid*  _scaleGrid[NO_NODE_FINE];

      static const ScaleGrid* scaleGrid(Node_Kind_Fine aNkf) {
                                return _scaleGrid[aNkf];
                              }

      // experimental, zwei bit zu gross, muesste also woanders gespeichert werden
      struct hdr_t {
        uint16_t _gridPart : 12 ; // to not use
        uint16_t _y        :  1 ; // y as in comment above
        uint16_t _x        :  1 ; // x as in comment above
        uint16_t _total    :  3 ; // total, compressed
        uint16_t _isLeaf   :  1 ; // leaf indicator
      };

      typedef union {
                hdr_t    _hdr;
                uint16_t _num; // to set everything to 0
              } hdr_ut;

      uint16_t  _arr;
      hdr_ut    _hdr;

      Node_S_Generic();

      inline const hdr_t&    hdr() const { return _hdr._hdr; }
      inline       hdr_t&    hdr()       { return _hdr._hdr; }
      inline uint32_t        total() const { return hdr()._total; }
      inline double          totalDecompressed() const { 
                               return _scaleTotal->decompressDouble(total());
                             }
      inline void            total(const uint16_t x) { hdr()._total = x; }

      inline bool     isLeaf() const { return (0 != hdr()._isLeaf); }
      inline bool     isLeafA1() const {
                         return isLeaf() && (0 == hdr()._x) && (0 == hdr()._y);
                      }

      inline void     setLeaf() { hdr()._isLeaf = 0x1; }

      inline void     setInnerA2() {
                        hdr()._isLeaf = 0x0;
                      }
      inline void     setLeafA1() {
                        hdr()._isLeaf = 0x1;
                        hdr()._x      = 0x0;
                        hdr()._y      = 0x0;
                      }
      inline void     setLeafA2() {
                        hdr()._isLeaf = 0x1;
                        hdr()._x      = 0x0;
                        hdr()._y      = 0x1;
                      }
      inline void     setLeafB2() {
                        hdr()._isLeaf = 0x1;
                        hdr()._x      = 0x1;
                      }



      inline uint32_t brKind() const { return 0x1; } // always of this kind
      inline uint16_t brCode() const { return _arr; }
      inline void     brCode(const uint16_t x) { _arr = x; }

      inline uint8_t* gridInnerA2NonConst() const { return (uint8_t*) &_hdr._num; }
      inline uint8_t* gridLeafA1NonConst() const { return (uint8_t*) &_hdr._num; }
      inline uint8_t* gridLeafA2NonConst() const { return (uint8_t*) &_arr; }
      inline uint8_t* gridLeafB2NonConst() const { return (uint8_t*) &_arr; }
      

      // must be of type S.Inner.A2
      inline uint16_t getPtr() const { return _arr; }
      inline void     setPtr(const uint16_t x) { _arr = x; }

      void extractBr(rectangle_t& aBr, bool& aHasBr, const rectangle_t& aTileRectangle) const;

      Node_Kind_Fine determineNodeKindFine() const;

      uint noChildren() const;

      double estimate(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aTileRectangle,
                      const GxTreeItp&   aGxtItp,
                      const bool         aTrace,
                      const uint         aNodeIdx,
                      const uint         aLevel) const;

      // print
      void print(std::ostream& os) const;
    };



  } // end namespace Gxt


} // end namespace H2D


#endif

