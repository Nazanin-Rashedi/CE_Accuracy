#ifndef H2D_GXT_ESTIMATE_T_HH
#define H2D_GXT_ESTIMATE_T_HH


// a) estimateInnerDirect:
//    only one ptr: no sBits, iBits, fBits are direct offsets
// b) estimateInnerSeparator
//    > 1ptr: sBits, iBits, fBits are split by separator into ranges
// c) estimateInnerIndirect
//    class Tchildaccessor
//    Tchildaccessor::advance(fBits)
//    Tchildaccessor::getChildPtr(fBits);
//    Tchildaccessor::getChildKind(fBits);
//

namespace H2D {
  namespace Gxt {

/*
 * estimation procedure for nodes allowing direct child access
 * nodes must be inner nodes
 * nodes must have iBits
 * nodes must have exactly one kind of children (G xor L xor M xor S)
 */

template<class Tnode, class Tgrid, class Tscale1, class Tscale2>
double
estimateInnerDirect(const rectangle_t&   aQueryRectangle,
                    const rectangle_t&   aBr, // true bounding rectangle (tr or br)
                    const Tnode&         aNode,
                    const Tgrid*         aGrid,
                    const GxTreeItp&     aGxt,
                    const Tscale1*       aScaleTotal,
                    const Tscale2*       aScale,
                    const uint32_t       aChildBaseIdx,
                    const node_type_t    aChildNodeType,
                    const bool           aTrace,   // for tracing
                    const uint           aNodeIdx, // for tracing
                    const uint           aLevel) { // for tracing

  const bool lTrace = aTrace;

  if(lTrace) {
    std::cout << std::string(2 * aLevel, ' ')
              << "estID: Node " << aNodeIdx << "  " 
              << aBr 
              << " tot = " << aNode.total()  << "  "
              << " grid: " << aGrid->GridSize
              << std::endl;
  }

  if(aQueryRectangle.contains(aBr)) {
    const double lRes = aScaleTotal->decompressDouble(aNode.total());
    if(lTrace) {
      std::cout << "  estID returns " << lRes 
                << " (fully contained)" << std::endl;
    }
    return lRes;
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);
  if(lIsec.hasZeroArea()) {
    if(lTrace) {
      std::cout << "  estID returns 0 (empty intersection)." 
                << std::endl;
    }
    if(lTrace) {
      std::cout << 0 << std::endl;
    }
    return 0;
  }

  if(lTrace) {
    std::cout << std::endl;
  }


  double lEstimate = 0;

  const uint nxy = Tgrid::GridSize;

  partitiondescxy_t lPd(aBr, nxy, nxy);
  rectangle_t       lTile;
  rectangle_t       lTileIsec;

  const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
  const uint lMaxI = std::min<uint>(nxy - 1, lPd.idxXcorrected(lIsec.xhi()));
  const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
  const uint lMaxJ = std::min<uint>(nxy - 1, lPd.idxYcorrected(lIsec.yhi()));

  if(lTrace) {
    std::cout << std::string(2*aLevel, ' ')
              << "   i-range: [" << lMinI << ", " << lMaxI << ']'
              << std::endl;
    std::cout << std::string(2*aLevel, ' ')
              << "   j-range: [" << lMinJ << ", " << lMaxJ << ']'
              << std::endl;
  }

  for(uint i = lMinI; i <= lMaxI; ++i) {
    for(uint j = lMinJ; j <= lMaxJ; ++j) {
      lPd.getRectangle(i, j, lTile);
      const uint32_t lTij   = aGrid->get(i,j);
      const uint32_t lFBits = (lTij & (Tgrid::iBitMask - 1));
      if(0 != (lTij & Tgrid::iBitMask)) {
        if(lTrace) {
          std::cout << std::string(2*aLevel, ' ') << "      "
                    << std::setw(2) << i << ' '
                    << std::setw(2) << j << ' '
                    << "   " << aNodeIdx << '@' << aLevel << ' '
                    << "     child call --> " << (aChildBaseIdx + lFBits) << ' '
                    << std::endl;
        }
        lEstimate += aGxt.estimate(aQueryRectangle,
                                   lTile,
                                   aChildBaseIdx + lFBits,
                                   aChildNodeType,
                                   aLevel + 1);
      } else {
        if(lTrace) {
          std::cout << std::string(2*aLevel, ' ') << "      "
                    << std::setw(2) << i << ' '
                    << std::setw(2) << j << ' '
                    << "   " << aNodeIdx << '@' << aLevel << ' '
                    << "     tile est"
                    << std::endl;
        }
        lTileIsec.isec(lIsec, lTile);
        if(!lTileIsec.hasZeroArea()) {
          const double lCumFreq = aScale->decompressDouble(lFBits);
          lEstimate += ((lTileIsec.area() / lTile.area()) * lCumFreq);
        }
      }
    }
  }

  if(lTrace) {
    std::cout <<  std::string(2*aLevel, ' ') << " estimate: " << lEstimate << std::endl;
  }
  return lEstimate;
}


// estimation for inner nodes with adressing using separators
// important: the last member of aSep must be huge such that
// any test of the fBits against this number succeeds

template<class Tnode, class Tgrid, class Tscale1, class Tscale2>
double
estimateInnerSeparator(const rectangle_t&   aQueryRectangle,
                       const rectangle_t&   aBr, // true bounding rectangle (tr or br)
                       const Tnode&         aNode,
                       const Tgrid*         aGrid,
                       const GxTreeItp&     aGxt,
                       const Tscale1*       aScaleTotal,
                       const Tscale2*       aScale,
                       const uint32_t*      aChildIdx, // [4]
                       const uint32_t*      aSep,      // [4]
                       const uint32_t       aGLMS,
                       const bool           aTrace,    // for tracing
                       const uint           aNodeIdx,  // for tracing
                       const uint           aLevel) {  // for tracing

  const bool lTrace = aTrace; 
  // const bool lTrace = true; // XXX

  if(lTrace) {
    std::cout << std::string(2 * aLevel, ' ')
              << "estIS Node " << aNodeIdx << "  " 
              << aBr << "  " << aNode.total() << "   "
              ;
  }

  if(aQueryRectangle.contains(aBr)) {
    const double lRes = aScaleTotal->decompressDouble(aNode.total());
    if(lTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "estIS returns " << lRes
                << " (fully contained)." << std::endl;
    }
    return lRes;
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);
  if(lIsec.hasZeroArea()) {
    if(lTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "estIS returns 0 (empty intersection)." 
                << std::endl;
    }
    return 0;
  }

  if(lTrace) {
    std::cout << std::endl;
  }

  // lChildKind contains the type (G,L,M,S) of the child node.
  // assumption: aSep[k] cumulates the number of child nodes of node_type < k
  // child indirection, only the first n of it are used,
  // depending on the number and kind of child pointers used.
  // (for example, it could be that a G-Node has G and L children only,
  //  or L and M, or G and S)
  // this allows the following:
  // 1) determining the type of the pointer
  //    (node_type_t) lChildKind[x]
  // 2) accessing the right pointer
  //    aChildIdx[lChildKind[x] + lOffset]
  // if z are the fBits of some tile
  // and x and y are calculated as follows:
  // z < aSep[0] : x = 0, y = z
  // z < aSep[1] : x = 1, y = z - aSep[0]
  // z < aSep[2] : x = 2, y = z - aSep[1]
  // z < aSep[3] : x = 3, y = z - aSep[2]
  // where one stops with x being at most as large as aNoSep
  // this is automatically achieved by storing in
  // aSep[n-1] a large number
  // THUS:
  // ASSUMPTION: aSep[n-1] > 1000 (we use 7777777)

  if(lTrace) {
    Bitvector32 lBv(aGLMS);
    std::cout << std::string(2 * aLevel, ' ')
              << "   GLMS = ";
    lBv.printReverse(std::cout, 4);
    std::cout << std::endl;
  }

  // init lChildKind
  uint32_t lChildKind[4];
  {
    uint x = 0;
    uint32_t lMask = (1 << 3);
    for(uint i = 0; i < 4; ++i) {
      if(lTrace) {
        // Bitvector32 lBv(lMask);
        // std::cout << "          mask = ";
        // lBv.printReverse(std::cout, 4);
        // std::cout << std::endl;
      }
      if(0 != (aGLMS & lMask)) {
        lChildKind[x] = i;
        ++x;
      }
      lMask >>= 1;
      // if(7777777 <= aSep[i]) { // ELIM THIS!! IT IS BAD!!
      //   break;
      // }
    }
    // the following can be eliminated, thus for having 
    // defined values in all of lChildKind entries.
    if(lTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "   x = " << x 
                << std::endl;
    }
    for(; x < 4; ++x) {
      lChildKind[x] = 88888888;
    }
  }

  if(lTrace) {
     std::cout << std::string(2*aLevel, ' ')
               << "  aChildIdx = ["
               << aChildIdx[0] << ", "
               << aChildIdx[1] << ", "
               << aChildIdx[2] << ", "
               << aChildIdx[3] << ']'
               << std::endl;
     std::cout << std::string(2*aLevel, ' ')
               << "  aSep = ["
               << aSep[0] << ", "
               << aSep[1] << ", "
               << aSep[2] << ", "
               << aSep[3] << ']'
               << std::endl;
     std::cout << std::string(2*aLevel, ' ')
               << "  lChildKind = ["
               << lChildKind[0] << ", "
               << lChildKind[1] << ", "
               << lChildKind[2] << ", "
               << lChildKind[3] << ']'
               << std::endl;
  }

  // start real work

  double lEstimate = 0;

  const uint nxy = Tgrid::GridSize;

  partitiondescxy_t lPd(aBr, nxy, nxy);
  rectangle_t       lTile;
  rectangle_t       lTileIsec;

  const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
  const uint lMaxI = std::min<uint>(nxy - 1, lPd.idxXcorrected(lIsec.xhi()));
  const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
  const uint lMaxJ = std::min<uint>(nxy - 1, lPd.idxYcorrected(lIsec.yhi()));

  if(lTrace) {
    std::cout << std::string(2*aLevel, ' ')
              << "   i-range: [" << lMinI << ", " << lMaxI << ']' 
              << std::endl;
    std::cout << std::string(2*aLevel, ' ')
              << "   j-range: [" << lMinJ << ", " << lMaxJ << ']' 
              << std::endl;
  }

  for(uint i = lMinI; i <= lMaxI; ++i) {
    for(uint j = lMinJ; j <= lMaxJ; ++j) {
      lPd.getRectangle(i, j, lTile);
      const uint32_t lTij   = aGrid->get(i,j);
      const uint32_t lFBits = (lTij & (Tgrid::iBitMask - 1));
      if(0 != (lTij & Tgrid::iBitMask)) {
        // determine child index
        uint lKind   = 0;
        uint lOffset = 0;
        if(lFBits < aSep[0]) {
          lKind   = lChildKind[0];
          lOffset = lFBits;
        } else
        if(lFBits < aSep[1]) {
          lKind   = lChildKind[1];
          lOffset = lFBits - aSep[0];
        } else
        if(lFBits < aSep[2]) {
          lKind   = lChildKind[2];
          lOffset = lFBits - aSep[1];
        } else {
          // never more than 4 different kind of pointers
          lKind   = lChildKind[3];
          lOffset = lFBits - aSep[2];
        }
        if(lTrace) {
          std::cout << std::string(2*aLevel, ' ')
                    << "  child:  " << i << ", " << j 
                    << " : lFBits = " << lFBits 
                    << ", lKind = " << lKind 
                    << ", lOffset = " << lOffset << std::endl;
        }
        lEstimate += aGxt.estimate(aQueryRectangle,
                                   lTile,
                                   aChildIdx[lKind] + lOffset,
                                   (node_type_t) lKind,
                                   aLevel + 1);
      } else {
        lTileIsec.isec(lIsec, lTile);
        if(!lTileIsec.hasZeroArea()) {
          const double lCumFreq = aScale->decompressDouble(lFBits);
          lEstimate += ((lTileIsec.area() / lTile.area()) * lCumFreq);
          if(lTrace) {
            std::cout << std::string(2*aLevel, ' ')
                      << "  tile: " << i << ", " << j
                      << "  lCumFreq = " << lCumFreq 
                      << std::endl;
          }
        }
      }
    }
  }

  if(lTrace) {
    std::cout <<  std::string(2*aLevel, ' ') 
              << "estIS Node " << aNodeIdx << " returns estimate: " << lEstimate 
              << std::endl;
  }
  return lEstimate;
}


// indirect child addressing
// for a given tile with its iBit set: its fBits encode a node_type_t
// thus, this requires to run over the full grid and count
// the number of nodes for each type seen so far.

template<class Tnode, class Tgrid, class Tscale1, class Tscale2>
double
estimateInnerIndirect(const rectangle_t&   aQueryRectangle,
                      const rectangle_t&   aBr, // true bounding rectangle (tr or br)
                      const Tnode&         aNode,
                      const Tgrid*         aGrid,
                      const GxTreeItp&     aGxt,
                      const Tscale1*       aScaleTotal,
                      const Tscale2*       aScale,
                      const uint32_t*      aChildIdx,
                      const bool           aTrace,   // for tracing
                      const uint           aNodeIdx, // for tracing
                      const uint           aLevel) { // for tracing
  
  if(aTrace) {
    std::cout << std::string(2 * aLevel, ' ')
              << "estIID Node enter with node " << aNodeIdx << "  TOTenc = " << aNode.total() << std::endl
              << std::string(2 * aLevel, ' ' )
              << "    br = " << aBr << "  "  << std::endl
              << std::string(2 * aLevel, ' ' )
              << "    qr = " << aQueryRectangle  << std::endl
              ;
  }

  if(aQueryRectangle.contains(aBr)) {
    const double lRes = aScaleTotal->decompressDouble(aNode.total());
    if(aTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "estIID Node " << aNodeIdx << " returns " << lRes 
                << " (fully contained)." << std::endl;
    }
    return lRes;
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);
  if(lIsec.hasZeroArea()) {
    if(aTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "estIID Node " << aNodeIdx << " returns 0 (empty intersection)." 
                << std::endl;
    }
    return 0;
  }


  double lEstimate = 0;

  const uint nxy = Tgrid::GridSize;

  if(aTrace) {
    std::cout << std::string(2 * aLevel, ' ')
              << "   nxy = " << nxy
              << std::endl;
  }

  partitiondescxy_t lPd(aBr, nxy, nxy);
  rectangle_t       lTile;
  rectangle_t       lTileIsec;

  const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
  const uint lMaxI = std::min<uint>(nxy - 1, lPd.idxXcorrected(lIsec.xhi()));
  const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
  const uint lMaxJ = std::min<uint>(nxy - 1, lPd.idxYcorrected(lIsec.yhi()));

  if(aTrace) {
    std::cout << std::string(2 * aLevel, ' ')
              << "   i-range: [" << lMinI << ", " << lMaxI << ']' << std::endl;
    std::cout << std::string(2 * aLevel, ' ')
              << "   j-range: [" << lMinJ << ", " << lMaxJ << ']' << std::endl;
    std::cout << std::string(2 * aLevel, ' ')
              << "   loop " << nxy << '*' << nxy
              << std::endl;

  }

  uint lChildCount[4] = {0, 0, 0, 0};

  for(uint i = 0; i < nxy; ++i) {
    for(uint j = 0; j < nxy; ++j) {
      const uint32_t lTij   = aGrid->get(i,j);
      const uint32_t lFBits = (lTij & (Tgrid::iBitMask - 1));
      if(0 != (lTij & Tgrid::iBitMask)) {
        const uint lKind   = (lFBits & 0x3); // 0x3: vorsichtshalber
        const uint lOffset = lChildCount[lKind];
        if(aTrace) {
          std::cout << std::string(2 * aLevel, ' ') << "      "
                    << std::setw(2) << i << ' '
                    << std::setw(2) << j << ' '
                    << "       has child of kind " << lKind << std::endl;
        }

        if(lMinI <= i && i <= lMaxI && lMinJ <= j && j <= lMaxJ) {
          lPd.getRectangle(i, j, lTile);
          if(aTrace) {
            std::cout << std::string(2*aLevel, ' ') << "      "
                      << std::setw(2) << i << ' '
                      << std::setw(2) << j << ' '
                      << "     child call with offset " << lOffset
                      << ", lTile = " << lTile
                      << std::endl;
          }
          const double lTileEst = aGxt.estimate(aQueryRectangle,
                                                lTile,
                                                aChildIdx[lKind] + lOffset,
                                                (node_type_t) lKind,
                                                aLevel + 1);
          lEstimate += lTileEst;
        }
        ++lChildCount[lKind];
      } else {
        if((lMinI <= i && i <= lMaxI && lMinJ <= j && j <= lMaxJ)) {
          lPd.getRectangle(i, j, lTile);
          lTileIsec.isec(lIsec, lTile);
          if(!lTileIsec.hasZeroArea()) {
            const double lCumFreq = aScale->decompressDouble(lFBits);
            const double lTileEst = ((lTileIsec.area() / lTile.area()) * lCumFreq);
            lEstimate += lTileEst;
            if(aTrace) {
              std::cout << std::string(2*aLevel, ' ') << "  " 
                        << std::setw(2) << i << ' '
                        << std::setw(2) << j << ' '
                        << std::setw(5) << lFBits << ' '
                        << std::setw(8) << lCumFreq << ' '
                        << std::setw(8) << lTileEst
                        << std::endl;
                         
            }
          }
        }
      }
    }
  }

  if(aTrace) {
    std::cout <<  std::string(2*aLevel, ' ') << " estIID estimate: " << lEstimate << std::endl;
  }
  return lEstimate;
}


template<class Tnode, class Tgrid, class Tscale1, class Tscale2>
double
estimateLeaf(const rectangle_t&   aQueryRectangle,
             const rectangle_t&   aBr, // true bounding rectangle (tr or br)
             const Tnode&         aNode,
             const Tgrid*         aGrid,
             const GxTreeItp&     aGxt,
             const Tscale1*       aScaleTotal, // scale for total compression
             const Tscale2*       aScale,      // scale for tile frequency compression
             const bool           aTrace,      // for tracing
             const uint           aNodeIdx,    // for tracing
             const uint           aLevel) {    // for tracing
  
  if(aTrace) {
    const double lTotal = aScaleTotal->decompressDouble(aNode.total());
    std::cout << std::string(2 * aLevel, ' ')
              << "estL: idx = " << aNodeIdx << ", " 
              << " TOT = " << lTotal << ",  "
              << " TOTenc = " << aNode.total() << std::endl
              << std::string(2 * aLevel, ' ')
              << "   tr = " << aBr << std::endl
              << std::string(2 * aLevel, ' ')
              << "   qr = " << aQueryRectangle << std::endl
              << std::endl;
  }

  if(aQueryRectangle.contains(aBr)) {
    const double lTotal = aScaleTotal->decompressDouble(aNode.total());
    if(aTrace) {
      std::cout << std::string(2 * aLevel, ' ')
                << "estL returns " << lTotal << " (fully contained)." << std::endl;
    }
    return lTotal;
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);

  if(lIsec.hasZeroArea()) {
    if(aTrace) {
      std::cout << std::string(2*aLevel, ' ')
                << "estL returns " << 0 << " note empty intersection." 
                << std::endl;
    }
    return 0;
  }


  double lEstimate = 0;

  const uint nxy = Tgrid::GridSize;

  partitiondescxy_t lPd(aBr, nxy, nxy);
  rectangle_t       lTile;
  rectangle_t       lTileIsec;

  const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
  const uint lMaxI = std::min<uint>(nxy - 1, lPd.idxXcorrected(lIsec.xhi()));
  const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
  const uint lMaxJ = std::min<uint>(nxy - 1, lPd.idxYcorrected(lIsec.yhi()));

  if(aTrace) {
    std::cout << std::string(2*aLevel, ' ')
              << "    estL: i-range: " << lMinI << ", " << lMaxI << std::endl
              << std::string(2*aLevel, ' ')
              << "          j-range: " << lMinJ << ", " << lMaxJ
              << std::endl;
    std::cout << std::endl;
    std::cout << std::string(2*aLevel, ' ')
              << "    i  j fBits   tf EstIJ"  << std::endl;
  }
  for(uint i = lMinI; i <= lMaxI; ++i) {
    for(uint j = lMinJ; j <= lMaxJ; ++j) {
      lPd.getRectangle(i, j, lTile);
      lTileIsec.isec(lIsec, lTile);
      if(!lTileIsec.hasZeroArea()) {
        const uint32_t lFBits   = aGrid->get(i,j);
        const double lTileFreq = aScale->decompressDouble(lFBits);
        const double lEstij = ((lTileIsec.area() / lTile.area()) * lTileFreq);
        lEstimate += lEstij;
        if(aTrace) {
           std::cout << std::string(2*aLevel, ' ')
                     << "   "
                     << std::setw(2) << i << ' '
                     << std::setw(2) << j << ' '
                     << std::setw(5) << lFBits << ' '
                     << std::setw(4) << lTileFreq << ' '
                     << std::setw(5) << lEstij << ' '
                     << std::endl;
        }
      }
    }
  }

  if(aTrace) {
    std::cout <<  std::string(2*aLevel, ' ') << "estL returns estimate " << lEstimate << std::endl;
  }
  return lEstimate;
}


// reconstruct_br 
// reconstruct the bounding rectangle from a given tile rectangle
// and a given code.
// it should only be called if reconstruction is really necessary.
// otherwise, it contains an assertion failure. (eliminated)

inline
void
reconstruct_br(      rectangle_t& aBrOut,
               const uint32_t     aBrKind,
               const uint32_t     aBrCode,
               const rectangle_t  aTileRectangle) {
    const uint     lNoZeroLoRows = (aBrCode >> 12) & 0xF;
    const uint     lNoZeroHiRows = (aBrCode >>  8) & 0xF;
    const uint     lNoZeroLoCols = (aBrCode >>  4) & 0xF;
    const uint     lNoZeroHiCols = (aBrCode >>  0) & 0xF;

    uint nxy = 16;

    switch(aBrKind) {
      case 1: nxy = 16; break;
      case 2: nxy = 32; break;
      case 3: nxy = 64; break;
      default: aBrOut = aTileRectangle;
               // std::cout << "aBrKind = " << aBrKind << std::endl;
               // std::cout << "aBrCode = " << aBrCode << std::endl;
               // assert(0 == 1);
               return;
               break;
    }

    // std::cout << "reconstruct_br: "
    //           << lNoZeroLoRows << ", " << lNoZeroHiRows << "; "
    //           << lNoZeroLoCols << ", " << lNoZeroHiCols << "; "
    //           << std::endl;

    partitiondescxy_t lPd(aTileRectangle, nxy, nxy);
    lPd.getRectangle( 0  + lNoZeroLoRows,
                     nxy - lNoZeroHiRows,
                      0  + lNoZeroLoCols,
                     nxy - lNoZeroHiCols,
                     aBrOut
                    );
    if(aBrOut.area() > 0.95 * aTileRectangle.area()) {
      std::cout << " rcbr: tr = " << aTileRectangle << std::endl
                << "       br = " << aBrOut << std::endl
                << " area(tr) = " << aTileRectangle.area() << std::endl
                << " area(br) = " << aBrOut.area() << std::endl;
      assert(aBrOut.area() < 0.95 * aTileRectangle.area());
    }
}

template<class Tnode>
void
extract_br_t(const Tnode&       aNode,
             const uint32_t     aBrKind,
                   rectangle_t& aBrOut,
                   bool&        aHasBrOut,
             const rectangle_t& aTileRectangle) {
  const bool lHasBr = (0 != aBrKind);
  if(lHasBr) {
    aHasBrOut = true;
    const uint32_t lBrCode = aNode.brCode();
    reconstruct_br(aBrOut, aBrKind, lBrCode, aTileRectangle);
  } else {
    aHasBrOut = false;
    aBrOut    = aTileRectangle;
  }
}

/*
template<class Tnode>
void
extract_br_t(const Tnode&       aNode,
             const uint32_t     aBrKind,
                   rectangle_t& aBrOut,
                   bool&        aHasBrOut,
             const rectangle_t& aTileRectangle) {
  const bool lHasBr = (0 != aBrKind);
  if(lHasBr) {
    aHasBrOut = true;
    const uint32_t lBrCode = aNode.brCode();
    const uint     lNoZeroLoRows = (lBrCode >> 12) & 0xF;
    const uint     lNoZeroHiRows = (lBrCode >>  8) & 0xF;
    const uint     lNoZeroLoCols = (lBrCode >>  4) & 0xF;
    const uint     lNoZeroHiCols = (lBrCode >>  0) & 0xF;

    uint nxy = 16;

    switch(aNode.brKind()) {
      case 1: nxy = 16; break;
      case 2: nxy = 32; break;
      case 3: nxy = 64; break;
      default: assert(0 == 1);
               break;
    }

    std::cout << "TT reconstructBr: "
              << lNoZeroLoRows << ", " << lNoZeroHiRows << "; "
              << lNoZeroLoCols << ", " << lNoZeroHiCols << "; "
              << std::endl;

    partitiondescxy_t lPd(aTileRectangle, nxy, nxy);
    lPd.getRectangle( 0  + lNoZeroLoRows,
                     nxy - lNoZeroHiRows,
                      0  + lNoZeroLoCols,
                     nxy - lNoZeroHiCols,
                     aBrOut
                    );
  } else {
    aHasBrOut = false;
    aBrOut    = aTileRectangle;
  }
}
*/


} // end namespace Gxt

} //end namespace H2D

#endif
