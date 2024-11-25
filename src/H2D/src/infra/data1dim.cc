#include "data2dim.hh"

namespace H2D {


/*
 *  Data2dim members
 */


std::istream&
operator>>(std::istream& is, xyc_t& x) {
  is >> x.x >> x.y >> x.c;
  return is;
}

std::ostream&
operator<<(std::ostream& os, const xyc_t& x) {
  os << x.x << ' ' << x.y << ' ' << x.c;
  return os;
}


std::istream&
operator>>(std::istream& is, xy_t& x) {
  is >> x.x >> x.y;
  return is;
}

std::ostream&
operator<<(std::ostream& os, const xy_t& x) {
  os << x.x << ' ' << x.y;
  return os;
}


Data2dim::Data2dim(const Data2dim& aData) : _data(aData._data),_colX(),_colY() {}

Data2dim::Data2dim(const Data2dim& aData, const rectangle_t& aRectangle) : _data(),_colX(),_colY() {
  init();
  for(xyc_vt::const_iterator lIter = aData._data.begin(); lIter != aData._data.end(); ++lIter) {
    if(aRectangle.containsHalfOpen((*lIter).x, (*lIter).y)) {
      _data.push_back(*lIter);
      // std::cout << "    contained: " << aRectangle << "   " << (*lIter).x << "  " << (*lIter).y << std::endl;
    } else {
      // std::cout << "not contained: " << aRectangle << "   " << (*lIter).x << "  " << (*lIter).y << std::endl;
    }
  }
  fin();
}

Data2dim&
Data2dim::operator=(const Data2dim& aData) {
  _data = aData._data;
  _colX = aData._colX;
  _colY = aData._colY;
  return (*this);
}

void
Data2dim::readHistFile(const std::string& aFileName, const size_t aLowLim) {
  size_t lTotal = 0;
  std::ifstream is(aFileName.c_str());
  if(!is) {
    std::cerr << "Could not open \'" << aFileName << "\'\n";
    return;
  }
  xyc_t x;
  while(!is.eof()) {
    is >> x;
    if(is.eof()) {
      return;
    }
    push_back(x);
    lTotal += x.c;
    if(0 < aLowLim && aLowLim < lTotal) {
      break;
    }
  }
}

void
Data2dim::readValueFile(const std::string& aFileName, const size_t aLowLim) {
  size_t lTotal = 0;
  std::ifstream is(aFileName.c_str());
  if(!is) {
    std::cerr << "Could not open \'" << aFileName << "\'\n";
    return;
  }

  xy_t x;
  while(!is.eof()) {
    is >> x;
    if(is.eof()) {
      return;
    }
    push_back(x.x, x.y, 1);
    ++lTotal;
    if(0 < aLowLim && aLowLim < lTotal) {
      break;
    }
  }
}

Data2dim&
Data2dim::push_back(const xyc_t& x) {
  _data.push_back(x);
  return (*this);
}

Data2dim&
Data2dim::push_back(const double x, const double y, const uint c) {
  _data.push_back(xyc_t(x,y,c));
  return (*this);
}

void
Data2dim::init() {
  _data.clear();
}

void
Data2dim::step(const double& x, const double& y, const uint& c) {
  push_back(x, y, c);
}

void
Data2dim::fin() {
}

uint
Data2dim::total() const {
  uint lRes = 0;
  for(uint i = 0; i < _data.size(); ++i) {
    lRes += _data[i].c;
  }
  return lRes;
}

uint
Data2dim::total(const uint aBegin, const uint aEnd) const {
  uint lRes = 0;
  for(uint i = aBegin; i < aEnd; ++i) {
    lRes += _data[i].c;
  }
  return lRes;
}
//////NR//////
void
Data2dim::fillCols(Data2dim& aData){
for(uint i = 0; i < aData.size(); ++i) {
    const xyc_t& lXyc = aData[i];
    for(uint j = 0; j < lXyc.c; ++j) {
      _colX.push_back(lXyc.x);
      _colY.push_back(lXyc.y);
    }
  }
}
/////////////
uint
Data2dim::maxFrequency() const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    lRes = std::max<uint>(lRes, _data[i].c);
  }
  return lRes;
}



void
Data2dim::getBoundingRectangle(rectangle_t& aRectangleOut) const {
  Aggregate<double> lAggrX;
  Aggregate<double> lAggrY;
  lAggrX.init(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
  lAggrY.init(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
  for(uint i = 0; i < _data.size(); ++i) {
    lAggrX.step(_data[i].x);
    lAggrY.step(_data[i].y);
  }
  lAggrX.fin();
  lAggrY.fin();
  aRectangleOut._pll.x = lAggrX.min();
  aRectangleOut._pll.y = lAggrY.min();
  aRectangleOut._pur.x = lAggrX.max();
  aRectangleOut._pur.y = lAggrY.max();
}

void
Data2dim::getBoundingRectangle(rectangle_t& aRectangleOut, const uint aBegin, const uint aEnd) const {
  Aggregate<double> lAggrX;
  Aggregate<double> lAggrY;
  lAggrX.init(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
  lAggrY.init(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
  for(uint i = aBegin; i < aEnd; ++i) {
    lAggrX.step(_data[i].x);
    lAggrY.step(_data[i].y);
  }
  lAggrX.fin();
  lAggrY.fin();
  aRectangleOut._pll.x = lAggrX.min();
  aRectangleOut._pll.y = lAggrY.min();
  aRectangleOut._pur.x = lAggrX.max();
  aRectangleOut._pur.y = lAggrY.max();
}

void
Data2dim::getPartitionDescX(partitiondesc_t& aPdX, const uint aNx) const {
  assert(0 < size());
  rectangle_t lRectangle;
  getBoundingRectangle(lRectangle);
  aPdX.set(lRectangle.xlo(), lRectangle.xhi(), aNx);
}

void
Data2dim::getPartitionDescY(partitiondesc_t& aPdY, const uint aNy) const {
  assert(0 < size());
  rectangle_t lRectangle;
  getBoundingRectangle(lRectangle);
  aPdY.set(lRectangle.ylo(), lRectangle.yhi(), aNy);
}

bool
Data2dim::getPartitionDescXY(partitiondescxy_t& aPdXY, const uint aNx, const uint aNy) const {
  if(0 == size()) {
    return false;
  }
  rectangle_t lRectangle;
  getBoundingRectangle(lRectangle);
  aPdXY._pdX.set(lRectangle.xlo(), lRectangle.xhi(), aNx);
  aPdXY._pdY.set(lRectangle.ylo(), lRectangle.yhi(), aNy);

  return true;
}

//////NR/////
// dim is considered as column index (from 0 onwards)
uint
Data2dim::countLine(const line_t& aLine, int dim) const {
  uint lRes = 0;
  if (dim ==0) {
  for(uint i = 0; i < size(); ++i) {
    if(aLine.containsLine(_data[i].x)) {
      lRes += _data[i].c;
    }
  }
}
else{
for(uint i = 0; i < size(); ++i) {
    if(aLine.containsLine(_data[i].y)) {
      lRes += _data[i].c;
    }
  }

}
  return lRes;
}



/////////////
uint
Data2dim::countWithin(const rectangle_t& aRectangle) const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    if(aRectangle.containsHalfOpen(_data[i].x, _data[i].y)) {
      lRes += _data[i].c;
    }
  }
  return lRes;
}

void
Data2dim::split(Data2dim& aRegular, Data2dim& aOutlier, const uint aPhi) const {
  aRegular._data.clear();
  aOutlier._data.clear();
  for(uint i = 0; i < size(); ++i) {
    const xyc_t& x = (*this)[i];
    if(aPhi >= x.c) {
      aRegular.push_back(x);
    } else {
      aOutlier.push_back(x);
    }
  }
}

void
Data2dim::splitX(Data2dim& aLeft, Data2dim& aRight, const double aValue) const {
  aLeft._data.clear();
  aRight._data.clear();
  for(uint i = 0; i < size(); ++i) {
    const xyc_t& p = (*this)[i];
    if(p.x <= aValue) {
      aLeft.push_back(p);
    } else {
      aRight.push_back(p);
    }
  }
}


void
Data2dim::splitY(Data2dim& aLeft, Data2dim& aRight, const double aValue) const {
  aLeft._data.clear();
  aRight._data.clear();
  for(uint i = 0; i < size(); ++i) {
    const xyc_t& p = (*this)[i];
    if(p.y <= aValue) {
      aLeft.push_back(p);
    } else {
      aRight.push_back(p);
    }
  }
}




uint
Data2dim::eqdFindEnd(const uint  aBegin, // beginning of new bucket
                     const uint  aEnd,   // maximal end of new bucket (will not belong to bucket)
                     const uint  aCumFreqWish,  // wished frequency per bucket
                           uint& aCumFreqOut) const {
  // highly frequent value at the beginning?

  if(aCumFreqWish <= _data[aBegin].c) {
    aCumFreqOut = _data[aBegin].c;
    return (aBegin + 1);
  }

  // from now on: at least two values are needed to exceed aCumFreqWish

  uint lCumFreqUpper = 0;
  uint lCumFreqLower = 0;

  for(uint i = aBegin; i < aEnd; ++i) {
    lCumFreqUpper += _data[i].c;
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
      lCumFreqLower = lCumFreqUpper - _data[i].c;
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


void
Data2dim::buildEqdHistX(const uint aBegin, const uint aEnd, 
                        const uint aNoBuckets,
                        histentry_vt& aOut) {
  const bool lTrace = false;

  // assert all y-values are equal
  const double lYVal = _data[aBegin].y;
  for(uint i = aBegin + 1; i < aEnd; ++i) {
    assert(lYVal == _data[i].y);
  }

  if(lTrace) {
    std::cout << "Data2dim::buildEqwHistX::params: "
              << aBegin << ", "
              << aEnd << ", "
              << std::endl;
  }

  aOut.clear();

  const uint lTotal = total(aBegin, aEnd);
        uint lCumFreqWish = lTotal / aNoBuckets;
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }
  if(aNoBuckets > (aEnd - aBegin)) {
    lCumFreqWish = 1;
  }

  if(lTrace) {
    std::cout << "Data2dim::buildEqdHist: data total, size : "
              << lTotal << ", " << (aEnd - aBegin)
              << std::endl;
  }

  // sort points in interval on x coordinate
  sortx(aBegin, aEnd);

  uint lBegin = aBegin; // beginning of current bucket, idx into aData
  uint lEnd   = aBegin; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;

  // first nx - 1 buckets
  for(uint k = 1; (k < aNoBuckets) && (lBegin < aEnd); ++k) {
    lEnd = eqdFindEnd(lBegin, aEnd, lCumFreqWish, lCumFreqActual);

    histentry_t e;
    e._lo = _data[lBegin].x;
    e._hi = _data[lEnd-1].x;
    e._cf = lCumFreqActual;
    e._noDv = (lEnd - lBegin);
    assert(0 < e._noDv);
    aOut.push_back(e);

    if(lTrace) {
      std::cout << "  init : x : k : " << std::setw(4) << k
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
    }
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aEnd;
  if(lBegin < lEnd) {
    lCumFreqActual = 0;
    for(uint i = lBegin; i < lEnd; ++i) {
      lCumFreqActual += _data[i].c;
    }

    histentry_t e;
    e._lo = _data[lBegin].x;
    e._hi = _data[lEnd - 1].x;
    e._cf = lCumFreqActual;
    e._noDv = (lEnd - lBegin);
    assert(0 < e._noDv);
    aOut.push_back(e);
  }

  if(lTrace) {
    std::cout << "  buildEqwHistX : #Buckets : k : " << std::setw(4) << aNoBuckets
              << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
              << ", cf: " << std::setw(6) << lCumFreqActual
              << ", cfw: " << std::setw(6) << lCumFreqWish
              << std::endl;
  }
}

void
Data2dim::buildEqdHistY(const uint aBegin, const uint aEnd, 
                        const uint aNoBuckets,
                        histentry_vt& aOut) {

  // assert all x-values are equal
  const double lXVal = _data[aBegin].x;
  for(uint i = aBegin + 1; i < aEnd; ++i) {
    assert(lXVal == _data[i].x);
  }


  const bool lTrace = false;
  if(lTrace) {
    std::cout << "Data2dim::buildEqwHistY::params: "
              << aBegin << ", "
              << aEnd << ", "
              << std::endl;
  }

  aOut.clear();

  const uint lTotal = total(aBegin, aEnd);
        uint lCumFreqWish = lTotal / aNoBuckets;
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }
  if(aNoBuckets > (aEnd - aBegin)) {
    lCumFreqWish = 1;
  }

  if(lTrace) {
    std::cout << "Data2dim::buildEqdHistY: data total, size : "
              << lTotal << ", " << (aEnd - aBegin)
              << std::endl;
  }

  // sort points in interval on y coordinate
  sorty(aBegin, aEnd);

  uint lBegin = aBegin; // beginning of current bucket, idx into aData
  uint lEnd   = aBegin; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;

  // first ny - 1 buckets
  for(uint k = 1; (k < aNoBuckets) && (lBegin < aEnd); ++k) {
    lEnd = eqdFindEnd(lBegin, aEnd, lCumFreqWish, lCumFreqActual);

    histentry_t e;
    e._lo = _data[lBegin].y;
    e._hi = _data[lEnd-1].y;
    e._cf = lCumFreqActual;
    e._noDv = (lEnd - lBegin);
    assert(0 < e._noDv);
    aOut.push_back(e);

    if(lTrace) {
      std::cout << "  init : y : k : " << std::setw(4) << k
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
    }
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aEnd;
  if(lBegin < lEnd) {
    lCumFreqActual = 0;
    for(uint i = lBegin; i < lEnd; ++i) {
      lCumFreqActual += _data[i].c;
    }

    histentry_t e;
    e._lo = _data[lBegin].y;
    e._hi = _data[lEnd - 1].y;
    e._cf = lCumFreqActual;
    e._noDv = (lEnd - lBegin);
    assert(0 < e._noDv);
    aOut.push_back(e);
  }

  if(lTrace) {
    std::cout << "  buildEqwHistY : #Buckets : k : " << std::setw(4) << aNoBuckets
              << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
              << ", cf: " << std::setw(6) << lCumFreqActual
              << ", cfw: " << std::setw(6) << lCumFreqWish
              << std::endl;
  }
}


double
Data2dim::estimateClosed(const double aLo, const double aHi, const histentry_vt& aHist) {
  assert(aLo <= aHi);

  double lRes = 0;
  for(uint i = 0; i < aHist.size(); ++i) {
    const histentry_t& e = aHist[i];
    if(e.hi() < aLo) { continue; }
    if(aHi < e.lo()) { break; }
    if(aLo <= e.lo() && e.hi() <= aHi) {
      // containment
      lRes += (double) e.cf();
    } else {
      const double lLo = std::max<double>(aLo, e.lo());
      const double lHi = std::min<double>(aHi, e.hi());
      if(lHi < lLo) { continue; }
      if(e.lo() < e.hi() && lLo < lHi) {
        // regular case
        const double lEstimate = ( (lHi - lLo) / (e.hi() - e.lo()) ) * (double) e.cf();
        lRes += lEstimate;
      } else {
        // currently:    e.hi() >= aLo 
        //               aHi >= e.lo()   
        //            && (aLo > e.lo() || e.hi() > a.Hi)
        //            && (e.lo() == e.hi || lLo >= lLo)
        //            && lLo <= lHi
        // thus
        if(e.lo() == e.hi()) {
          lRes += e.cf();
        } else
        if(lLo == lHi) {
          lRes += (double) e.cf() / (double) e.noDv(); // NYI
        }
      }
    }
  }

  return lRes;
}

void
Data2dim::sortx(const uint aBegin, const uint aEnd) {
  std::sort(_data.begin() + aBegin, _data.begin() + aEnd, LessX());
}

void
Data2dim::sorty(const uint aBegin, const uint aEnd) {
  std::sort(_data.begin() + aBegin, _data.begin() + aEnd, LessY());
}

inline bool 
compareX(xyc_t a, xyc_t b) { return a.x < b.x || (a.x == b.x && a.y < b.y); };


void
Data2dim::sortX(xyc_vt::iterator& aFirst, xyc_vt::iterator& aLast) {
  std::sort(aFirst, aLast, compareX);
}

inline bool 
compareY(xyc_t a, xyc_t b) { return a.y < b.y; };

void
Data2dim::sortY(xyc_vt::iterator& aFirst, xyc_vt::iterator& aLast) {
  std::sort(aFirst, aLast, compareY);
}



uint
Data2dim::maxDiffSpreadX(const uint aBegin, const uint aEnd, double& aMaxDiffSpreadX) const {
  uint   lRes = 0;
  double lSpreadLag = spreadX(aBegin);
  double lMaxDiffSpread = 0;
  for(uint i = aBegin + 1; i < (aEnd - 1); ++i) {
     const double lSpread = spreadX(i);
     const double lDiffSpread = fabs(lSpread - lSpreadLag);
     if(lMaxDiffSpread < lDiffSpread) {
        lMaxDiffSpread = lDiffSpread;
        lRes = i - 1;
     }
     lSpreadLag = lSpread;
  }
  aMaxDiffSpreadX = lMaxDiffSpread;
  return lRes;
}


uint
Data2dim::maxDiffSpreadY(const uint aBegin, const uint aEnd, double& aMaxDiffSpreadY) const {
  uint   lRes = 0;
  double lSpreadLag = spreadY(aBegin);
  double lMaxDiffSpread = 0;
  for(uint i = aBegin + 1; i < (aEnd - 1); ++i) {
     const double lSpread = spreadY(i);
     const double lDiffSpread = fabs(lSpread - lSpreadLag);
     if(lMaxDiffSpread < lDiffSpread) {
        lMaxDiffSpread = lDiffSpread;
        lRes = i - 1;
     }
     lSpreadLag = lSpread;
  }
  aMaxDiffSpreadY = lMaxDiffSpread;
  return lRes;
}



uint
Data2dim::maxDiffAreaX(const uint aBegin, const uint aEnd, double& aMaxDiffAreaX) const {
  uint   lRes = 0;
  double lSpreadLag = areaX(aBegin);
  double lMaxDiffSpread = 0;
  for(uint i = aBegin + 1; i < (aEnd - 1); ++i) {
     const double lSpread = areaX(i);
     const double lDiffSpread = fabs(lSpread - lSpreadLag);
     if(lMaxDiffSpread < lDiffSpread) {
        lMaxDiffSpread = lDiffSpread;
        lRes = i - 1;
     }
     lSpreadLag = lSpread;
  }
  aMaxDiffAreaX = lMaxDiffSpread;
  return lRes;
}


uint
Data2dim::maxDiffAreaY(const uint aBegin, const uint aEnd, double& aMaxDiffAreaY) const {
  uint   lRes = 0;
  double lSpreadLag = areaY(aBegin);
  double lMaxDiffSpread = 0;
  for(uint i = aBegin + 1; i < (aEnd - 1); ++i) {
     const double lSpread = areaY(i);
     const double lDiffSpread = fabs(lSpread - lSpreadLag);
     if(lMaxDiffSpread < lDiffSpread) {
        lMaxDiffSpread = lDiffSpread;
        lRes = i - 1;
     }
     lSpreadLag = lSpread;
  }
  aMaxDiffAreaY = lMaxDiffSpread;
  return lRes;
}


uint
Data2dim::minVarianceX(const uint aBegin, const uint aEnd, double& aMinVarianceX) const {
  Variance<double> lVarianceA2I;
  Variance<double> lVarianceI2Z;
  lVarianceA2I.init();
  lVarianceI2Z.init();
  for(uint i = aBegin; i < (aEnd - 1); ++i) {
    lVarianceI2Z.step(_data[i].x);
  }

  double lMinVariance = std::numeric_limits<double>::max();
  double lMinIndex    = 0;
  for(uint i = aBegin; i < (aEnd - 1); ++i) {
    lVarianceA2I.step(_data[i].x);
    lVarianceI2Z.reverseStep(_data[i].x);
    // add all equal values
    while(_data[i].x == _data[i+1].x) {
      lVarianceA2I.step(_data[i].x);
      lVarianceI2Z.reverseStep(_data[i].x);
      ++i;
    }
    const double lTotalVariance = lVarianceA2I.variance() + lVarianceI2Z.variance();
    if(lTotalVariance < lMinVariance) {
      lMinVariance = lTotalVariance;
      lMinIndex    = i;
    }
  }
  aMinVarianceX = lMinVariance;
  return lMinIndex;
}


uint
Data2dim::minVarianceY(const uint aBegin, const uint aEnd, double& aMinVarianceY) const {
  Variance<double> lVarianceA2I;
  Variance<double> lVarianceI2Z;
  lVarianceA2I.init();
  lVarianceI2Z.init();
  for(uint i = aBegin; i < aEnd; ++i) {
    lVarianceI2Z.step(_data[i].y);
  }

  double lMinVariance = std::numeric_limits<double>::max();
  double lMinIndex    = 0;
  for(uint i = aBegin; i < (aEnd - 1); ++i) {
    lVarianceA2I.step(_data[i].y);
    lVarianceI2Z.reverseStep(_data[i].y);
    // add all equal values
    uint lToSubtract = 0;
    while((_data[i].y == _data[i+1].y) && (i < aEnd)) {
      lVarianceA2I.step(_data[i].y);
      lVarianceI2Z.reverseStep(_data[i].y);
      ++i;
      lToSubtract = 1;
    }
    if(aEnd <= i) {
      break;
    }
    i -= lToSubtract;
    const double lTotalVariance = lVarianceA2I.variance() + lVarianceI2Z.variance();
    // std::cout << lTotalVariance << ' ' ;
    if(lTotalVariance < lMinVariance) {
      lMinVariance = lTotalVariance;
      lMinIndex    = i;
    }
  }
  aMinVarianceY = lMinVariance;
  return lMinIndex;
}



double
Data2dim::variance(const uint aBegin, const uint aEnd) const {
  Variance<double> lVarianceX;
  Variance<double> lVarianceY;
 
  lVarianceX.init(); 
  lVarianceY.init(); 
  for(uint i = 0; i < aEnd; ++i) {
    const xyc_t& p = _data[i];
    lVarianceX.step(p.x);
    lVarianceY.step(p.y);
  }
  lVarianceX.fin();
  lVarianceY.fin();
  return (lVarianceX.variance() + lVarianceY.variance());
}

double
Data2dim::sse(const uint aBegin, const uint aEnd) const {
  Variance<double> lVarianceX;
  Variance<double> lVarianceY;
 
  lVarianceX.init(); 
  lVarianceY.init(); 
  for(uint i = 0; i < aEnd; ++i) {
    const xyc_t& p = _data[i];
    lVarianceX.step(p.x);
    lVarianceY.step(p.y);
  }
  lVarianceX.fin();
  lVarianceY.fin();
  return (lVarianceX.sse() + lVarianceY.sse());
}


uint
Data2dim::cumFreq(const uint aBegin, const uint aEnd) const {
  uint lRes = 0;
  for(uint i = aBegin; i < aEnd; ++i) {
    lRes += _data[i].c;
  }
  return lRes;
}

void
Data2dim::printHist(std::ostream& os, const hist1dim_t& aHist) {
  for(uint i = 0; i < aHist.size(); ++i) {
    os << '[' << aHist[i].lo() << ','
              << aHist[i].hi() << ','
              << aHist[i].cf() << ','
              << aHist[i].noDv()
       << ']'
       << ' ';
  }
}

bool
Data2dim::partition3x3(Data2dim lPartition[3][3], uint lCumFreq[3][3]) const {
  // clear output arguments
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      lPartition[i][j].clear();
      lCumFreq[i][j] = 0;
    }
  }


  // prepare partition descriptor 
  rectangle_t lRectangle;
  getBoundingRectangle(lRectangle);
  if(lRectangle.xlo() == lRectangle.xhi() || lRectangle.ylo() == lRectangle.yhi()) {
    return false;
  }
  partitiondesc_t lDescX;
  partitiondesc_t lDescY;
  lDescX.set(lRectangle.xlo(), lRectangle.xhi(), 3);
  lDescY.set(lRectangle.ylo(), lRectangle.yhi(), 3);
  rectangle_t lTiles[3][3];
  partitiondescxy_t lPdXY(lDescX, lDescY);

  // get tile rectangles for all tiles
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      lPdXY.getRectangle(i, j, lTiles[i][j]);
    }
  }

  for(uint i = 0; i < size(); ++i) {
    const xyc_t p = (*this)[i]; // p is the data point
    // determine indices of tile
    uint idxX = lDescX.idx(p.x);
    uint idxY = lDescY.idx(p.y);
    if(2 < idxX) { idxX = 2; }
    if(2 < idxY) { idxY = 2; }
    rectangle_t& lTile = lTiles[idxX][idxY];
    // correct indices (due to double rounding problems)
    if(!lTile.containsHalfOpen(p.x, p.y)) {
      // correct
      if(p.x < lTile.xlo() && 0 < idxX) {
        --idxX;
      } else
      if(p.x >= lTile.xhi() && 2 > idxX) {
        ++idxX;
      }
      if(p.y < lTile.ylo() && 0 < idxY) {
         --idxY;
      } else
      if(p.y >= lTile.yhi() && 2 > idxY) {
        ++idxY;
      }
    }
    // add data point p to tile
    lPartition[idxX][idxY].push_back(p);
    lCumFreq[idxX][idxY] += p.c;
  }
  return true;
}

void
Data2dim::partitionX(Data2dim& aData0, Data2dim& aData1, const double aBoundary) const {
  for(uint i = 0; i < size(); ++i) {
    const xyc_t p = (*this)[i]; // p is the data point
    if(p.x < aBoundary) {
      aData0.push_back(p);
    } else {
      aData1.push_back(p);
    }
  }
}


void
Data2dim::partitionY(Data2dim& aData0, Data2dim& aData1, const double aBoundary) const {
  for(uint i = 0; i < size(); ++i) {
    const xyc_t p = (*this)[i]; // p is the data point
    if(p.y < aBoundary) {
      aData0.push_back(p);
    } else {
      aData1.push_back(p);
    }
  }
}



/*

void 
Data2dim::analyze(const uint nx, const uint ny) {
  Aggregate<double> lAggrX;
  Aggregate<double> lAggrY;
  for(uint i = 0; i < _data.size(); ++i) {
    lAggrX.step(_data[i].x);
    lAggrY.step(_data[i].y);
  }
  _descX.set(lAggrX.min(), lAggrX.max(), nx);
  _descY.set(lAggrY.min(), lAggrY.max(), ny);
  std::cout << "X: " << _descX << std::endl;
  std::cout << "Y: " << _descY << std::endl;

  ana_t lAna(nx,ny);  

  for(uint i = 0; i < _data.size(); ++i) {
    const uint idxX = _descX.idx(_data[i].x);
    const uint idxY = _descY.idx(_data[i].y);
    lAna._vx[idxX] += 1;
    lAna._vy[idxY] += 1;
    lAna._m(idxX, idxY) += 1;
  }
  lAna.print(std::cout);

  std::cout << std::endl;

  std::cout << "distribution x  " << std::endl;

  for(uint i = 0; i < nx; ++i) {
    std::cout << std::setw(7) << _descX.boundary(i) << ' ';
  }
  std::cout << std::endl;
  for(uint i = 0; i < nx; ++i) {
    std::cout << std::setw(7) << lAna._vx[i] << ' ';
  }
  std::cout << std::endl;

  std::cout << "distribution y  " << std::endl;

  for(uint i = 0; i < ny; ++i) {
    std::cout << std::setw(7) << _descY.boundary(i) << ' ';
  }
  std::cout << std::endl;
  for(uint i = 0; i < ny; ++i) {
    std::cout << std::setw(7) << lAna._vy[i] << ' ';
  }
  std::cout << std::endl;


}

*/


} // end namspace



