#include "EqDepHist.hh"

namespace H2D {


EqDepHist::EqDepHist(const Data2dim& aData,
                     const kind_t    aKind,
                     const uint      aNx,
                     const uint      aNy,
                     const uint      aRx, // for k_matrix == kind(): RegP no x
                     const uint      aRy, // for k_matrix == kind(): RegP no y
                     const uint      aPhi,
                     const double    aQ,
                     const double    aTheta,
                     const bool      aTrace)
              : EstimatorBase2dim(aQ, aTheta),
                _outlier(), 
                _kind(aKind),
                _nx(aNx), _ny(aNy), 
                _rx(aRx), _ry(aRy), 
                _phi(aPhi),
                _vx(),
                _vy(),
                _regp(0),
                _trace(aTrace) {
  if(trace()) {
    std::cout << "Equi-Depth-Histogram: params: " << aNx << ", " << aNy << ", " << aPhi << std::endl;
  }


  // determine _nx and _ny if not both are given in input
  if(1 >= aNx || 1 >= aNy) {
    const uint lTotal = aData.total();
    const double lPerc1 = round((double) ((2 * 4 * lTotal)) / (double) 100);
    if(k_matrix == kind()) {
      // const uint lEntrySize = 2 * domValSize() + freqValSize();
      _nx = _ny = (uint) floor(sqrt((lPerc1 / (12 + rx() * ry())) + 0.25) - 0.5);
    } else {
      _nx = _ny = (uint) floor(sqrt((lPerc1 / 12) + 0.25) - 0.5);
    }
    
    while(lPerc1 > size()) {
     ++_nx;
    }
  }

  if(2 > nx()) { _nx = 2; }
  if(2 > ny()) { _ny = 2; }


  if(trace()) {
    std::cout << "Equi-Depth-Histogram: nx, ny : " << nx() << ", " << ny() 
              << ", number of outliers: " << outlier().size() 
              << ", Equi-Depth-Hist::size : " << size()
              << ", Data::size: " << aData.size() 
              << ", Data::total: " << aData.total()
              << std::endl;
  }

  _vx.resize(nx());
  _vy.resize(ny());

  if(k_matrix == kind()) {
    _regp = new RegularPartitioning2dim[nx() * ny()];
  }

  Data2dim lRegular;
  if(8 < aPhi) {
    aData.split(lRegular, _outlier, aPhi);
  } else {
    lRegular = aData;
  }

  initVx(lRegular);


  // first test
  if(false) {
    rectangle_t lQueryRectangle(80.0, 30.0, 120.0, 40.0);
    double lTrueVal = aData.countWithin(lQueryRectangle);
    double lEstVal  = estimate(lQueryRectangle);
    std::cout << "query rectangle: " << lQueryRectangle << std::endl;
    std::cout << "est  val: " << lEstVal  << std::endl;
    std::cout << "true val: " << lTrueVal << std::endl;
  }
}

EqDepHist::~EqDepHist() {}

void
EqDepHist::initVx(Data2dim& aData) {
  const uint lTotal = aData.total();
  uint lCumFreqWish = lTotal / nx();
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }

  aData.sortx(0, aData.size());

  uint lCount = 0; // count entries assigned to vector 
  uint lBegin = 0; // beginning of current bucket, idx into aData
  uint lEnd   = 0; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;
  // first nx - 1 buckets
  for(uint k = 1; (k < nx()) && (lEnd < aData.size()); ++k) {
    lEnd = findEnd(aData, lBegin, aData.size(), lCumFreqWish, lCumFreqActual);

    _vx[lCount]._lo = aData[lBegin].x;
    _vx[lCount]._hi = aData[lEnd-1].x;
    _vx[lCount]._cf = lCumFreqActual;
    if(k_matrix == kind()) {
      initVyR(aData, lBegin, lEnd, lCount);
    } else {
      initVy(aData, lBegin, lEnd, lCount);
    }
    ++lCount;

    if(trace()) {
      std::cout << "init : x : k : " << std::setw(4) << k 
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual 
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
    }
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aData.size();
  lCumFreqActual = 0;
  for(uint i = lBegin; i < lEnd; ++i) {
    lCumFreqActual += aData[i].c;
  }

  if(0 < lCumFreqActual) {
    _vx[lCount]._lo = aData[lBegin].x;
    _vx[lCount]._hi = aData[lEnd - 1].x;
    _vx[lCount]._cf = lCumFreqActual;
    if(k_matrix == kind()) {
      initVyR(aData, lBegin, lEnd, lCount);
    } else {
      initVy(aData, lBegin, lEnd, lCount);
    }
    ++lCount;
  }

  if(trace()) {
    std::cout << "init : x : k : " << std::setw(4) << nx() 
              << ", [" << std::setw(6) << lEnd << ", " << std::setw(6) << aData.size() << "] "
              << ", cf: " << std::setw(6) << lCumFreqActual 
              << ", cfw: " << std::setw(6) << lCumFreqWish
              << std::endl;
  }

  
  if(trace()) {
    std::cout << "end of EqDepHist::initVy: lCount " << lCount << std::endl;
    print(std::cout);
  }
}


void
EqDepHist::initVy(Data2dim& aData, const uint aBegin, const uint aEnd, const uint aIdxX) {
  const bool lTrace = trace();

  if(lTrace) {
    std::cout << "EqDepHist::initVy::params: "
              << aBegin << ", "
              << aEnd << ", "
              << aIdxX << ", "
              << std::endl;
  }


  const uint lTotal = aData.total(aBegin, aEnd);
        uint lCumFreqWish = lTotal / ny();
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }
  if(ny() > (aEnd - aBegin)) {
    lCumFreqWish = 1;
  }

  if(lTrace) {
    std::cout << "EqDepHist::initVy: data total, size : " 
              << lTotal << ", " << (aEnd - aBegin)
              << std::endl;
  }

  entry_vt& lVy = _vy[aIdxX];

  // sort points in interval on y coordinate
  aData.sorty(aBegin, aEnd);

  uint lBegin = aBegin; // beginning of current bucket, idx into aData
  uint lEnd   = aBegin; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;

  // first ny - 1 buckets
  for(uint k = 1; (k < ny()) && (lEnd < aEnd); ++k) {
    lEnd = findEnd(aData, lBegin, aEnd, lCumFreqWish, lCumFreqActual);

    entry_t e;
    e._lo = aData[lBegin].y;
    e._hi = aData[lEnd-1].y;
    e._cf = lCumFreqActual;
    lVy.push_back(e);

    if(lTrace) {
      std::cout << "  init : y : k : " << std::setw(4) << k
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
      std::cout << "regular 6x6 partitioning:" << std::endl;
      RegularPartitioning2dim lRegP;
      lRegP.initFromData2dim(aData, lBegin, lEnd, 6, 6);
      lRegP.print(std::cout, 4);
    }
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aEnd;
  if(lBegin < lEnd) {
    lCumFreqActual = 0;
    for(uint i = lBegin; i < lEnd; ++i) {
      lCumFreqActual += aData[i].c;
    }

    entry_t e;
    e._lo = aData[lBegin].y;
    e._hi = aData[lEnd - 1].y;
    e._cf = lCumFreqActual;
    lVy.push_back(e);
    if(lTrace) {
      std::cout << "  init : y : "
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
      std::cout << "regular 6x6 partitioning:" << std::endl;
      RegularPartitioning2dim lRegP;
      lRegP.initFromData2dim(aData, lBegin, lEnd, 6, 6);
      lRegP.print(std::cout, 4);
    }
  }

  if(lTrace) {
    std::cout << "  init : y : k : " << std::setw(4) << ny()
              << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
              << ", cf: " << std::setw(6) << lCumFreqActual
              << ", cfw: " << std::setw(6) << lCumFreqWish
              << std::endl;
  }

}


void
EqDepHist::initVyR(Data2dim& aData, const uint aBegin, const uint aEnd, const uint aIdxX) {
  const bool lTrace = trace();
  const bool lHasWidthX = (_vx[aIdxX].lo() < _vx[aIdxX].hi());
  if(lTrace) {
    std::cout << "EqDepHist::initVyR::params: "
              << aBegin << ", "
              << aEnd << ", "
              << aIdxX << ", "
              << std::endl;
  }


  const uint lTotal = aData.total(aBegin, aEnd);
        uint lCumFreqWish = lTotal / ny();
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }
  if(ny() > (aEnd - aBegin)) {
    lCumFreqWish = 1;
  }

  if(lTrace) {
    std::cout << "EqDepHist::initVyR: data total, size : " 
              << lTotal << ", " << (aEnd - aBegin)
              << std::endl;
  }

  entry_vt& lVy = _vy[aIdxX];

  // sort points in interval on y coordinate
  aData.sorty(aBegin, aEnd);

  uint lBegin = aBegin; // beginning of current bucket, idx into aData
  uint lEnd   = aBegin; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;

  // first ny - 1 buckets
  for(uint k = 1; (k < ny()) && (lEnd < aEnd); ++k) {
    lEnd = findEnd(aData, lBegin, aEnd, lCumFreqWish, lCumFreqActual);

    entry_t e;
    e._lo = aData[lBegin].y;
    e._hi = aData[lEnd-1].y;
    e._cf = lCumFreqActual;
    lVy.push_back(e);

    const bool lHasWidthY = (e.lo() < e.hi());

    if(k_matrix == kind() && lHasWidthX && lHasWidthY) {
      RegularPartitioning2dim& lRegP = _regp[aIdxX * ny() + (k-1)];
      lRegP.initFromData2dim(aData, lBegin, lEnd, rx(), ry());
      if(trace()) {
        std::cout << "regular rx X ry partitioning:" << std::endl;
        lRegP.print(std::cout, 4);
      }
    }


    if(lTrace) {
      std::cout << "  init : y : k : " << std::setw(4) << k
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
      if(k_simple == kind()) {
        std::cout << "regular 6x6 partitioning:" << std::endl;
        RegularPartitioning2dim lRegP;
        lRegP.initFromData2dim(aData, lBegin, lEnd, 6, 6);
        lRegP.print(std::cout, 4);
      }
    }
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aEnd;
  if(lBegin < lEnd) {
    lCumFreqActual = 0;
    for(uint i = lBegin; i < lEnd; ++i) {
      lCumFreqActual += aData[i].c;
    }

    entry_t e;
    e._lo = aData[lBegin].y;
    e._hi = aData[lEnd - 1].y;
    e._cf = lCumFreqActual;
    lVy.push_back(e);
    if(k_matrix == kind()) {
      RegularPartitioning2dim& lRegP = _regp[aIdxX * ny() + (lVy.size()-1)];
      lRegP.initFromData2dim(aData, lBegin, lEnd, rx(), ry());
      if(trace()) {
        std::cout << "regular rx X ry partitioning:" << std::endl;
        lRegP.print(std::cout, 4);
      }
    }
    if(lTrace) {
      std::cout << "  init : y : "
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
      if(k_simple == kind()) {
        std::cout << "regular 6x6 partitioning:" << std::endl;
        RegularPartitioning2dim lRegP;
        lRegP.initFromData2dim(aData, lBegin, lEnd, 6, 6);
        lRegP.print(std::cout, 4);
      }
    }
  }

  if(lTrace) {
    std::cout << "  initVyR : y : k : " << std::setw(4) << ny()
              << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
              << ", cf: " << std::setw(6) << lCumFreqActual
              << ", cfw: " << std::setw(6) << lCumFreqWish
              << std::endl;
  }

}

uint
EqDepHist::findEnd(const Data2dim& aData,
                   const uint aBegin, // beginning of new bucket
                   const uint aEnd,   // maximal end of new bucket, (will not belong to bucket)
                   uint       aCumFreqWish,  // approximate cumulated frequency of bucket to be build
                   uint&      aCumFreqOut) const {

  // highly frequent value at the beginning?

  if(aCumFreqWish <= aData[aBegin].c) {
    aCumFreqOut = aData[aBegin].c;
    return (aBegin + 1);
  }

  // from now on: at least two values are needed to exceed aCumFreqWish

  uint lCumFreqUpper = 0;
  uint lCumFreqLower = 0;

  for(uint i = aBegin; i < aEnd; ++i) {
    lCumFreqUpper += aData[i].c;
    if(aCumFreqWish <= lCumFreqUpper) {
      if(aBegin == i) { 
        // should never be executed anyway
        aCumFreqOut = lCumFreqUpper;
        return aBegin + 1;
      }
      if(aCumFreqWish == lCumFreqUpper) {
        aCumFreqOut = lCumFreqUpper;
        return i+1;
      }
      lCumFreqLower = lCumFreqUpper - aData[i].c;
      if(lowerIsCloser(lCumFreqLower, lCumFreqUpper, aCumFreqWish)) {
        aCumFreqOut = lCumFreqLower;
        return i;
      } else {
        aCumFreqOut = lCumFreqUpper;
        return i + 1;
      }
    }
  }
  aCumFreqOut = lCumFreqUpper;
  return aEnd;
}

bool
EqDepHist::lowerIsCloser(uint aLo, uint aHi, uint aGoal) {
  const uint lDiffLo = (aGoal - aLo);
  const uint lDiffHi = (aHi - aGoal);
  return (lDiffLo <= lDiffHi);
}

double
EqDepHist::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}

double
EqDepHist::estimate(const rectangle_t& r) const {
  double lEstimate = 0;

  if(k_simple == kind()) {
    lEstimate += estimateIntervals(r);
  } else
  if(k_matrix == kind()) {
    lEstimate += estimateIntervalsR(r);
  }
  lEstimate += (double) outlierCount(r);

  // return std::max<double>(1.0, lEstimate);
  return std::max<double>(minEstimate(), lEstimate);
}

double
EqDepHist::estimateIntervals(const rectangle_t& r) const {
  double lRes = 0.0;
  for(uint i = 0; i < vx().size(); ++i) {
    if(vx()[i].hi() <= r.xlo()) {
      continue;
    }
    if(r.xhi() <= vx()[i].lo()) {
      break;
    }
    lRes += estimateY(r, i, vx()[i].lo(), vx()[i].hi());
  }
  return lRes;
}


double
EqDepHist::estimateY(const rectangle_t& aRecQuery,
                  const uint aIdxX,
                  const double aXlo,
                  const double aXhi) const {
  const bool lTrace = false;
  double lRes = 0.0;

  const entry_vt& lVy = vy()[aIdxX];

  rectangle_t lRecIsec; // will contain intersection query rect, tile rect
  rectangle_t lRecTile; // tile rectangle
  
  lRecTile.xlo(aXlo);
  lRecTile.xhi(aXhi);

  if(lTrace) {
    std::cout << "EqDepHist::estimateY: params: " 
              << aRecQuery << ",  "
              << aIdxX << ", "
              << aXlo  << ", "
              << aXhi
              << std::endl;
  }

  for(uint j = 0; j < lVy.size(); ++j) {
    const entry_t& e = lVy[j];
    if(e.hi() <= aRecQuery.ylo()) {
      continue;
    }
    if(aRecQuery.yhi() <= e.lo()) {
      break;
    }

    // problematic cases:
    // e.lo() == e.hi()
    // aXlo() == aXhi()
    lRecTile.ylo(e.lo());
    lRecTile.yhi(e.hi());
    if(aRecQuery.contains(lRecTile)) {
      const double lEstimate = (double) e.cf();
      lRes += lEstimate;
    } else
    if(e.lo() < e.hi() && aXlo < aXhi) {
      lRecIsec.isec(aRecQuery, lRecTile);

      const double lEstimate = (lRecIsec.area() / lRecTile.area()) * e.cf();
      lRes += lEstimate;

      if(lTrace) {
        std::cout << "  query rectangle: " << aRecQuery << std::endl;
        std::cout << "  tile  rectangle: " << lRecTile  << std::endl;
        std::cout << "  isec  rectangle: " << lRecIsec  << std::endl;
        std::cout << "  cum   frequency: " << e.cf()    << std::endl;
        std::cout << "  isec  area     : " << lRecIsec.area() << std::endl;
        std::cout << "  tile  area     : " << lRecTile.area() << std::endl;
        std::cout << "  estimate       : " << lEstimate       << std::endl;
      }
    } else {
      if(e.lo() >= e.hi()) {
        // not e.hi() <= aRecQuery.ylo()
        // and
        // not aRecQuery.yhi() <= e.lo()
        // and e.lo() >= e.hi()
        // implies 
        // aRecQuery.ylo() < e.hi() <= e.lo() < aRecQuery.yhi()
        // thus: check only on aXlo, aXhi
        const double lXIsecLo = std::max<double>(aRecQuery.xlo(), aXlo);
        const double lXIsecHi = std::min<double>(aRecQuery.xhi(), aXhi);
        const double lEstimate = ((lXIsecHi - lXIsecLo) / (aXhi - aXlo)) * e.cf();
        lRes += lEstimate;
        // std::cout << "BOOM Y" << std::endl;
        // std::cerr << "BOOM Y" << std::endl;
        // std::cout << "stop: " << "continue!" << std::endl;
      } else
      if(aXlo >= aXhi) {
        const double lYIsecLo = std::max<double>(aRecQuery.ylo(), e.lo());
        const double lYIsecHi = std::min<double>(aRecQuery.yhi(), e.hi());
        const double lEstimate = ((lYIsecHi - lYIsecLo) / (e.hi() - e.lo())) * e.cf();
        lRes += lEstimate;
        // std::cout << "BOOM X" << std::endl;
        // std::cerr << "BOOM X" << std::endl;
        // std::cout << "stop: " << (1/0) << std::endl;
      }
    }
  }

  return lRes;
}



double
EqDepHist::estimateIntervalsR(const rectangle_t& r) const {
  double lRes = 0.0;
  for(uint i = 0; i < vx().size(); ++i) {
    if(vx()[i].hi() <= r.xlo()) {
      continue;
    }
    if(r.xhi() <= vx()[i].lo()) {
      break;
    }
    lRes += estimateYR(r, i, vx()[i].lo(), vx()[i].hi());
  }
  return lRes;
}


double
EqDepHist::estimateYR(const rectangle_t& aRecQuery,
                      const uint aIdxX,
                      const double aXlo,
                      const double aXhi) const {
  const bool lTrace = false;
  double lRes = 0.0;

  const entry_vt& lVy = vy()[aIdxX];

  rectangle_t lRecIsec; // will contain intersection query rect, tile rect
  rectangle_t lRecTile; // tile rectangle
  
  lRecTile.xlo(aXlo);
  lRecTile.xhi(aXhi);

  if(lTrace) {
    std::cout << "EqDepHist::estimateY: params: " 
              << aRecQuery << ",  "
              << aIdxX << ", "
              << aXlo  << ", "
              << aXhi
              << std::endl;
  }

  for(uint j = 0; j < lVy.size(); ++j) {
    const entry_t& e = lVy[j];
    if(e.hi() <= aRecQuery.ylo()) {
      continue;
    }
    if(aRecQuery.yhi() <= e.lo()) {
      break;
    }

    // problematic cases:
    // e.lo() == e.hi()
    // aXlo() == aXhi()
    lRecTile.ylo(e.lo());
    lRecTile.yhi(e.hi());
    if(aRecQuery.contains(lRecTile)) {
      const double lEstimate = (double) e.cf();
      lRes += lEstimate;
    } else
    if(e.lo() < e.hi() && aXlo < aXhi) {
      lRecIsec.isec(aRecQuery, lRecTile);
      double lEstimate = 0;
      if(k_matrix == kind()) {
        lEstimate = _regp[aIdxX * ny() + j].estimate(aRecQuery);
        lRes += lEstimate;
      } else {
        lEstimate = (lRecIsec.area() / lRecTile.area()) * e.cf();
        lRes += lEstimate;
      }

      if(lTrace) {
        std::cout << "  query rectangle: " << aRecQuery << std::endl;
        std::cout << "  tile  rectangle: " << lRecTile  << std::endl;
        std::cout << "  isec  rectangle: " << lRecIsec  << std::endl;
        std::cout << "  cum   frequency: " << e.cf()    << std::endl;
        std::cout << "  isec  area     : " << lRecIsec.area() << std::endl;
        std::cout << "  tile  area     : " << lRecTile.area() << std::endl;
        std::cout << "  estimate       : " << lEstimate       << std::endl;
      }
    } else {
      if(e.lo() >= e.hi()) {
        // not e.hi() <= aRecQuery.ylo()
        // and
        // not aRecQuery.yhi() <= e.lo()
        // and e.lo() >= e.hi()
        // implies 
        // aRecQuery.ylo() < e.hi() <= e.lo() < aRecQuery.yhi()
        // thus: check only on aXlo, aXhi
        const double lXIsecLo = std::max<double>(aRecQuery.xlo(), aXlo);
        const double lXIsecHi = std::min<double>(aRecQuery.xhi(), aXhi);
        const double lEstimate = ((lXIsecHi - lXIsecLo) / (aXhi - aXlo)) * e.cf();
        lRes += lEstimate;
        // std::cout << "BOOM Y" << std::endl;
        // std::cerr << "BOOM Y" << std::endl;
        // std::cout << "stop: " << "continue!" << std::endl;
      } else
      if(aXlo >= aXhi) {
        const double lYIsecLo = std::max<double>(aRecQuery.ylo(), e.lo());
        const double lYIsecHi = std::min<double>(aRecQuery.yhi(), e.hi());
        const double lEstimate = ((lYIsecHi - lYIsecLo) / (e.hi() - e.lo())) * e.cf();
        lRes += lEstimate;
        // std::cout << "BOOM X" << std::endl;
        // std::cerr << "BOOM X" << std::endl;
        // std::cout << "stop: " << (1/0) << std::endl;
      }
    }
  }

  return lRes;
}


uint
EqDepHist::outlierCount(const rectangle_t& r) const {
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
EqDepHist::size() const {
  // const uint lEntrySize = 2 * domValSize() + freqValSize();
  const uint lEntrySize = 2 * domValSize() + 1; // 1 byte suffices if q-compression is used
  uint lRes = 0;
  if(k_simple == kind()) {
    //      nx   ny 
    lRes +=  4 +  4 + nx() * lEntrySize + nx() * ny() * lEntrySize;
  } else
  if(k_matrix == kind()) {
    lRes += 4 + 4 + nx() * lEntrySize + nx() * ny() * (lEntrySize + rx() * ry());
  }
  lRes += outlier().size() * (2 * domValSize() + freqValSize());
  return lRes;
}


std::ostream&
operator<<(std::ostream& os, const EqDepHist::entry_t& x) {
  os << '[' << std::setw( 8) << x._lo << ", "
            << std::setw( 8) << x._hi << ", "
            << std::setw( 8) << x._cf
     << ']';

  return os;
}

std::ostream&
EqDepHist::print(std::ostream& os) const {
  std::cout << std::endl;
  std::cout << "#Equi-Depth-Histogram("
            << "nx, ny, phi: " << nx() << ", " << ny() << ", " << phi() 
            << ')'
            << std::endl;
  std::cout << "vx: " << std::endl;
  for(size_t i = 0; i < vx().size(); ++i) {
    std::cout << "  " << vx()[i] << std::endl;
    std::cout << "  vy " << i << std::endl;
    for(size_t j = 0; j < vy()[i].size(); ++j) {
      std::cout << "      " << vy()[i][j] << std::endl;
    }
  }
  return os;
}

std::ostream&
EqDepHist::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_EquiDepth) << ' '
            << 0 // subkind
            << "nx, ny, phi: " << nx() << ", " << ny() << ", " << phi() 
            << std::endl;
  return os;
} 


} // end namespace


