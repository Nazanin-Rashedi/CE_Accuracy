#include "Gxt_Node_M.hh"
#include "GxTree.hh"
#include "estimate_t.hh"

namespace H2D {
  namespace Gxt {

    /*
     *   Node_M_Generic
     */

    const char* Node_M_Generic::_name[NO_NODE_FINE + 1] = {
                   "M.I.A1",
                   "M.I.A2",
                   "M.I.B2",
                   "M.L.A1",
                   "M.L.A2",
                   "M.L.B1",
                   "M.L.B2",
                   "M.L.C2",
                   "M.X.XX"
                };

    const uint32_t Node_M_Generic::_gridOffset[NO_NODE_FINE] = {
                     2, // BR, PTR, GRID  Inner_A1 = 0,
                     1, //     PTR, GRID  Inner_A2 = 0,
                     2, // Ptr,PTR, GRID  Inner_B2 = 1,
                     1, // BR       GRID  Leaf_A1 = 2, // with BR  
                     0, //          GRID  Leaf_A2 = 3, // without BR
                     1, // BR       GRID  Leaf_B1 = 4, // with BR   
                     0, //          GRID  Leaf_B2 = 5, // without BR 
                     0, //          GRID  Leaf_C2 = 6, // without BR  
                   };

    const uint32_t Node_M_Generic::_ptrOffset[Leaf_A1] = {
                     1, // BR, PTR, GRID     Inner_A1 = 0,
                     0, //     PTR, GRID     Inner_A2 = 0,
                     0, // Ptr,PTR, GRID     Inner_B2 = 1,
                   };

    const Node_M_Generic::ScaleTotal*
    Node_M_Generic::_scaleTotalS = q::ScaleMgr::instance()->scale_S_3_4();

    const Node_M_Generic::ScaleTotal*
    Node_M_Generic::_scaleTotalX = q::ScaleMgr::instance()->scale_S_5_1();

    const Node_M_Generic::ScaleGrid*
    Node_M_Generic::_scaleGrid[2][NO_NODE_FINE] = {
      { 
        // rough scales
        q::ScaleMgr::instance()->scale_S_2_2(),  // INNER_A1
        q::ScaleMgr::instance()->scale_S_3_3(),  // INNER_A2
        q::ScaleMgr::instance()->scale_S_2_2(),  // INNER_B2
        q::ScaleMgr::instance()->scale_S_4_2(),  // Leaf_A1
        q::ScaleMgr::instance()->scale_S_5_1(),  // Leaf_A2
        q::ScaleMgr::instance()->scale_S_2_2(),  // Leaf_B1
        q::ScaleMgr::instance()->scale_S_3_3(),  // Leaf_B2
        q::ScaleMgr::instance()->scale_S_2_2(),  // Leaf_C2
      },
      {
        // refined, more precise scales
        q::ScaleMgr::instance()->scale_S_2_2(),  // INNER_A1
        q::ScaleMgr::instance()->scale_S_3_2(),  // INNER_A2
        q::ScaleMgr::instance()->scale_S_2_2(),  // INNER_B2
        q::ScaleMgr::instance()->scale_S_4_1(),  // Leaf_A1
        q::ScaleMgr::instance()->scale_S_5_1(),  // Leaf_A2
        q::ScaleMgr::instance()->scale_S_2_1(),  // Leaf_B1
        q::ScaleMgr::instance()->scale_S_3_2(),  // Leaf_B2
        q::ScaleMgr::instance()->scale_S_2_1(),  // Leaf_C2
      }
    };


    Node_M_Generic::Node_M_Generic() : _arr(), _hdr() {
      _arr[0] = 0;
      _arr[1] = 0;
      _arr[2] = 0;
      _hdr._num = 0;
    }

    
    void
    Node_M_Generic::extractBr(rectangle_t& aBrOut, 
                              bool& aHasBr, 
                              const rectangle_t& aTileRectangle) const {
      uint32_t lBrKind = 0;
      bool     lHasBr = false;
      if(isInnerA1()) {
        lBrKind = 0x1;
        lHasBr  = true;
      } else
      if(isLeaf()) {
        lBrKind = brKindX();
        lHasBr  = (0 != lBrKind);
      } else {
        // must be M.Inner.A2 or M.Inner.B2, which have no Br
        lHasBr  = false;
        lBrKind = 0x0;
      }

      if(lHasBr) {
        extract_br_t((*this), lBrKind, aBrOut, aHasBr, aTileRectangle);
      } else {
        aHasBr = false;
        aBrOut = aTileRectangle;
      }

    }


    Node_M_Generic::Node_Kind_Fine
    Node_M_Generic::determineNodeKindFine() const {
      if(isLeaf()) {
        const int lHasBr = hasBrLeaf();
        const int lKind  = leafKind();
        const int lRes   = (2 == lKind) ? Leaf_C2 : (Leaf_A1 + 1 + 2 * lKind - lHasBr);
        return (Node_Kind_Fine) lRes;
      }
      return (Node_Kind_Fine) nodeType();
    }


    double
    Node_M_Generic::estimate(const rectangle_t& aQueryRectangle,
                             const rectangle_t& aTileRectangle,
                             const GxTreeItp&   aGxtItp,
                             const bool         aTrace,
                             const uint         aNodeIdx,
                             const uint         aLevel) const {
      const  bool lTrace = aTrace;
      double lRes = 0;
      const Node_Kind_Fine lNkf = determineNodeKindFine();
      if(lTrace) {
        std::cout << std::string(2 * aLevel, ' ')
                  << "N_M_Generic::est lNkf = " << (int) lNkf  << " = " << _name[lNkf] << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "                  idx = " << aNodeIdx << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "                level = " << aLevel << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "                   tr = " << aTileRectangle << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "                   qr = " << aQueryRectangle 
                  << std::endl;
      }
      switch(lNkf) {
        case Inner_A1: // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKindA1(),
                              brCode(),
                              aTileRectangle);

               if(lTrace) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "                   br = " << aQueryRectangle << std::endl
                           << std::endl;
                 std::cout << std::string(2* aLevel, ' ')
                           << "node " << aNodeIdx << ": ";
                 print(std::cout);
                 std::cout << std::endl;
               }
               uint32_t lChildIdx[4];
               lChildIdx[N_G] = 0;
               lChildIdx[N_L] = 0;
               lChildIdx[N_M] = 0;
               lChildIdx[N_S] = 0;

               uint16_t lPtr = getPtr(lNkf);

               if(isPtrToNode_M(lPtr)) {
                 lChildIdx[N_M] = thePtr(lPtr);
               } else {
                 lChildIdx[N_S] = thePtr(lPtr);
               }

               Grid_I_A1 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateInnerIndirect(aQueryRectangle,
                                            lBr,
                                            (*this),
                                            &lGrid,
                                            aGxtItp,
                                            _scaleTotalS,
                                            scaleGrid(0, lNkf),
                                            lChildIdx,
                                            lTrace,
                                            aNodeIdx,
                                            aLevel);
             }
             break;
        case Inner_A2: // without BR
             {
               Grid_I_A2 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               node_type_t lChildNodeType = N_M;
               if(0x1 == ptrKindBit()) {
                 lChildNodeType = N_S;
               }
             
               lRes = estimateInnerDirect(aQueryRectangle,
                                          aTileRectangle,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotalX,
                                          scaleGrid(scaleBitX(), lNkf),
                                          getPtr(lNkf),
                                          lChildNodeType,
                                          lTrace,
                                          aNodeIdx,
                                          aLevel);

             }
             break;
        case Inner_B2: // without BR
             {
               
               uint32_t lChildIdx[4];
               lChildIdx[N_G] = 0;
               lChildIdx[N_L] = 0;
               lChildIdx[N_M] = getPtrToM();
               lChildIdx[N_S] = getPtrToS();

               Grid_I_B2 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateInnerIndirect(aQueryRectangle,
                                            aTileRectangle,
                                            (*this),
                                            &lGrid,
                                            aGxtItp,
                                            _scaleTotalS,
                                            scaleGrid(0, lNkf),
                                            lChildIdx,
                                            lTrace,
                                            aNodeIdx,
                                            aLevel);
             }
             break;
        case Leaf_A1: // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKindX(),
                              brCode(),
                              aTileRectangle);

               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_M_L_A1  BR " << hasBr()  << ' '
                                                << brKindX() << ' ';
                 printBrCode(std::cout, brCode()) << std::endl;
                 std::cout << std::string(2 * aLevel, ' ')
                           << "            tr = " << aTileRectangle << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            br = " << lBr << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            qr = " << aQueryRectangle << std::endl
                           ;
               }

               Grid_L_A1 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateLeaf(aQueryRectangle,
                                   lBr,
                                   (*this),
                                   &lGrid,
                                   aGxtItp,
                                   _scaleTotalX,
                                   scaleGrid(scaleBitX(), lNkf),
                                   lTrace,
                                   aNodeIdx,
                                   aLevel);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "   M.Leaf.A1 est = " << lRes << std::endl;
               }
             }
             break;
        case Leaf_A2: // without BR
             {
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_M_L_A2  BR " << hasBr()  << ' '
                                                << brKindX() << ' ';
                 printBrCode(std::cout, brCode()) << std::endl;
                 std::cout << std::string(2 * aLevel, ' ')
                           << "            tr = " << aTileRectangle << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            qr = " << aQueryRectangle << std::endl
                           ;
               }

               Grid_L_A2 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateLeaf(aQueryRectangle,
                                   aTileRectangle,
                                   (*this),
                                   &lGrid,
                                   aGxtItp,
                                   _scaleTotalX,
                                   scaleGrid(scaleBitX(), lNkf),
                                   lTrace,
                                   aNodeIdx,
                                   aLevel);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "   M.Leaf.A2 est = " << lRes << std::endl;
               }
             }
             break;
        case Leaf_B1: // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKindX(),
                              brCode(),
                              aTileRectangle);

               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_M_L_B1  BR " << hasBr()  << ' '
                                                << brKindX() << ' ';
                 printBrCode(std::cout, brCode()) << std::endl;
                 std::cout << std::string(2 * aLevel, ' ')
                           << "            tr = " << aTileRectangle << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            br = " << lBr << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            qr = " << aQueryRectangle << std::endl
                           ;
               }

               Grid_L_B1 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateLeaf(aQueryRectangle,
                                   lBr,
                                   (*this),
                                   &lGrid,
                                   aGxtItp,
                                   _scaleTotalX,
                                   scaleGrid(scaleBitX(), lNkf),
                                   lTrace,
                                   aNodeIdx,
                                   aLevel);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "   M.Leaf.B1 est = " << lRes << std::endl;
               }
             }
             break;
        case Leaf_B2: // without BR
             {
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_M_L_B2  BR " << hasBr()  << ' '
                                                << brKindX() << ' ';
                 printBrCode(std::cout, brCode()) << std::endl;
                 std::cout << std::string(2 * aLevel, ' ')
                           << "            tr = " << aTileRectangle << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            qr = " << aQueryRectangle << std::endl
                           ;
               }

               Grid_L_B2 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateLeaf(aQueryRectangle,
                                   aTileRectangle,
                                   (*this),
                                   &lGrid,
                                   aGxtItp,
                                   _scaleTotalX,
                                   scaleGrid(scaleBitX(), lNkf),
                                   lTrace,
                                   aNodeIdx,
                                   aLevel);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "   M.Leaf.B2 est = " << lRes << std::endl;
               }
             }
             break;
        case Leaf_C2: // without BR
             {
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_M_L_C2  BR " << hasBr()  << ' '
                                                << brKindX() << ' ';
                 printBrCode(std::cout, brCode()) << std::endl;
                 std::cout << std::string(2 * aLevel, ' ')
                           << "            tr = " << aTileRectangle << std::endl
                           << std::string(2 * aLevel, ' ')
                           << "            qr = " << aQueryRectangle << std::endl
                           ;
               }

               Grid_L_C2 lGrid;
               lGrid.attach(gridNonConst(lNkf));

               lRes = estimateLeaf(aQueryRectangle,
                                   aTileRectangle,
                                   (*this),
                                   &lGrid,
                                   aGxtItp,
                                   _scaleTotalX,
                                   scaleGrid(scaleBitX(), lNkf),
                                   lTrace,
                                   aNodeIdx,
                                   aLevel);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "   M.Leaf.C2 est = " << lRes << std::endl;
               }
             }
             break;
        default:
             assert(0 == 1);
             break;
      }
      if(aGxtItp.trace()) {
        std::cout << std::string(2 * aLevel, ' ')
                  << "N_M_Generic::est returns " << lRes
                  << std::endl;
      }
      return lRes;
    }


    void
    Node_M_Generic::print(std::ostream& os) const {
      Node_Kind_Fine lNkf = determineNodeKindFine();
      if(NO_NODE_FINE < lNkf) {
        lNkf = NO_NODE_FINE;
      }
      os << std::setw(6) << _name[lNkf] << ' ';
      os << " TOT " << totalDecompressed();
      switch(lNkf) {
        case Inner_A1: {
                         os << " PTR " << getPtr(lNkf) << " NO CHILD = " << noChildren();
                       }
                       break;
        case Inner_A2: {
                         os << " PTR " << getPtr(lNkf) << " NO CHILD = " << noChildren();
                       }
                       break;
        case Inner_B2: {
                         os << " PTR M " << getPtrToM() 
                            << " S " << getPtrToS()
                            << " NO CHILD = " << noChildren();
                       }
                       break;
        case Leaf_A1: 
        case Leaf_B1: 
                      os << " BR " << hasBr() << " BrCode ";
                      printBrCode(os, brCode());
                      break;
        case Leaf_A2: 
        case Leaf_B2: 
        case Leaf_C2: 
                      break;
        default: os << "XXX";
                 break;
      }
      os << std::endl;
    }

    uint
    Node_M_Generic::noChildren() const {
      Node_Kind_Fine lNkf = determineNodeKindFine();
      if(NO_NODE_FINE < lNkf) {
        lNkf = NO_NODE_FINE;
      }
      if(isLeaf()) {
        return 0;
      }
      uint lRes = 0;
      switch(lNkf) {
       case Inner_A1: {
                        Grid_I_A1 lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        lRes = lGrid.noChildren();
                      }
                      break;
       case Inner_A2: {
                        Grid_I_A2 lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        lRes = lGrid.noChildren();
                      }
                      break;
       case Inner_B2: {
                        Grid_I_B2 lGrid;
                        lGrid.attach(gridNonConst(lNkf));
                        lRes = lGrid.noChildren();
                      }
                      break;
       default: assert(0 == 1);
                break;
      }
      return lRes;
    }

  } // end namespace Gxt

} // end namespace H2D


