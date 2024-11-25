#include "Gxt_Node_S.hh"
#include "GxTree.hh"
#include "estimate_t.hh"

namespace H2D {
  namespace Gxt {


    // Node_S_Generic

    // static members

    const char*
    Node_S_Generic::_name[NO_NODE_FINE + 1] = { "S.I.A2",
                                                "S.L.A1",
                                                "S.L.A2",
                                                "S.L.B2",
                                                "S.X.XX" };

    const Node_S_Generic::ScaleTotal*
    Node_S_Generic::_scaleTotal = q::ScaleMgr::instance()->scale_S_3_3();

    const Node_S_Generic::ScaleGrid*
    Node_S_Generic::_scaleGrid[NO_NODE_FINE] = {
      q::ScaleMgr::instance()->scale_S_2_2(), // INNER_A2
      q::ScaleMgr::instance()->scale_S_2_2(), // LEAF_A1
      q::ScaleMgr::instance()->scale_S_4_1(), // LEAF_A2
      q::ScaleMgr::instance()->scale_S_3_2()  // LEAF_B2
    };

    // functions
    Node_S_Generic::Node_S_Generic() : _arr(0), _hdr() {
      _hdr._num = 0;
    }




    void
    Node_S_Generic::extractBr(rectangle_t& aBrOut, bool& aHasBr, const rectangle_t& aTileRectangle) const {
      if(isLeafA1()) {
        const uint32_t lBrKind = 0x1;
        aHasBr = true;
        extract_br_t((*this), lBrKind, aBrOut, aHasBr, aTileRectangle);
      } else {
        aHasBr = false;
        aBrOut = aTileRectangle;
      }
    }




    Node_S_Generic::Node_Kind_Fine
    Node_S_Generic::determineNodeKindFine() const {
     Node_Kind_Fine lNkf = NO_NODE_FINE;
     if(isLeaf()) {
       if(0 == hdr()._x) {
         if(0 == hdr()._y) {
           lNkf = Leaf_A1;
         } else {
           lNkf = Leaf_A2;
         }
       } else {
         lNkf = Leaf_B2;
       }
     } else {
       lNkf = Inner_A2;
     }
     return lNkf;
    }


    double
    Node_S_Generic::estimate(const rectangle_t& aQueryRectangle,
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
                  << "N_S_Generic::est lNkf = " << (int) lNkf  << " = " << _name[lNkf] << std::endl
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
        case Inner_A2: {
                         Grid_I_A2 lGrid;
                         lGrid.attach(gridInnerA2NonConst());
                         lRes = estimateInnerDirect(aQueryRectangle,
                                                    aTileRectangle,
                                                    (*this),
                                                    &lGrid,
                                                    aGxtItp,
                                                    _scaleTotal,
                                                    scaleGrid(lNkf),
                                                    getPtr(),
                                                    N_S,
                                                    aTrace,
                                                    aNodeIdx,
                                                    aLevel);
                         if(aGxtItp.trace()) {
                           std::cout << std::string(2 * aLevel, ' ')
                                     << "  S.Inner.A1 est = " << lRes << std::endl;
                         }
                       }
                       break;
        case Leaf_A1:  {
                         rectangle_t lBr;
                         reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
                         if(aGxtItp.trace()) {
                           std::cout << std::string(2 * aLevel, ' ')
                                     << "est_S_L_A1  BRcode =  " ;
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
                         lGrid.attach(gridLeafA1NonConst());
                         lRes = estimateLeaf(aQueryRectangle,
                                             lBr,
                                             (*this),
                                             &lGrid,
                                             aGxtItp,
                                             _scaleTotal,
                                             _scaleGrid[lNkf],
                                             aTrace,
                                             aNodeIdx,
                                             aLevel);
                         if(aGxtItp.trace()) {
                           std::cout << std::string(2 * aLevel, ' ')
                                     << "  S.Leaf.A1 est = " << lRes << std::endl;
                         }
                       }
                       break;
        case Leaf_A2:  {
                         Grid_L_A2 lGrid;
                         lGrid.attach(gridLeafA2NonConst());
                         lRes = estimateLeaf(aQueryRectangle,
                                             aTileRectangle,
                                             (*this),
                                             &lGrid,
                                             aGxtItp,
                                             _scaleTotal,
                                             _scaleGrid[lNkf],
                                             aTrace,
                                             aNodeIdx,
                                             aLevel);
                         if(aGxtItp.trace()) {
                           std::cout << std::string(2 * aLevel, ' ')
                                     << "  S.Leaf.A2 est = " << lRes << std::endl;
                         }
                       }
                       break;
        case Leaf_B2:  {
                         Grid_L_B2 lGrid;
                         lGrid.attach(gridLeafB2NonConst());
                         lRes = estimateLeaf(aQueryRectangle,
                                             aTileRectangle,
                                             (*this),
                                             &lGrid,
                                             aGxtItp,
                                             _scaleTotal,
                                             _scaleGrid[lNkf],
                                             aTrace,
                                             aNodeIdx,
                                             aLevel);
                         if(aGxtItp.trace()) {
                           std::cout << std::string(2 * aLevel, ' ')
                                     << "  S.Leaf.B2 est = " << lRes << std::endl;
                         }
                       }
                       break;
        default: assert(0 == 1);
                 break;
      }
      if(lTrace) {
        std::cout << std::string(2 * aLevel, ' ')
                  << "N_S_Generic::est returns " << lRes
                  << std::endl;

      }
      return lRes;
    }

    uint
    Node_S_Generic::noChildren() const {
      Node_Kind_Fine lNkf = determineNodeKindFine();
      if(NO_NODE_FINE < lNkf) {
        lNkf = NO_NODE_FINE;
      }
      if(isLeaf()) {
        return 0;
      }
      uint lRes = 0;
      // only type of inner node
      Grid_I_A2 lGrid;
      lGrid.attach(gridInnerA2NonConst());
      lRes = lGrid.noChildren();
      return lRes;
    }

    void
    Node_S_Generic::print(std::ostream& os) const {
      Node_Kind_Fine lNkf = determineNodeKindFine();
      if(NO_NODE_FINE < lNkf) {
        lNkf = NO_NODE_FINE;
      }
      os << std::setw(6) << _name[lNkf] << ' ';
      os << " TOT " << totalDecompressed();
      switch(lNkf) {
        case Inner_A2: {
                         os << " PTR " << getPtr() << " NO CHILD = " << noChildren();
                       }
                       break;
        case Leaf_A1:
                      os << " BrCode ";
                      printBrCode(os, brCode());
                      break;
        case Leaf_A2:
        case Leaf_B2:
                      break;
        default: os << "XXX";
                 break;
      }
      os << std::endl;
    }


  } // end namespace Gxt

} // end namespace H2D


