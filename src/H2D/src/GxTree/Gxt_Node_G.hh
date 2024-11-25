#ifndef H2D_GXTREE_G_NODE_HH
#define H2D_GXTREE_G_NODE_HH

#include <iostream>
#include <iomanip>
#include <inttypes.h>
// #include <asm-ia64/gcc_intrin.h>

#include "infra/types.hh"
#include "infra/grid_tt.hh"

#include "GxtTypes.hh"


/*
 * NODE_G_GENERIC
 * is the common infrastructure for
 * the specific node kinds 
 * Gxt_G_INNER_A, B, C, D
 * for inner nodes with 1, 2, 3, 4 child pointers
 * and Gxt_G_Leaf_A, B, C, D
 * for the different kinds of leaf nodes.
 * these are documented in ../FlexTree/doc
 * 
 * inner nodes:
 * hdr: 8 bit: LBBPPPPS
 * where  L=0: inner node, L=1: leaf node
 *      BB=00: no bounding rectangle encoding
 *         01: 16x16 bounding rectangle encoding
 *         10: 32x32 bounding rectangle encoding // currently not used
 *         11: 64x64 bounding rectangle encoding // currently not used
 *      PPPP = GLMS: has G-Node ptr, L-Node ptr, M-Node ptr, S-Node ptr if bit is set
 *          S: scale bit for grid for G_Inner_A: 0: Scale_6_6_3, 1: Scale_6_6_1
 *          S: scale bit for grid for G_Leaf_A : 0: Scale_L_7_2, 1: Scale_L_7_1
 *          S: 0 for other node kinds except G_Leaf_C
 *
 * hdr: 8 bit: leaf nodes:
 *    G_LEAF_A: 1BB0001S
 *    G_LEAF_B: 1BB00100
 *    G_LEAF_C: 1BB1rrrr
 *    G_LEAF_D: 1BB00110
 */

namespace H2D {

  class GxTreeItp;

  namespace Gxt {

    struct Node_G_Generic {

      enum Node_Kind_Fine {
        Inner_A =  0,
        Inner_B =  1,
        Inner_C =  2,
        Inner_D =  3,
        Leaf_A  =  4,
        Leaf_B  =  5,
        Leaf_C  =  6,
        Leaf_D  =  7,
        NO_NODE_FINE = 8
      };


      typedef q::Scale_L ScaleTotal;
      typedef q::Scale_S ScaleGrid; // NICHT FUER N_G_L_A

      static const ScaleTotal* _scaleTotal;
      static const ScaleGrid*  _scaleGrid[NO_NODE_FINE];

      static const uint32_t    _gridOffset[NO_NODE_FINE];
      typedef Grid_TT<5,7> Grid_I_A;
      typedef Grid_TT<5,6> Grid_I_B;
      typedef Grid_TT<5,5> Grid_I_C;
      typedef Grid_TT<5,4> Grid_I_D;

      typedef Grid_TT<5,7> Grid_L_A;
      typedef Grid_TT<6,5> Grid_L_B;
      typedef Grid_TT<7,4> Grid_L_C;
      typedef Grid_TT<8,3> Grid_L_D;

      uint16_t  _arr[13];
      uint8_t   _hdr;
      uint8_t   _total;

      Node_G_Generic();

      inline uint32_t hdr() const { return _hdr; }
      inline void     hdr(const uint8_t x) { _hdr = x; }
      inline uint32_t total() const { return _total; }
      inline void     total(const uint8_t x) { _total = x; }
      inline bool     hasBr() const { return (0 != (_hdr & 0x60)); }
      inline uint32_t brKind() const { return ((uint32_t) (_hdr >> 5) & 0x3); }
      inline void     brKind(const uint8_t x) { _hdr = ((_hdr & ~0x60) | ((x & 0x3) << 5)); }
      inline uint32_t brCode() const { return ((uint32_t) (_arr[0])); }
      inline void     brCode(const uint16_t x) { _arr[0] = x; }
      inline bool     isLeaf() const { return (0 != (_hdr & 0x80)); }
      inline bool     isSetLeafC() const { return (0 != (_hdr & 0x10)); }
      inline bool     isLeaf_C() const { return (isLeaf() && isSetLeafC()); }
      inline uint32_t innerDeterminingBits() const { return ((_hdr & 0x1F) >> 1); }
      inline uint32_t leafDeterminingBits() const { return ((_hdr >> 1) & 0xF); }
      inline uint32_t scaleBit() const { return (_hdr & 0x1); }

      inline const uint32_t ptrKinds() const { return ((_hdr >> 1) & 0xF); }
      inline const uint32_t noPtr() const { return number_of_bits_set(ptrKinds()); }

      Node_G_Inner_T  getInnerSubkind() const;
      Node_G_Leaf_T   getLeafSubkind() const;

      Node_Kind_Fine  determineNodeKindFine() const;
      
      // only for G.Inner.B and higher
      // the 5 separator bits needed to separate between two pointer
      // are stored in the upmost 5 bits of the last element in _arr
      inline uint32_t separatorBits_B() const { return ((_arr[12] >> 11) & 0x1F); }
      inline void     separatorBits_B(const uint16_t x) { 
                         const uint16_t lMask = (uint16_t) ~(0x1F << 11);
                         _arr[12] = ((_arr[12] & lMask) | ((x & (uint16_t) 0x1F) << 11));
                      }

      // // only for G.Inner.C and higher
      // inline uint32_t separatorBits_C() const { return ((_arr[12] >>  6) & 0x1F); }
      // inline uint32_t separatorBits_D() const { return ((_arr[12] >>  1) & 0x1F); }

      // the grid position depends on the number of pointers only
      // the lower _arr element is the BR, then follow the pointers,
      // then comes the grid
      inline const uint8_t* grid_Inner_A() const { return (const uint8_t*) &(_arr[2]); }
      inline const uint8_t* grid_Inner_B() const { return (const uint8_t*) &(_arr[3]); }
      inline const uint8_t* grid_Inner_C() const { return (const uint8_t*) &(_arr[4]); }
      inline const uint8_t* grid_Inner_D() const { return (const uint8_t*) &(_arr[5]); }
      inline const uint8_t* grid_Leaf()    const { return (const uint8_t*) &(_arr[1]); }
      inline const uint8_t* grid_Leaf_A()  const { return (const uint8_t*) &(_arr[1]); }
      inline const uint8_t* grid_Leaf_B()  const { return (const uint8_t*) &(_arr[1]); }
      inline const uint8_t* grid_Leaf_C()  const { return (const uint8_t*) &(_arr[1]); }
      inline const uint8_t* grid_Leaf_D()  const { return (const uint8_t*) &(_arr[1]); }

      inline uint8_t* grid_Inner_A() { return (uint8_t*) &(_arr[2]); }
      inline uint8_t* grid_Inner_B() { return (uint8_t*) &(_arr[3]); }
      inline uint8_t* grid_Inner_C() { return (uint8_t*) &(_arr[4]); }
      inline uint8_t* grid_Inner_D() { return (uint8_t*) &(_arr[5]); }
      inline uint8_t* grid_Leaf()    { return (uint8_t*) &(_arr[1]); }
      inline uint8_t* grid_Leaf_A()  { return (uint8_t*) &(_arr[1]); }
      inline uint8_t* grid_Leaf_B()  { return (uint8_t*) &(_arr[1]); }
      inline uint8_t* grid_Leaf_C()  { return (uint8_t*) &(_arr[1]); }
      inline uint8_t* grid_Leaf_D()  { return (uint8_t*) &(_arr[1]); }

      inline uint32_t gridOffset(const Node_Kind_Fine aNkf) const {
                        return (_gridOffset[aNkf]);
                      }

      inline uint8_t* gridNonConst(const Node_Kind_Fine aNkf) const {
                        return ((uint8_t*) &(_arr[gridOffset(aNkf)]));
                      }

      // the following is convenient to extract all pointers
      // those not present are initialized with NULL 
      // in the array given as input
      void getPointer(uint32_t* aPtrOut /* [4]*/ ) const;

      // same as above but also fills in separators
      // for kind B nodes:
      void getPointerAndSeparator_B(uint32_t*   aPtrOut /* [4]*/,
                                    uint32_t*   aSep    /* [4]*/) const;
      // for kind C nodes:
      void getPointerAndSeparator_C(uint32_t*   aPtrOut /* [4]*/,
                                    uint32_t*   aSep    /* [4]*/) const;

      // for kind D nodes:
      void getPointerAndSeparator_D(uint32_t*   aPtrOut /* [4]*/,
                                    uint32_t*   aSep    /* [4]*/) const;

      // only used internally during attach 
      // to extract the bounding rectangle from the encoding
      void extractBr(rectangle_t& aBr, bool& aHasBr, const rectangle_t& aTileRectangle) const;

      // if there is only one ptr
      void getOnePointerAndKind(uint32_t& aPtrOut, node_type_t& aKindOut) const;

      // the estimation function
      double estimate(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aTileRectangle,
                      const GxTreeItp&   aGxtItp,
                      const bool         aTrace,
                      const uint         aNodeIdx,
                      const uint         aLevel) const;

      // print header bits
      void printHeader(std::ostream& os) const;

      void print(std::ostream& os) const;
    };


    class Node_G_Inner_A {
      public:
        typedef Grid_TT<5,7> grid_t;
      public:
        Node_G_Inner_A();
      public:
        void attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle);
        double estimate(const rectangle_t& aQueryRectangle,
                        const GxTreeItp&   aGxtItp,
                        const bool         aTrace,
                        const uint         aIdx,
                        const uint         aLevel) const;
        void detach();
      public:
        bool iBitSetQ(const uint i, const uint j) const;
      public:
        uint32_t    getChildOffset(const uint32_t fBits) const; 
        node_type_t getChildType(const uint32_t fBits) const;
      public:
        inline const grid_t* grid() const { return &_grid; }
        inline uint32_t      scaleBit() const { return _content->scaleBit(); }
      public:
        double total() const;
      private:
        void initBr();
      private:
        const Node_G_Generic* _content;
        grid_t       _grid;
        bool         _hasBr;
        rectangle_t  _br;
    };


    class Node_G_Inner_B {
      public:
        typedef Grid_TT<5,6> grid_t;
      public:
        Node_G_Inner_B();
      public:
        void attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle);
        double estimate(const rectangle_t& aQueryRectangle,
                        const GxTreeItp&   aGxtItp,
                        const bool         aTrace,
                        const uint         aIdx,
                        const uint         aLevel) const;
        void detach();
      public:
        bool iBitSetQ(const uint i, const uint j) const;
      public:
        // void        advanceChildPtrs(const uint32_t fBits) const; // iBit case
        uint32_t    getChildOffset(const uint32_t fBits) const;
        node_type_t getChildType(const uint32_t fBits) const;
      public:
        inline const grid_t* grid() const { return &_grid; }
        inline uint32_t      scaleBit() const { return _content->scaleBit(); }
        inline uint32_t innerDeterminingBits() const { return _content->innerDeterminingBits(); }
      public:
        double total() const;
      private:
        void initBr();
      private:
        const Node_G_Generic* _content;
        grid_t       _grid;
        bool         _hasBr;
        rectangle_t  _br;
    };

    class Node_G_Inner_C {
      public:
        typedef Grid_TT<5,5> grid_t;
      public:
        Node_G_Inner_C();
      public:
        void attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle);
        double estimate(const rectangle_t& aQueryRectangle,
                        const GxTreeItp&   aGxtItp,
                        const bool         aTrace,
                        const uint         aIdx,
                        const uint         aLevel) const;
        void detach();
      public:
        bool iBitSetQ(const uint i, const uint j) const;
      public:
        uint32_t    getChildOffset(const uint32_t fBits) const;
        node_type_t getChildType(const uint32_t fBits) const;
      public:
        inline const grid_t* grid() const { return &_grid; }
        inline uint32_t      scaleBit() const { return _content->scaleBit(); }
      public:
        double total() const;
      private:
        void initBr();
        void advanceChildPtrs(const uint32_t fBits) const;
      private:
        const Node_G_Generic* _content;
        grid_t       _grid;
        uint32_t     _ptr[4];
        bool         _hasBr;
        rectangle_t  _br;
    };

    class Node_G_Inner_D {
      public:
        typedef Grid_TT<5,4> grid_t;
      public:
        Node_G_Inner_D();
      public:
        void attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle);
        double estimate(const rectangle_t& aQueryRectangle,
                        const GxTreeItp&   aGxtItp,
                        const bool         aTrace,
                        const uint         aIdx,
                        const uint         aLevel) const;
        void detach();
      public:
        bool iBitSetQ(const uint i, const uint j) const;
      public:
        uint32_t    getChildOffset(const uint32_t fBits) const;
        node_type_t getChildType(const uint32_t fBits) const;
      public:
        inline const grid_t* grid() const { return &_grid; }
        inline uint32_t      scaleBit() const { return _content->scaleBit(); }
      public:
        double total() const;
      private:
        void initBr();
        void advanceChildPtrs(const uint32_t fBits) const;
      private:
        const Node_G_Generic* _content;
        grid_t       _grid;
        uint32_t     _ptr[4];
        bool         _hasBr;
        rectangle_t  _br;
    };



  } // end namespace Gxt


} // end namespace H2D


#endif

