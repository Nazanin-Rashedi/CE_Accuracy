#include "IQTS.hh"


namespace H2D {


/*
 *   IQTS::Node members
 */


IQTS::Node::Node(const Data2dim&     aData,
                const rectangle_t&   aBr,
                const uint           aLevel,
                const IQTS&          aIQTS,
                const bool           a11) // for this one, no cum freq stored
            : _data(aData),
              _br(aBr),
              _cumFreq(aData.total()),
              _sse(0),
              _child(),
              _level(aLevel),
              _nodeId(aIQTS.getNodeId()),
              _sizeInBits(2 + (a11 ? 32 : 0)),
              _hasRefinement(false),

	      _code(0LL) {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      _child[i][j] = 0;
    }
  }

  prepareEncoding(aData, aBr, aIQTS);
  if(hasRefinement()) {
    _sizeInBits += 64;
  }

  if(k_l2_sse == aIQTS.kind() || k_lq_sse == aIQTS.kind()) {
    _sse = aData.sse(0, aData.size()); // in paper: sse! 
  } else
  if(k_l2_var == aIQTS.kind() || k_lq_var == aIQTS.kind()) {
    _sse = aData.variance(0, aData.size()); 
  } else
  if(k_l2_card == aIQTS.kind() || k_lq_card == aIQTS.kind()) {
    _sse = aData.total();
  } else {
    assert(0 > 1);
  }
}

IQTS::Node::~Node() {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i,j)) {
        delete _child[i][j];
      }
    }
  }
}


void
IQTS::Node::performSplit(const IQTS& aIQTS) {
  if(aIQTS.trace()) {
    std::cout << "split Node" << nodeId() << '@' << level() << ": "
              << "   w = " << sse()
              << std::endl;
  }

  data2dim_at        lPartition(2,2);
  partitiondescxy_t  lPdXY(br(), 2, 2);
  for(uint i = 0; i < data().size(); ++i) {
    const xyc_t& e = data()[i];
    const uint lIdxX = lPdXY.idxXcorrected(e.x);
    const uint lIdxY = lPdXY.idxYcorrected(e.y);
    if(lIdxX < 2 && lIdxY < 2) {
      lPartition(lIdxX, lIdxY).push_back(e);
    }
  }


  rectangle_t lChildBr;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 < lPartition(i,j).size()) {
        lPdXY.getRectangle(i, j, lChildBr);
        _child[i][j]  = new Node(lPartition(i,j), lChildBr, level() + 1, aIQTS, 2 == (i+j));
      }
      if(aIQTS.trace()) {
        std::cout << "   size(" << i << ',' << j << ") = " << lPartition(i,j).size() << std::endl;
      }
    }
  }
}

void
IQTS::Node::prepareEncoding(const Data2dim& aData, const rectangle_t aBr, const IQTS& aIQTS) {
  const double lTotal = aData.total();

  if(0 >= lTotal) {
    _hasRefinement = false;
    _code = 0LL;
    return;
  }

  RegularPartitioning2dim lRegP4x4(aBr, 4, 4, aData);
  RegularPartitioning2dim lRegP8x8(aBr, 8, 8, aData);

  Matrix M4x4(4,4);
  Matrix M8x8(8,8);


  const double lAvg   = lRegP8x8.matrix().sumAllElements() / (8*8);
  const double lTheta = lAvg * aIQTS.q(); // aIQTS.theta() / 8;

  double lMinErr    = (aIQTS.useL2() ? errorL2(lRegP8x8.matrix(), lAvg) : errorLQ(lRegP8x8.matrix(), lAvg, lTheta));
  double lErr       = 0;
  uint64_t lCode = 0LL;


  if(aIQTS.trace()) {
    std::cout << "IQTS::Node::prepareEncoding" << std::endl;
    std::cout << "  lAvg    = " << lAvg << std::endl;
    std::cout << "  lMinErr = " << lMinErr << " after avg only" << std::endl;
  }

  _hasRefinement = false;
  _code = 0LL;

  // check 2/3-LT
  {
    lCode = encode23LT(lRegP4x4.matrix(), 0, 0);
    assert(k_lt_23 == getKindLT(lCode));
    decode23LT(lCode, lTotal, M4x4, 0, 0);
    if(aIQTS.useL2()) {
      lErr = errorL2_4x4(lRegP8x8.matrix(), M4x4);
    } else {
      lErr = errorLQ_4x4(lRegP8x8.matrix(), M4x4, lTheta);
    }
  }
  if(lErr < lMinErr) {
    lMinErr = lErr;
    _hasRefinement = true;
    _code = lCode;
  }


  if(aIQTS.trace()) {
    std::cout << "  lErr    = " << lErr << " of 2/3-LT" << std::endl;
    std::cout << "  lMinErr = " << lMinErr << std::endl;
  }

  // check all 2/4 LTs
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      for(uint k = 0; k < 2; ++k) {
        for(uint l = 0; l < 2; ++l) {
          if(i == k && j ==l) { continue; }
          const uint lEnc = (i << 3) | (j << 2) | (k << 1) | (l << 0);
          lCode = encode24LT(lRegP8x8.matrix(), lEnc);
          assert(k_lt_24 == getKindLT(lCode));
          decode24LT(lCode, lTotal, M8x8);
          if(aIQTS.useL2()) {
            lErr = errorL2_4x4(lRegP8x8.matrix(), M8x8);
          } else {
            lErr = errorLQ_4x4(lRegP8x8.matrix(), M8x8, lTheta);
          }
          if(lErr < lMinErr) {
            lMinErr = lErr;
            _hasRefinement = true;
            _code = lCode;
          }
          if(aIQTS.trace()) {
            std::cout << "  lErr    = " << lErr << " of 2/4-LT : " << lEnc << std::endl;
            std::cout << "  lMinErr = " << lMinErr << std::endl;
          }
        }
      }
    }
  }
}


double
IQTS::Node::errorL2(const Matrix& M8x8Tru, const double aAvg) {
  double lRes = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = M8x8Tru(i,j);
      lRes += (x - aAvg) * (x - aAvg);
    }
  }
  return lRes;
}

double 
IQTS::Node::errorLQ(const Matrix& M8x8Tru, const double aAvg, const double aTheta) {
  double lRes = 0;
  const double lAvg = (aAvg > 0) ? aAvg : 1;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = M8x8Tru(i,j);
      if(x > aTheta) {
        const double lErr = q::qerrorT1<double>(x, lAvg);
        if(lErr > lRes) {
           lRes = lErr;
        }
      }
    }
  }
  return lRes;
}


double 
IQTS::Node::errorL2_4x4(const Matrix& M8x8Tru, const Matrix& M4x4Est) {
  double lRes = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = M8x8Tru(i,j);
      const double y = M4x4Est(i >> 1,j >> 1);
      lRes += (x - y) * (x - y);
    }
  }
  return lRes;
}

double 
IQTS::Node::errorLQ_4x4(const Matrix& M8x8Tru, const Matrix& M4x4Est, const double aTheta) {
  double lRes = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = std::max<double>(1, M8x8Tru(i,j));
      const double y = std::max<double>(1, M4x4Est(i >> 1,j >> 1));
      if(x > aTheta || y > aTheta) {
        const double lErr = q::qerrorT1<double>(x, y);
        if(lErr > lRes) {
          lRes = lErr;
        }
      }
    }
  }
  return lRes;
}

double 
IQTS::Node::errorL2_8x8(const Matrix& M8x8Tru, const Matrix& M8x8Est) {
  double lRes = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = M8x8Tru(i,j);
      const double y = M8x8Est(i,j);
      lRes += (x - y) * (x - y);
    }
  }
  return lRes;
}

double 
IQTS::Node::errorLQ_8x8(const Matrix& M8x8Tru, const Matrix& M8x8Est, const double aTheta) {
  double lRes = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      const double x = std::max<double>(1, M8x8Tru(i,j));
      const double y = std::max<double>(1, M8x8Est(i,j));
      if(x > aTheta || y > aTheta) {
        const double lErr = q::qerrorT1<double>(x, y);
        if(lErr > lRes) {
          lRes = lErr;
        }
      }
    }
  }
  return lRes;
}





double
IQTS::Node::estimate(const rectangle_t& aQueryRectangle, const rectangle_t& aBr, const IQTS& aIQTS) const {
  if(aIQTS.trace()) {
    std::cout << std::string(2*level(), ' ')
              << "Node[" << nodeId() << "]::estimate: " << aQueryRectangle << "  " << aBr << std::endl;
  }

  // if the query rectangle contains the bounding rectangle, we are done
  if(aQueryRectangle.contains(aBr)) {
    if(aIQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note: fully contained." << std::endl;
    }
    return cumFreq();
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);

  // if the intersection between query rectangle and bounding rectangle is empty, we are done
  if(lIsec.hasZeroArea()) {
    if(aIQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note empty intersection." << std::endl;
    }
    return 0;
  }


  if(isLeaf()) {
    double lEstimate = 0;
    if(hasRefinement()) {
      switch(getKindLT()) {
        case k_lt_23:
           lEstimate = estimateLT23(aQueryRectangle, aBr, aIQTS);
           break;
        case k_lt_24:
           lEstimate = estimateLT24(aQueryRectangle, aBr, aIQTS);
           break;
        case k_lt_2p:
           std::cout << "IQTS::Node::estimate: k_lt_2p: NYI." << std::endl;
           std::cerr << "IQTS::Node::estimate: k_lt_2p: NYI." << std::endl;
           assert(0 > 1);
           break;
        default:
           std::cout << "IQTS::Node::estimate: OOPS." << std::endl;
           std::cerr << "IQTS::Node::estimate: OOPS." << std::endl;
           assert(0 > 1);
           break;
      }
    } else {
      lEstimate =  ((lIsec.area() / aBr.area()) * cumFreq());
      if(aIQTS.trace()) {
        std::cout << std::string(2*level(), ' ')
                  << "note: non-refined leaf with cumfreq: " << cumFreq() 
                  << " results in estimate " << lEstimate
                  << std::endl;
      }
    }
    return lEstimate;
  }

  double lEstimate = 0;

  partitiondescxy_t lPd(aBr, 2, 2);
  rectangle_t       lTile;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lPd.getRectangle(i, j, lTile);
        lEstimate += child(i, j)->estimate(lIsec, lTile, aIQTS);
      }
    }
  }

  return lEstimate;
}


double
IQTS::Node::estimateLT23(const rectangle_t& aQueryRectangle,
                         const rectangle_t& aBoundingRectangle,
                         const IQTS&         aIQTS) const {
  double lEstimate = 0;

  Matrix M4x4(4,4);
  decode23LT(code(), cumFreq(), M4x4, 0, 0);

  partitiondescxy_t lPd(aBoundingRectangle, 4, 4);

  rectangle_t lTile;
  rectangle_t lIsec;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      lPd.getRectangle(i, j, lTile);
      lIsec.isec(aQueryRectangle, lTile);
      if(!lIsec.hasZeroArea()) {
        lEstimate += (lIsec.area() / lTile.area()) * M4x4(i,j);
      }
    }
  }

  if(aIQTS.trace()) {
    std::cout << std::string(2*level(), ' ')
              << "note: 2/3-LT leaf: " << cumFreq() 
              << " results in estimate " << lEstimate
              << std::endl;
  }

  return lEstimate;
}

double
IQTS::Node::estimateLT24(const rectangle_t& aQueryRectangle,
                         const rectangle_t& aBoundingRectangle,
                         const IQTS&         aIQTS) const {
  double lEstimate = 0;
  Matrix M8x8(8,8);
  decode24LT(code(), cumFreq(), M8x8);

  partitiondescxy_t lPd(aBoundingRectangle, 4, 4);

  rectangle_t lTile;
  rectangle_t lIsec;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      lPd.getRectangle(i, j, lTile);
      lIsec.isec(aQueryRectangle, lTile);
      if(!lIsec.hasZeroArea()) {
        lEstimate += (lIsec.area() / lTile.area()) * M8x8(i,j);
      }
    }
  }

  if(aIQTS.trace()) {
    std::cout << std::string(2*level(), ' ')
              << "note: 2/4-LT leaf: " << cumFreq() 
              << " results in estimate " << lEstimate
              << std::endl;
  }

  return lEstimate;
}

uint
IQTS::Node::depth() const {
  uint lChildDepth = 0;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        const uint d = child(i, j)->depth();
        if(d > lChildDepth) {
          lChildDepth = 0;
        }
      }
    }
  }
  return 1 + lChildDepth;
}

uint
IQTS::Node::noNodes() const {
  uint lRes = 1; // this
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noNodes();
      }
    }
  } 
  return lRes;
}


uint
IQTS::Node::noNotRefined() const {
  uint lRes = hasRefinement() ? 0 : 1; // this
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noNotRefined();
      }
    }
  }
  return lRes;
}

uint
IQTS::Node::noLT23() const {
  uint lRes = 0;
  if(hasRefinement() && k_lt_23 == getKindLT()) {
    lRes = 1;
  }
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noLT23();
      }
    }
  }
  return lRes;
}

uint
IQTS::Node::noLT24() const {
  uint lRes = 0;
  if(hasRefinement() && k_lt_24 == getKindLT()) {
    lRes = 1;
  }
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noLT24();
      }
    }
  }
  return lRes;
}


uint
IQTS::Node::noLT2p() const {
  uint lRes = 0;
  if(hasRefinement() && k_lt_2p == getKindLT()) {
    lRes = 1;
  }
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noLT2p();
      }
    }
  }
  return lRes;
}


std::ostream&
IQTS::Node::print(std::ostream& os) const {
  std::cout << std::string(2 * level(), ' ') 
            << "node[" << nodeId() << "] "
            << " br " << br() 
            << " cf " << cumFreq() 
            << " ref " << hasRefinement() 
            << " LTk " << getKindLT() 
            << " bv  " << Bitvector64(code())
            << std::endl;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        child(i, j)->print(os);
      }
    }
  }
  return os;
}


uint64_t
IQTS::Node::encode2x2(const double f00, const double f01, const double f10, const double f11,
                      const double aTotal, const uint aNoBits1, const uint aNoBits2) {
  uint64_t lRes = 0LL;
  lRes = ((((encode(f00 + f01, aTotal, aNoBits1) << aNoBits1) |
             encode(f00 + f10, aTotal, aNoBits1)) << aNoBits2) |
             encode(f00, std::min<double>(f00 + f01, f00 + f10), aNoBits2));
  // std::cout << "CA = "   << encode(f00 + f01, aTotal, aNoBits1)
  //           << ", CB = " << encode(f00 + f10, aTotal, aNoBits1)
  //           << ", CC = " << encode(f00, std::min<double>(f00 + f01, f00 + f10), aNoBits2)
  //           << std::endl;
  return lRes;
}

void
IQTS::Node::decode2x2(const uint64_t aCode, const double aTotal, const uint aNoBits1, const uint aNoBits2,
                      double& f00, double& f01, double& f10, double& f11) {
  const uint64_t lCodeA = (aCode >> (aNoBits1 + aNoBits2)) & ((1 << aNoBits1) - 1);
  const uint64_t lCodeB = (aCode >> (           aNoBits2)) & ((1 << aNoBits1) - 1);
  const uint64_t lCodeC = (aCode >> (                  0)) & ((1 << aNoBits2) - 1);
  // std::cout << "CA = " << lCodeA << ", CB = " << lCodeB << ", CC = " << lCodeC << std::endl;
  const double A = decode(lCodeA, aTotal, aNoBits1);
  const double B = decode(lCodeB, aTotal, aNoBits1);
  const double C = decode(lCodeC, std::min<double>(A,B), aNoBits2);
  // std::cout << "A = " << A << ", B = " << B << ", C = " << C << std::endl;
  f00 = C;
  f01 = A - C;
  f10 = B - C;
  f11 = aTotal - A - B + C;
}

uint64_t
IQTS::Node::encode2x2(const double f00, const double f01, const double f10, const double f11,
                      const uint aNoBits1, const uint aNoBits2) {
  const double lTotal = f00 + f01 + f10 + f11;
  return encode2x2(f00, f01, f10, f11, lTotal, aNoBits1, aNoBits2);
}

uint64_t
IQTS::Node::encode2x2(const Matrix& M, const uint aStartRowId, const uint aStartColId,
                      const uint aNoBits1, const uint aNoBits2) {
  const uint i = aStartRowId;
  const uint j = aStartColId;
  return encode2x2(M(i,j), M(i,j+1), M(i+1,j), M(i+1, j+1), aNoBits1, aNoBits2);
}

void
IQTS::Node::decode2x2(const uint64_t aCode, const double aTotal, const uint aNoBits1, const uint aNoBits2,
                      Matrix& M, const uint aStartRowId, const uint aStartColId) {
  const uint i = aStartRowId;
  const uint j = aStartColId;
  decode2x2(aCode, aTotal, aNoBits1, aNoBits2, M(i,j), M(i, j+1), M(i+1, j), M(i+1, j+1));
}

uint64_t
IQTS::Node::encode4x4(const Matrix& M, const uint aStartRowId, const uint aStartColId,
                      const uint aNoBits01, const uint aNoBits02,
                      const uint aNoBits11, const uint aNoBits12) {
  const bool lTrace = false;
  // const uint lNoBits01 = 6;
  // const uint lNoBits02 = 5;
  // const uint lNoBits11 = 4;
  // const uint lNoBits12 = 3;
  // const uint lNoBits0 = lNoBits01 + lNoBits01 + lNoBits02;

  const uint lNoBits1 = aNoBits11 + aNoBits11 + aNoBits12;

  Matrix M2x2(2,2);

  // reduce M to M2x2
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      double lSum = 0;
      for(uint k = 0; k < 2; ++k) {
        for(uint l = 0; l < 2; ++l) {
          lSum += M(aStartRowId + 2*i + k, aStartColId + 2*j + l);
        }
      }
      M2x2(i,j) = lSum;
    }
  }


  if(lTrace) {
    std::cout << "encode4x4: M2x2:" << std::endl;
    M2x2.print(std::cout, 3);
  }

  uint64_t lRes = 0LL;
  // const double lTotal = M2x2(0,0) + M2x2(0,1) + M2x2(1,0) + M2x2(1,1);
  lRes = encode2x2(M2x2, 0, 0, aNoBits01, aNoBits02);

  if(lTrace) {
    Bitvector64 lBv(lRes);
    std::cout << "encode4x4: bitvector for level 1: " << std::endl << lBv << std::endl;
  }

  // encode submatrices
  for(uint i = 0; i < 4; i += 2) {
    for(uint j = 0; j < 4; j += 2) {
      lRes <<= lNoBits1;
      const uint64_t lEnc2x2 = encode2x2(M, i, j, aNoBits11, aNoBits12);
      lRes |= lEnc2x2;
      // std::cout << "encode4x4: 2x2:" << Bitvector64(lEnc2x2) << std::endl;
      // std::cout << "encode4x4: sub:" << Bitvector64(lRes) << std::endl;
    }
  }

  if(lTrace) {
    Bitvector64 lBv = lRes;
    std::cout << "encode4x4: complete bv:" << std::endl << lBv << std::endl;
  }

  return lRes;
}



void
IQTS::Node::decode4x4(const uint64_t aCode, const double aTotal, 
                      Matrix& M, const uint aStartRowId, const uint aStartColId,
                      const uint aNoBits01, const uint aNoBits02,
                      const uint aNoBits11, const uint aNoBits12) {
  // const uint lNoBits0 = lNoBits01 + lNoBits01 + lNoBits02;
  const bool lTrace = false;

  const uint lNoBits1 = aNoBits11 + aNoBits11 + aNoBits12;

  Matrix M2x2(2,2);

  if(lTrace) {
    std::cout << "decode4x4: aTotal = " << aTotal << std::endl;
    Bitvector64 lBv(aCode >> (4 * lNoBits1));
    std::cout << "decode4x4: shifted back to top:" << std::endl << lBv << std::endl;
  }
  
  decode2x2((aCode >> (4 * lNoBits1)), aTotal, aNoBits01, aNoBits02, M2x2, 0, 0);

  if(lTrace) { 
    std::cout << "decode4x4 M2x2:" << std::endl;
    M2x2.print(std::cout, 3);
    std::cout << std::endl;
  }


  // encode submatrices
  uint64_t lCode = aCode;
  for(int i = 1; i >= 0; --i) {
    for(int j = 1; j >= 0; --j) {
      decode2x2(lCode, M2x2(i, j), aNoBits11, aNoBits12, M, aStartRowId + 2*i, aStartColId + 2*j);
      lCode >>= lNoBits1;
    }
  }
 
}

uint64_t
IQTS::Node::encode24LT(const Matrix& M8x8, const uint aQEnc) {
  // aQEnc is 4 bit encoding of quadrants with 4x4 and 2x2 resolution
  Matrix M4x4;
  Matrix M2x2;
  M4x4.setToShrink2Of(M8x8);
  M2x2.setToShrink2Of(M4x4);

  const bool lTrace = false;

  if(lTrace) {
     std::cout << "encode24LT M8x8:" << std::endl;
     M8x8.print(std::cout, 4);
     std::cout << "encode24LT M4x4:" << std::endl;
     M4x4.print(std::cout, 4);
     std::cout << "encode24LT M2x2:" << std::endl;
     M2x2.print(std::cout, 4);
  }
  
  const uint64_t lEnc2x2 = encode2x2(M2x2, 0, 0, 6, 5); // total 6 + 6 + 5 = 17 bits
  const uint i = (aQEnc & 0x2);        // ((aQEnc & 0x2) >> 1) << 1;
  const uint j = ((aQEnc & 0x1) << 1); // ((aQEnc & 0x1) >> 0) << 1;
  const uint64_t lEnc4x4 = encode2x2(M4x4, i, j, 4, 3);       // total 4 + 4 + 3 = 11 bits
  const uint k = (aQEnc & 0x8) >> 1;
  const uint l = (aQEnc & 0x4);

  if(lTrace) {
    std::cout << "(i,j) = (" << i << "," << j << "), (k,l) = (" << k << ", " << l << ")" << std::endl;
  }

  const uint64_t lEnc8x8 = encode4x4(M8x8, k, l, 4, 3, 2, 1); // total 4 + 4 + 3 + 4*(2 + 2 + 1) = 31 bits
  const uint64_t lRes = (((uint64_t) 0x1 << 63) |
                         ((uint64_t) aQEnc << 59) |
                         (lEnc2x2 << 42) |
                         (lEnc4x4 << 31) |
                         (lEnc8x8));

  /*
   std::cout << "encode8x8: 2x2, 4x4, 8x8, res:" << std::endl
             << Bitvector64((uint64_t) 0x1 << 63) << std::endl
             << Bitvector64((uint64_t) aQEnc << 59) << std::endl
             << Bitvector64(lEnc2x2) << std::endl
             << Bitvector64(lEnc4x4) << std::endl
             << Bitvector64(lEnc8x8) << std::endl
             << Bitvector64(lRes)     << std::endl
             << std::endl;
  */


  return lRes;
}


bool
IQTS::Node::decode24LT(const uint64_t aCode, const double aTotal, Matrix& M8x8) {
  const bool lTrace = false;

  const uint lThe24Bit = ((aCode >> 63) & 0x1);
  if(0x1 != lThe24Bit) {
    std::cout << "IQTS::Node::decode24LT: OOPS" << std::endl;
    std::cerr << "IQTS::Node::decode24LT: OOPS" << std::endl;
    assert(0 > 1);
    return false;
  }

  const uint lEnc = (aCode >> 59) & 0xF;
  const uint lEnc2x2 = (aCode >> 42) & ((1 << 17) - 1);
  const uint lEnc4x4 = (aCode >> 31) & ((1 << 11) - 1);
  const uint lEnc8x8 = (aCode >> 0) & (((uint32_t) 1 << 31) - 1);

  // Bitvector32 lBvEnc(lEnc);
  // std::cout << "decode8x8: lEnc = ";
  // lBvEnc.printReverse(std::cout, 4);
  // std::cout << "  " << lEnc << ", 2x2, 4x4, 8x8: " << std::endl
  //           << Bitvector64(lEnc2x2) << std::endl
  //           << Bitvector64(lEnc4x4) << std::endl
  //           << Bitvector64(lEnc8x8) << std::endl
  //           << std::endl;

  Matrix M2x2(2,2);
  Matrix M4x4(4,4);

  decode2x2(lEnc2x2, aTotal, 6, 5, M2x2, 0, 0);
  const uint i = (lEnc & 0x2);
  const uint j = ((lEnc & 0x1) << 1);
  const uint k = (lEnc & 0x8) >> 1;
  const uint l = (lEnc & 0x4);

  if(lTrace) {
    std::cout << "(i,j) = (" << i << "," << j << "), (k,l) = (" << k << ", " << l << ")" << std::endl;
  }

  decode2x2(lEnc4x4, M2x2(i >> 1,j >> 1), 4, 3, M4x4, i, j);
  decode4x4(lEnc8x8, M2x2(((lEnc >> 3) & 0x1), ((lEnc >> 2) & 0x1)), M8x8, k, l, 4, 3, 2, 1);


  if(lTrace) {
    std::cout << "decode: M2x2, M4x4, M8x8: " << std::endl;
    std::cout << "M2x2:" << std::endl;
    M2x2.print(std::cout, 6);
    std::cout << "M4x4:" << std::endl;
    M4x4.print(std::cout, 6);
    std::cout << "M8x8:" << std::endl;
    M8x8.print(std::cout, 6);
    std::cout << std::endl;\
  }

  // std::cout << "M8x8 before copying:" << std::endl;
  // M8x8.print(std::cout, 6);
  // std::cout << "----------------------------" << std::endl;

  // copy stuff to M8x8
  // start with copying M4x4 averages to M8x8
  for(uint ii = 0; ii < 2; ++ii) {
    for(uint jj = 0; jj < 2; ++jj) {
      const double lAvgFreq = M4x4(i + ii, j + jj) / 4;
      // std::cout << "* i+ii, j+jj, M4x4(..) = " << (i+ii) << ", " << (j+jj) << ", " 
      //           << M4x4(i + ii, j + jj)  << ", " << lAvgFreq
      //           << std::endl;
      for(uint ll = 0; ll < 2; ++ll) {
        for(uint kk = 0; kk < 2; ++kk) {
           // std::cout << "x, y = " << ((i << 1) + (ii << 1) + ll) << ", " << ((j << 1) + (jj << 1) + kk) << std::endl;
           M8x8(((i << 1) + (ii << 1) + ll), ((j << 1) + (jj << 1) + kk)) = lAvgFreq;
           // M8x8.print(std::cout, 6);
        }
      }
    }
  }

  if(lTrace) {
    std::cout << "M8x8 after copying 1:" << std::endl;
    M8x8.print(std::cout, 6);
  }

  // copy 2x2 average to remaining quadrants of M8x8
  for(uint qi = 0; qi < 2; ++qi) {
    for(uint qj = 0; qj < 2; ++qj) {
      const uint qij = (qi << 1) | qj;
      if(qij == (lEnc & 0x3) || qij == ((lEnc >> 2) & 0x3)) {
        continue;
      }
      const double lAvg = M2x2(qi, qj) / (4*4);
      const uint   lStartRow = qi << 2;
      const uint   lStartCol = qj << 2;

      if(lTrace) {
        Bitvector32 lBvQij(qij);
        std::cout << "lBvQij: ";
        lBvQij.print(std::cout, 2);
        std::cout << ", sr, sc = " << lStartRow << ", " << lStartCol << std::endl;
      }

      for(uint ii = lStartRow; ii < lStartRow + 4; ++ii) {
        for(uint jj = lStartCol; jj < lStartCol + 4; ++jj) {
          M8x8(ii, jj) = lAvg;
        }
      }


    }
  }

  if(lTrace) {
    std::cout << "M8x8 after copying 2:" << std::endl;
    M8x8.print(std::cout, 6);
  }



  return true;
}

uint64_t
IQTS::Node::encode23LT(const Matrix& M, const uint aStartRowId, const uint aStartColId) {
  // const uint lNoBits01 = 6;
  // const uint lNoBits02 = 5;
  // const uint lNoBits11 = 4;
  // const uint lNoBits12 = 3;
  uint64_t lCode = encode4x4(M, aStartRowId, aStartColId, 6, 5, 4, 3);
  // uint64_t lIdct = (((uint64_t) 0x0LL) << 61);
  // return (lIdct | lCode);

  // std::cout << "encode23LT: " << Bitvector64(lCode) << std::endl;
  return lCode;
}


bool
IQTS::Node::decode23LT(const uint64_t aCode, const double aTotal,
                       Matrix& M, const uint aStartRowId, const uint aStartColId) {
  uint32_t lLtKind = ((aCode >> 61) & 0x7);
  if(0 != lLtKind) {
    return false;
  }

  decode4x4(aCode, aTotal, M, aStartRowId, aStartColId, 6, 5, 4, 3);

  return true;
}

/* 
 *   IQTS members 
 */

IQTS::IQTS(const Data2dim& aData,
         const kind_t    aKind,
         const uint      aBudget,
         const uint      aPhi,
         const double    aQ,
         const double    aTheta,
         const bool      aTrace) : EstimatorBase2dim(aQ, aTheta),
                                   _outlier(),
                                   _br(),
                                   _kind(aKind),
                                   _budget(aBudget * 8),
                                   _size(0),
                                   _phi(aPhi),
                                   _root(0),
                                   _nodeCount(0),
                                   _trace(aTrace) {
  init(aData);

  // print(std::cout);

  // first test [only for longlat.dat]
  if(false) {
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

IQTS::~IQTS() {
  if(0 != _root) {
    delete _root;
  }
}



void
IQTS::init(const Data2dim& aData) {
  Data2dim lRegular;
  if(8 < phi()) {
    aData.split(lRegular, _outlier, phi());
  } else {
    lRegular = aData;
  }

  lRegular.getBoundingRectangle(_br);

  heap_t lHeap;
  _root = new Node(aData, br(), 0, (*this), false);
  lHeap.push(_root);

  uint lTotalSize = root()->sizeInBits();

  if(trace()) {
    std::cout << "before main loop: " << budget() << " <?= " << lTotalSize << std::endl;
  }
  while(budget() > lTotalSize) {
    Node* lTop = lHeap.top();
    lHeap.pop();
    if(7 > lTop->data().size()) { continue; }
    lTop->performSplit(*this);
    for(uint i = 0; i < 2; ++i) {
      for(uint j = 0; j < 2; ++j) {
        Node* lChild = lTop->child(i,j);
        if(0 != lChild) {
          lHeap.push(lChild);
          lTotalSize += lChild->sizeInBits();
        }
      }
    }
    if(trace()) {
      std::cout << "within main loop: " << budget() << " <?= " << lTotalSize << std::endl;
    }
  }

  _size = lTotalSize;

}



double
IQTS::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}



double
IQTS::estimate(const rectangle_t& r) const {
  double lEstimate = 0;
  if(0 != root()) {
    lEstimate = root()->estimate(r, br(), (*this));
  }
  lEstimate += (double) outlierCount(r);
  return std::max<double>(1.0, lEstimate);
}



uint
IQTS::outlierCount(const rectangle_t& r) const {
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
IQTS::size() const {
  return (_size / 8);
}

uint
IQTS::depth() const {
  if(0 == root()) {
    return 0;
  }
  return root()->depth();
}

uint
IQTS::noNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noNodes();
}

uint
IQTS::noNotRefined() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noNotRefined();
}

uint
IQTS::noLT23() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noLT23();
}

uint
IQTS::noLT24() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noLT24();
}


uint
IQTS::noLT2p() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noLT2p();
}




std::ostream&
IQTS::print(std::ostream& os) const {
  std::cout << "IQTS: " << std::endl;
  std::cout << "  tree:" << std::endl;
  _root->print(os);
  return os;
}


std::ostream&
IQTS::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_IQTS) << ' '
            << kind() // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 



} // end namespace


