#include "Gxt_Node_G.hh"
#include "GxTree.hh"
#include "encode_t.hh"
#include "estimate_t.hh"

namespace H2D {
  namespace Gxt {


   // Node_G_Generic

   const Node_G_Generic::ScaleTotal*
   Node_G_Generic::_scaleTotal = q::ScaleMgr::instance()->scale_L_8_2();

   const Node_G_Generic::ScaleGrid*
   Node_G_Generic::_scaleGrid[NO_NODE_FINE] = {
     0                                     , // G_INNER_A  6-SCALE (6_1/6_3)
     q::ScaleMgr::instance()->scale_S_5_2(), // G_INNER_B
     q::ScaleMgr::instance()->scale_S_4_2(), // G_INNER_C
     q::ScaleMgr::instance()->scale_S_3_3(), // G_INNER_D
     0                                     , // G_LEAF_A   L-SCALE (7_1/7_2)
     q::ScaleMgr::instance()->scale_S_5_2(), // G_LEAF_B
     q::ScaleMgr::instance()->scale_S_4_2(), // G_LEAF_C
     q::ScaleMgr::instance()->scale_S_3_3(), // G_LEAF_D
   };


   const uint32_t
   Node_G_Generic::_gridOffset[NO_NODE_FINE] = {
     2, // G_INNER_A
     3, // G_INNER_B
     4, // G_INNER_C
     5, // G_INNER_D
     1, // G_LEAF_A
     1, // G_LEAF_B
     1, // G_LEAF_C
     1, // G_LEAF_D
   };

   Node_G_Generic::Node_G_Generic() : _arr(), _hdr(0), _total(0) {}

   void 
   Node_G_Generic::getPointer(uint32_t* aPtrOut) const {
     uint32_t lPtrIdx = 1;
     if(0 != (_hdr & 0x10)) {
       aPtrOut[0] = _arr[lPtrIdx];
       ++lPtrIdx;
     } else {
       aPtrOut[0] = 0;
     }
     if(0 != (_hdr & 0x08)) {
       aPtrOut[1] = _arr[lPtrIdx];
       ++lPtrIdx;
     } else {
       aPtrOut[1] = 0;
     }
     if(0 != (_hdr & 0x04)) {
       aPtrOut[2] = _arr[lPtrIdx];
       ++lPtrIdx;
     } else {
       aPtrOut[2] = 0;
     }
     if(0 != (_hdr & 0x02)) {
       aPtrOut[3] = _arr[lPtrIdx];
     } else {
       aPtrOut[3] = 0;
     }
   }
 
   void
   Node_G_Generic::getPointerAndSeparator_B(uint32_t* aPtrOut,
                                            uint32_t* aSepOut) const {
     aSepOut[0] = separatorBits_B();
     aSepOut[1] = 7777777;
     aSepOut[2] = 7777777;
     aSepOut[3] = 7777777;
     getPointer(aPtrOut);
   }

/* 
   void
   Node_G_Generic::getPointerAndSeparator_C(uint32_t* aPtrOut,
                                            uint32_t* aSepOut) const {
     aSepOut[0] = separatorBits_B();
     aSepOut[1] = separatorBits_C();
     aSepOut[2] = 7777777;
     aSepOut[3] = 7777777;
     getPointer(aPtrOut);
   }

   void
   Node_G_Generic::getPointerAndSeparator_D(uint32_t* aPtrOut,
                                            uint32_t* aSepOut) const {
     aSepOut[0] = separatorBits_B();
     aSepOut[1] = separatorBits_C();
     aSepOut[2] = separatorBits_D();
     aSepOut[3] = 7777777;
     getPointer(aPtrOut);
   }
*/


   void
   Node_G_Generic::getOnePointerAndKind(uint32_t&    aPtrOut,
                                        node_type_t& aKindOut) const {
     aPtrOut = _arr[1]; // _arr[0] is br
     if(0 != (_hdr & 0x10)) {
       aKindOut = N_G;
     } else
     if(0 != (_hdr & 0x08)) {
       aKindOut = N_L;
     } else
     if(0 != (_hdr & 0x04)) {
       aKindOut = N_M;
     } else
     if(0 != (_hdr & 0x02)) {
       aKindOut = N_S;
     } else {
       assert(0 == 1);
     }
    
   }
      

   void 
   Node_G_Generic::extractBr(rectangle_t& aBrOut, 
                             bool& aHasBrOut, 
                             const rectangle_t& aTileRectangle) const {


     if(hasBr()) {
       const uint32_t lBrKind = (0 != brCode()); // as long as there is only one kind of BR
       extract_br_t((*this), lBrKind, aBrOut, aHasBrOut, aTileRectangle);

       // std::cout << "Xtr br: kind = " << lBrKind << std::endl
       //           << "        code = " << brCode() << std::endl
       //           << "          tr = " << aTileRectangle << std::endl
       //           << "          br = " << aBrOut << std::endl;

     } else {
       aHasBrOut = false;
       aBrOut    = aTileRectangle;
     }

   }



    Node_G_Inner_T
    Node_G_Generic::getInnerSubkind() const {
      assert(!isLeaf());
      return (Node_G_Inner_T) number_of_bits_set(innerDeterminingBits());
    }

    Node_G_Leaf_T
    Node_G_Generic::getLeafSubkind() const {
      assert(isLeaf());
      if(isSetLeafC()) {
        return N_G_L_C;
      }
      uint32_t lKindBits = leafDeterminingBits();
      if(0x1 == lKindBits) {
        return N_G_L_A;
      } else
      if(0x2 == lKindBits) {
        return N_G_L_B;
      } else
      if(0x3 == lKindBits) {
        return N_G_L_D;
      } else {
        assert(0 == 1);
      }
    }

    Node_G_Generic::Node_Kind_Fine
    Node_G_Generic::determineNodeKindFine() const {
      if(isLeaf()) {
        return ((Node_Kind_Fine) ((int) Leaf_A + (int) getLeafSubkind()));
      } else {
        return ((Node_Kind_Fine) ((int) Inner_A + (int) getInnerSubkind() - 1));
      }
    }

    void
    Node_G_Generic::printHeader(std::ostream& os) const {
      Bitvector8 lBv(hdr());
      lBv.printReverse(os);
    }

    void
    Node_G_Generic::print(std::ostream& os) const {
      if(isLeaf()) {
        os << "G.L." << getLeafSubkind() << ' ';
      } else {
        os << "G.I." << getInnerSubkind() << ' ';
      }
      printHeader(os);
      os << std::endl;
    }


    double
    Node_G_Generic::estimate(const rectangle_t& aQueryRectangle,
                             const rectangle_t& aTileRectangle,
                             const GxTreeItp&   aGxtItp,
                             const bool         aTrace,
                             const uint         aNodeIdx,
                             const uint         aLevel) const {
      double lRes = 0;
      const Node_Kind_Fine lNkf = determineNodeKindFine();
      if(aTrace) {
        std::cout << "N_G_Generic::estimate: lNkf = " << (int) lNkf  << std::endl
                  << "                        idx = " << aNodeIdx << std::endl
                  << "                      level = " << aLevel << std::endl
                  << std::endl;
      }

      rectangle_t lBr;
      reconstruct_br(lBr, brKind(), brCode(), aTileRectangle);

      switch(lNkf) {
        case Inner_A: {
                        Grid_I_A lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        uint lChildBaseIdx = 0;
                        node_type_t lKind = N_NO_TYPE;
                        getOnePointerAndKind(lChildBaseIdx, lKind);
                        const q::Scale_6* lScaleGrid = ((0 == scaleBit()) ? 
                                                          q::ScaleMgr::instance()->scale_6_6_3()
                                                        : q::ScaleMgr::instance()->scale_6_6_1());
                        lRes = estimateInnerDirect(aQueryRectangle,
                                                   lBr,
                                                   (*this),
                                                   &lGrid,
                                                   aGxtItp,
                                                   _scaleTotal,
                                                   lScaleGrid,
                                                   lChildBaseIdx,
                                                   lKind,
                                                   aTrace,
                                                   aNodeIdx,
                                                   aLevel);
                      }
                      break;
        case Inner_B: {
                        Grid_I_B lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        uint32_t lChildIdx[4];
                        uint32_t lSep[4];
                        getPointerAndSeparator_B(lChildIdx, lSep);
                        if(aTrace) {
                          std::cout << std::string(2 * aLevel, ' ')
                                    << "Node_G_Generic::estimate::Inner_B: " << std::endl
                                    << std::string(2 * aLevel, ' ')
                                    << "   lChildIdx = " << lChildIdx[0] << ", "
                                                         << lChildIdx[1] << ", "
                                                         << lChildIdx[2] << ", "
                                                         << lChildIdx[3] << std::endl
                                    << "        lSep = " << lSep[0] << ", "
                                                         << lSep[1] << ", "
                                                         << lSep[2] << ", "
                                                         << lSep[3] << std::endl
                                    << "        GLMS = " << innerDeterminingBits() << std::endl
                                    ;
                        }
                        lRes = estimateInnerSeparator(aQueryRectangle,
                                                      lBr,
                                                      (*this),
                                                      &lGrid,
                                                      aGxtItp,
                                                      _scaleTotal,
                                                      _scaleGrid[lNkf],
                                                      lChildIdx,
                                                      lSep,
                                                      innerDeterminingBits(),
                                                      aTrace,
                                                      aNodeIdx,
                                                      aLevel);
                      }
                      break;
        case Inner_C: {
                        Grid_I_C lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        uint32_t lChildIdx[4];
                        getPointer(lChildIdx);
                        lRes = estimateInnerIndirect(aQueryRectangle,
                                                     lBr,
                                                     (*this),
                                                     &lGrid,
                                                     aGxtItp,
                                                     _scaleTotal,
                                                     _scaleGrid[lNkf],
                                                     lChildIdx,
                                                     aTrace,
                                                     aNodeIdx,
                                                     aLevel);
                      }
                      break;
        case Inner_D: {
                        Grid_I_D lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        uint32_t lChildIdx[4];
                        getPointer(lChildIdx);
                        lRes = estimateInnerIndirect(aQueryRectangle,
                                                     lBr,
                                                     (*this),
                                                     &lGrid,
                                                     aGxtItp,
                                                     _scaleTotal,
                                                     _scaleGrid[lNkf],
                                                     lChildIdx,
                                                     aTrace,
                                                     aNodeIdx,
                                                     aLevel);
                      }
                      break;
        case Leaf_A: {
                       Grid_L_A lGrid;
                       lGrid.attach(gridNonConst(lNkf));
                       const q::Scale_L* lScale = ((0 == scaleBit()) ?
                                                     q::ScaleMgr::instance()->scale_L_7_2()
                                                   : q::ScaleMgr::instance()->scale_L_7_1());
                       if(aTrace) {
                         std::cout << "   estimateG::N_G_L_A, idx = " << aNodeIdx << std::endl;
                         std::cout << "   grid:" << std::endl;
                         printGridMinusWithScale(lGrid, lScale, 7);
                       }

                       lRes = estimateLeaf(aQueryRectangle,
                                 lBr,
                                 (*this),
                                 &lGrid,
                                 aGxtItp,
                                 _scaleTotal,
                                 lScale,
                                 aTrace,
                                 aNodeIdx,
                                 aLevel);

                     }
                     break;
        case Leaf_B: {
                       Grid_L_B lGrid;
                       lGrid.attach(gridNonConst(lNkf));
                       const ScaleGrid* lScaleGrid = _scaleGrid[lNkf];
                       if(aTrace) {
                         std::cout << "estimate N_G_L_B for node with idx = " << aNodeIdx << std::endl;
                         std::cout << "grid:" << std::endl;
                         printGridMinusWithScale(lGrid, lScaleGrid, 5);
                       } 
                       lRes = estimateLeaf(aQueryRectangle,
                                 lBr,
                                 (*this),
                                 &lGrid,
                                 aGxtItp,
                                 _scaleTotal,
                                 lScaleGrid,
                                 aTrace,
                                 aNodeIdx,
                                 aLevel);
                     }
                     break;
        case Leaf_C: {
                       Grid_L_C lGrid;
                       lGrid.attach(gridNonConst(lNkf));
                       const ScaleGrid* lScaleGrid = _scaleGrid[lNkf];
                       if(aTrace) {
                         std::cout << "estimate N_G_L_C for note with idx = " << aNodeIdx << std::endl;
                         std::cout << "grid:" << std::endl;
                         printGridMinusWithScale(lGrid, lScaleGrid, 4);
                       }
                       lRes = estimateLeaf(aQueryRectangle,
                                 lBr,
                                 (*this),
                                 &lGrid,
                                 aGxtItp,
                                 _scaleTotal,
                                 lScaleGrid,
                                 aTrace,
                                 aNodeIdx,
                                 aLevel);
                     }
                     break;
        case Leaf_D: {
                       Grid_L_D lGrid;
                       lGrid.attach(gridNonConst(lNkf));
                       const ScaleGrid* lScaleGrid = _scaleGrid[lNkf];
                       if(aTrace) {
                         std::cout << "estimate N_G_L_D for note with idx = " << aNodeIdx << std::endl;
                         std::cout << "grid:" << std::endl;
                         printGridMinusWithScale(lGrid, lScaleGrid, 3);
                       }
                       lRes = estimateLeaf(aQueryRectangle,
                                 lBr,
                                 (*this),
                                 &lGrid,
                                 aGxtItp,
                                 _scaleTotal,
                                 lScaleGrid,
                                 aTrace,
                                 aNodeIdx,
                                 aLevel);
                     }
                     break;
        default: std::cout << "Node_G_Generic::estimate: bad node kind fine: " 
                           << lNkf << std::endl;
                 assert(0 == 1);
                 break;
      }
 
      return lRes;
    }



   /* 
    * G.INNER.A
    */


    Node_G_Inner_A::Node_G_Inner_A() : _content(0), _grid(), _hasBr(false), _br() { 
    }



    void
    Node_G_Inner_A::attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle) {
      _content = x;
      _grid.attach((uint8_t*) _content->grid_Inner_A());
      x->extractBr(_br, _hasBr, aTileRectangle);
    }

    void
    Node_G_Inner_A::detach() {
      _content = 0;
    }


   double
   Node_G_Inner_A::total() const {
     // return q::ScaleMgr::instance()->scale_L_8_2()->decompressDouble(_content->total());
     return (_content->total());
   }

   double
   Node_G_Inner_A::estimate(const rectangle_t& aQueryRectangle,
                           const GxTreeItp&    aGxtItp,
                           const bool          aTrace,
                           const uint          aNodeIdx, // of self
                           const uint          aLevel) const {
    double      lRes = 0;
    uint32_t    lChildBaseIdx = 0;
    node_type_t lKind = N_NO_TYPE;
    _content->getOnePointerAndKind(lChildBaseIdx, lKind);

    const q::Scale_L* lScaleTotal = q::ScaleMgr::instance()->scale_L_8_2();
    lRes = estimateInnerDirect(aQueryRectangle,
                               _br,
                               (*this),
                               grid(),
                               aGxtItp,
                               lScaleTotal,
                               (0 == scaleBit()) ? q::ScaleMgr::instance()->scale_6_6_3()
                                                 : q::ScaleMgr::instance()->scale_6_6_1(),
                               lChildBaseIdx,
                               lKind,
                               aTrace,
                               aNodeIdx,
                               aLevel);
    return lRes;
   }



  /* 
   * G.INNER.B
   */

   Node_G_Inner_B::Node_G_Inner_B() : _content(0), _grid(), _hasBr(false), _br() {
   }

   void
   Node_G_Inner_B::attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle) {
     _content = x;
      _grid.attach((uint8_t*) _content->grid_Inner_B());
     _content->extractBr(_br, _hasBr, aTileRectangle);
   }

   void
   Node_G_Inner_B::detach() {
     _content = 0;
   }

   double
   Node_G_Inner_B::total() const {
    // return q::ScaleMgr::instance()->scale_L_8_2()->decompressDouble(_content->total());
    return (_content->total());
   }

   double
   Node_G_Inner_B::estimate(const rectangle_t& aQueryRectangle,
                           const GxTreeItp&   aGxtItp,
                           const bool         aTrace,
                           const uint         aNodeIdx,
                           const uint         aLevel) const {
     const bool lTrace = aTrace;
     double   lRes = 0;
     uint32_t lChildIdx[4];
     uint32_t lSep[4];
     _content->getPointerAndSeparator_B(lChildIdx, lSep);
  
     if(lTrace) {
       std::cout << std::string(2 * aLevel, ' ')
                 << "Node_G_Inner_B::estimate: " << std::endl
                 << std::string(2 * aLevel, ' ')
                 << "   lChildIdx = " << lChildIdx[0] << ", "
                                      << lChildIdx[1] << ", "
                                      << lChildIdx[2] << ", "
                                      << lChildIdx[3] << std::endl
                 << "        lSep = " << lSep[0] << ", "
                                      << lSep[1] << ", "
                                      << lSep[2] << ", "
                                      << lSep[3] << std::endl
                 << "        GLMS = " << innerDeterminingBits() << std::endl
                 ;
      }

       
     const q::Scale_L* lScaleTotal = q::ScaleMgr::instance()->scale_L_8_2();
     lRes = estimateInnerSeparator(aQueryRectangle,
                                   _br,
                                   (*this),
                                   grid(),
                                   aGxtItp,
                                   lScaleTotal,
                                   q::ScaleMgr::instance()->scale_S_5_2(),
                                   lChildIdx,
                                   lSep,
                                   innerDeterminingBits(),
                                   aTrace,
                                   aNodeIdx,
                                   aLevel);

     return lRes;
   }

   /*
    * G.INNER.C
    */

   Node_G_Inner_C::Node_G_Inner_C() : _content(0), _grid(), _hasBr(false), _br() {
   }

   void
   Node_G_Inner_C::attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle) {
     _content = x;
      _grid.attach((uint8_t*) _content->grid_Inner_C());
     _content->extractBr(_br, _hasBr, aTileRectangle);
   }

   void
   Node_G_Inner_C::detach() {
     _content = 0;
   }

   double
   Node_G_Inner_C::total() const {
    // return q::ScaleMgr::instance()->scale_L_8_2()->decompressDouble(_content->total());
    return (_content->total());
   }

   double
   Node_G_Inner_C::estimate(const rectangle_t& aQueryRectangle,
                           const GxTreeItp&   aGxtItp,
                           const bool         aTrace,
                           const uint         aNodeIdx,
                           const uint         aLevel) const {
     double   lRes = 0;
     uint32_t lChildIdx[4];
     _content->getPointer(lChildIdx);

     const q::Scale_L* lScaleTotal = q::ScaleMgr::instance()->scale_L_8_2();
     lRes = estimateInnerIndirect(aQueryRectangle,
                                  _br,
                                  (*this),
                                  grid(),
                                  aGxtItp,
                                  lScaleTotal,
                                  q::ScaleMgr::instance()->scale_S_4_2(),
                                  lChildIdx,
                                  aTrace,
                                  aNodeIdx,
                                  aLevel);

     return lRes;
   }
   

   /*
    * G.INNER.D
    */

      Node_G_Inner_D::Node_G_Inner_D() : _content(0), _grid(), _hasBr(false), _br() {
   }

   void
   Node_G_Inner_D::attach(const Node_G_Generic* x, const rectangle_t& aTileRectangle) {
     _content = x;
      _grid.attach((uint8_t*) _content->grid_Inner_D());
     _content->extractBr(_br, _hasBr, aTileRectangle);
   }

   void
   Node_G_Inner_D::detach() {
     _content = 0;
   }

   double
   Node_G_Inner_D::total() const {
    // return q::ScaleMgr::instance()->scale_L_8_2()->decompressDouble(_content->total());
    return (_content->total());
   }

   double
   Node_G_Inner_D::estimate(const rectangle_t& aQueryRectangle,
                           const GxTreeItp&   aGxtItp,
                           const bool         aTrace,
                           const uint         aNodeIdx,
                           const uint         aLevel) const {
     double   lRes = 0;
     uint32_t lChildIdx[4];
     _content->getPointer(lChildIdx);

     const q::Scale_L* lScaleTotal = q::ScaleMgr::instance()->scale_L_8_2();

     lRes = estimateInnerIndirect(aQueryRectangle,
                                  _br,
                                  (*this),
                                  grid(),
                                  aGxtItp,
                                  lScaleTotal,
                                  q::ScaleMgr::instance()->scale_S_3_3(),
                                  lChildIdx,
                                  aTrace,
                                  aNodeIdx,
                                  aLevel);

     return lRes;
   }



  } // end namespace Gxt

} // end namespace H2D
