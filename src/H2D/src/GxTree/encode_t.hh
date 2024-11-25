#ifndef H2D_GXT_ENCODE_T_HH
#define H2D_GXT_ENCODE_T_HH


// template functions to encode grids
// a) for inner nodes depending on the type of child addressing
//    1) direct
//    2) indirect (cumulative)
//    3) separator based
// b) for leaf nodes



namespace H2D {

  namespace Gxt {



    template<class Tgrid, class Tscale, class Tnumarr, class Tchildarr>
    void
    encodeGridDirect(      Tgrid*     aGrid,
                     const Tscale*    aScale,
                     const Tnumarr&   aChildIdx, // raw indices by kind
                     const Tchildarr& aChildPtr, // child ptrs to recognize (un)split tiles
                     const RegularPartitioning2dim& aRegP) {
      const uint nxy = Tgrid::GridSize;
      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          if(0 == aChildPtr(i,j)) {
            // not child for this tile
            const uint32_t lVal = aScale->compress(aRegP(i,j));
            aGrid->set(i, j, lVal);
          } else {
            // child index encoding
            const uint32_t lVal = (aChildIdx(i,j) | aGrid->iBitMask);
            aGrid->set(i, j, lVal);
          }
        }
      }
    }

    template<class Tgrid, class Tscale, class Tnumarr, class Tchildarr>
    void
    encodeGridIndirect(      Tgrid*     aGrid,
                       const Tscale*    aScale,
                       const Tnumarr&   aChildIdx, // raw indices by kind
                       const Tchildarr& aChildPtr,
                       const RegularPartitioning2dim& aRegP) {
      const uint nxy = Tgrid::GridSize;
      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          if(0 == aChildPtr(i,j)) {
            // not child for this tile
            const uint32_t lVal = aScale->compress(aRegP(i,j));
            aGrid->set(i, j, lVal);
          } else {
            // child index encoding
            assert(4 >  aChildPtr(i,j)->nodeType());
            assert(4 <= aGrid->iBitMask);
            const uint32_t lVal = aChildPtr(i,j)->nodeType() | aGrid->iBitMask;
            aGrid->set(i,j,lVal);
          }
        }
      }
    }
 
    template<class Tgrid, class Tscale, class Tnumarr, class Tchildarr>
    void
    encodeGridSeparator(      Tgrid*     aGrid,
                        const Tscale*    aScale,
                        const Tnumarr&   aChildIdx,    // raw indices by kind
                        const uint       aChildCnt[4], // #children of each kind
                        const Tchildarr& aChildPtr,
                        const RegularPartitioning2dim& aRegP) {
      const uint nxy = Tgrid::GridSize;
      Tnumarr lChildIdx(aChildIdx);
      uint    lChildCntCum[4];
      // 0. cumulate for each node type the number of children of a lesser node type
      lChildCntCum[0] = 0;
      lChildCntCum[1] = aChildCnt[0];
      lChildCntCum[2] = aChildCnt[0] + aChildCnt[1];
      lChildCntCum[3] = aChildCnt[0] + aChildCnt[1] + aChildCnt[2];
      // 1. adjust child indices by adding the number of nodes of lesser node type
      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          if(0 != aChildPtr(i,j)) {
            lChildIdx(i,j) = lChildCntCum[aChildPtr(i,j)->nodeType()] + aChildIdx(i,j);
          }
        }
      }
      // 2. store the tile frequencies and adjusted child indices in the grid
      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          if(0 == aChildPtr(i,j)) {
            // not child for this tile
            const uint32_t lVal = aScale->compress(aRegP(i,j));
            aGrid->set(i, j, lVal);
          } else {
            // child index encoding
            const uint32_t lVal = (lChildIdx(i,j) | aGrid->iBitMask);
            aGrid->set(i,j,lVal);
          }
        }
      } 
    }

    template<class Tgrid, class Tscale, class Tnumarr>
    void
    encodeGridLeaf(      Tgrid*   aGrid,
                   const Tscale*  aScale,
                   const Tnumarr& aFreqArr) {
      const uint nxy = Tgrid::GridSize;
      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          const uint32_t lVal = aScale->compress(aFreqArr(i,j));
          aGrid->set(i, j, lVal);
        }
      }
    }


    template<class Tgrid, class Tscale, class Tnumarr, class Tchildarr>
    bool
    checkGridInner(const Tgrid*     aGrid,
                   const Tscale*    aScale,
                   const Tnumarr&   aFreqArr,
                   const Tchildarr& aChildPtr,
                   const double     aMaxQ) {
      const uint nxy = Tgrid::GridSize;

      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          if(0 == aChildPtr(i,j)) {
            if(aGrid->isIBitSet(i,j)) {
              // ERROR
              std::cerr << "wrongly set iBit for grid (" << i << ',' << j << ')' << std::endl;
              assert(0 == 1);
              return false;
            }
            const uint   lCode = aGrid->get(i,j);
            const double lEst  = aScale->decompressDouble(lCode);
            const double lVal  = aFreqArr(i,j);
            if(lEst < 20 && lVal < 20) {
              continue;
            }
            if(!GxTree::Node::maxQErrOk(lEst, lVal, aMaxQ)) {
               std::cerr << "Bad estimate from encoded grid: "
                         << "estimate = "     << lEst 
                         << "; true value = " << lVal
                         << "; code = " << lCode
                         << std::endl;
              return false;
            }
          } else {
            if(!aGrid->isIBitSet(i,j)) {
              // ERROR
              std::cerr << "wrongly unset iBit for grid (" << i << ',' << j << ')' << std::endl;
              assert(0 == 1);
              return false;
            }
          }
        }
      }
      return true;
    }


    template<class Tgrid, class Tscale, class Tnumarr>
    bool
    checkGridLeaf(const Tgrid*     aGrid,
                  const Tscale*    aScale,
                  const Tnumarr&   aFreqArr,
                  const double     aMaxQ) {
      const uint nxy = Tgrid::GridSize;

      for(uint i = 0; i < nxy; ++i) {
        for(uint j = 0; j < nxy; ++j) {
          const uint   lCode = aGrid->get(i,j);
          const double lEst  = aScale->decompressDouble(lCode);
          const double lVal  = aFreqArr(i,j);
          if(lEst < 30 && lVal < 30) {
            continue;
          }
          if(!GxTree::Node::maxQErrOk(lEst, lVal, aMaxQ)) {
             std::cerr << "Bad estimate from encoded grid at pos(" << i << ',' << j << "): "
                       << "estimate = "     << lEst 
                       << "; true value = " << lVal
                       << "; code = " << lCode
                       << std::endl;
             const uint lNewCode  = aScale->compress(aFreqArr(i,j));
             std::cerr << "aFreqArr(i,j) = " << aFreqArr(i,j) << std::endl
                       << "aScale->compress(aFreqArr(i,j)) = " 
                              << lNewCode << std::endl
                       << "aScale->decompress(lNewCode) = " << 
                             aScale->decompress(lNewCode)
                       << std::endl;
            return false;
          }
        }
      }
      return true;
    }





    template<class Tgrid, class Tscale>
    void
    printGridPlusWithScale(const Tgrid&  aGrid, 
                           const Tscale* aScale, 
                           const uint    aNoFBits) {
      const uint32_t lMask = (((uint32_t) 1) << aNoFBits) - 1;
      for(uint i = 0; i < aGrid.gridSize(); ++i) {
        for(uint j = 0; j < aGrid.gridSize(); ++j) {
          std::cout << std::setw(5)
                    << aScale->decompress(aGrid.get(i,j) & lMask)
                    << ((0 == (aGrid.get(i,j) & Tgrid::iBitMask)) ? ' ' : '*')
                    << "  ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }

    template<class Tgrid, class Tscale>
    void
    printGridMinusWithScale(const Tgrid&  aGrid,
                            const Tscale* aScale,
                            const uint    aNoFBits) {
      const uint32_t lMask = (((uint32_t) 1) << aNoFBits) - 1;
      for(uint i = 0; i < aGrid.gridSize(); ++i) {
        for(uint j = 0; j < aGrid.gridSize(); ++j) {
          std::cout << std::setw(5)
                    << aScale->decompress(aGrid.get(i,j) & lMask)
                    << "  ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }




  } // end namespace Gxt

} // end namespace H2D

#endif
