#include "Gxt_Node_L.hh"
#include "GxTree.hh"
#include "estimate_t.hh"

namespace H2D {
  namespace Gxt {


    // Node_L_Generic

    const q::Scale_S* Node_L_Generic::_scaleTotal =
                      q::ScaleMgr::instance()->scale_S_5_2();

    const q::Scale_S*
    Node_L_Generic::_scaleGrid[NO_NODE_FINE] = {
      q::ScaleMgr::instance()->scale_S_4_2(), // Inner_A1
      q::ScaleMgr::instance()->scale_S_5_2(), // Inner_A2
      q::ScaleMgr::instance()->scale_S_3_3(), // Inner_B1
      q::ScaleMgr::instance()->scale_S_4_2(), // Inner_B2
      q::ScaleMgr::instance()->scale_S_2_2(), // Inner_C1
      q::ScaleMgr::instance()->scale_S_3_3(), // Inner_C2
      q::ScaleMgr::instance()->scale_S_5_2(), // Leaf_A1
      q::ScaleMgr::instance()->scale_S_3_3(), // Leaf_B1
      q::ScaleMgr::instance()->scale_S_4_2(), // Leaf_B2
      q::ScaleMgr::instance()->scale_S_2_2(), // Leaf_C1
      q::ScaleMgr::instance()->scale_S_3_2()  // Leaf_C2
    };

    const uint32_t
    Node_L_Generic::_gridOffset[NO_NODE_FINE] = {
      2, // Inner_A1 // with BR     // BR PTR GRID
      1, // Inner_A2 // without BR  // PTR GRID
      3, // Inner_B1 // with BR     // BR PTR PTR GRID
      2, // Inner_B2 // without BR  // PTR PTR GRID
      4, // Inner_C1 // with BR     // BR PTR PTR PTR GRID
      3, // Inner_C2 // without BR  // PTR PTR PTR GRID
      1, // Leaf_A1  // with BR     // BR GRID
      1, // Leaf_B1  // with BR     // BR GRID
      0, // Leaf_B2  // without BR  // GRID
      1, // Leaf_C1  // with BR     // BR GRID
      0  // Leaf_C2  // without BR  // GRID
    };

    const uint32_t
    Node_L_Generic::_ptrOffset[Leaf_A1] = {
      1, // Inner_A1 // with BR     // BR PTR GRID
      0, // Inner_A2 // without BR  // PTR GRID
      1, // Inner_B1 // with BR     // BR PTR PTR GRID
      0, // Inner_B2 // without BR  // PTR PTR GRID
      1, // Inner_C1 // with BR     // BR PTR PTR PTR GRID
      0  // Inner_C2 // without BR  // PTR PTR PTR GRID
    };

    const char* Node_L_Generic::_name[NO_NODE_FINE+1] = {
        "A1",
        "A2",
        "B1",
        "B2",
        "C1",
        "C2",
        "A1",
        "B1",
        "B2",
        "C1",
        "C2",
        "XX"
    };

    Node_L_Generic::Node_L_Generic() : _arr(), _hdr() {}

    void
    Node_L_Generic::getPointerAndSeparator(uint32_t*  aPtr, /* [4] */
                                           uint32_t*  aSep, /* [4] */
                                           const Node_Kind_Fine aLnkf) const {
      const uint32_t lNoPtrBits = noPtrBits();
      const uint32_t lNoPtr     = ((0 == lNoPtrBits) ? 2 : lNoPtrBits);
      const uint32_t lSepLMS    = ((0 == lNoPtrBits) ? 0x2 : sepLMS()); // separates either L,M or M,S, L,S
      const uint32_t lPtrOffset = _ptrOffset[aLnkf];

      assert(2 == lNoPtr); // must be 2 (currently)
      assert(Inner_B2 == aLnkf); // only  L_Inner_B2 has separators (currently)

      aSep[0] = sep();
      aSep[1] = 7777777; 
      aSep[2] = 7777777; 
      aSep[3] = 7777777; 

      aPtr[0] = 0; // no G ptrs

      if(0x1 == lSepLMS) {
        aPtr[1] = _arr[lPtrOffset + 0];
        aPtr[2] = _arr[lPtrOffset + 1];
        aPtr[3] = 0;
      } else
      if(0x0 == lSepLMS) {
        aPtr[1] = 0; // no L ptr
        aPtr[2] = _arr[lPtrOffset + 0];
        aPtr[3] = _arr[lPtrOffset + 1];
      } else {
        aPtr[1] = _arr[lPtrOffset + 0];
        aPtr[2] = 0; // no M ptr
        aPtr[3] = _arr[lPtrOffset + 1];
      }

    }


    void
    Node_L_Generic::extractBr(      rectangle_t& aBrOut, 
                                    bool&        aHasBrOut,
                              const uint32_t     aBrKind,
                              const rectangle_t& aTileRectangle) const {

      extract_br_t((*this), aBrKind, aBrOut, aHasBrOut, aTileRectangle);

    }

/*
    L_Node_Kind
    Node_L_Generic::determineNodeKind() const {
      if(isLeaf()) {
        return ((L_Node_Kind) (L_Leaf_A_K + leafKind()));
      } else {
        return ((L_Node_Kind) (L_Inner_A_K + (noPtr() - 1)));
      }
    }
*/

    Node_L_Generic::Node_Kind_Fine
    Node_L_Generic::determineNodeKindFine() const {
      // std::cout << "detNKF:   isLeaf() = " << isLeaf() << std::endl;
      // std::cout << "detNKF: leafKind() = " << leafKind() << "  /noPtr" << std::endl;
      if(isLeaf()) {
        const uint32_t lLeafKind = leafKind();
        // std::cout << "detNKF: hasBr() = " << hasBr() << std::endl;
        if(0 == lLeafKind) {
          return Leaf_A1;
        }
        const uint32_t x = ((lLeafKind -1) << 1);
        // std::cout << "x = " << x << std::endl;
        // Leaf_B1  =  7, // with BR
        // Leaf_B2  =  8, // without BR
        // Leaf_C1  =  9, // with BR
        // Leaf_C2  = 10, // without BR
        return ((Node_Kind_Fine) (Leaf_B2 + x - hasBr()));
      } else {
        const uint32_t lNoPtr = noPtr();
        const uint32_t x = (lNoPtr << 1) - 1;
        // std::cout << "detNKF: noPtr = " << lNoPtr << std::endl;
        // std::cout << "detNKF:     x = " << x      << std::endl;
        return ((Node_Kind_Fine) (Inner_A1 + x - hasBr()));
      }
    }
  
    double
    Node_L_Generic::estimate(const rectangle_t& aQueryRectangle,
                             const rectangle_t& aTileRectangle,
                             const GxTreeItp&   aGxtItp,
                             const bool         aTrace,
                             const uint         aNodeIdx,
                             const uint         aLevel) const {
      double lRes = 0;
      const Node_Kind_Fine lNkf = determineNodeKindFine();
      if(aTrace) {
        std::cout << std::string(2 * aLevel, ' ' )
                  << "Node_L_Generic::estimate: lNkf = " << lNkf << ' ' << _name[lNkf]
                  << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "     idx = " << aNodeIdx << std::endl
                  << std::string(2 * aLevel, ' ')
                  << "   level = " << aLevel << std::endl
                  << std::endl;
      }
      switch(lNkf) {
        case Inner_A1: // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               Grid_I_A1 lGrid;
               lGrid.attach(gridNonConst(lNkf)); 
               lRes = estimateInnerDirect<Node_L_Generic, Grid_I_A1, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          lBr,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          getPtr(lNkf, 0),
                                          childNodeKind(),
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        case Inner_A2: // without BR
             {
               Grid_I_A2 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               if(aTrace) {
                 std::cout << std::string(2 * aLevel, ' ' )
                           << "Node_L_Generic::estimate: "
                           << "getPtr(...) --> " << getPtr(lNkf, 0)
                           << std::endl;
               }
               lRes = estimateInnerDirect<Node_L_Generic, Grid_I_A2, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          aTileRectangle,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          getPtr(lNkf, 0),
                                          childNodeKind(),
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        case Inner_B1: // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               Grid_I_B1 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               uint32_t lChildIdx[4] = {0, 0, 0, 0};
               if(sepLMS()) {
                 lChildIdx[1] = getPtr(lNkf, 0);
                 lChildIdx[2] = getPtr(lNkf, 1);
               } else {
                 lChildIdx[2] = getPtr(lNkf, 0);
                 lChildIdx[3] = getPtr(lNkf, 1);
               }
               lRes = estimateInnerIndirect<Node_L_Generic, Grid_I_B1, ScaleTotal, ScaleGrid>
                                           (aQueryRectangle,
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
        case Inner_B2: // without BR
            {
               Grid_I_B2 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               uint32_t lChildIdx[4] = {0, 0, 0, 0};
               uint32_t lSeparator[4] = {0, 0, 0, 0};
               getPointerAndSeparator(lChildIdx, lSeparator, lNkf);
               const uint32_t lNoPtrBits = noPtrBits();
               const uint32_t lGLMS = ((0 == lNoPtrBits) ? 
                                                    0x5 :
                                        (sepLMS() ? 0x6 : 0x3));
               if(aTrace) {
                 std::cout << std::string(2 * aLevel, ' ' )
                           << "Node_L_Generic::estimate: L.I.B2: "
                           << "noPtrBits = " << lNoPtrBits << ", "
                           << "sepLMS = " << sepLMS() << ", "
                           << "lGLMS = " << lGLMS << ", "
                           << " PTR = (" << getPtr(lNkf, 0) << ',' << getPtr(lNkf, 1) << "), "
                           << std::endl
                           << std::string(2 * aLevel, ' ' ) << "   "
                           << " lChildIdx = [" << lChildIdx[0] << ','
                                               << lChildIdx[1] << ','
                                               << lChildIdx[2] << ','
                                               << lChildIdx[3] << ']'
                           << std::endl;
               }
               lRes = estimateInnerSeparator<Node_L_Generic, Grid_I_B2, ScaleTotal, ScaleGrid>
                                           (aQueryRectangle,
                                            aTileRectangle,
                                            (*this),
                                            &lGrid,
                                            aGxtItp,
                                            _scaleTotal,
                                            _scaleGrid[lNkf],
                                            lChildIdx,
                                            lSeparator,
                                            lGLMS,
                                            aTrace,
                                            aNodeIdx,
                                            aLevel + 1);
             }
             break;
        case Inner_C1: // with BR, three ptr
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               Grid_I_C1 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               const uint32_t lChildIdx[4] = {0, 
                                              getPtr(lNkf, 0), 
                                              getPtr(lNkf, 1), 
                                              getPtr(lNkf, 2)
                                             };
               lRes = estimateInnerIndirect<Node_L_Generic, Grid_I_C1, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
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
        case Inner_C2: // without BR
             {
               Grid_I_C2 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               const uint32_t lChildIdx[4] = {0,
                                              getPtr(lNkf, 0),
                                              getPtr(lNkf, 1),
                                              getPtr(lNkf, 2)
                                             };
               lRes = estimateInnerIndirect<Node_L_Generic, Grid_I_C2, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          aTileRectangle,
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
        case Leaf_A1 : // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               if(aGxtItp.trace()) {
                 std::cout << std::string(2 * aLevel, ' ')
                           << "est_L_L_A1  BR " << hasBr()  << ' '
                                                << brKind() << ' ';
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
               lRes = estimateLeaf<Node_L_Generic, Grid_L_A1, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
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
                           << "  Leaf.A1 est = " << lRes << std::endl;
               }
             }
             break;
        case Leaf_B1 : // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               Grid_L_B1 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               lRes = estimateLeaf<Node_L_Generic, Grid_L_B1, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          lBr,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        case Leaf_B2 : // without BR
             {
               Grid_L_B2 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               lRes = estimateLeaf<Node_L_Generic, Grid_L_B2, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          aTileRectangle,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        case Leaf_C1 : // with BR
             {
               rectangle_t lBr;
               reconstruct_br(lBr,
                              brKind(),
                              brCode(),
                              aTileRectangle);
               Grid_L_C1 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               lRes = estimateLeaf<Node_L_Generic, Grid_L_C1, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          lBr,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        case Leaf_C2 : // without BR
             {
               Grid_L_C2 lGrid;
               lGrid.attach(gridNonConst(lNkf));
               lRes = estimateLeaf<Node_L_Generic, Grid_L_C2, ScaleTotal, ScaleGrid>
                                         (aQueryRectangle,
                                          aTileRectangle,
                                          (*this),
                                          &lGrid,
                                          aGxtItp,
                                          _scaleTotal,
                                          _scaleGrid[lNkf],
                                          aTrace,
                                          aNodeIdx,
                                          aLevel);
             }
             break;
        default:
             assert(0 == 1);
             break;
      }
      if(aTrace) {
        std::cout << std::string(2 * aLevel, ' ' )
                  << "Node_L_Generic::estimate Node " << aNodeIdx
                  << " returns estimate " << lRes
                  << std::endl;
      }
      return lRes;
    }

    void
    Node_L_Generic::print(std::ostream& os) const {
      Node_Kind_Fine lNkf = determineNodeKindFine();
      if(NO_NODE_FINE < lNkf) {
        lNkf = NO_NODE_FINE;
      }
      if(isLeaf()) {
        os << "L.L.";
      } else {
        os << "L.I.";
      }
      os << _name[lNkf] << ' ';
      os << totalDecompressed();
      switch(lNkf) {
        case Inner_A1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
               os << " PTR " << noPtr()
                  << ' ' << getPtr(lNkf, 0)
                  ;
             
             }
             break;
        case Inner_A2:
             {
               os << " BR " << hasBr();
               os << " PTR " << noPtr()
                  << ' ' << getPtr(lNkf, 0)
                  ;
             }
             break;
        case Inner_B1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
               os << " PTR " << noPtr()
                  << ' ' << getPtr(lNkf, 0)
                  << ' ' << getPtr(lNkf, 1)
                  ;
             }
             break;
        case Inner_B2:
             {
               os << " BR " << hasBr();
               os << " PTR " << noPtr() << '/' << noPtrBits() << '/' << sepLMS() << ' '
                  << ' ' << getPtr(lNkf, 0)
                  << ' ' << getPtr(lNkf, 1)
                  ;
             }
             break;
        case Inner_C1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
               os << " PTR " << noPtr()
                  << ' ' << getPtr(lNkf, 0)
                  << ' ' << getPtr(lNkf, 1)
                  << ' ' << getPtr(lNkf, 2)
                  ;
             }
             break;
        case Inner_C2:
             {
               os << " BR " << hasBr();
               os << " PTR " << noPtr()
                  << ' ' << getPtr(lNkf, 0)
                  << ' ' << getPtr(lNkf, 1)
                  << ' ' << getPtr(lNkf, 2)
                  ;
             }
             break;
        case Leaf_A1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
             }
             break;
        case Leaf_B1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
             }
             break;
        case Leaf_B2:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
             }
             break;
        case Leaf_C1:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
             }
             break;
        case Leaf_C2:
             {
               os << " BR " << hasBr() << ' ';
               printBrCode(os, brCode());
             }
             break;
        default: break;
      };
 
      os << std::endl;
    }

  } // end namespace Gxt

} // end namespace H2D


