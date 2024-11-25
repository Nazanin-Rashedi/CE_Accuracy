#include "GxTree.hh"

#include "estimate_t.hh"
#include "encode_t.hh"

namespace H2D {


using namespace Gxt;


/*
 *   GxTree::Node members
 */


GxTree::Node::Node(const Data2dim&    aData,
                   const rectangle_t& aTr,
                   const uint         aLevel,
                   const Node*        aParent,
                   const GxTree&      aGxTree)
               : _data(aData),
                 _tr(aTr),
                 _br(),
                 _brd(),
                 _brdd(),
                 _cumFreq(aData.total()),
                 _tr_regp16x16(),
                 _br_regp16x16(),
                 _tr_mm2(),
                 _br_mm2(),
                 _mm3(),
                 _mm5(),
                 _regpPxO(),
                 _dataPxO(),
                 _childrenPxO(),
                 _level(aLevel),
                 _nodeId(aGxTree.getNodeId()),
                 _brCode(0),
                 _hasBr(-1),
                 _anares(),
                 _myEncoding() {

  init(aData, aTr, aParent, aGxTree);

  assert(0 < cumFreq());
}

GxTree::Node::~Node() {
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        // std::cout << "delete node number " << child(i,j)->nodeId() << std::endl;
        delete _childrenPxO(i, j);
      }
    }
  }
}



GxTree::Node*
GxTree::Node::expandChild(const uint i, const uint j, const GxTree& aGxTree) {
  if(aGxTree.trace()) {
    std::cout << "ENTER: expandChild of " << nodeId() << '@' << level() << ": "
              << "   cumFreq = " << cumFreq() << ": "
              << '(' << i << ',' << j << "): "
              << std::endl;
  }


  rectangle_t lChildTile;
  regpPxO().mkRectangle(i, j, lChildTile);
  Node* lChildNode = new Node(dataPxO(i,j), lChildTile, level() + 1, this, aGxTree);

  _childrenPxO(i,j) = lChildNode; 
  if(aGxTree.trace()) {
    std::cout << "EXIT: expandChild of " << nodeId() << '@' << level() << ": "
              << "child(" << i << ',' << j << ") = "
              << lChildNode->nodeId() << '@' << lChildNode->level()
              << " (cumfreq = " << regpPxO(i,j)  << ')'
              << std::endl;
  }
  return lChildNode; 
}

void
GxTree::Node::init(const Data2dim& aData, const rectangle_t& aTr, 
                   const Node* aParent, const GxTree& aGxTree) {
  // set cumFreq
  _cumFreq = aData.total();

  // set bounding parent tile rectangle
  _tr = aTr; // must have happened in constructor

  // set bounding rectangle by data
  aData.getBoundingRectangle(_br);

  // _tr is the outer tile rectangle and is the input
  // for those datapoints falling into _tr, we calculate their true
  // bounding rectangle and store it in _br
  // then, we calculate a 16x16 discretization of the bounding
  // rectantle and store it in _brd;
  // a copy of it is stored in brdd (the br decided to be used,
  // it can either be _tr or _brd, depending on the node type used.
  // if the node type is not able to hold a brCode, _tr must be used.

  RegularPartitioning2dim lRegP16x16(tr(), 16, 16, aData);
  _tr_regp16x16.initFromData2dim(tr(), 16, 16, aData);
  _brCode = calcNeedsBr(_tr_regp16x16, &_tr, &_brd);
  _brdd = _brd;  // _brdd possibly changed later on, if br does not fit

  // eliminieren nach testen:
  if(0 != _brCode) {
    rectangle_t       lBrReconstructed;
    partitiondescxy_t lPd16x16(lRegP16x16.pdX(), lRegP16x16.pdY());
    reconstructBr(lBrReconstructed, lPd16x16);

    if(brd() != lBrReconstructed) {
      std::cout << "reconstructed BR differs from BR: "
                << brd() << " != " << lBrReconstructed
                << std::endl;
      std::cout.flush();
      assert(0 == 1);
    }
  }

  if(aGxTree.trace()) {
    std::cout << "INIT: cf = " << cumFreq() << std::endl 
              << "    tr: " << tr()   << std::endl
              << "    br: " << br()   << ", shrinkage = " << shrinkageBrTr() << std::endl
              << "   brd: " << brd()  << ", shrinkage = " << shrinkageBrdTr() << std::endl
              << "  brdd: " << brdd() << ", shrinkage = " << shrinkageBrdTr() << std::endl
              << "  tr 16x16:" << std::endl;
    _tr_regp16x16.print(std::cout);
  }


  // initialze _regp16x16 using the correct bounding rectangle
  
  if(needsBr()) {
    _br_regp16x16.initFromData2dim(brd(), 16, 16, aData);
    if(aGxTree.trace()) {
      std::cout << "brd 16x16:" << std::endl;
      br_regp16x16().print(std::cout);
    }
  } else {
    _br_regp16x16.initFromData2dim(tr(), 16, 16, aData); 
  }


  // analyze the situation, missing: distribution nodes
  analyze(_anares, aParent, aGxTree);

  // if some inner node kind cannot store brCode,
  // we must use tr for all future calculations for inner nodes.
  // things may differ again for leaf nodes
  _hasBr = needsBr() & anares().brDoesFit();

  if(needsBr() && (0 == hasBr())) {
    _brdd = tr();
  }

  if(aGxTree.trace()) {
    std::cout << "anares for " << nodeId() << '@' << level() 
              << ": " << nodeName(anares().nodeType()) 
              << ", " << anares().gridType() << 'x' << anares().gridType()
              << ", br fits: " << anares().brDoesFit()
              << ", hasbr: " << _hasBr
              << ", brdd = " << brdd()
              << std::endl;
  }



  if((0 != aParent) && (aParent->nodeType() > anares().nodeType())) {
    std::cout << "parent " << nodeName(aParent->nodeType()) 
              << " and child " << nodeName(anares().nodeType())
              << std::endl;
    if(GxTree::K_GLMS != aGxTree.kind()) {
      assert(aParent->nodeType() <= anares().nodeType());
    } else {
      _anares._leafEnforced = true;
    }
    // assert(aParent->nodeType() <= anares().nodeType()); // XXX
  }

  // initialize node depending on its type
  switch(anares().nodeType()) {
    case N_G: initG(aData, anares(), aGxTree);
              aGxTree.incNoG();
              break;
    case N_L: initL(aData, anares(), aGxTree);
              aGxTree.incNoL();
              break;
    case N_M: initM(aData, anares(), aGxTree);
              aGxTree.incNoM();
              break;
    case N_S: initS(aData, anares(), aGxTree);
              aGxTree.incNoS();
              break;
    default: std::cerr << "Fatal Error" << std::endl;
             assert(0 == 1);
  }

}


void
GxTree::Node::fill_mm2(      mm2_t&                   aMM2,
                       const RegularPartitioning2dim& aRegp16x16, 
                       const GxTree&                  aGxTree) {
  if(aMM2._filled) {
    return;
  }

  for(uint i = 0; i < 16; ++i) {
    for(uint j = 0; j < 16; ++j) {
      aMM2._16x16(i,j) = (uint) aRegp16x16(i,j);
    }
  }
  aMM2._8x8.setToShrink2Of(aMM2._16x16);
  aMM2._4x4.setToShrink2Of(aMM2._8x8);
  aMM2._2x2.setToShrink2Of(aMM2._4x4);
  aMM2._filled = true;

  if(false && aGxTree.trace()) {
    std::cout << "4x4:" << std::endl;
    aMM2._4x4.print(std::cout, 5);
    std::cout << std::endl;
  }
}

void
GxTree::Node::fill_mm3(mm3_t&             aMM3,
                       const rectangle_t& aRectangle,
                       const Data2dim&    aData, 
                       const GxTree&      aGxTree) {
  if(aMM3._filled) {
    return;
  }
  RegularPartitioning2dim lRegP12x12(aRectangle, 12, 12, aData);
  for(uint i = 0; i < 12; ++i) {
    for(uint j = 0; j < 12; ++j) {
      aMM3._12x12(i,j) = (uint) lRegP12x12(i,j);
    }
  }
  aMM3._6x6.setToShrink2Of(aMM3._12x12);
  aMM3._3x3.setToShrink2Of(aMM3._6x6);

  RegularPartitioning2dim lRegP9x9(aRectangle, 9, 9, aData);
  for(uint i = 0; i < 9; ++i) {
    for(uint j = 0; j < 9; ++j) {
      aMM3._9x9(i,j) = (uint) lRegP9x9(i,j);
    }
  }
  aMM3._filled = true;

  if(false && aGxTree.trace()) {
    std::cout << "3x3:" << std::endl;
    aMM3._3x3.print(std::cout, 5);
    std::cout << std::endl;
    std::cout << "9x9:" << std::endl;
    aMM3._9x9.print(std::cout, 5);
    std::cout << std::endl;
  }

}

void
GxTree::Node::fill_mm5(mm5_t&          aMM5,
                       const Data2dim& aData, 
                       const GxTree&   aGxTree) {
  if(aMM5._filled) {
    return;
  }

  RegularPartitioning2dim lRegP20x20(brdd(), 20, 20, aData);
  for(uint i = 0; i < 20; ++i) {
    for(uint j = 0; j < 20; ++j) {
      aMM5._20x20(i,j) = (uint) lRegP20x20(i,j);
    }
  }
  aMM5._10x10.setToShrink2Of(aMM5._20x20);
  aMM5._5x5.setToShrink2Of(aMM5._10x10);
  aMM5._filled = true;

  if(false && aGxTree.trace()) {
    std::cout << "5x5:" << std::endl;
    aMM5._5x5.print(std::cout, 5);
    std::cout << std::endl;
  }
}



bool
GxTree::Node::initG(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree) {
  if(5 == aAnaRes.gridType()) {
    prepare(aData, 5, 5);
  } else {
    assert(5 == aAnaRes.gridType());
  }


  if(aGxTree.trace()) {
    print(std::cout);
  }

  return true;
}

bool
GxTree::Node::initL(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree) {
  if(4 == aAnaRes.gridType()) {
    prepare(aData, 4, 4);
  } else {
    assert(4 == aAnaRes.gridType());
  }

  if(aGxTree.trace()) {
    print(std::cout);
  }

  return true;
}

bool
GxTree::Node::initM(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree) {
  if(3 == aAnaRes.gridType()) {
    prepare(aData, 3, 3);
  } else {
    assert(3 == aAnaRes.gridType());
  }

  if(aGxTree.trace()) {
    print(std::cout);
  }

  return true;
}


bool
GxTree::Node::initS(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree) {
  if(2 == aAnaRes.gridType()) {
    prepare(aData,2,2);
  } else {
    assert(2 == aAnaRes.gridType());
  }
  return true;
}


void
GxTree::Node::prepare(const Data2dim& aData, const uint aNx, const uint aNy) {
  _regpPxO.initFromData2dim(brdd(), aNx, aNy, aData);
 distributeData(aData, aNx, aNy, _dataPxO);
 _childrenPxO.resize(aNx, aNy);
 clearChildren();
}

void
GxTree::Node::distributeData(const Data2dim& aData, const uint aNx, const uint aNy, data2dim_at& aDataArr) {
  _dataPxO.resize(aNx, aNy);
  partitiondescxy_t  lPdXY(brdd(), aNx, aNy);
  for(uint i = 0; i < aData.size(); ++i) {
    const xyc_t& p = aData[i];
    const uint lIdxX = lPdXY.idxXcorrected(p.x);
    const uint lIdxY = lPdXY.idxYcorrected(p.y);
    if(lIdxX < aNx && lIdxY < aNy) {
      _dataPxO(lIdxX, lIdxY).push_back(p);
    }
  }
}




void
GxTree::Node::clearChildren() {
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      _childrenPxO(i,j) = 0;
    }
  }
}

// returns code of BR, == 0 if no br needed, i.e., tile doesn't shrink
uint32_t
GxTree::Node::calcNeedsBr(const RegularPartitioning2dim& aRegP16x16,
                          const rectangle_t* aTr,
                                rectangle_t* aBrNew) const {
  const uint32_t lNoZeroLoRows = aRegP16x16.matrix().noZeroLoRows();
  const uint32_t lNoZeroHiRows = aRegP16x16.matrix().noZeroHiRows();
  const uint32_t lNoZeroLoCols = aRegP16x16.matrix().noZeroLoCols();
  const uint32_t lNoZeroHiCols = aRegP16x16.matrix().noZeroHiCols();
  if((0 == lNoZeroLoRows) && (0 == lNoZeroHiRows) &&
     (0 == lNoZeroLoCols) && (0 == lNoZeroHiCols)) {
    if((0 != aTr) && (0 != aBrNew)) {
      *aBrNew = *aTr;
    }
    return (uint32_t) 0;
  }

  const uint32_t lBrCode = ((lNoZeroLoRows & 0xF) << 12) |
                           ((lNoZeroHiRows & 0xF) <<  8) |
                           ((lNoZeroLoCols & 0xF) <<  4) |
                           ((lNoZeroHiCols & 0xF) <<  0) ;

  if((0 != aTr) && (0 != aBrNew)) {
    reconstruct_br(*aBrNew, 0x1, lBrCode, *aTr);
  }
  return lBrCode;
}


void
GxTree::Node::reconstructBr(      rectangle_t&       aBrOut,
                            const partitiondescxy_t& aPd // for 16x16 tile partition
                           ) const {
  const uint32_t lBrCode = _brCode;
  if(0 != lBrCode) {
    reconstruct_br(aBrOut, 1, lBrCode, tr());
  } else {
    aBrOut = tr();
  }
}


void
GxTree::Node::insertChildrenIntoHeap(heap_t& aHeap, const GxTree& aGxTree) {
  if(GxTree::K_GLMS == aGxTree.kind() && _anares._leafEnforced) {
    return;
  }
  heapentry_t lHeapEntry;
  lHeapEntry._node = this;
  for(uint i = 0; i < nx(); ++i) {
    for(uint j = 0; j < ny(); ++j) {
      if(aGxTree.minimumNodeTotal() <= cumFreq(i,j)) {
        lHeapEntry._i = i;
        lHeapEntry._j = j;
        aHeap.push(lHeapEntry);
      }
    }
  }
}


double
GxTree::Node::estimate(const rectangle_t& aQueryRectangle, 
                       const rectangle_t& aTileRectangle, 
                       const GxTree&      aGxTree) const {

  // calculate the estimate produced by (*this)
  const double lEstimate = estimateSub(aQueryRectangle, 
                                       aTileRectangle,
                                       aGxTree);

  // compare lEstimate with the one produced from the encoding in GxtItp
  // this is always done if aGxTree._gxtitp is not null
  if(0 != aGxTree._gxtitp) {
    // o.k. can get estimate produced by GxtItp
    const uint   lMyIdx = _myEncoding.myIdx();
    const double lItpEstimate = aGxTree._gxtitp->estimate(aQueryRectangle,
                                                          aTileRectangle,
                                                          lMyIdx,
                                                          nodeType(),
                                                          level()
                                                          );
    const double lQError = q::qerrorT1<double>(lEstimate, lItpEstimate);
    const double lMaxQError = (GxTree::K_GLMS == aGxTree.kind() ? 4.0 : 2.0);
    if(lMaxQError < lQError && ((10 < lItpEstimate) || (10 < lEstimate))) {
      std::cout << std::endl;
      rectangle_t lIsec;
      lIsec.isec(aQueryRectangle, aTileRectangle);
      std::cout << "tree/itp estimate deviation at node "
                << nodeId() << '@' << level() 
                << " with idx = " << lMyIdx << ':' << std::endl
                << "           query = " << aQueryRectangle << std::endl
                << "            tile = " << aTileRectangle << std::endl
                << "            isec = " << lIsec << std::endl
                << "   tree estimate = " << lEstimate << std::endl
                << "   itp  estimate = " << lItpEstimate << std::endl
                << "   node type fine: " << _myEncoding._nodeType << std::endl
                << "type from generic: ";
      switch(nodeType()) {
        case N_G: {
                    const Node_G_Generic& lNode = aGxTree._gxtitp->encoding()._G[lMyIdx];
                    if(lNode.isLeaf()) {
                      std::cout << "G.Leaf." << lNode.getLeafSubkind() << std::endl;
                    } else {
                      std::cout << "G.Inner." << lNode.getInnerSubkind() << std::endl;
                    }
                  }
                  break;
        case N_L: {
                    const Node_L_Generic& lNode = aGxTree._gxtitp->encoding()._L[lMyIdx];
                    std::cout << lNode.determineNodeKindFine() << std::endl;
                  }
                  break;
        case N_M:
                  {
                    const Node_M_Generic& lNode = aGxTree._gxtitp->encoding()._M[lMyIdx];
                    std::cout << lNode.determineNodeKindFine() << std::endl;
                  }
                  break;
        case N_S:
                  {
                    const Node_S_Generic& lNode = aGxTree._gxtitp->encoding()._S[lMyIdx];
                    std::cout << lNode.determineNodeKindFine() << std::endl;
                  }
                  break;
        default: std::cerr << "unknown node type: " << nodeType() << std::endl;
      }
      std::cout << std::endl;
      std::cout << "query rectangle" << aQueryRectangle << std::endl
                << "run" << std::endl
                << "./main \"" << aQueryRectangle << '\"' << std::endl
                << "with          budget: " << aGxTree.budget() << std::endl
                << "     leaf refinement: " << aGxTree.leafRefinement() << std::endl
                ;
      std::cout << std::endl;

      std::cout << "BEGIN TRACE TREE ESTIMATE" << std::endl;
      const_cast<GxTree&>(aGxTree)._trace = true;
      estimateSub(aQueryRectangle, aTileRectangle, aGxTree);
      std::cout << "END TRACE TREE ESTIMATE" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;

      std::cout << "BEGIN TRACE ITP ESTIMATE" << std::endl;      
      const_cast<GxTreeItp*>(aGxTree._gxtitp)->trace(true);
      aGxTree._gxtitp->estimate(aQueryRectangle, aTileRectangle, _myEncoding.myIdx(), nodeType(), level());
      std::cout << "END TRACE ITP ESTIMATE" << std::endl;      
      std::cout << std::endl;
      std::cout << std::endl;

      assert(0 == 1);
    } else {
      if(aGxTree.trace()) {
        std::cout << "estimate at node "
                  << nodeId() << '@' << level() << " with idx = "
                  << lMyIdx << " is o.k: "
                  << "itp est = " << lItpEstimate << ", "
                  << "raw est = " << lEstimate
                  << std::endl;
      }
    }
  }

  return lEstimate;
}

double
GxTree::Node::estimateSub(const rectangle_t& aQueryRectangle, 
                          const rectangle_t& aBr, 
                          const GxTree&      aGxTree) const {
  if(aGxTree.trace()) {
    std::cout << std::string(2*level(), ' ')
              << "TE Node " << nodeId() 
              << ' ' << nodeType() << ' '
              << ", idx = " << _myEncoding.myIdx()
              << ", cf = " << cumFreq() 
              << ", brinfo = " << hasBr() << ' ' << needsBr()
              << ", brdd = " << brdd()
              << std::endl;
  }

  assert(0 <= _hasBr);

  rectangle_t lBr;
  if(1 == hasBr()) {
    lBr = brdd();
  } else {
    lBr = aBr;
  }

  if(aGxTree.trace()) {
    std::cout << std::string(2 * level(), ' ') 
              << "  TE Node " << nodeId()
              << "  brinfo = " << hasBr() << ' ' << needsBr() << ' ';
    printBrCode(std::cout, brCode()) << std::endl;
    std::cout << std::string(2 * level(), ' ') << "     tr = " << aBr << std::endl
              << std::string(2 * level(), ' ') << "     br = " << lBr << std::endl
              << std::string(2 * level(), ' ') << "     qr = " << aQueryRectangle << std::endl
              << std::endl;
  }

  if(aQueryRectangle.contains(lBr)) {
    if(aGxTree.trace()) {
      std::cout << std::string(2 * level(), ' ') 
                << "TE Node" << nodeId() << '@' << level() 
                << " returns " << cumFreq() << " (fully contained)." 
                << std::endl;
    }
    return cumFreq();
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, lBr);
  if(lIsec.hasZeroArea()) {
    if(aGxTree.trace()) {
      std::cout << std::string(2 * level(), ' ')
                << "TE Node" << nodeId() << '@' << level()
                << "   isec = " << lIsec
                << "  returns 0 (empty intersection)."  
                << std::endl;
    }
    return 0;
  }


  double lEstimate = 0;

  partitiondescxy_t lPd(lBr, nx(), ny());
  rectangle_t       lTile;
  rectangle_t       lTileIsec;

  if(isLeaf()) {
    const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
    const uint lMaxI = std::min<uint>(nx() - 1, lPd.idxXcorrected(lIsec.xhi()));
    const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
    const uint lMaxJ = std::min<uint>(ny() - 1, lPd.idxYcorrected(lIsec.yhi()));

    if(aGxTree.trace()) {
       std::cout << std::string(2 * level(), ' ')
                 << "TE Leaf:" << std::endl;
       std::cout << std::string(2 * level(), ' ')
                 << "    i-range: [" << lMinI << ", " << lMaxI << ']' << std::endl;
       std::cout << std::string(2 * level(), ' ')
                 << "    j-range: [" << lMinJ << ", " << lMaxJ << ']' << std::endl;
    }
    // for(uint i = 0; i < nx(); ++i) 
      // for(uint j = 0; j < ny(); ++j) 
   
    if(aGxTree.trace()) {
      std::cout << std::string(2 * level(), ' ')
                << "     i  j   tf   TijEst  node" << std::endl;
    } 
    for(uint i = lMinI; i <= lMaxI; ++i) {
      for(uint j = lMinJ; j <= lMaxJ; ++j) {
        lPd.getRectangle(i, j, lTile);
        lTileIsec.isec(lIsec, lTile);
        if(!lTileIsec.hasZeroArea()) {
          const double lTileEstimate = ((lTileIsec.area() / lTile.area()) * cumFreq(i, j));
          lEstimate += lTileEstimate;
          if(aGxTree.trace()) {
            std::cout << std::string(2 * level(), ' ')
                      << "    " 
                      << std::setw(2) << i << ' '
                      << std::setw(2) << j << ' '
                      << std::setw(4) << cumFreq(i, j) << ' '
                      << std::setw(8) << lTileEstimate
                      << "   " << nodeId() << '@' << level() 
                      << std::endl;
          }
        }
      }
    }
  } else {
    // in this case nx == cnx
    const uint lMinI = lPd.idxXcorrected(lIsec.xlo());
    const uint lMaxI = std::min<uint>(nx() - 1, lPd.idxXcorrected(lIsec.xhi()));
    const uint lMinJ = lPd.idxYcorrected(lIsec.ylo());
    const uint lMaxJ = std::min<uint>(ny() - 1, lPd.idxYcorrected(lIsec.yhi()));

    if(aGxTree.trace()) {
       std::cout << std::string(2 * level(), ' ')
                 << "   TE Inner:" << std::endl;
       std::cout << std::string(2 * level(), ' ')
                 << "   i-range: [" << lMinI << ", " << lMaxI << ']' << std::endl;
       std::cout << std::string(2 * level(), ' ')
                 << "   j-range: [" << lMinJ << ", " << lMaxJ << ']' << std::endl;
    }
    // for(uint i = 0; i < cnx(); ++i)
      // for(uint j = 0; j < cny(); ++j)
    if(aGxTree.trace()) {
      std::cout << std::string(2 * level(), ' ')
                << "     i  j   tf  TijEst    node " << std::endl;
    } 
    for(uint i = lMinI; i <= lMaxI; ++i) {
      for(uint j = lMinJ; j <= lMaxJ; ++j) {
        lPd.getRectangle(i, j, lTile);
        if(0 != child(i, j)) {
          if(aGxTree.trace()) {
            std::cout << std::string(2 * level(), ' ')
                      << "    "
                      << std::setw(2) << i << ' '
                      << std::setw(2) << j << ' '
                      << "                " << child(i,j)->nodeId() 
                      << '@' << child(i,j)->level()
                      << "    child call  lTile = " << lTile
                      << std::endl;
          }
          lEstimate += child(i, j)->estimate(lIsec, lTile, aGxTree);
        } else {
          lTileIsec.isec(lIsec, lTile);
          if(!lTileIsec.hasZeroArea()) {
            const double lTileEstimate = ((lTileIsec.area() / lTile.area()) * cumFreq(i, j));
            lEstimate += lTileEstimate;
            if(aGxTree.trace()) {
              std::cout << std::string(2 * level(), ' ')
                        << "    "
                        << std::setw(2) << i << ' '
                        << std::setw(2) << j << ' '
                        << std::setw(4) << cumFreq(i, j) << ' '
                        << std::setw(8) << lTileEstimate
                        << "   " << nodeId() << '@' << level() 
                        << std::endl;
            }
          }
        }
      }
    }
  }

  if(aGxTree.trace()) {
    std::cout <<  std::string(2*level(), ' ') 
              << "TE estimate[" << nodeId()
              << "]: " << lEstimate << std::endl;
  }

  return lEstimate;
}


uint
GxTree::Node::depth() const {
  if(isLeaf()) {
    return 1;
  }
  uint lChildDepth = 0;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        const uint d = child(i, j)->depth();
        if(d > lChildDepth) {
          lChildDepth = d;
        }
      }
    }
  }
  return 1 + lChildDepth;
}

uint
GxTree::Node::noNodes() const {
  uint lRes = 1; // this
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noNodes();
      }
    }
  } 
  return lRes;
}

uint
GxTree::Node::noLeafNodes() const {
  if(isLeaf()) {
    return 1;
  }
  uint lRes = 0;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        lRes += child(i,j)->noLeafNodes();
      }
    }
  }
  return lRes;
}

uint
GxTree::Node::noInnerNodes() const {
  if(isLeaf()) {
    return 0;
  }
  uint lRes = 1;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        lRes += child(i,j)->noInnerNodes();
      }
    }
  }
  return lRes;
}

uint
GxTree::Node::noGNodes() const {
  uint lRes = (N_G == nodeType() ? 1 : 0); // this
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noGNodes();
      }
    }
  }
  return lRes;
}

uint
GxTree::Node::noLNodes() const {
  uint lRes = (N_L == nodeType() ? 0 : 1); // this
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noLNodes();
      }
    }
  }
  return lRes;
}


bool
GxTree::Node::isLeaf() const {
  if(0 == cnx() || 0 == cny()) {
    return true;
  }
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        return false;
      }
    }
  }
  return true;
}

uint
GxTree::Node::noChildren() const {
  if(0 == cnx() || 0 == cny()) {
    return 0;
  }
  uint lRes = 0;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        ++lRes;
      }
    }
  }
  return lRes;
}

uint
GxTree::Node::calcSize() const {
  uint lRes = size();
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->calcSize();
      }
    }
  }

  return lRes;
}

double
GxTree::Node::minLeafCumFreq() const {
  if(isLeaf()) {
    return cumFreq();
  }

  double lRes = std::numeric_limits<double>::max();
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        const double cf = child(i,j)->minLeafCumFreq();
        if(cf < lRes) {
          lRes = cf;
        }
      }
    }
  }
  return lRes;
}


double
GxTree::Node::maxLeafCumFreq() const {
  if(isLeaf()) {
    return cumFreq();
  }

  double lRes = 0;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        const double cf = child(i,j)->maxLeafCumFreq();
        if(cf > lRes) {
          lRes = cf;
        }
      }
    }
  }
  return lRes;
}


/*
 * analyze
 * goal: determine node type G, L, M or S
 * thereby determine whether BR can (safely/always) be stored or not 
 * and also store the size of the grid
 * none of the _mm_X should have been set up to this point
 * _tr_mm2 and _br_mm2 exist for BR/TR in any case
 * _mm3 and _mm5 should always be on brdd()
 * if some code fragment alters brdd() or changes _mm3 to something not in brdd(),
 * then it must also clear it afterwards
 *
 * in some rare cases, this may fail.
 * 
 */

void
GxTree::Node::analyze(anares_t& aAnaRes, const Node* aParent, const GxTree& aGxTree) {
  const bool lTrace  = aGxTree.trace();
  const uint lTotal  = cumFreq();

  q::ScaleMgr* lScaleMgr = q::ScaleMgr::instance();
  const uint lLim2 = lScaleMgr->scale_S_2_2()->limit(); //    84
  const uint lLim3 = lScaleMgr->scale_S_3_3()->limit(); //   231
  const uint lLim4 = lScaleMgr->scale_S_4_2()->limit(); //   497
  const uint lLim5 = lScaleMgr->scale_S_5_2()->limit(); //   18904 // original

// FOR TESTING ONLY
  // const uint lLim5 =  200;  // for testing only XXX
  // const uint lLim5 =  500;  // for testing only XXX
  // const uint lLim5 = 1000;  // for testing only XXX
  // const uint lLim5 = 2000;  // for testing only XXX
  // const uint lLim5 = 3000;  // for testing only XXX
  // const uint lLim5 = 5000;  // for testing only XXX
  // const uint lLim5 = 7000;  // for testing only XXX


  if(lTrace) {
    std::cout << "Node::analyze: " << nodeId() << '@' << level();
    if(0 != aParent) {
      std::cout << " with parent " << aParent->nodeId() << '@' << aParent->level() 
                << std::endl;
    } else {
      std::cout << " with no parent (is root)" << std::endl;
    }
    std::cout << "    total = " << lTotal << std::endl;
    std::cout << "    lLim2 = " << std::setw(5) << lLim2 << std::endl;
    std::cout << "    lLim3 = " << std::setw(5) << lLim3 << std::endl;
    std::cout << "    lLim4 = " << std::setw(5) << lLim4 << std::endl;
    std::cout << "    lLim5 = " << std::setw(5) << lLim5 << std::endl;
  }


  // the next 6 lines can go at the beginning if tracing is no longer wanted

  aAnaRes._nodeType = N_G;
  aAnaRes._gridType = 5;
  aAnaRes._brDoesFit = true;

  if(K_Gxxx == aGxTree.gxKind()) {
    if(lTrace) {
      std::cout << "Node::analyze returns '" << nodeName(aAnaRes._nodeType) << "'." << std::endl;
    }
    return;
  }

  // 2. can reduce size to L?

  // 2.1 total too big?
  if(lTotal > lLim5) {
    if(lTrace) {
      std::cout << "Node::analyze returns '" << nodeName(aAnaRes._nodeType) << "'." << std::endl;
    }
    return;
  }

  // 2.2 observe that for inner nodes 5 fBits are present
  // thus, we don't have to check anything here

  // 2.3 tile frequencies,
  // needs subcases for w/o BR

  // large enough to go for Node_L
  aAnaRes._nodeType = N_L;
  aAnaRes._gridType = 4;
  if(needsBr()) {
    fill_mm2(_br_mm2, _br_regp16x16, aGxTree);
    const uint lNoGtLim_LIA1 = _br_mm2._4x4.noGt(Node_L_Generic::scaleGrid(
                                                      Node_L_Generic::Inner_A1)->limit());
    if(K_GLxx == aGxTree.gxKind()) {
      aAnaRes._brDoesFit = (1 >= lNoGtLim_LIA1);
    } else {
      const uint lNoGtLim_LIB1 = _br_mm2._4x4.noGt(Node_L_Generic::scaleGrid(
                                                        Node_L_Generic::Inner_B1)->limit());
      if(K_GLMx == aGxTree.gxKind()) {
        aAnaRes._brDoesFit = (1 >= lNoGtLim_LIA1) &
                             (2 >= lNoGtLim_LIB1);
      } else {
       // K_GLMS == aGxTree.gxKind()
        const uint lNoGtLim_LIC1 = _br_mm2._4x4.noGt(lLim4);
        // 1. Fall: 3 ptr
        // 2. Fall: 2 ptr, nur zu M und S
        // const uint lNoGtLimTotM = _br_mm2._4x4.noGt(Node_M_Generic::_scaleTotalS->limit());
        aAnaRes._brDoesFit = ((1 >= lNoGtLim_LIA1) &
                              (2 >= lNoGtLim_LIB1) &
                              (3 >= lNoGtLim_LIC1));
      }
    }
  } else {
    aAnaRes._brDoesFit = true; // doesn't matter anyway
  }

  // 3. can (possibly) reduce size to M? if not return
  if(K_GLxx == aGxTree.gxKind() || lTotal > lLim4) {
    if(lTrace) {
      std::cout << "Node::analyze after L returns '" << nodeName(aAnaRes._nodeType) 
                << "'." << std::endl;
    }
    return;
  }



  // lTotal <= lLim4
  // o.k. total can be encoded in 4 bits
  // now make sure that all tile frequencies but at most one can be encoded in 3 bits
  // cannot always be applied, even if lTotal <= lLim4

  // if two pointers are a possibility, no BR can be stored
  if(needsBr() && (K_GLMx == aGxTree.gxKind())) {
    fill_mm3(_mm3, brd(), data(), aGxTree);
    const uint lNoGtLim2_3x3 = _mm3._3x3.noGt(Node_M_Generic::scaleGrid(0,
                                                   Node_M_Generic::Inner_A1)->limit());
    if((1 >= lNoGtLim2_3x3)) {
      if(aGxTree.trace()) {
        std::cout << "analyze: case M.1" << std::endl;
      }
      aAnaRes._nodeType  = N_M;
      aAnaRes._gridType  = 3;
      aAnaRes._brDoesFit = true;
    } else {
      _mm3._filled = false;
      fill_mm3(_mm3, tr(), data(), aGxTree);
      const uint lNoGtLim3_3x3 = _mm3._3x3.noGt(lLim3);
      const uint lNoGtLim3_9x9 = _mm3._9x9.noGt(lLim3); // see comment below
      if(1 >= lNoGtLim3_3x3 && (1 >= lNoGtLim3_9x9)) {
      if(aGxTree.trace()) {
        std::cout << "analyze: case M.2" << std::endl;
      }
        aAnaRes._nodeType  = N_M;
        aAnaRes._gridType  = 3;
        aAnaRes._brDoesFit = false;
      }
    }
  } else {
    fill_mm3(_mm3, tr(), data(), aGxTree);
    if(K_GLMx == aGxTree.gxKind()) {
      const uint lNoGtLim3_3x3   = _mm3._3x3.noGt(lLim3);   // for M without BR
      const uint lNoGtLim3_9x9   = _mm3._9x9.noGt(lLim3);   // for M without BR
      // the second condition is necessary since
      // 240 > lLim3 (= 231) and 240 + 240 = 480 < lLim4 (= 497)
      // thus, we have to make sure that children's children can be encoded as
      // M-Nodes
      if((1 >= lNoGtLim3_3x3) && (1 >= lNoGtLim3_9x9)) {
        if(aGxTree.trace()) {
          std::cout << "analyze: case M.3" << std::endl;
        }
        aAnaRes._nodeType  = N_M;
        aAnaRes._gridType  = 3;
        aAnaRes._brDoesFit = false; 
      }
    } else {
      // must check that things can be encoded in INNER_B2
      const uint lLimIB2 = Node_M_Generic::scaleGrid(0, Node_M_Generic::Inner_B2)->limit();
      if(aGxTree.trace()) {
        std::cout << "analyze: M.I.B2: tile limit: " << lLimIB2 << std::endl;
      }
      const uint lNoGtLimIB2_3x3 = _mm3._3x3.noGt(lLimIB2);
      if(aGxTree.trace()) {
        std::cout << "analyze: M.I.B2: 3x3: no gt limit: " << lNoGtLimIB2_3x3 << std::endl;
      }
      
      if(1 >= lNoGtLimIB2_3x3) {
        const uint lNoGtLimIB2_9x9 = _mm3._9x9.noGt(lLimIB2);
        if(aGxTree.trace()) {
          std::cout << "analyze: M.I.B2: 9x9: no gt limit: " << lNoGtLimIB2_9x9 << std::endl;
        }
        if(1 > lNoGtLimIB2_9x9) {  // VORHER: 1 >= lNoGtIB2_9x9 ; 21.01.2016
          if(aGxTree.trace()) {
            std::cout << "analyze: case M.4" << std::endl;
          }
          aAnaRes._nodeType  = N_M;
          aAnaRes._gridType  = 3;
          aAnaRes._brDoesFit = false; 
        }
      }
    }
  }

  if(N_L == aAnaRes._nodeType && N_M == aParent->nodeType()) {
    std::cout << "analyze: Failed to refine (parent M, child L) " << std::endl
              << "_mm3:" << std::endl;
    _mm3._3x3.print(std::cout, 4);
    _mm3._6x6.print(std::cout, 4);
    _mm3._9x9.print(std::cout, 4);
    std::cout << "parent: " << aParent->nodeId() << std::endl
              << " child: " << nodeId() << std::endl;
  }

  // is there any chance to use an S-Node?
  // a) still L: definitely can't reduce to S.
  // b) GLMx: not allowed to use S
  // c) total too large, can't reduce to S
  if(   (N_L == aAnaRes._nodeType)
     || (K_GLMx == aGxTree.gxKind())
     || (lTotal > lLim3)) {
    if(lTrace) {
      std::cout << "Node::analyze after M returns '" << nodeName(aAnaRes._nodeType) << "'." << std::endl;
    }
    return;
  }


  // 4. can reduce to size S?
  // NO S so far

  // here: lTotal <= lLim3
  // can never store BR (in inner node)
  {
    fill_mm2(_tr_mm2, _tr_regp16x16, aGxTree);
    const uint lNoGtLim2 = _tr_mm2._2x2.noGt(Node_S_Generic::scaleGrid(
                                               Node_S_Generic::Inner_A2)->limit());
    const uint lMaxTF4x4 = _tr_mm2._4x4.max();
    const uint lMaxTF2x2 = _tr_mm2._2x2.max();

    if(lTrace) {
      std::cout << "analyze: " << nodeId() << '@' << level() << std::endl;
      _tr_mm2._2x2.print(std::cout, 4);
      _tr_mm2._4x4.print(std::cout, 4);
    }

    if(   (1 >= lNoGtLim2)
       && (lMaxTF4x4 <= Node_S_Generic::scaleGrid(Node_S_Generic::Inner_A2)->limit())
       && (lMaxTF2x2 <= Node_S_Generic::scaleGrid(Node_S_Generic::Leaf_A1)->limit())) {
      aAnaRes._nodeType  = N_S;
      aAnaRes._gridType  = 2;
      aAnaRes._brDoesFit = false; 
    }
  }

  // fin
  if(lTrace) {
    std::cout << "Node::analyze returns '" << nodeName(aAnaRes._nodeType) << "'." << std::endl;
  }

}

uint
GxTree::Node::maxUnsplitTileFrequency() const {
  uint lRes = 0;
  for(uint i = 0; i < nx(); ++i) {
    for(uint j = 0; j < ny(); ++j) {
      if(isUnsplit(i,j)) {
        if(lRes < regpPxO(i,j)) {
          lRes = regpPxO(i,j);
        }
      }
    }
  }
  return lRes;
}


std::ostream&
GxTree::Node::print(std::ostream& os) const {
  const std::string lIndent(2 * level(), ' ');

  os << lIndent
     << Gxt::nodeName(nodeType())
     << "-Node " <<  nodeId() << '@' << level() << "   "
                << cnx() << 'x' << cny() 
                << "   sz " << size()
                << "   cf " << cumFreq() 
                << "   br " << hasBr()  << needsBr() << ' ';
  printBrCode(os, brCode());
  os            << "   mustf " << maxUnsplitTileFrequency()
                << "   idx " << _myEncoding.myIdx()
                << std::endl
     << lIndent << "   tr  " << tr() << std::endl
     << lIndent << "   br  " << br() << " shrinkage: " << shrinkageBrTr() << std::endl
     << lIndent << "  brd  " << brd() << " shrinkage: " << shrinkageBrdTr() << std::endl
     << lIndent << "  brdd " << brdd() << std::endl
     ;
  printThePxO(os);
  os << "  children: ";
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        os << "  (" << i << ',' << j << "):" << child(i,j)->nodeId()
           << ':' << child(i,j)->nodeType();
      }
    }
  }
  os << std::endl;

  const bool lWithPartitionings = false;
  if(lWithPartitionings) {
    RegularPartitioning2dim lRegPXX(brd(), 60, 60, data());

    os << "2x2:" << std::endl;
    printPxO(os, 2, 2, &lRegPXX);

    os << "4x4:" << std::endl;
    printPxO(os, 4, 4, &lRegPXX);

    os << "8x8:" << std::endl;
    printPxO(os, 8, 8, &lRegPXX);

    os << "16x16:" << std::endl;
    printPxO(os, 16, 16, &lRegPXX);

    os << "32x32:" << std::endl;
    printPxO(os, 32, 32, &lRegPXX);

    os << "3x3:" << std::endl;
    printPxO(os, 3, 3, &lRegPXX);

    os << "6x6:" << std::endl;
    printPxO(os, 6, 6, &lRegPXX);

    os << "12x12:" << std::endl;
    printPxO(os, 12, 12, &lRegPXX);

    os << "5x5:" << std::endl;
    printPxO(os, 5, 5, &lRegPXX);

    os << "10x10:" << std::endl;
    printPxO(os, 10, 10, &lRegPXX);

    os << "20x20:" << std::endl;
    printPxO(os, 20, 20, &lRegPXX);

    os << "40x40:" << std::endl;
    printPxO(os, 40, 40, &lRegPXX);
  }

  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i, j)) {
        child(i, j)->print(os);
      }
    }
  }
  return os;
}

void
GxTree::Node::printPxO(std::ostream& os, const uint aNx, const uint aNy, const RegularPartitioning2dim* aRegP) const {
  RegularPartitioning2dim lRegP(brd(), aNx, aNy, data());
  lRegP.matrix().print(os, 4);
  if(0 != aRegP) {
    std::cout << "QERROR: " << maxQError(*aRegP, lRegP) << std::endl;
  }
}

void
GxTree::Node::printThePxO(std::ostream& os) const {
  std::string lIndent(2 * level(), ' ');
  for(uint i = 0; i < nx(); ++i) {
    os << lIndent;
    for(uint j = 0; j < ny(); ++j) {
      os << std::setw(5) << regpPxO(i,j) << ' ';
    }
    os << std::endl;
  }
}



double
GxTree::Node::maxQError(const RegularPartitioning2dim& aTru,
                          const RegularPartitioning2dim& aEst) const {
  double lRes = 0;

  rectangle_t lQr; // query rectangle is tile of aTru
  double lQError = 0;
  for(uint i = 0; i < aTru.nx(); ++i) {
    for(uint j = 0; j < aTru.ny(); ++j) {
      aTru.mkRectangle(i,j, lQr);
      const double lEst = std::max<double>(1, aEst.estimate(lQr));
      const double lTru = std::max<double>(1, aTru(i,j));
      if(lEst > 10 || lTru > 10) {
        lQError = q::qerrorT1<double>(lEst, lTru);
      }
      if(lQError > lRes) {
        lRes = lQError;
      }
    }
  }
  return lRes;
}

double
GxTree::Node::qerrOfAvg(const uint_at& aMatrix) const {
  uint lSum = 0;
  uint lMin = std::numeric_limits<uint>::max();
  uint lMax = 0;
  for(uint i = 0; i < aMatrix.noRows(); ++i) {
    for(uint j = 0; j < aMatrix.noCols(); ++j) {
      const uint x = aMatrix(i,j);
      lSum += x;
      if(x < lMin) {
        lMin = x;
      }
      if(x > lMax) {
        lMax = x;
      }
    }
  }

  if(0 == lMin) {
    lMin = 1;
  }
  const double lAvg = (double) lSum / ((double) (aMatrix.noRows() * aMatrix.noCols()));

  const bool lTrace = false;
  if(lTrace) {
    std::cout << "min of " << aMatrix.noCols() << 'x' << aMatrix.noRows() << " = " << lMin << std::endl;
    std::cout << "max of " << aMatrix.noCols() << 'x' << aMatrix.noRows() << " = " << lMax << std::endl;
    std::cout << "avg of " << aMatrix.noCols() << 'x' << aMatrix.noRows() << " = " << lAvg << std::endl;
  }

  return std::max<double>((double) lMax / lAvg, lAvg / (double) lMin);
}

double
GxTree::Node::qerrIncrease() const {
  double lRes = -1.7;
  const double lQErr16x16 = qerrOfAvg(hasBr() ? _br_mm2._16x16 : _tr_mm2._16x16);
  const double lQErr20x20 = qerrOfAvg(_mm5._20x20);

  const bool lTrace = false;
  if(lTrace) {
    std::cout << "qerror from avg 16x16 = " << lQErr16x16 << std::endl;
    std::cout << "qerror from avg 20x20 = " << lQErr20x20 << std::endl;
  }
  return lRes;
}


std::ostream&
GxTree::Node::printDot(std::ostream& os, const int aParentId) const {
  // print current node
  os << 'n' << nodeId() << " [label=\""
     << nodeId() << ":" << cumFreq() << ":" << hasBr()
     << "\"];"
     << std::endl;

  // print edge to parent
  if(0 <= aParentId) {
    os << 'n' << aParentId << " -> n" << nodeId() << ';' << std::endl;
  }

  // print subtrees

  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        child(i,j)->printDot(os, nodeId());
      }
    }
  }

  return os;

}

/*
 *   GxTree::node_encoding_t members
 */

uint
GxTree::node_encoding_t::noChildPtr() const {
  uint lRes = 0;
  for(uint i = 0; i < 4; ++i) {
    lRes += (0 < _childCount[i]);
  }
  return lRes;
}


/* 
 *   GxTree members 
 */

GxTree::GxTree(const Data2dim& aData,
               const gx_kind_t aGxKind,
               const uint      aBudget,
               const int       aLeafRefinement,
               const double    aLrf,
               const uint      aMinimumNodeTotal,
               const uint      aPhi,
               const double    aQ,
               const double    aTheta,
               const bool      aTrace) : EstimatorBase2dim(aQ, aTheta),
                                         _outlier(),
                                         _br(),
                                         _gxKind(aGxKind),
                                         _budget(aBudget),
                                         _leafRefinement(aLeafRefinement),
                                         _lrf(aLrf),
                                         _minimumNodeTotal(10),
                                         _size(0),
                                         _phi(aPhi),
                                         _root(0),
                                         _nodeCount(0),
                                         _nodeCountG(0),
                                         _nodeCountL(0),
                                         _nodeCountM(0),
                                         _nodeCountS(0),
                                         _trace(aTrace),
                                         _minSplit(0),
                                         _maxUnsplit(0),
                                         _encoding(),
                                         _gxtitp(0) {
  if(trace()) {
    std::cout << "Building Gx-Tree with parameters: " << std::endl;
    printParameters(std::cout);
  }

  // build the tree
  init(aData);

  // first test [only for longlat.dat]
  if(trace()) {
    std::cout << std::endl;
    std::cout << "=== first estimate ===" << std::endl;
    rectangle_t lQueryRectangle(80.0, 30.0, 120.0, 40.0);
    double lTrueVal = aData.countWithin(lQueryRectangle);
    double lEstVal  = estimate(lQueryRectangle);
    std::cout << "query rectangle: " << lQueryRectangle << std::endl;
    std::cout << "true val: " << lTrueVal << std::endl;
    std::cout << "est  val: " << lEstVal  << std::endl;
    std::cout << "=== first estimate ===" << std::endl;
    std::cout << std::endl;
  }

}

GxTree::~GxTree() {
  if(0 != _root) {
    delete _root;
  }
}


void
GxTree::init(const Data2dim& aData) {
  Data2dim lRegular;
  if(8 < phi()) {
    aData.split(lRegular, _outlier, phi());
  } else {
    lRegular = aData;
  }

  lRegular.getBoundingRectangle(_br);

  heap_t lHeap;
  _root = new Node(aData, br(), 0, 0, (*this));
  _root->insertChildrenIntoHeap(lHeap, (*this));

  uint lTotalSize = root()->size();

  if(trace()) {
    std::cout << "before main loop: heap size: " << lHeap.size() << std::endl;
  }
  while((budget() > lTotalSize) && 
        (!lHeap.empty())        &&
        (65500 > _nodeCountG)   &&
        (65500 > _nodeCountL)   && 
        (65500 > _nodeCountM)   &&
        (65500 > _nodeCountS) ) {
    const heapentry_t& lTop = lHeap.top();
    lHeap.pop();
    _minSplit = lTop._node->cumFreq(lTop._i, lTop._j);
    Node* lChildNode = lTop._node->expandChild(lTop._i, lTop._j, (*this));
    lTotalSize += lChildNode->size();
    lChildNode->insertChildrenIntoHeap(lHeap, (*this));
    if(trace()) {
      std::cout << "within main loop: total size = " << lTotalSize << " <?> " << budget() << " = budget" << std::endl;
    }
  }
  if(!lHeap.empty()) {
    const heapentry_t& lTop = lHeap.top();
    _maxUnsplit = lTop._node->cumFreq(lTop._i, lTop._j);
    if(trace()) {
      std::cout << "final top's cumFreq: " << maxUnsplit() << std::endl;
    }
  } else {
    if(trace()) {
      std::cout << "heap is empty" << std::endl;
    }
  }

  _size = lTotalSize;

}





double
GxTree::estimate(const rectangle_t& r) const {
  double lEstimate = 0;
  if(trace()) {
    std::cout << "NEW ESTIMATE for query rectangle = " << r << std::endl;
  }
  if(0 != root()) {
    lEstimate = root()->estimate(r, br(), (*this));
  }
  lEstimate += (double) outlierCount(r);
  return std::max<double>(1.0, lEstimate);
}




double
GxTree::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}



uint
GxTree::outlierCount(const rectangle_t& r) const {
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(r.containsHalfOpen(p.x, p.y)) {
      lRes += p.c;
    }
  }
  return lRes;
}


uint
GxTree::size() const {
  return _size;
}

uint
GxTree::calcSize() const {
  if(0 == root()) {
    return 0;
  }
  return root()->calcSize();
}

uint
GxTree::depth() const {
  if(0 == root()) {
    return 0;
  }
  return root()->depth();
}

uint
GxTree::noNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noNodes();
}

uint
GxTree::noLeafNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noLeafNodes();
}

uint
GxTree::noInnerNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noInnerNodes();
}

uint
GxTree::noGNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noGNodes();
}

uint
GxTree::noLNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noLNodes();
}

double
GxTree::minLeafCumFreq() const {
  if(0 == root()) {
    return -1;
  }
  return root()->minLeafCumFreq();
}

double
GxTree::maxLeafCumFreq() const {
  if(0 == root()) {
    return -1;
  }
  return root()->maxLeafCumFreq();
}


std::ostream&
GxTree::printParameters(std::ostream& os) const {
  std::cout << "            budget: " << std::setw(4) << budget() << std::endl
            << "   gx_kind_t kind : " << kind() << std::endl
            << "   leaf refinement: " << leafRefinement() << std::endl
            ;
  return os;
}



std::ostream&
GxTree::print(std::ostream& os) const {
  std::cout << "GxTree_" << gxKind() << ": " << std::endl;
  if(0 != root()) {
    _root->print(os);
  }
  return os;
}


std::ostream&
GxTree::printDot(std::ostream& os) const {
  os << "digraph GxTree {" << std::endl
     << "node [shape=box,fontsize=12,height=.4,width=.8,fixedsize=true]" << std::endl
     << "edge [arrowhead=none]"
     << std::endl;
  if(0 != root()) { 
    root()->printDot(os, -1);
  }
  os << '}' << std::endl;
  return os;
}



/*
 * anares_t
 */

std::ostream& 
GxTree::anares_t::print(std::ostream& os) const {
  os << Gxt::nodeName(nodeType()) 
     << '_' << gridType() << 'x' << gridType()
     << "BR(" << _brDoesFit<< ')';
  return os;
}

std::ostream&
operator<<(std::ostream& os, const GxTree::anares_t& x) {
  return x.print(os);
}


/*
 *  mm2_t, mm3_t, and mm5_t
 */


std::ostream&
GxTree::mm2_t::print(std::ostream& os) const {
  _2x2.print(os, 5);
  _4x4.print(os, 5);
  _8x8.print(os, 5);
  _16x16.print(os, 5);
  return os;

}

std::ostream&
GxTree::mm3_t::print(std::ostream& os) const {
  _3x3.print(os, 5);
  _6x6.print(os, 5);
  _12x12.print(os, 5);
  return os;

}

std::ostream&
GxTree::mm5_t::print(std::ostream& os) const {
  _5x5.print(os, 5);
  _10x10.print(os, 5);
  _20x20.print(os, 5);
  return os;

}


/*
 *    GxTree encoding related member functions
 */

void
GxTree::encode(const bool aCheckEncoding) {
  encode_arg_t lEncArg;
  // _childCount[x] is always the first non-allocated node of type x
  lEncArg._childCount[0] = 0;
  lEncArg._childCount[1] = 0;
  lEncArg._childCount[2] = 0;
  lEncArg._childCount[3] = 0;

  assert(0 != root());

  // 1) prepare encoding
  lEncArg._childCount[root()->nodeType()] = 1; // root needs some space 
  _root->prepareEncoding(lEncArg, 0, (*this)); // _root node always has index zero

  // 2) initialize _encoding struct
  _encoding._rootType = root()->nodeType();
  _encoding._topBr = br();
  _encoding._no[N_G] = lEncArg._childCount[N_G];
  _encoding._no[N_L] = lEncArg._childCount[N_L];
  _encoding._no[N_M] = lEncArg._childCount[N_M];
  _encoding._no[N_S] = lEncArg._childCount[N_S];
  _encoding._gxKind  = gxKind(); 
  if(trace()) {
    std::cout << "GxTree::encode after prepare: "
              << "#G = " << lEncArg._childCount[N_G] << ", "
              << "#L = " << lEncArg._childCount[N_L] << ", "
              << "#M = " << lEncArg._childCount[N_M] << ", "
              << "#S = " << lEncArg._childCount[N_S]
              << std::endl;
  } 
  {
    const uint lNoG = lEncArg._childCount[N_G];
    if(0 != lNoG) {
      _encoding._G = new Gxt::Node_G_Generic[lNoG];
    } else {
      _encoding._G = 0;
    }
  }

  {
    const uint lNoL = lEncArg._childCount[N_L];
    if(0 != lNoL) {
      _encoding._L = new Gxt::Node_L_Generic[lNoL];
    } else {
      _encoding._L = 0;
    }
  }

  {
    const uint lNoM = lEncArg._childCount[N_M];
    if(0 != lNoM) {
      _encoding._M = new Gxt::Node_M_Generic[lNoM];
    } else {
      _encoding._M = 0;
    }
  }

  {
    const uint lNoS = lEncArg._childCount[N_S];
    if(0 != lNoS) {
      _encoding._S = new Gxt::Node_S_Generic[lNoS];
    } else {
      _encoding._S = 0;
    }
  }

  // 3) perform encoding 

  _root->performEncoding(_encoding, aCheckEncoding, (*this));

  // 4) print encoding info
  if(trace()) {
    std::cout << std::endl;
    std::cout << "after performEncoding: headers of Nodes:" << std::endl;
    std::cout << std::setw(3) << "id" << "  "
              << std::setw(3) << "idx" << "  "
              << std::setw(7) << "BR Code" << "  "
              << std::setw(8) << "header" << "  "
              << "node type"
              << std::endl;
    _root->printEncodingInfo(std::cout, _encoding);
  }
}

void
GxTree::Node::prepareEncoding(encode_arg_t& aArg, 
                              const uint    aMyIdx, 
                              const GxTree& aGxTree) {

   // 0. store my index (there i'll put my node encoding)
   // aMyIdx must therefore be the absolute index 
   // into the according node array (_G, _L, _M, _S of encoding_t)
   // it is 0 for the root
   // otherwise is must be calculated as the sum of the index of the start
   // of the subarray for the children of a specific type plus the offset therein
   // the former ist called childBaseIdx the latter is simply called childIdx

   _myEncoding._myIdx = aMyIdx;

   // 1. materialize isLeaf
   const bool lIsLeaf = isLeaf();
   _myEncoding._isLeaf = lIsLeaf;

   if(aGxTree.trace()) {
     std::cout << "prepareEncoding of "
               << nodeId() << '@' << level() << ": "
               << nodeName(nodeType()) << '.'
               << (isLeaf() ? "Leaf" : "Inner") << ".???"
               << ", myIdx = " << _myEncoding.myIdx() << ' '
               << ", cumFreq = " << cumFreq() << ' '
	       << std::endl;
   }

   // 2. calculate direct child indices per node type
   if(!lIsLeaf) {

     // 2.0 allocate/resize _childIdx
     _myEncoding._childIdx.resize(nx(), ny());

     // 2.1 count number of children of each type, thereby init _childIdx
     for(uint i = 0; i < cnx(); ++i) {
       for(uint j = 0; j < cny(); ++j) {
         if(0 != child(i,j)) {
           uint& lChildCount_ij = _myEncoding._childCount[child(i,j)->nodeType()];
           _myEncoding._childIdx(i,j) = lChildCount_ij;
           ++lChildCount_ij;
         }
       }
     }

     // 2.2 remember child base idxes
     _myEncoding._childBaseIdx[0] = aArg._childCount[0];
     _myEncoding._childBaseIdx[1] = aArg._childCount[1];
     _myEncoding._childBaseIdx[2] = aArg._childCount[2];
     _myEncoding._childBaseIdx[3] = aArg._childCount[3];

     // 2.3 register occupied space
     aArg._childCount[0] += _myEncoding._childCount[0];
     aArg._childCount[1] += _myEncoding._childCount[1];
     aArg._childCount[2] += _myEncoding._childCount[2];
     aArg._childCount[3] += _myEncoding._childCount[3];

   }

   // 3. determine the node kind and subkind (Gxt::NodeType from GxtTypes.hh)
   //    This one is also responsible for setting _myEncoding._brCode.
   //    If this differs from _brCode, it must adjust _brd

   determineDetailedNodeType(aGxTree);

   if(aGxTree.trace()) {
     std::cout << "       node type: " <<  _myEncoding._nodeType << std::endl;
   }

   // 5. call prepare child nodes
   if(!lIsLeaf) {
     for(uint i = 0; i < cnx(); ++i) {
       for(uint j = 0; j < cny(); ++j) {
         if(0 != child(i,j)) {
           // child idx is sum of child base idx and idx of child within children
           const uint lChildIdx =   _myEncoding._childBaseIdx[child(i,j)->nodeType()]
                                  + _myEncoding._childIdx(i,j);
           child(i,j)->prepareEncoding(aArg, lChildIdx, aGxTree);
         }
       }
     }
   }
}


void
GxTree::Node::performEncoding(encoding_t&   aEnc,
                              const bool    aCheckEncoding,
                              const GxTree& aGxTree) {
  // 0. trace
  if(aGxTree.trace()) {
    std::cout << "performEncoding of "
              << nodeId() << '@' << level() << " as "
              << _myEncoding._nodeType
              << " at idx = " << _myEncoding.myIdx()
              << std::endl;
             
  }

  // 1. encode this based on node type
  switch(_myEncoding._nodeType.nodeType()) {
    case N_G: performEncoding_G(aEnc, aCheckEncoding, aGxTree);
              break;
    case N_L: performEncoding_L(aEnc, aCheckEncoding, aGxTree);
              break;
    case N_M: performEncoding_M(aEnc, aCheckEncoding, aGxTree);
              break;
    case N_S: performEncoding_S(aEnc, aCheckEncoding, aGxTree);
              break;
    default: assert(0 == 1);
             break;
  }

  // 2. encode children
  if(!_myEncoding._isLeaf) {
    for(uint i = 0; i < cnx(); ++i) {
      for(uint j = 0; j < cny(); ++j) {
        if(0 != child(i,j)) {
          child(i,j)->performEncoding(aEnc, aCheckEncoding, aGxTree);
        }
      }
    }
  }
}


void
GxTree::Node::performEncoding_G(encoding_t&   aEnc,
                                const bool    aCheckEncoding,
                                const GxTree& aGxTree) {

  assert(N_G == _myEncoding._nodeType._nodeType);

  q::ScaleMgr* lScaleMgr = q::ScaleMgr::instance();

  Node_G_Generic& lNode = aEnc._G[_myEncoding.myIdx()];

  const Node_G_Generic::ScaleTotal* lScaleTotal = lScaleMgr->scale_L_8_2();
  const uint32_t lCompressedTotal = lScaleTotal->compress(cumFreq());

  if(aCheckEncoding) {
    assert(lScaleTotal->limit() >= cumFreq());
  }

  // total is the same for all G-Nodes
  lNode.total(lCompressedTotal);

  // position of BR is the same for all G-Nodes
  // br kind is 0x1 if present for all G-Nodes
  lNode.brCode( _myEncoding._brCode );


        uint8_t  lHdr  = 0; // complete header, consisting of:
  const uint32_t lHdrB = _myEncoding._brKind & 0x3;  // two bounding rectangle indicator bits
        uint32_t lHdrS = 0; // Scale bit to indicate scale

  Node_G_Generic::Node_Kind_Fine lNkf = Node_G_Generic::NO_NODE_FINE;

  if(_myEncoding._isLeaf) {
    // uint32_t lHdrLK = 0; // kind encoding for leaf nodes
    switch(_myEncoding._nodeType.get_G_Leaf()) {
      case N_G_L_A: {
                      lNkf = Node_G_Generic::Leaf_A;
                      const uint lMaxTF = _regpPxO.max();
                      // 1. Determine Scale
                      const q::Scale_L* lScale = 0;
                      if(lMaxTF <= lScaleMgr->scale_L_7_1()->limit()) {
                        lScale = lScaleMgr->scale_L_7_1();
                        lHdrS = 0x1;
                      } else {
                        lScale = lScaleMgr->scale_L_7_2();
                        lHdrS = 0x0;
                      }

                      // 2 compose and store hdr
                      const uint32_t lHdrLK = 0x1;
                      lHdr = (0x1 << 7) | (lHdrB << 5) | (lHdrLK << 1) | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_L_A lGrid(lNode.grid_Leaf_A());
                      encodeGridLeaf(&lGrid,
                                      lScale,
                                      _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridLeaf(&lGrid,
                                          lScale,
                                          _regpPxO,
                                          1.11)) {
                          std::cerr << "FAIL checkGrid failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGrid succeded." << std::endl;
                          }
                        }
                      }

                    }

                    break;
      case N_G_L_B: {
                      lNkf = Node_G_Generic::Leaf_B;
                      // 1. Determine Scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0;

                      // 2 compose and store hdr
                      const uint32_t lHdrLK = 0x2;
                      lHdr = (0x1 << 7) | (lHdrB << 5) | (lHdrLK << 1) | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_L_B lGrid(lNode.grid_Leaf_B());
                      encodeGridLeaf(&lGrid,
                                      lScale,
                                      _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridLeaf(&lGrid,
                                          lScale,
                                          _regpPxO,
                                          1.20)) {
                          std::cerr << "FAIL checkGrid failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGrid succeded." << std::endl;
                          }
                        }
                      }

                    }
                    break;
      case N_G_L_C: {
                      lNkf = Node_G_Generic::Leaf_C;
                      // 1. Determine Scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0;

                      // 2 compose and store hdr
                      const uint32_t lHdrLK = 0x8;
                      lHdr = (0x1 << 7) | (lHdrB << 5) | (lHdrLK << 1) | lHdrS;
                      lNode.hdr(lHdr);

                      if(aGxTree.trace()) {
                        std::cout << "   ... ";
                        lNode.printHeader(std::cout);
                        std::cout << std::endl;
                      }

                      if(aCheckEncoding) {
                        if(lHdr != (lNode.hdr() & 0xF0)) {
                          std::cerr << "   ... FAIL hdr encoding failed (1). " << std::endl;
                          assert(0 == 1);
                        }
                      }

                      // 3. encode grid
                      Node_G_Generic::Grid_L_C lGrid(lNode.grid_Leaf_C());
                      encodeGridLeaf(&lGrid,
                                      lScale,
                                      _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridLeaf(&lGrid,
                                          lScale,
                                          _regpPxO,
                                          1.19)) {
                          std::cerr << "FAIL checkGrid failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGrid succeded." << std::endl;
                          }
                        }
                      }

                      if(aCheckEncoding) {
                        if(lHdr != (lNode.hdr() & 0xF0)) {
                          std::cerr << "   ... FAIL hdr encoding failed (2). " << std::endl;
                          assert(0 == 1);
                        }
                      }
                    }
                    break;
      case N_G_L_D: {
                      lNkf = Node_G_Generic::Leaf_D;
                      // 1. Determine Scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0;

                      // 2 compose and store hdr
                      const uint32_t lHdrLK = 0x3;
                      lHdr = (0x1 << 7) | (lHdrB << 5) | (lHdrLK << 1) | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_L_D lGrid(lNode.grid_Leaf_D());
                      encodeGridLeaf(&lGrid,
                                      lScale,
                                      _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridLeaf(&lGrid,
                                          lScale,
                                          _regpPxO,
                                          1.48)) {
                          std::cerr << "FAIL checkGrid failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGrid succeded." << std::endl;
                          }
                        }
                      }
                    }
                    break;
      default: std::cerr << "ERROR: Unknown leaf kind." << std::endl;
               assert(0 == 1);
               break;
    }

    // check encoding
    if(aCheckEncoding) {
    }

  } else {
    // inner node
    // infer ptr type code and set ptrs
    uint32_t lHdrP = 0; // GLMS PtrPattern
    uint32_t lPtrIdx = 1; // _arr[0]: BR, _arr[1]: first childBaseIdx
    uint16_t lSep = 0; // separator, not always needed
    for(uint i = 0; i < 4; ++i) {
      if(0 != _myEncoding._childCount[i]) {
        lHdrP |= 0x1;
        lNode._arr[lPtrIdx] = _myEncoding._childBaseIdx[i];
        ++lPtrIdx;
        lSep <<= 5;
        lSep |= (_myEncoding._childCount[i] & 0x1F);
      }
      lHdrP <<= 1;
    }
    lSep >>= 5;
    if(aGxTree.trace()) {
      const std::string lIndent(2 * level(), ' ');
      std::cout << lIndent
                << "perform Encoding G_I: #Ptr = " << (lPtrIdx - 1)
                << std::endl;
      std::cout << lIndent 
                << "                       Ptr = ";
      for(uint i = 0; i < (lPtrIdx - 1); ++i) {
        std::cout << lNode._arr[1 + i] << "  ";
      }
      std::cout << std::endl;
      std::cout << lIndent
                << "               child counts: ";
      for(uint i = 0; i < 4; ++i) {
        std::cout << _myEncoding._childCount[i] << "  ";
      }
      std::cout << std::endl;
    }
    // due to the last shift, the lowest bit is still zero and used for the S-Bit
    // thus no further shifting of lHdrP in order to insert it into lHdr is necessary

    switch(_myEncoding._nodeType.get_G_Inner()) {
      case N_G_I_A:
                    {
                      lNkf = Node_G_Generic::Inner_A;
                      const q::Scale_6* lScale = 0;

                      // 1. determine scale
                      if(lScaleMgr->scale_6_6_1()->limit() >= maxUnsplitTileFrequency()) {
                        lHdrS = 0x1;
                        lScale = lScaleMgr->scale_6_6_1();
                      } else {
                        lHdrS = 0x0;
                        lScale = lScaleMgr->scale_6_6_3();
                      }
                      if(aCheckEncoding) {
                        assert(lScale->limit() >= maxUnsplitTileFrequency());
                      }
                      // 2 compose and store hdr
                      lHdr = (0x0 << 7) | (lHdrB << 5) | lHdrP | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_I_A lGrid(lNode.grid_Inner_A());
                      encodeGridDirect(&lGrid,
                                       lScale,
                                       _myEncoding._childIdx,
                                       _childrenPxO,
                                       _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridInner(&lGrid,
                                           lScale,
                                           _regpPxO,
                                           _childrenPxO,
                                           1.21)) {
                          std::cerr << "FAIL checkGridInner failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGridInner succeded." << std::endl;
                          }
                        }
                                        
                      }

                    }
                    break;
      case N_G_I_B:
                    {
                      lNkf = Node_G_Generic::Inner_B;
                      // 1. determine scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0x0;

                      // 2 compose and store hdr
                      lHdr = (0x0 << 7) | (lHdrB << 5) | lHdrP | lHdrS;
                      lNode.hdr(lHdr);

                      if(aCheckEncoding) { 
                        if((lHdrP >> 1) != lNode.innerDeterminingBits()) {
                          std::cerr << "FAIL check GMLS pattern (early)" << std::endl;
                          assert(0 == 1);
                        }
                      }

                      // 3. store separator bits
                      lNode.separatorBits_B(lSep);

                      if(aGxTree.trace()) {
                        std::cout << std::string(2 * level(), ' ')
                                  << "perform encoding G_I_B: lSep = " << lSep
                                  << std::endl;
                      }
                      if(aCheckEncoding) {
                        if(lNode.separatorBits_B() != lSep) {
                          std::cout << "lNode.separatorBits_B() = "
                                    << lNode.separatorBits_B() 
                                    << std::endl;
                          std::cerr << "FAIL check separator encoding (early)" << std::endl;
                          assert(0 == 1);
                        }
                      }

                      // 4. encode grid
                      Node_G_Generic::Grid_I_B lGrid(lNode.grid_Inner_B());
                      encodeGridSeparator(&lGrid,
                                           lScale,
                                           _myEncoding._childIdx,
                                           _myEncoding._childCount,
                                           _childrenPxO,
                                           _regpPxO);
                       
                      // 5. check encoding
                      if(aCheckEncoding) {
                        // 5.1 check grid
                        if(!checkGridInner(&lGrid,
                                           lScale,
                                           _regpPxO,
                                           _childrenPxO,
                                           1.19)) {
                          std::cerr << "FAIL checkGridInner failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGridInner succeded." << std::endl;
                          }
                        }
                        // 5.2 check GMLS pattern
                        if((lHdrP >> 1) != lNode.innerDeterminingBits()) {
                          std::cerr << "FAIL check GMLS pattern" << std::endl;
                          assert(0 == 1);
                        }
                        // 5.2 check separator
                        if(lNode.separatorBits_B() != lSep) {
                          std::cout << "lNode.separatorBits_B() = "
                                    << lNode.separatorBits_B() 
                                    << std::endl;
                          std::cerr << "FAIL check separator encoding (early)" << std::endl;
                          assert(0 == 1);
                        }
                      }
                    }
                    break;
      case N_G_I_C:
                    {
                      lNkf = Node_G_Generic::Inner_C;
                      // 1. determine scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0x0;

                      // 2 compose and store hdr
                      lHdr = (0x0 << 7) | (lHdrB << 5) | lHdrP | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_I_C lGrid(lNode.grid_Inner_C());
                      encodeGridIndirect(&lGrid,
                                         lScale,
                                         _myEncoding._childIdx,
                                         _childrenPxO,
                                         _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridInner(&lGrid,
                                           lScale,
                                           _regpPxO,
                                           _childrenPxO,
                                           1.25)) {
                          std::cerr << "FAIL checkGridInner failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGridInner succeded." << std::endl;
                          }
                        }
                                        
                      }
                    }
                    break;
      case N_G_I_D:
                    {
                      lNkf = Node_G_Generic::Inner_D;
                      // 1. determine scale
                      const Node_G_Generic::ScaleGrid* lScale = Node_G_Generic::_scaleGrid[lNkf];
                      lHdrS = 0x0;

                      // 2 compose and store hdr
                      lHdr = (0x0 << 7) | (lHdrB << 5) | lHdrP | lHdrS;
                      lNode.hdr(lHdr);

                      // 3. encode grid
                      Node_G_Generic::Grid_I_D lGrid(lNode.grid_Inner_D());
                      encodeGridIndirect(&lGrid,
                                          lScale,
                                          _myEncoding._childIdx,
                                          _childrenPxO,
                                          _regpPxO);

                      // 4. checkgrid
                      if(aCheckEncoding) {
                        if(!checkGridInner(&lGrid,
                                           lScale,
                                           _regpPxO,
                                           _childrenPxO,
                                           1.48)) {
                          std::cerr << "FAIL checkGridInner failed." << std::endl;
                          assert(0 == 1);
                        } else {
                          if(aGxTree.trace()) {
                            std::cout << "   ... checkGridInner succeded." << std::endl;
                          }
                        }
                                        
                      }
                    }
                    break;
      default: std::cerr << "FAIL GxTree::Node::performEncoding_G: "
                         << "wrong node type: " << _myEncoding._nodeType.get_G_Inner()
                         << std::endl;
               assert(0 == 1);
               break;
    }

    // check encoding
    if(aCheckEncoding) {
      if((lHdrP >> 1) != lNode.ptrKinds()) {
        std::cerr << " FAIL: ";
        Bitvector32 lBv1(lHdrP >> 1);
        Bitvector32 lBv2(lNode.ptrKinds());
        lBv1.print(std::cerr, 8);
        std::cerr << " !=! ";
        lBv2.print(std::cerr, 8);
        std::cerr << std::endl;
        assert(0 == 1);
      }
      if(_myEncoding._nodeType.subkind() != lNode.noPtr()) {
        std::cerr << " FAIL: " << nodeType() << " !=! " << lNode.noPtr() << std::endl;
        assert(0 == 1);
      }
    }
  }


  if(aGxTree.trace()) {
   std::cout << "   ... ";
   lNode.printHeader(std::cout);
   std::cout << std::endl;
  }
 
  if(aCheckEncoding) {
    if(!_myEncoding._nodeType.is_G_L_C()) {
      if(lHdr != lNode.hdr()) {
        std::cerr << " FAIL: " << "header encoding failed. " << std::endl;
        Bitvector8 lBvHdr(lHdr);
        std::cerr << "         hdr = ";
        lBvHdr.printReverse(std::cerr);
        std::cerr << std::endl;

        Bitvector8 lBvNodeHdr(lNode.hdr());
        std::cerr << "    node hdr = ";
        lBvNodeHdr.printReverse(std::cerr);
        std::cerr << std::endl;

        assert(0 == 1);
      }
      if(lHdrS != lNode.scaleBit()) {
        std::cerr << " FAIL: " << "scaleBit encoding failed. " << std::endl;
        assert(0 == 1);
      }
    }

    if(lNode.total() != lCompressedTotal) {
      std::cerr << " Fail: " << "total encoding failed (1). " << std::endl;
      assert(0 == 1);
    }

    if(!maxQErrOk(cumFreq(), lScaleTotal->decompress(lNode.total()), 1.21)) {
      std::cerr << " FAIL: " << "total encoding failed (2). " << std::endl;
      assert(0 == 1);
    }

    if((0 != (_myEncoding._isLeaf & 0x1)) != lNode.isLeaf()) {
      std::cerr << " FAIL: " << "isLeaf encoding failed. " << std::endl;
      assert(0 == 1);
    }

    if(lHdrB != lNode.brKind()) {
      std::cerr << " FAIL: " << "brKind encoding failed. " << std::endl;
      assert(0 == 1);
    }


    // ToDo more checks here
  }

}

void
GxTree::Node::performEncoding_L(encoding_t&   aEnc,
                                const bool    aCheckEncoding,
                                const GxTree& aGxTree) {
  assert(N_L == _myEncoding._nodeType._nodeType);

  Node_L_Generic& lNode = aEnc._L[_myEncoding.myIdx()];

  const Node_L_Generic::ScaleTotal* lScaleTotal = Node_L_Generic::_scaleTotal;
  const uint32_t lCompressedTotal = lScaleTotal->compress(cumFreq());

  if(aCheckEncoding) {
    assert(lScaleTotal->limit() >= cumFreq());
  }

  lNode.total(lCompressedTotal);

  Node_L_Generic::Node_Kind_Fine lNkf = Node_L_Generic::NO_NODE_FINE;

  // Node_L_Generic header parts:
  
  if(_myEncoding._isLeaf) {
    // leaf node
    lNode.isLeaf(0x1);
    assert(lNode.isLeaf());
    switch(_myEncoding._nodeType.get_L_Leaf()) {
      case N_L_L_A: 
           {
             lNode.leafKind(0x0);
             assert(lNode.isLeaf());
             lNkf = Node_L_Generic::Leaf_A1;
             if(_myEncoding.hasBr()) {
               lNode.brKind(0x1);
               lNode.brCode(_myEncoding._brCode);
               assert(lNode.isLeaf());
               assert(0x1 == lNode.brKind());
               assert(lNode.brCode() == _myEncoding._brCode);
             } else {
               lNode.brKind(0);
               lNode.brCode(0);
               assert(lNode.isLeaf());
             }
             Node_L_Generic::Grid_L_A1 lGrid;
             lGrid.attach(lNode.gridNonConst(lNkf));
             encodeGridLeaf(&lGrid,
                            Node_L_Generic::_scaleGrid[lNkf],
                            _regpPxO);
             assert(lNode.isLeaf());
             if(aCheckEncoding) {
               assert(lNode.leafKind() == 0x0);
             }
             if(aGxTree.trace()) {
               std::cout << "grid:" << std::endl;
               printGridMinusWithScale(lGrid, 
                                       Node_L_Generic::_scaleGrid[lNkf],
                                       Node_L_Generic::Grid_L_A1::NoTileBits
                                      );
             }
             if(aCheckEncoding) {
               assert(checkGridLeaf(&lGrid, Node_L_Generic::_scaleGrid[lNkf], _regpPxO, 1.2));
               if(_myEncoding.hasBr()) {
                 assert(lNode.brCode() == _myEncoding._brCode);
               }
             }
           }
           break;
      case N_L_L_B: 
           {
             lNode.leafKind(0x1);
             assert(lNode.isLeaf());
             if(_myEncoding.hasBr()) {
               lNkf = Node_L_Generic::Leaf_B1;
               lNode.brKind(0x1);
               lNode.brCode(_myEncoding._brCode);
               assert(0 != lNode.brCode());
               assert(lNode.isLeaf());
               assert(lNode.hasBr());
               Node_L_Generic::Grid_L_B1 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridLeaf(&lGrid,
                              Node_L_Generic::_scaleGrid[lNkf],
                              _regpPxO);
               if(aGxTree.trace()) {
                 std::cout << "GridOffset = " << lNode.gridOffset(lNkf) << std::endl;
                 std::cout << "GridSize   = " << lGrid.GridSize   << std::endl;
                 std::cout << "NoTileBits = " << lGrid.NoTileBits << std::endl;
                 std::cout << "nx()=ny()  = " << _regpPxO.nx() << std::endl;
               }
               assert(lNode.isLeaf());
               if(aGxTree.trace()) {
                 std::cout << "grid:" << std::endl;
                 printGridMinusWithScale(lGrid, 
                                         Node_L_Generic::_scaleGrid[lNkf],
                                         Node_L_Generic::Grid_L_B1::NoTileBits
                                        );
               }
               if(aCheckEncoding) {
                 assert(checkGridLeaf(&lGrid, Node_L_Generic::_scaleGrid[lNkf], _regpPxO, 1.30));
               }
             } else {
               lNkf = Node_L_Generic::Leaf_B2;
               // std::cout << "lNkf(B2) = " << lNkf << std::endl;
               lNode.brKind(0);
               assert(lNode.isLeaf());
               Node_L_Generic::Grid_L_B2 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridLeaf(&lGrid,
                              Node_L_Generic::_scaleGrid[lNkf],
                              _regpPxO);
               assert(lNode.isLeaf());
               assert(0 == lNode.brKind());
               if(aGxTree.trace()) {
                 std::cout << "grid:" << std::endl;
                 printGridMinusWithScale(lGrid, 
                                         Node_L_Generic::_scaleGrid[lNkf],
                                         Node_L_Generic::Grid_L_B2::NoTileBits
                                        );
               }
               if(aCheckEncoding) {
                 assert(checkGridLeaf(&lGrid, Node_L_Generic::_scaleGrid[lNkf], _regpPxO, 1.2));
               }
             }
             if(aCheckEncoding) {
               assert(lNode.leafKind() == 0x1);
             }
           }
           break;
      case N_L_L_C: 
           {
             lNode.leafKind(0x2);
             if(_myEncoding.hasBr()) {
               lNkf = Node_L_Generic::Leaf_C1;
               lNode.brKind(_myEncoding._brKind);
               lNode.brCode(_myEncoding._brCode);
               Node_L_Generic::Grid_L_C1 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridLeaf(&lGrid,
                              Node_L_Generic::_scaleGrid[lNkf],
                              _regpPxO);
               if(aGxTree.trace()) {
                 std::cout << "grid:" << std::endl;
                 printGridMinusWithScale(lGrid, 
                                         Node_L_Generic::_scaleGrid[lNkf],
                                         Node_L_Generic::Grid_L_C1::NoTileBits
                                        );
               }
               if(aCheckEncoding) {
                 assert(checkGridLeaf(&lGrid, Node_L_Generic::_scaleGrid[lNkf], _regpPxO, 2.0));
               }
             } else {
               lNkf = Node_L_Generic::Leaf_C2;
               lNode.brKind(0);
               lNode.brCode(0);
               Node_L_Generic::Grid_L_C2 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridLeaf(&lGrid,
                              Node_L_Generic::_scaleGrid[lNkf],
                              _regpPxO);

               if(aGxTree.trace()) {
                 std::cout << "grid:" << std::endl;
                 printGridMinusWithScale(lGrid, 
                                         Node_L_Generic::_scaleGrid[lNkf],
                                         Node_L_Generic::Grid_L_C2::NoTileBits
                                        );
               }
               if(aCheckEncoding) {
                 assert(checkGridLeaf(&lGrid, Node_L_Generic::_scaleGrid[lNkf], _regpPxO, 1.21));
               }
             }
             if(aCheckEncoding) {
               assert(lNode.leafKind() == 0x2);
             }
           }
           break;
      default: assert(0 == 1);
               break;
    }
  } else {
    // inner node
    lNode.isLeaf(0x0);
    assert(!lNode.isLeaf());
    if(aGxTree.trace()) {
      std::cout << "performEncodingL: inner: "
                << _myEncoding._nodeType
                << std::endl;
    }
    switch(_myEncoding._nodeType.get_L_Inner()) {
      case N_L_I_A: 
           {
             // determine child node type
             node_type_t lChildNodeType = N_NO_TYPE;
             for(uint i = 1; i < 4; ++i) {
               if(0 != _myEncoding._childCount[i]) {
                 lChildNodeType = (node_type_t) i;
                 break;
               }
             }
             // encode as INNER_A1 if there is a BR
             //     or as INNER_A2 if there is NO BR
             if(_myEncoding.hasBr()) {
               lNkf = Node_L_Generic::Inner_A1;
               if(aGxTree.trace()) {
                 std::cout << "performEncodingL: with BR" << std::endl;
               }
               if(aGxTree.trace() || aCheckEncoding) {
                 const uint lMUSTF = maxUnsplitTileFrequency();
                 if(aGxTree.trace()) {
                   std::cout << "performEncodingL: MUSTF = " << lMUSTF << std::endl;
                 }
                 assert(lMUSTF <= 497);
               }

               lNode.brKind(0x1);
               lNode.brCode(_myEncoding._brCode);
               assert(0 != lNode.brCode());

               lNode.noPtr(1);
               assert(1 == lNode.noPtr());

               lNode.setPtr(lNkf, 0, _myEncoding._childBaseIdx[lChildNodeType]);
               assert(1 == lNode.noPtr());
               if(aGxTree.trace()) {
                 std::cout << "performEncodingL: N_L_I_A1: ptr = " 
                           << _myEncoding._childBaseIdx[lChildNodeType]
                           << std::endl;
               }

               lNode.childNodeKind(lChildNodeType);

               Node_L_Generic::Grid_I_A1 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridDirect(&lGrid,
                                Node_L_Generic::_scaleGrid[lNkf],
                                _myEncoding._childIdx,
                                _childrenPxO,
                                _regpPxO
                               );

               if(aCheckEncoding) {
                 assert(1 == lNode.noPtr());
                 assert(lNode.getPtr(lNkf, 0) < aEnc._no[lChildNodeType]);
                 assert(0x1 == lNode.brKind());
                 assert(_myEncoding._brCode == lNode.brCode());
               }
             } else {
               lNkf = Node_L_Generic::Inner_A2;
               if(aGxTree.trace()) {
                 std::cout << "performEncodingL: no BR" << std::endl;
               }

               lNode.brKind(0x0);

               lNode.noPtr(1);
               assert(1 == lNode.noPtr());
               assert(_myEncoding._childBaseIdx[lChildNodeType] < aEnc._no[lChildNodeType]);

               lNode.setPtr(lNkf, 0, _myEncoding._childBaseIdx[lChildNodeType]);
               assert(1 == lNode.noPtr());
               assert(lNode.getPtr(lNkf, 0) < aEnc._no[lChildNodeType]);

               if(aGxTree.trace()) {
                 std::cout << "performEncodingL: N_L_I_A2: ptr = " 
                           << _myEncoding._childBaseIdx[lChildNodeType]
                           << std::endl;
               }
               lNode.childNodeKind(lChildNodeType);
               
               Node_L_Generic::Grid_I_A2 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridDirect(&lGrid,
                                Node_L_Generic::_scaleGrid[lNkf],
                                _myEncoding._childIdx,
                                _childrenPxO,
                                _regpPxO
                               );
               if(aCheckEncoding) {
                 assert(1 == lNode.noPtr());
                 assert(lNode.getPtr(lNkf, 0) < aEnc._no[lChildNodeType]);
                 assert(0 == lNode.brKind());
               }
             }

             // some checks
             if(aCheckEncoding) {
               if(aGxTree.trace()) {
                 if(1 != lNode.noPtr()) {
                   std::cout << "number of pointers 1 =?= " << lNode.noPtr() 
                             << std::endl;
                 }
               }
               assert(1 == lNode.noPtr());
             }
           }
           break;
      case N_L_I_B: 
           {
             // determine values for noPtr and sepLMS
             // ptr
             // note: addressing is indirect
             //     GLMS   noPtr _sepLMS  lSepLMS
             // 6 = 0110    10    0x0       0x0
             // 3 = 0011    10    0x1       0x1
             // 5 = 0101    00    0x2       0x2

             uint32_t lSepLMS = 0x0;
             if(0 == _myEncoding._childCount[N_L]) {
               // GLMS
               // 0011
               lSepLMS = 0x1;
             } else
             if(0 == _myEncoding._childCount[N_M]) {
               // GLMS
               // 0101
               lSepLMS = 0x2; // just here, not in encoded L-Nodes.
             } else {
               // GLMS
               // 0110
               lSepLMS = 0x0;
             }

             if(_myEncoding.hasBr()) {
               lNkf = Node_L_Generic::Inner_B1;
               if(aGxTree.trace() || aCheckEncoding) {
                 const uint lMUSTF = maxUnsplitTileFrequency();
                 if(aGxTree.trace()) {
                   std::cout << std::string(2 * level(), ' ')
                             << "performEncoding_L: MUSTF = " << lMUSTF
                             << std::endl;
                   assert(lMUSTF <= Node_L_Generic::_scaleGrid[0][lNkf].limit());
                 }
               }

               // the BR
               lNode.brKind(0x1);
               lNode.brCode(_myEncoding.brCode());
               if(aCheckEncoding) {
                 assert(0 != lNode.brCode());
                 assert(lNode.brCode() == _myEncoding.brCode());
               }

               if(0x2 == lSepLMS) {
                 lNode.noPtr(0);
                 lNode.sepLMS(0x0);
               } else {
                 lNode.noPtr(2);
                 lNode.sepLMS(lSepLMS);
               }

               uint lPtrIdx = 0;
               for(uint i = 1; i < 4; ++i) {
                 if(0 != _myEncoding._childCount[i]) {
                   lNode.setPtr(lNkf, lPtrIdx, _myEncoding._childBaseIdx[i]);
                   ++lPtrIdx;
                 }
               }

               Node_L_Generic::Grid_I_B1 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridIndirect(&lGrid,
                                  Node_L_Generic::_scaleGrid[lNkf],
                                  _myEncoding._childIdx,
                                  _childrenPxO,
                                  _regpPxO);
               if(aCheckEncoding) {
                 assert(0x1 == lNode.brKind());
                 assert(2 == lNode.noPtr());
                 assert(!lNode.isLeaf());
                 assert(checkGridInner(&lGrid,
                                       Node_L_Generic::_scaleGrid[lNkf],
                                       _regpPxO,
                                       _childrenPxO,
                                       2.1));
               }
             } else {
               lNkf = Node_L_Generic::Inner_B2;
               if(aGxTree.trace() || aCheckEncoding) {
                 const uint lMUSTF = maxUnsplitTileFrequency();
                 if(aGxTree.trace()) {
                   std::cout << std::string(2 * level(), ' ')
                             << "performEncoding_L: MUSTF = " << lMUSTF
                             << std::endl;
                   assert(lMUSTF <= Node_L_Generic::_scaleGrid[0][lNkf].limit());
                 }
               }
               // no BR
               lNode.brKind(0x0);

               // ptr
               // note: addressing is separator-based
               if(0x2 == lSepLMS) {
                 lNode.noPtr(0);
                 lNode.sepLMS(0x0);
               } else {
                 lNode.noPtr(2);
                 lNode.sepLMS(lSepLMS);
               }

               uint     lPtrIdx = 0;
               uint16_t lSep = 0;
               for(uint i = 1; i < 4; ++i) {
                 if(0 != _myEncoding._childCount[i]) {
                   lNode.setPtr(lNkf, lPtrIdx, _myEncoding._childBaseIdx[i]);
                   if(0 == lPtrIdx) {
                     lSep = (_myEncoding._childCount[i] & 0xF);
                   }
                   ++lPtrIdx;
                 }
               }
               lNode.sep(lSep);

               Node_L_Generic::Grid_I_B2 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridSeparator(&lGrid,
                                   Node_L_Generic::_scaleGrid[lNkf],
                                   _myEncoding._childIdx,
                                   _myEncoding._childCount,
                                   _childrenPxO,
                                   _regpPxO);

               if(aCheckEncoding) {
                 assert(0x0 == lNode.brKind());
                 assert(2 == lNode.noPtr());
                 assert(!lNode.isLeaf());
                 assert(checkGridInner(&lGrid,
                                       Node_L_Generic::_scaleGrid[lNkf],
                                       _regpPxO,
                                       _childrenPxO,
                                       2.1));
                                           
               }

             }
             if(aCheckEncoding) {
               assert(!lNode.isLeaf());
               assert(2 == lNode.noPtr());
             }
           }
           break;
      case N_L_I_C: 
           {
             lNode.noPtr(3);

             if(_myEncoding.hasBr()) {
               lNkf = Node_L_Generic::Inner_C1;
               if(aGxTree.trace()) {
                 std::cout << "performEncodingL: with BR" << std::endl;
               }
               // BR
               lNode.brKind(0x1);
               lNode.brCode(brCode());
               bool lMUSTFViolation = false;
               if(aGxTree.trace() || aCheckEncoding) {
                 const uint lMUSTF = maxUnsplitTileFrequency();
                 if(aGxTree.trace()) {
                   std::cout << "performEncodingL: MUSTF = " << lMUSTF << std::endl;
                 }
                 // redundant
                 if(lMUSTF > Node_L_Generic::scaleGrid(lNkf)->limit()) {
                   lMUSTFViolation = true;
                   std::cout << "!W  lMUSTF  = "<< lMUSTF << " > " <<  Node_L_Generic::scaleGrid(lNkf)->limit()
                             << " =  Node_L_Generic::scaleGrid(lNkf)->limit()"
                             << std::endl;
                 }
                 if(GxTree::K_GLMS != aGxTree.kind()) {
                   assert(lMUSTF <= Node_L_Generic::scaleGrid(lNkf)->limit());
                 }
                 // assert(lMUSTF <= Node_L_Generic::scaleGrid(lNkf)->limit()); // XXX YYY
               }
               // ptr
               lNode.setPtr(lNkf, 0, _myEncoding._childBaseIdx[N_L]);
               lNode.setPtr(lNkf, 1, _myEncoding._childBaseIdx[N_M]);
               lNode.setPtr(lNkf, 2, _myEncoding._childBaseIdx[N_S]);

               // grid
               Node_L_Generic::Grid_I_C1 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridIndirect(&lGrid,
                                  Node_L_Generic::_scaleGrid[lNkf],
                                  _myEncoding._childIdx,
                                  _childrenPxO,
                                  _regpPxO);
               if(aCheckEncoding) {
                 if(!lMUSTFViolation) {
                   assert(checkGridInner(&lGrid,
                                         Node_L_Generic::_scaleGrid[lNkf],
                                         _regpPxO,
                                         _childrenPxO,
                                         2.1));
                 }
               }
          
             } else {
               lNkf = Node_L_Generic::Inner_C2;
               // BR
               lNode.brKind(0x0);
               if(aGxTree.trace() || aCheckEncoding) {
                 const uint lMUSTF = maxUnsplitTileFrequency();
                 if(aGxTree.trace()) {
                   std::cout << "performEncodingL: MUSTF = " << lMUSTF << std::endl;
                 }
                 assert(lMUSTF <= Node_L_Generic::scaleGrid(lNkf)->limit());
               }
               // ptr
               lNode.setPtr(lNkf, 0, _myEncoding._childBaseIdx[N_L]);
               lNode.setPtr(lNkf, 1, _myEncoding._childBaseIdx[N_M]);
               lNode.setPtr(lNkf, 2, _myEncoding._childBaseIdx[N_S]);

               // grid
               Node_L_Generic::Grid_I_C2 lGrid;
               lGrid.attach(lNode.gridNonConst(lNkf));
               encodeGridIndirect(&lGrid,
                                  Node_L_Generic::_scaleGrid[lNkf],
                                  _myEncoding._childIdx,
                                  _childrenPxO,
                                  _regpPxO);
               if(aCheckEncoding) {
                 assert(checkGridInner(&lGrid,
                                       Node_L_Generic::_scaleGrid[lNkf],
                                       _regpPxO,
                                       _childrenPxO,
                                       2.1));
               }
             }

             if(aCheckEncoding) {
               assert(lNode.determineNodeKindFine() == lNkf);
               assert(!lNode.isLeaf());
               assert(3 == lNode.noPtr());
               assert(lNode.getPtr(lNkf, 0) == _myEncoding._childBaseIdx[N_L]);
               assert(lNode.getPtr(lNkf, 1) == _myEncoding._childBaseIdx[N_M]);
               assert(lNode.getPtr(lNkf, 2) == _myEncoding._childBaseIdx[N_S]);
             }

           }
           break;
      default: std::cerr << "performEncodingL: bad L inner kind = " 
                         << _myEncoding._nodeType.get_L_Inner()
                         << std::endl;
               assert(0 == 1);
               break;
    }

  }

  if(aCheckEncoding) {
    if(lNode.determineNodeKindFine() != lNkf) {
      std::cout << "external lNKF = " << lNkf 
                << " != " 
                << lNode.determineNodeKindFine()
                << " = interal NKF"
                << std::endl;
      std::cout << "node id: " << nodeId() << std::endl
                << "node type: " << _myEncoding._nodeType << std::endl
                << "     leaf: " << _myEncoding._isLeaf << std::endl
                << "leaf kind: " << lNode.leafKind() << std::endl
                << std::endl;
      assert(0==1);
    }
  }

}

void
GxTree::Node::performEncoding_M(encoding_t&   aEnc,
                                const bool    aCheckEncoding,
                                const GxTree& aGxTree) {

  Node_M_Generic& lNode = aEnc._M[_myEncoding.myIdx()];

  Node_M_Generic::Node_Kind_Fine lNkf = Node_M_Generic::NO_NODE_FINE;

  if(_myEncoding._isLeaf) {
    // leaf node
    // set to leaf
    lNode.setLeaf();
    // encode total
    lNode.totalX(Node_M_Generic::_scaleTotalX->compress(cumFreq()));
    if(aCheckEncoding) {
      assert(Node_M_Generic::_scaleTotalX->limit() >= cumFreq());
    }

    const uint lMaxTF = _regpPxO.max();

    // rest: 
    // a) kind, 
    // b) brKind, BrCode (only if applies)
    // c) scalebit, grid 
    switch(_myEncoding._nodeType.get_M_Leaf()) {
      case N_M_L_A: {
                      lNode.leafKind(0x0);
                      if(_myEncoding.hasBr()) {
                        // M_L_A1
                        lNkf = Node_M_Generic::Leaf_A1;
                        lNode.brKindX(0x1);
                        lNode.brCode(_myEncoding.brCode());
                        assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                        Node_M_Generic::Grid_L_A1 lGrid;
                        lGrid.attach(lNode.gridNonConst(lNkf));
                        if(lMaxTF <= Node_M_Generic::_scaleGrid[1][lNkf]->limit()) {
                          lNode.scaleBitX(0x1);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[1][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x1 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid, 
                                                 Node_M_Generic::_scaleGrid[1][lNkf],
                                                 _regpPxO,
                                                 1.2));
                          }
                          if(aGxTree.trace()) {
                            std::cout << "grid: " << std::endl;
                            printGridMinusWithScale(lGrid,
                                                    Node_M_Generic::_scaleGrid[1][lNkf],
                                                    lGrid.NoTileBits);
                          }
                        } else {
                          lNode.scaleBitX(0x0);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[0][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x0 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid,
                                                 Node_M_Generic::_scaleGrid[0][lNkf],
                                                 _regpPxO,
                                                 1.2));
                          }
                        }
                        if(aCheckEncoding) {
                          assert(0x1 == lNode.brKindX());
                          assert(_myEncoding.brCode() == lNode.brCode());
                        }
                      } else {
                        // M_L_A2
                        lNkf = Node_M_Generic::Leaf_A2;
                        lNode.brKindX(0);
                        assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                        Node_M_Generic::Grid_L_A2 lGrid;
                        lGrid.attach(lNode.gridNonConst(lNkf));
                        lNode.scaleBitX(0x0);
                        encodeGridLeaf(&lGrid,
                                       Node_M_Generic::_scaleGrid[0][lNkf],
                                       _regpPxO);
                        if(aCheckEncoding) {
                          assert(0x0 == lNode.brKindX());
                          assert(checkGridLeaf(&lGrid,
                                               Node_M_Generic::_scaleGrid[0][lNkf],
                                               _regpPxO,
                                               1.2));
                        }
                      }
                      if(aCheckEncoding) {
                        assert(lNode.isLeaf());
                        assert(0x0 == lNode.leafKind());
                      }
                    }
                    break;
      case N_M_L_B: {
                      lNode.leafKind(0x1);
                      if(_myEncoding.hasBr()) {
                        // M_L_B1
                        lNkf = Node_M_Generic::Leaf_B1;
                        lNode.brKindX(0x1);
                        lNode.brCode(_myEncoding.brCode());
                        assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                        Node_M_Generic::Grid_L_B1 lGrid;
                        lGrid.attach(lNode.gridNonConst(lNkf));
                        if(lMaxTF <= Node_M_Generic::_scaleGrid[1][lNkf]->limit()) {
                          lNode.scaleBitX(0x1);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[1][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x1 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid,
                                                 Node_M_Generic::_scaleGrid[1][lNkf],
                                                 _regpPxO,
                                                 2.1));
                          }
                        } else {
                          lNode.scaleBitX(0x0);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[0][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x0 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid,
                                                 Node_M_Generic::_scaleGrid[0][lNkf],
                                                 _regpPxO,
                                                 2.1));
                          }
                        }
                        if(aCheckEncoding) {
                          assert(0x1 == lNode.leafKind());
                          assert(lNode.isLeaf());
                          assert(0x1 == lNode.brKindX());
                          assert(_myEncoding.brCode() == lNode.brCode());
                        }
                      } else {
                        // M_L_B2
                        lNkf = Node_M_Generic::Leaf_B2;
                        lNode.brKindX(0);
                        assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                        Node_M_Generic::Grid_L_B2 lGrid;
                        lGrid.attach(lNode.gridNonConst(lNkf));
                        if(lMaxTF <= Node_M_Generic::_scaleGrid[1][lNkf]->limit()) {
                          lNode.scaleBitX(0x1);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[1][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x1 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid,
                                                 Node_M_Generic::_scaleGrid[1][lNkf],
                                                 _regpPxO,
                                                 1.3));
                          }
                        } else {
                          lNode.scaleBitX(0x0);
                          encodeGridLeaf(&lGrid,
                                         Node_M_Generic::_scaleGrid[0][lNkf],
                                         _regpPxO);
                          if(aCheckEncoding) {
                            assert(0x0 == lNode.scaleBitX());
                            assert(checkGridLeaf(&lGrid,
                                                 Node_M_Generic::_scaleGrid[0][lNkf],
                                                 _regpPxO,
                                                 1.3));
                          }
                        }
                        if(aCheckEncoding) {
                          assert(0x1 == lNode.leafKind());
                          assert(lNode.isLeaf());
                          assert(0x0 == lNode.brKindX());
                        }
                      }
                    }
                    break;
      case N_M_L_C: {
                      // M_L_C2
                      lNode.leafKind(0x2);
                      lNkf = Node_M_Generic::Leaf_C2;
                      assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                      Node_M_Generic::Grid_L_C2 lGrid;
                      lGrid.attach(lNode.gridNonConst(lNkf));
                      if(lMaxTF <= Node_M_Generic::_scaleGrid[1][lNkf]->limit()) {
                        lNode.scaleBitX(0x1);
                        encodeGridLeaf(&lGrid,
                                       Node_M_Generic::_scaleGrid[1][lNkf],
                                       _regpPxO);
                        if(aCheckEncoding) {
                          assert(0x1 == lNode.scaleBitX());
                          assert(checkGridLeaf(&lGrid,
                                               Node_M_Generic::_scaleGrid[1][lNkf],
                                               _regpPxO,
                                               2.1));
                        }
                      } else {
                        lNode.scaleBitX(0x0);
                        encodeGridLeaf(&lGrid,
                                       Node_M_Generic::_scaleGrid[0][lNkf],
                                       _regpPxO);
                        if(aCheckEncoding) {
                          assert(0x0 == lNode.scaleBitX());
                          assert(checkGridLeaf(&lGrid,
                                               Node_M_Generic::_scaleGrid[0][lNkf],
                                               _regpPxO,
                                               2.1));
                        }
                      }
                      if(aCheckEncoding) {
                        assert(0x2 == lNode.leafKind());
                        assert(lNode.isLeaf());
                        assert(0x0 == lNode.brKindX());
                      }
                    }
                    break;
      default: std::cout << "performEncoding_M: bad leaf node type" << std::endl;
               assert(0 == 1);
    }
  } else {
    // inner node
    bool lOnlyMChildren = false;
    switch(_myEncoding._nodeType.get_M_Inner()) {
      case N_M_I_A1: {
                       assert(0 != _myEncoding.brKind()); // A should only be used with BR
                       assert(3 == nx());
                       assert(3 == ny());
                       assert(3 == cnx());
                       assert(3 == cny());
                       lNkf = Node_M_Generic::Inner_A1;
                       lNode.setInnerA1();
                       lNode.totalS(Node_M_Generic::_scaleTotalS->compress(cumFreq()));
                       lNode.brKind(0x1);
                       lNode.brCode(_myEncoding.brCode());
                       // determine kind of ptr and set ptr accordingly
                       // !!! ToDo !!! ToDo !!! ToDo !!!
                       if(K_GLMS == aGxTree.gxKind()) {
                         if(0 == _myEncoding._childCount[N_S]) {
                           // M child
                           lOnlyMChildren = true;
                           lNode.ptrKindBit(0x0);
                           lNode.setPtr(lNkf, _myEncoding._childBaseIdx[N_M]);
                           assert(_myEncoding._childBaseIdx[N_M] == lNode.getPtr(lNkf));
                         } else {
                           // S child
                           std::cout << "N_M_I_A1: no S children allowed." << std::endl;
                           assert(0 == 1);
                         }
                       } else {
                         lOnlyMChildren = true;
                         lNode.setPtr(lNkf, _myEncoding._childBaseIdx[N_M]);
                         assert(_myEncoding._childBaseIdx[N_M] == lNode.getPtr(lNkf));
                       }

                       Node_M_Generic::Grid_I_A1 lGrid;
                       lGrid.attach(lNode.gridNonConst(lNkf));
                       encodeGridIndirect(&lGrid,
                                          Node_M_Generic::_scaleGrid[0][lNkf],
                                          _myEncoding._childIdx,
                                         _childrenPxO,
                                          _regpPxO);
                       if(aCheckEncoding) {
                         assert(lNode.isInnerA1());
                         assert(_myEncoding._childBaseIdx[N_M] == lNode.getPtr(lNkf));
                         assert(0x1 == _myEncoding.brKind());
                         assert(_myEncoding.brCode() == lNode.brCode());
                         assert(checkGridInner(&lGrid,
                                               Node_M_Generic::_scaleGrid[0][lNkf],
                                               _regpPxO,
                                               _childrenPxO,
                                               2.1));
                         Node_M_Generic::Grid_I_A1 lGridFromNode;
                         lGridFromNode.attach(lNode.gridNonConst(lNkf));
                         uint lCount = 0;
                         for(uint i = 0; i < cnx(); ++i) {
                           for(uint j = 0; j < cny(); ++j) {
                             if(lGridFromNode.isIBitSet(i,j)) {
                               ++lCount;
                               assert(0 != child(i,j));
                             } else {
                               assert(0 == child(i,j));
                             }
                           }
                         }
                         if(!maxQErrOk(cumFreq(), lNode.totalDecompressed(), 2.1)) {
                           std::cout << " total decompressed = " << lNode.totalDecompressed() << std::endl
                                     << " total   compressed = " << lNode.totalS() << std::endl
                                     << "           cum freq = " << cumFreq() << std::endl
                                     << " total   scale code = " 
                                                  << Node_M_Generic::_scaleTotalS->compress(cumFreq()) 
                                                  << std::endl
                                     << "          node type = " << lNode.determineNodeKindFine() 
                                     << std::endl;
                           assert(0 == 1);
                         }
                         if(noChildren() != lNode.noChildren()) {
                           std::cout << "N_M_I_A: noChildren = " << noChildren() << std::endl
                                     << "   lNode.noChildren = " << lNode.noChildren() << std::endl
                                     << "             lCount = " << lCount << std::endl
                                     << std::endl;
                           assert(0 == 1);
                         }
                       }
                     }
                     break;
      case N_M_I_A2: {
                       // cannot store bounding rectangle
                       assert(0 == _myEncoding.brKind());
                       // has only one pointer
                       assert(    (0 == _myEncoding._childCount[N_M])
                               || (0 == _myEncoding._childCount[N_S]) );

                       // std::cout << "Node_M_Generic::Grid_I_B::NoBytes = "
                       //           << Node_M_Generic::Grid_I_B::NoBytes << ", "
                       //           << "Node_M_Generic::Grid_I_B::NoBits = "
                       //           << Node_M_Generic::Grid_I_B::NoBits << ", "
                       //           << std::endl;
 
                       lNkf = Node_M_Generic::Inner_A2;
                       lNode.setInnerA2();
                       lNode.brKind(0x0);
                       lNode.totalX(Node_M_Generic::_scaleTotalX->compress(cumFreq()));
 
                       // set ptr and ptr kind
                       // ToDo
                     
                       // if(aEnc._no[N_M] <= _myEncoding._childBaseIdx[N_M]) {
                       //   std::cout << "aEnc._no[N_M] = " << aEnc._no[N_M] << std::endl
                       //             << "idx = " << _myEncoding._childBaseIdx[N_M]
                       //             << std::endl;
                       // }
                       // assert(aEnc._no[N_M] > _myEncoding._childBaseIdx[N_M]);

                       if(K_GLMS == aGxTree.gxKind()) {
                         if(0 == _myEncoding._childCount[N_S]) {
                           // M child
                           lOnlyMChildren = true;
                           lNode.ptrKindBit(0x0);
                           lNode.setPtr(lNkf, _myEncoding._childBaseIdx[N_M]);
                           assert(lNode.getPtr(lNkf) == _myEncoding._childBaseIdx[N_M]);
                         } else {
                           // S child
                           lOnlyMChildren = false;
                           lNode.ptrKindBit(0x1);
                           lNode.setPtr(lNkf, _myEncoding._childBaseIdx[N_S]);
                           assert(lNode.getPtr(lNkf) == _myEncoding._childBaseIdx[N_S]);
                         }
                       } else {
                         lOnlyMChildren = true;
                         lNode.ptrKindBit(0x0);
                         lNode.setPtr(lNkf, _myEncoding._childBaseIdx[N_M]);
                         assert(lNode.getPtr(lNkf) == _myEncoding._childBaseIdx[N_M]);
                       }

                       // std::cout << "ptr  offset: " << lNode._ptrOffset[lNkf] << std::endl;
                       // std::cout << "grid offset: " << lNode._gridOffset[lNkf] << std::endl;
                       // encode grid
                       Node_M_Generic::Grid_I_A2 lGrid;
                       lGrid.attach(lNode.gridNonConst(lNkf));

                       const uint lMaxTF = maxUnsplitTileFrequency();
                       assert(lMaxTF <= Node_M_Generic::_scaleGrid[0][lNkf]->limit());
                       if(lMaxTF <= Node_M_Generic::_scaleGrid[1][lNkf]->limit()) {
                         lNode.scaleBitX(0x1);
                         encodeGridDirect(&lGrid,
                                          Node_M_Generic::_scaleGrid[1][lNkf],
                                          _myEncoding._childIdx,
                                          _childrenPxO,
                                          _regpPxO);
                         if(aCheckEncoding) {
                           assert(0x1 == lNode.scaleBitX());
                           assert(checkGridInner(&lGrid,
                                                 Node_M_Generic::_scaleGrid[1][lNkf],
                                                 _regpPxO,
                                                 _childrenPxO,
                                                 2.1));
                           for(uint i = 0; i < cnx(); ++i) {
                             for(uint j = 0; j < cny(); ++j) {
                               if(lGrid.isIBitSet(i,j)) {
                                 assert(0 != child(i,j));
                               } else {
                                 assert(0 == child(i,j));
                               }
                             }
                           }
                         }
                       } else {
                         lNode.scaleBitX(0x0);
                         encodeGridDirect(&lGrid,
                                          Node_M_Generic::_scaleGrid[0][lNkf],
                                          _myEncoding._childIdx,
                                          _childrenPxO,
                                          _regpPxO);
                         if(aCheckEncoding) {
                           assert(0x0 == lNode.scaleBitX());
                           assert(checkGridInner(&lGrid,
                                                 Node_M_Generic::_scaleGrid[0][lNkf],
                                                 _regpPxO,
                                                 _childrenPxO,
                                                 2.1));
                           for(uint i = 0; i < cnx(); ++i) {
                             for(uint j = 0; j < cny(); ++j) {
                               if(lGrid.isIBitSet(i,j)) {
                                 assert(0 != child(i,j));
                               } else {
                                 assert(0 == child(i,j));
                               }
                             }
                           }
                         }
                       }
                       if(aCheckEncoding) {
                         assert(lNode.isInnerA2());
                         if(lOnlyMChildren) {
                           assert(0x0 == lNode.ptrKindBit()); 
                           assert(lNode.getPtr(lNkf) == _myEncoding._childBaseIdx[N_M]);
                         } else {
                           assert(0x1 == lNode.ptrKindBit()); 
                           assert(lNode.getPtr(lNkf) == _myEncoding._childBaseIdx[N_S]);
                         }
                         for(uint i = 0; i < cnx(); ++i) {
                           for(uint j = 0; j < cny(); ++j) {
                             if(lGrid.isIBitSet(i,j)) {
                               assert(0 != child(i,j));
                             } else {
                               assert(0 == child(i,j));
                             }
                           }
                         }
                       }
                     }
                     break;
      case N_M_I_B2: {
                       // may not have BR
                       assert(0 == _myEncoding.brKind());

                       // must have two child ptrs
                       assert(0 != _myEncoding._childCount[N_M]);
                       assert(0 != _myEncoding._childCount[N_S]);


                       lNkf = Node_M_Generic::Inner_B2;

                       // prep header
                       lNode.setInnerB2();
                       lNode.totalS(Node_M_Generic::_scaleTotalS->compress(cumFreq()));

                       // set ptr
                       lNode.setPtrToM(_myEncoding._childBaseIdx[N_M]);
                       lNode.setPtrToS(_myEncoding._childBaseIdx[N_S]);


                       // encode grid
                       Node_M_Generic::Grid_I_B2 lGrid;
                       lGrid.attach(lNode.gridNonConst(lNkf));
                       encodeGridIndirect(&lGrid,
                                          Node_M_Generic::_scaleGrid[0][lNkf],
                                          _myEncoding._childIdx,
                                          _childrenPxO,
                                          _regpPxO);

                       
                       // usual checks

                       if(aCheckEncoding) {
                         assert(lNode.isInnerB2());
                         assert(lNode.getPtrToM() == _myEncoding._childBaseIdx[N_M]);
                         assert(lNode.getPtrToS() == _myEncoding._childBaseIdx[N_S]);
                         if(!checkGridInner(&lGrid,
                                               Node_M_Generic::_scaleGrid[0][lNkf],
                                               _regpPxO,
                                               _childrenPxO,
                                               2.1)) {
                           assert(0 == 1);
                         }
                         for(uint i = 0; i < cnx(); ++i) {
                           for(uint j = 0; j < cny(); ++j) {
                             if(lGrid.isIBitSet(i,j)) {
                               assert(0 != child(i,j));
                             } else {
                               assert(0 == child(i,j));
                             }
                           }
                         }
                       }
                     }
                     break;
      default: std::cerr << "performEncoding_M: bad inner node type" << std::endl;
               assert(0 == 1);
    }
    if(aCheckEncoding) {
      if(noChildren() != lNode.noChildren()) {
        std::cout << "Problem:" << std::endl
                  << "               lNkf = " << lNkf << std::endl
                  << "     nodeKindFine() = " << lNode.determineNodeKindFine() << std::endl
                  << "tree node #children = " << noChildren() << std::endl
                  << "  encoded #children = " << lNode.noChildren() << std::endl;
        assert(0 == 1);
      }
    }
  }

  if(aCheckEncoding) {
    if(!maxQErrOk(cumFreq(), lNode.totalDecompressed(), 2.1)) {
      std::cout << " total decompressed = " << lNode.totalDecompressed() << std::endl
                << "           cum freq = " << cumFreq() << std::endl
                << "          node type = " << lNode.determineNodeKindFine() << std::endl;
                ;
      assert(0 == 1);
    }
    if(lNode.determineNodeKindFine() != lNkf) {
      std::cout << " performEncoding_M:        lNkf = " << lNkf << std::endl
                << "                    lNode.nkf() = " << lNode.determineNodeKindFine()
                << std::endl;
      assert(0 == 1);
    }
  }
}

void
GxTree::Node::performEncoding_S(encoding_t&   aEnc,
                                const bool    aCheckEncoding,
                                const GxTree& aGxTree) {


  Node_S_Generic& lNode = aEnc._S[_myEncoding.myIdx()];

  Node_S_Generic::Node_Kind_Fine lNkf = Node_S_Generic::NO_NODE_FINE;

  // encode total (equal code in all cases)
  lNode.total(Node_S_Generic::_scaleTotal->compress(cumFreq()));
 
  if(aCheckEncoding) {
    assert(Node_S_Generic::_scaleTotal->limit() >= cumFreq());
  }
  
  if(_myEncoding._isLeaf) {
    // LEAF NODE

    if(N_S_L_A == _myEncoding._nodeType.get_S_Leaf()) {
      if(hasBr()) {
        lNkf = Node_S_Generic::Leaf_A1;
        if(aCheckEncoding) {
          const uint lMaxTF = _regpPxO.max();
          assert(lMaxTF <= Node_S_Generic::scaleGrid(lNkf)->limit());
        }
        lNode.setLeafA1();
        lNode.brCode(brCode());
        Node_S_Generic::Grid_L_A1 lGrid;
        lGrid.attach(lNode.gridLeafA1NonConst());
        encodeGridLeaf(&lGrid,
                       Node_S_Generic::scaleGrid(lNkf),
                       _regpPxO);
        if(aCheckEncoding) {
          assert(checkGridLeaf(&lGrid,
                               Node_S_Generic::scaleGrid(lNkf),
                               _regpPxO,
                               2.0));
        }
      } else {
        lNkf = Node_S_Generic::Leaf_A2;
        const q::Scale_S* lGridScale = Node_S_Generic::scaleGrid(lNkf);
        if(aCheckEncoding) {
          const uint lMaxTF = _regpPxO.max();
          if(lMaxTF > lGridScale->limit()) {
            std::cout << "encoding node " << nodeId() << '@' << level() 
                      << "of type " << Node_S_Generic::_name[lNkf]
                      << ": max tf too high:"
                      << std::endl;
            std::cout << "  lMaxTF = " << lMaxTF << std::endl
                      << "   limit = " << lGridScale->limit() << std::endl;
            std::cout << "  regpPxO:" << std::endl;
            _regpPxO.print(std::cout, 4);
            std::cout << std::endl;
          }
          assert(lMaxTF <= lGridScale->limit());
        }
        lNode.setLeafA2();
        Node_S_Generic::Grid_L_A2 lGrid;
        lGrid.attach(lNode.gridLeafA2NonConst());
        encodeGridLeaf(&lGrid,
                       lGridScale,
                       _regpPxO);
        if(aCheckEncoding) {
          if(!checkGridLeaf(&lGrid, lGridScale, _regpPxO, 2.0)) {
            std::cout << "_regpPxO: " << std::endl;
            _regpPxO.print(std::cout);
            std::cout << std::endl;
            std::cout << "grid: " << std::endl;
            printGridMinusWithScale(lGrid, lGridScale, 4);
            std::cout << std::endl;
            assert(0 == 1);
          }
        }
      }
    } else
    if(N_S_L_B == _myEncoding._nodeType.get_S_Leaf()) {
      assert(!hasBr());
      lNkf = Node_S_Generic::Leaf_B2;
      if(aCheckEncoding) {
        const uint lMaxTF = _regpPxO.max();
        assert(lMaxTF <= Node_S_Generic::scaleGrid(lNkf)->limit());
      }
      lNode.setLeafB2();
      Node_S_Generic::Grid_L_B2 lGrid;
      lGrid.attach(lNode.gridLeafB2NonConst());
      encodeGridLeaf(&lGrid,
                     Node_S_Generic::scaleGrid(lNkf),
                     _regpPxO);
      if(aCheckEncoding) {
        assert(checkGridLeaf(&lGrid,
                             Node_S_Generic::scaleGrid(lNkf),
                             _regpPxO,
                             2.0));
      }


    } else {
      std::cout << "BAD node type in _myEncoding._nodeType.get_S_Inner()" << std::endl;
      assert(0 == 1);
    }

  } else {
    // INNER NODE, can only be S.Inner.A2
    assert(!hasBr());
    assert(N_S_I_A == _myEncoding._nodeType.get_S_Inner());
    lNkf = Node_S_Generic::Inner_A2;
    if(aCheckEncoding) {
      const uint lMaxTF = maxUnsplitTileFrequency();
      if(Node_S_Generic::_scaleGrid[lNkf]->limit() < lMaxTF) {
        std::cout << "lMaxTF = " << lMaxTF << std::endl;
        std::cout << "limit  = " << Node_S_Generic::scaleGrid(lNkf)->limit() << std::endl;
        print(std::cout);
      }
      assert(lMaxTF <= Node_S_Generic::scaleGrid(lNkf)->limit());
    }
    lNode.setInnerA2();
    // ptr
    lNode.setPtr(_myEncoding._childBaseIdx[N_S]); // can only be ptr to N_S node
    // grid
    Node_S_Generic::Grid_I_A2 lGrid;
    lGrid.attach(lNode.gridInnerA2NonConst());
    encodeGridDirect(&lGrid,
                     Node_S_Generic::scaleGrid(lNkf),
                     _myEncoding._childIdx,
                     _childrenPxO,
                     _regpPxO
                     );
    if(aCheckEncoding) {
      assert(checkGridInner(&lGrid, Node_S_Generic::scaleGrid(lNkf), _regpPxO, _childrenPxO, 2.1));
    }

  }

  if(aCheckEncoding) {
    if(!maxQErrOk(cumFreq(), lNode.totalDecompressed(), 2.1)) {
      std::cout << " total decompressed = " << lNode.totalDecompressed() << std::endl
                << "           cum freq = " << cumFreq() << std::endl
                << "          node type = " << lNode.determineNodeKindFine() << std::endl;
                ;
      assert(0 == 1);
    }
    if(lNode.determineNodeKindFine() != lNkf) {
      std::cout << " performEncoding_M:        lNkf = " << lNkf << std::endl
                << "                    lNode.nkf() = " << lNode.determineNodeKindFine()
                << std::endl;
      assert(0 == 1);
    }
  }

}



// determineDetailedNoteType
// sets: _myEncoding._nodeType 
//       _myEncoding._brKind
//       _myEncoding._brCode
//       _myEncoding._tileFreq for leaf nodes
// requires: _myEncoding._isLeaf and all others must have been set already

void
GxTree::Node::determineDetailedNodeType(const GxTree& aGxTree) {
  _myEncoding._nodeType._nodeType = nodeType();
  _myEncoding._nodeType._isLeaf   = _myEncoding._isLeaf;

  // determined below:
  // _myEncoding._nodeType._subkind


  // check whether BR decision has been made
  assert(0 <= hasBr());

  // determine subkind 

  const double lLrf = aGxTree.lrf();
  switch(nodeType()) {
    case N_G: {
                // currently:
                // 1. all G-Nodes have sufficient space to contain a br
                // 2. the only BR kind currently supported is 0x1 (16x16)
                // thus:
                if(0 == brCode()) {
                  _myEncoding._brKind = 0;
                  _myEncoding._brCode = 0;
                } else {
                  _myEncoding._brKind = 0x1;
                  _myEncoding._brCode = brCode();
                }
                if(_myEncoding._isLeaf) {
                  // leaf node
                  if(0 >= aGxTree.leafRefinement()) {
                    _myEncoding._nodeType._subkind = N_G_L_A;
                  } else {
                    const uint lMaxTF5x5 = _regpPxO.max();
                    _mm3._filled = false;
                    fill_mm3(_mm3, brdd(), data(), aGxTree);
                    const uint lMaxTF6x6 = _mm3._6x6.max();

                    Node_G_Leaf_T lSubKind  = N_G_L_A;
                    uint          lMinMaxTF = lMaxTF5x5;

                    if((lMaxTF6x6 <  (lLrf * lMinMaxTF)) &&
                       (lMaxTF6x6 <= Node_G_Generic::_scaleGrid[Node_G_Generic::Leaf_B]->limit())) {
                      lMinMaxTF = lMaxTF6x6;
                      lSubKind  = N_G_L_B;
                    }

                    if(2 <= aGxTree.leafRefinement()) {
                       RegularPartitioning2dim lRegP7x7(brdd(), 7, 7, data());
                       const uint lMaxTF7x7 = lRegP7x7.max();
                       if((lMaxTF7x7 <  (lLrf * lMinMaxTF)) && 
                          (lMaxTF7x7 <= Node_G_Generic::_scaleGrid[Node_G_Generic::Leaf_C]->limit())) {
                         lMinMaxTF = lMaxTF7x7;
                         lSubKind  = N_G_L_C;
                       }
                    }

                    if(3 <= aGxTree.leafRefinement()) {
                      uint lMaxTF8x8 = 0;
                      if(hasBr()) {
                        fill_mm2(_br_mm2, _br_regp16x16, aGxTree);
                        lMaxTF8x8 = _br_mm2._8x8.max();
                      } else {
                        lMaxTF8x8 = _tr_mm2._8x8.max();
                      }
                      if((lMaxTF8x8 <  (lLrf * lMinMaxTF)) &&
                         (lMaxTF8x8 <= Node_G_Generic::_scaleGrid[Node_G_Generic::Leaf_D]->limit())) {
                        lMinMaxTF = lMaxTF8x8;
                        lSubKind  = N_G_L_D;
                      }
                    }

                    _myEncoding._nodeType._subkind = lSubKind;
                   
                    // need to copy leaf RegP to _regpPxO

                    switch(lSubKind) {
                      case N_G_L_A:
                                    break;
                      case N_G_L_B:
                                    _regpPxO.initFromData2dim(brdd(), 6, 6, data());
                                    break;
                      case N_G_L_C:
                                    _regpPxO.initFromData2dim(brdd(), 7, 7, data());
                                    break;
                      case N_G_L_D:
                                    _regpPxO.initFromData2dim(brdd(), 8, 8, data());
                                    break;
                      default: assert(0 == 1);
                               break;
                    }
                  }
                } else {
                  // inner node
                  uint x = _myEncoding.noChildPtr();
                  _myEncoding._nodeType._subkind = x;
                }
              }
              break;
    case N_L: {
                // currently:
                // 1. all L-Leaf-Nodes have sufficient space to contain a br
                //    but the L-Inner Nodes do not
                // 2. the only BR kind currently supported is 0x1 (16x16)
                if(needsBr() && hasBr()) {
                  _mm3._filled = false;
                  _mm5._filled = false;
                  _myEncoding._brKind = 0x1;
                  _myEncoding._brCode = brCode();
                } else {
                  _myEncoding._brKind = 0;
                  _myEncoding._brCode = 0;
                }
                if(_myEncoding._isLeaf) {
                // leaf node
                  if(0 >= aGxTree.leafRefinement()) {
                    _myEncoding._nodeType._subkind = N_L_L_A;
                  } else {
                    uint lMaxTF4x4 = 0;
                    if(hasBr()) {
                      fill_mm2(_br_mm2, _br_regp16x16, aGxTree);
                      lMaxTF4x4 = _br_mm2._4x4.max();
                    } else {
                      fill_mm2(_tr_mm2, _tr_regp16x16, aGxTree);
                      lMaxTF4x4 = _tr_mm2._4x4.max();
                    }

                    uint lMinMaxTF = lMaxTF4x4;

                    Node_G_Leaf_T lSubKind  = N_G_L_A;

                    if(1 <= aGxTree.leafRefinement()) {
                      RegularPartitioning2dim lRegP5x5(brdd(), 5, 5, data());
                      uint lNkf = (hasBr() ? Gxt::Node_L_Generic::Leaf_B1
                                           : Gxt::Node_L_Generic::Leaf_B2);
                      uint lMaxTF5x5 = lRegP5x5.max();
                      if((lMaxTF5x5 <  (lLrf * lMinMaxTF)) &&
                         (lMaxTF5x5 <= Gxt::Node_L_Generic::_scaleGrid[lNkf]->limit())) {
                        lMinMaxTF = lMaxTF5x5;
                        lSubKind  = N_G_L_B;
                      }
                    }

                    if(2 <= aGxTree.leafRefinement()) {
                      RegularPartitioning2dim lRegP6x6(brdd(), 6, 6, data());
                      uint lNkf = (hasBr() ? Gxt::Node_L_Generic::Leaf_C1
                                           : Gxt::Node_L_Generic::Leaf_C2);
                      uint lMaxTF6x6 = lRegP6x6.max();
                      if((lMaxTF6x6 <  (lLrf * lMinMaxTF)) &&
                         (lMaxTF6x6 <= Gxt::Node_L_Generic::_scaleGrid[lNkf]->limit())) {
                        lMinMaxTF = lMaxTF6x6;
                        lSubKind  = N_G_L_C;
                      }
                    }

                    _myEncoding._nodeType._subkind = lSubKind;

                    // need to adjust _regpPxO

                    switch(lSubKind) {
                      case N_L_L_A:
                                    break;
                      case N_L_L_B:
                                    _regpPxO.initFromData2dim(brdd(), 5, 5, data());
                                    break;
                      case N_L_L_C:
                                    _regpPxO.initFromData2dim(brdd(), 6, 6, data());
                                    break;
                     default: assert(0 == 1);
                              break;
                    }
                  }
                } else {
                  // inner node
                  uint x = _myEncoding.noChildPtr();
                  _myEncoding._nodeType._subkind = x;
                }
              }
              break;
    case N_M: {
                
                if(_myEncoding._isLeaf) {
                  // leaf node
                  // Leaf_A1 = 2, // with BR     // kind code 01 //  3x3, 4 bit
                  // Leaf_A2 = 3, // without BR  // kind code 00 //  2x2, 5 bit
                  // Leaf_B1 = 4, // with BR     // kind code 01 //  4x4, 2 bit
                  // Leaf_B2 = 5, // without BR  // kind code 01 //  4x4, 3 bit
                  // Leaf_C2 = 6, // without BR  // kind code 10 //  5x5, 2 bit
                  // Leaf_A1 can hold a BR in case relevant for N_M.
                  if(needsBr()) {
                    _hasBr = 1;
                    _myEncoding._brKind = 0x1;
                    _myEncoding._brCode = brCode();
                    // can only be Leaf_A1 or Leaf_B1
                    // for a first test:
                    // ToDo
                    _myEncoding._nodeType._subkind = N_M_L_A;
                    _brdd = brd();
                    if(1 <= aGxTree.leafRefinement()) {
                      fill_mm2(_br_mm2, _br_regp16x16, aGxTree);
                      const uint lMaxTF3x3 = _regpPxO.max();
                      const uint lMaxTF4x4 = _br_mm2._4x4.max();
                      if((lMaxTF4x4 <= Node_M_Generic::_scaleGrid[0][Node_M_Generic::Leaf_B1]->limit())
                         && (lMaxTF4x4 < (lLrf * lMaxTF3x3))) {
                        _myEncoding._nodeType._subkind = N_M_L_B;
                        // std::cout << "DDNT N_M_L_B_1" << std::endl;
                      }
                    }
                  } else {
                    _hasBr = 0;
                    _myEncoding._brKind = 0;
                    _myEncoding._brCode = 0;
                    // can be Leaf_A2, Leaf_B2, Leaf_C2
                    // for a first test:
                    _brdd = tr();
                    _myEncoding._nodeType._subkind = N_M_L_A;
                    if(1 <= aGxTree.leafRefinement()) {
                      fill_mm2(_tr_mm2, _tr_regp16x16, aGxTree);
                      const uint lMaxTF3x3 = _regpPxO.max();
                            uint lMinMaxTF = lMaxTF3x3;
                      const uint lMaxTF4x4 = _tr_mm2._4x4.max();
                      if((lMaxTF4x4 <= Node_M_Generic::_scaleGrid[0][Node_M_Generic::Leaf_B2]->limit())
                         && (lMaxTF4x4 < lMaxTF3x3)) {
                        _myEncoding._nodeType._subkind = N_M_L_B;
                        lMinMaxTF = lMaxTF4x4;
                        if(2 <= aGxTree.leafRefinement()) {
                          RegularPartitioning2dim lRegP5x5(brdd(), 5, 5, data());
                          const uint lMaxTF5x5 = lRegP5x5.max();
                          if((lMaxTF5x5 <= Node_M_Generic::_scaleGrid[0][Node_M_Generic::Leaf_C2]->limit())
                            && (lMaxTF5x5 < (lLrf * lMinMaxTF))) {
                            _myEncoding._nodeType._subkind = N_M_L_C;
                            // std::cout << "DDNT N_M_L_C_2" << std::endl;
                          }
                        } else {
                          // std::cout << "DDNT N_M_L_B_2" << std::endl;
                        }
                      }
                    }
                  }

                  // need to adjust _regpPxO

                  switch(_myEncoding._nodeType._subkind) {
                    case N_M_L_A:
                                  break;
                    case N_M_L_B: 
                                  _regpPxO.initFromData2dim(brdd(), 4, 4, data());
                                  break;
                    case N_M_L_C:
                                  _regpPxO.initFromData2dim(brdd(), 5, 5, data());
                                  break;
                    default: assert(0 == 1);
                             break;
                  }

                } else {
                  // inner node
                  // only Inner_A1, with BR
                  //      Inner_A2, noBR
                  //      Inner_B2, noBR
                  // ToDo CHECK ToDo
                  if(anares().brDoesFit()) {
                    _myEncoding._nodeType._subkind = N_M_I_A1;
                    _hasBr = 1;
                    _myEncoding._brKind = 0x1;
                    _myEncoding._brCode = brCode();
                  } else {
                    if(   (0 == _myEncoding._childCount[N_M])
                       || (0 == _myEncoding._childCount[N_S])) {
                      _myEncoding._nodeType._subkind = N_M_I_A2;
                      _hasBr = 0;
                      _myEncoding._brKind = 0;
                      _myEncoding._brCode = 0;
                    } else {
                      _myEncoding._nodeType._subkind = N_M_I_B2;
                      _hasBr = 0;
                      _myEncoding._brKind = 0;
                      _myEncoding._brCode = 0;
                    }
                  }
                }
              }
              break;
    case N_S: {

                if(_myEncoding._isLeaf) {
                  if(needsBr()) {
                    // leaf node, BR needed
                    uint lMinMaxTF = _br_mm2._2x2.max();
                    if(lMinMaxTF <= Node_S_Generic::scaleGrid(
                                        Node_S_Generic::Leaf_A1)->limit()) {
                      _myEncoding._nodeType._subkind = N_S_L_A;
                      _hasBr = 1;
                      _myEncoding._brKind = 0x1;
                      _myEncoding._brCode = brCode();
                      _brdd = brd();
                    } else {
                      _myEncoding._nodeType._subkind = N_S_L_A;
                      _hasBr = 0;
                      _myEncoding._brKind = 0x0;
                      _myEncoding._brCode = 0x0;
                      _brdd = tr();
                      lMinMaxTF = _tr_mm2._2x2.max();
                    }

                    if(1 <= aGxTree.leafRefinement()) {
                      _mm3._filled = false;
                      fill_mm3(_mm3, tr(), data(), aGxTree);
                      const uint lMax3x3 = _mm3._3x3.max();
                      if( (lMax3x3 <= Node_S_Generic::scaleGrid(
                                      Node_S_Generic::Leaf_B2)->limit())
                          && lMax3x3 < (lLrf * lMinMaxTF)) {
                        _myEncoding._nodeType._subkind = N_S_L_B;
                        _hasBr = 0;
                        _myEncoding._brKind = 0x0;
                        _myEncoding._brCode = 0x0;
                        _brdd = tr();
                      }
                    }
                  } else {
                    // leaf node, no BR needed
                    uint lMinMaxTF = _tr_mm2._2x2.max();
                    _myEncoding._nodeType._subkind = N_S_L_A;
                    _hasBr = 0x0;
                    _myEncoding._brKind = 0x0;
                    _myEncoding._brCode = 0x0;
                    _brdd = tr();
                    if(1 <= aGxTree.leafRefinement()) {
                      _mm3._filled = false;
                      fill_mm3(_mm3, tr(), data(), aGxTree);
                      const uint lMax3x3 = _mm3._3x3.max();
                      if( (lMax3x3 <= Node_S_Generic::scaleGrid(
                                      Node_S_Generic::Leaf_B2)->limit())
                          && lMax3x3 < (lLrf * lMinMaxTF)) {
                        _myEncoding._nodeType._subkind = N_S_L_B;
                      }
                    }
                  }

                  // need to adjust _regpPxO
                  switch(_myEncoding._nodeType._subkind) {
                    case N_S_L_A:
                             // _regpPxO.initFromData2dim(brdd(), 2, 2, data());
                             break;
                    case N_S_L_B: 
                             _regpPxO.initFromData2dim(brdd(), 3, 3, data());
                             break;
                    default: assert(0 == 1);
                             break;
                  }
                } else {
                  // inner node: only Inner_A2, no BR
                  // can only have S children
                  _myEncoding._nodeType._subkind = N_S_I_A;
                  _hasBr = 0;
                  _myEncoding._brKind = 0;
                  _myEncoding._brCode = 0;
                }
              }
              break;
    default: assert(0 == 1);
             break;
  }
}



void
GxTree::Node::printEncodingInfo(std::ostream& os, const encoding_t& aEncoding) const {
  std::cout << std::setw( 3) << nodeId() << "  "
            << std::setw( 3) << _myEncoding.myIdx() << "  "
            ;
  switch(nodeType()) {
    case N_G: aEncoding._G[_myEncoding.myIdx()].printHeader(std::cout);
              break;
    case N_L: {
                const Node_L_Generic& lNode = aEncoding._L[_myEncoding.myIdx()];
                std::cout << " BR-E ";
                if(lNode.hasBr()) {
                  printBrCode(std::cout, lNode.brCode());
                  assert(lNode.brCode() == _myEncoding._brCode); // DANGEROUS
                } else {
                  std::cout << "----";
                }
              }
              break;
    case N_M:
    case N_S:
    default: std::cout << "  xhdrxxxx";
  };
  std::cout << "  " << cumFreq() << "   ";
  printBrCode(std::cout, _myEncoding._brCode);
  std::cout << "   "
            << _myEncoding._nodeType
            << std::endl;
  for(uint i = 0; i < cnx(); ++i) {
    for(uint j = 0; j < cny(); ++j) {
      if(0 != child(i,j)) {
        child(i,j)->printEncodingInfo(os, aEncoding);
      }
    }
  }
}


std::ostream&
GxTree::printEncodingInfo(std::ostream& os) const {
  if(0 != root()) {
     root()->printEncodingInfo(os, _encoding);
  }
  return os;
}

std::ostream&
GxTree::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_GXTREE) << ' '
            << kind() // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 


/*
 *   GxTreeItp
 */

// aligned according to node_type_t
GxTreeItp::estfun_t  GxTreeItp::_estfun[4] = { &GxTreeItp::estimateG,
                                               &GxTreeItp::estimateL,
                                               &GxTreeItp::estimateM,
                                               &GxTreeItp::estimateS };

GxTreeItp::GxTreeItp(const Data2dim&   aOutlier,
                     const encoding_t& aGxTreeEncoding,
                     const bool        aTrace)
          : EstimatorBase2dim(), _outlier(aOutlier), _encoding(aGxTreeEncoding), _trace(aTrace) {
}

GxTreeItp::~GxTreeItp() {
}

double
GxTreeItp::estimate(const rectangle_t& aQueryRectangle) const {
  double lRes = 0;
  lRes = estimate(aQueryRectangle,
                  _encoding._topBr,
                  0,
                  _encoding._rootType,
                  0);
  lRes += (double) outlierCount(aQueryRectangle);
  // const double lMinTheta = std::min<double>(theta(), 1000.0);
  // return ((lRes < lMinTheta) ? lMinTheta : lRes);
  return std::max<double>(1.0, lRes);
}


double
GxTreeItp::estimate(const query_t& lQuery) const {
  rectangle_t aQueryRectangle = lQuery.rectangle();
  return estimate(aQueryRectangle);
}

double
GxTreeItp::estimate(const rectangle_t& aQueryRectangle,
                    const rectangle_t& aTileRectangle,
                    const uint         aIdx,
                    const node_type_t  aNodeType,
                    const uint         aLevel) const {
  double lRes = 0;
  if(trace()) {
    std::cout << std::string(2 * aLevel, ' ') 
              << "GxTreeItp::estimate " 
              << aIdx << '@' << aLevel 
              << " of type " << aNodeType
              << std::endl;
  }
  if(aIdx >= _encoding._no[aNodeType]) {
    std::cout << "GxTreeItp: idx out of range: idx = " << aIdx << std::endl
              << "                             _no = " <<  _encoding._no[aNodeType] << std::endl
              << "                        nodetype = " << aNodeType << std::endl
              << "                           query = " << aQueryRectangle
              << std::endl;
  }

  assert(aIdx < _encoding._no[aNodeType]);   // XXX YYY

  lRes = (this->*_estfun[aNodeType])(aQueryRectangle,
                                     aTileRectangle,
                                     aIdx,
                                     aLevel);

  if(trace()) {
    std::cout << std::string(2 * aLevel, ' ') << "GxTreeItp::estimate " 
              << aIdx << '@' << aLevel 
              << " of type " << aNodeType
              << " returns " << lRes
              << std::endl;
  }

  return lRes;
}

double
GxTreeItp::estimateG(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aIdx,
                     const uint         aLevel) const {
  double lRes = 0;
  Node_G_Generic& lNodeGG = (_encoding._G[aIdx]);

  lRes = lNodeGG.estimate(aQueryRectangle,
                          aTileRectangle,
                          (*this),
                          trace(),
                          aIdx,
                          aLevel);

  return lRes;
}

double
GxTreeItp::estimateL(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aNodeIdx,
                     const uint         aLevel) const {
  double lRes = 0;
  Node_L_Generic& lNode = (_encoding._L[aNodeIdx]);
  lRes = lNode.estimate(aQueryRectangle,
                        aTileRectangle,
                        (*this),
                        trace(),
                        aNodeIdx,
                        aLevel);
  return lRes;
}

double
GxTreeItp::estimateM(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aNodeIdx,
                     const uint         aLevel) const {
  double lRes = 0;
  Node_M_Generic& lNode = (_encoding._M[aNodeIdx]);
  lRes = lNode.estimate(aQueryRectangle,
                        aTileRectangle,
                        (*this),
                        trace(),
                        aNodeIdx,
                        aLevel);
  return lRes;
}

double
GxTreeItp::estimateS(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aNodeIdx,
                     const uint         aLevel) const {
  double lRes = 0;
  Node_S_Generic& lNode = (_encoding._S[aNodeIdx]);
  lRes = lNode.estimate(aQueryRectangle,
                        aTileRectangle,
                        (*this),
                        trace(),
                        aNodeIdx,
                        aLevel);
  return lRes;
}


uint
GxTreeItp::outlierCount(const rectangle_t& r) const {
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(r.containsHalfOpen(p.x, p.y)) {
      lRes += p.c;
    }
  }
  return lRes;
}


std::ostream&
GxTreeItp::print(std::ostream& os) const {
  os << "GxTreeItp: " << std::endl;

  os << "G-Nodes" << std::endl;
  for(uint i = 0; i < _encoding._no[0]; ++i) {
    os << std::setw(3) << i << "  ";
    _encoding._G[i].print(os);
  }

  os << "L-Nodes" << std::endl;
  for(uint i = 0; i < _encoding._no[1]; ++i) {
    os << std::setw(3) << i << "  ";
    _encoding._L[i].print(os);
  }


  os << "M-Nodes" << std::endl;
  for(uint i = 0; i < _encoding._no[2]; ++i) {
    os << std::setw(3) << i << "  ";
    _encoding._M[i].print(os);
  }

  os << "S-Nodes" << std::endl;
  for(uint i = 0; i < _encoding._no[3]; ++i) {
    os << std::setw(3) << i << "  ";
    _encoding._S[i].print(os);
  }

  return os;
}

std::ostream&
GxTreeItp::printNodeTypes(std::ostream& os) const {
  uint n = 0;
  os << "NodeTypes: " << std::endl;

  os << "  #NKF#G: " << no_G() << ": ";
  {
    n = Node_G_Generic::NO_NODE_FINE;
    uint lCountG[n];
  
    for(uint i = 0; i < n; ++i) {
      lCountG[i] = 0;
    }

    for(uint i = 0; i < no_G(); ++i) {
      ++(lCountG[get_G(i).determineNodeKindFine()]);
    }

    for(uint i = 0; i < n; ++i) {
      os << ' ' << lCountG[i];
    }
    os << std::endl;
  }

  os << "  #NKF#L: " << no_L() << ": ";
  {
    n = Node_L_Generic::NO_NODE_FINE;
    uint lCountL[n];

    for(uint i = 0; i < n; ++i) {
      lCountL[i] = 0;
    }

    for(uint i = 0; i < no_L(); ++i) {
      ++(lCountL[get_L(i).determineNodeKindFine()]);
    }

    for(uint i = 0; i < n; ++i) {
      os << ' ' << lCountL[i];
    }
  }
  os << std::endl;


  os << "  #NKF#M: " << no_M() << ": ";
  {
    n = Node_M_Generic::NO_NODE_FINE;
    uint lCountM[n];
    for(uint i = 0; i < n; ++i) {
      lCountM[i] = 0;
    }

    for(uint i = 0; i < no_M(); ++i) {
      ++(lCountM[get_M(i).determineNodeKindFine()]);
    }

    for(uint i = 0; i < n; ++i) {
      os << ' ' << lCountM[i];
    }
  } 
  os << std::endl;

  os << "  #NKF#S: " << no_S() << ": ";
  {
    n = Node_S_Generic::NO_NODE_FINE;
    uint lCountS[n];
    for(uint i = 0; i < n; ++i) {
      lCountS[i] = 0;
    }
    for(uint i = 0; i < no_S(); ++i) {
      ++(lCountS[get_S(i).determineNodeKindFine()]);
    }
    for(uint i = 0; i < n; ++i) {
      os << ' ' << lCountS[i];
    }
  }
  os << std::endl;

  return os;
}

std::ostream&
GxTreeItp::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_GXTREE) << ' '
            << 0 // subkind
            << '(' 
            << ')'
            << std::endl;
  return os;
}

uint
GxTreeItp::size() const {
  return ( //  4 * 4   // topBf
            ( _encoding._no[0] * 28 ) // G
          + ( _encoding._no[1] * 16 ) // L
          + ( _encoding._no[2] *  8 ) // M
          + ( _encoding._no[3] *  4 ) // S
         );
}

} // end namespace


