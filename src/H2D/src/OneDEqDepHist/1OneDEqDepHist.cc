#include "OneDEqDepHist.hh"

namespace H2D {


OneDEqDepHist::OneDEqDepHist(const Data2dim& aData,
                     const uint      aPhi,
                     const double    aQ,
                     const double    aTheta,
		     const uint      aDim,
                     const bool      aTrace)
              : EstimatorBase2dim(aQ, aTheta),
                _outlier(), 
                _n(std::min(static_cast<int>(aData.total()/aTheta),10000)), 
                _phi(aPhi),
                _v(),
                _trace(aTrace),
                _totalCard(aData.total()),
		_dim(aDim){

  if(trace()) {
    std::cout << "1D Equi-Depth-Histogram: param: " << n() << ", " << ", " << aPhi << std::endl;
  }



  

  if(trace()) {
    std::cout << "1D Equi-Depth-Histogram: nx : " << n() 
              << ", number of outliers: " << outlier().size() 
              << ", Equi-Depth-Hist::size : " << size()
              << ", Data::size: " << aData.size() 
              << ", Data::total: " << aData.total()
              << std::endl;
  }

  _v.resize(n());
  //_vy.resize(nx());

  Data2dim lRegular;
  if(8 < aPhi) {
    aData.split(lRegular, _outlier, aPhi);
  } else {
    lRegular = aData;
  }

  initV(lRegular);

}

OneDEqDepHist::~OneDEqDepHist() {}

void
OneDEqDepHist::initV(Data2dim& aData) {
  const uint lTotal = aData.total();
  uint lCumFreqWish = lTotal / n();
  if(1 > lCumFreqWish) {
    lCumFreqWish = 1;
  }
  if(_dim==0){
  aData.sortx(0, aData.size());
  }
  else{
  aData.sorty(0, aData.size());	  
  }
  uint lCount = 0; // count entries assigned to vector 
  uint lBegin = 0; // beginning of current bucket, idx into aData
  uint lEnd   = 0; // end (exclusive) of current bucket, idx into aData
  uint lCumFreqActual = 0;
  // first nx - 1 buckets
  for(uint k = 1; (k < n()) && (lEnd < aData.size()); ++k) {
    lEnd = findEnd(aData, lBegin, aData.size(), lCumFreqWish, lCumFreqActual);

    _v[lCount]._lo = (_dim == 0) ? aData[lBegin].x : aData[lBegin].y;
    _v[lCount]._hi = (_dim == 0) ? aData[lEnd-1].x : aData[lEnd-1].y;
    _v[lCount]._cf = lCumFreqActual;
    ++lCount;
/*
    if(trace()) {
      std::cout << "init : x : k : " << std::setw(4) << k 
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual 
                << ", cfw: " << std::setw(6) << lCumFreqWish
                << std::endl;
    }
    */
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aData.size();
  lCumFreqActual = 0;
  for(uint i = lBegin; i < lEnd; ++i) {
    lCumFreqActual += aData[i].c;
  }

  if(0 < lCumFreqActual) {
    _v[lCount]._lo = (_dim ==0 ) ? aData[lBegin].x : aData[lBegin].y;
    _v[lCount]._hi = (_dim ==0 ) ? aData[lEnd - 1].x: aData[lEnd-1].y;
    _v[lCount]._cf = lCumFreqActual;
    ++lCount;
  }


}



uint
OneDEqDepHist::findEnd(const Data2dim& aData,
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
OneDEqDepHist::lowerIsCloser(uint aLo, uint aHi, uint aGoal) {
  const uint lDiffLo = (aGoal - aLo);
  const uint lDiffHi = (aHi - aGoal);
  return (lDiffLo <= lDiffHi);
}

double
OneDEqDepHist::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}

double
OneDEqDepHist::estimate(const rectangle_t& r) const {
  
  double lEstimate = estimateIntervals(r) + outlierCount(r);
  return std::max<double>(minEstimate(), lEstimate);
}

double
OneDEqDepHist::estimateIntervals(const rectangle_t& r) const {
  double lRes = 0.0;
  uint lower = 0;
  uint upper = v().size();
  double lo = (_dim ==0 ) ? r.xlo() : r.ylo();
  double hi = (_dim ==0 ) ? r.xhi() : r.yhi();

  for(uint i = 0; i < v().size(); ++i) {
    if(v()[i].hi() <= lo) {
      continue;
    }
    lower = i;
    if(hi <= v()[i].lo()) {
      upper = i; //actually next bucket after r	    
      break;
    }
  }
  lRes = (upper - lower) * (_totalCard / n()); 
  return lRes;
}




uint
OneDEqDepHist::outlierCount(const rectangle_t& r) const {
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    bool cond = (_dim == 0) ? r._pll.x <= p.x && r._pur.x > p.x : r._pll.y <= p.y && r._pur.y > p.y;
    if(cond) {
      lRes += p.c;
    }
  }
  return lRes;
}



uint
OneDEqDepHist::size() const {
  // const uint lEntrySize = 2 * domValSize() + freqValSize();
  //const uint lEntrySize = 2 * domValSize() + 1; // 1 byte suffices if q-compression is used
  //uint lRes = 0;
  
    //      nx   ny 
  //lRes +=  4 +  4 + 2 * nx() * lEntrySize;
  //lRes += outlier().size() * (2 * domValSize() + freqValSize());
  return n() * ( 8 + 8 + 4 ); //NR Check with Guido
}


std::ostream&
operator<<(std::ostream& os, const OneDEqDepHist::entry_t& x) {
  os << '[' << std::setw( 8) << x._lo << ", "
            << std::setw( 8) << x._hi << ", "
            << std::setw( 8) << x._cf
     << ']';

  return os;
}

std::ostream&
OneDEqDepHist::print(std::ostream& os) const {
  std::cout << std::endl;
  std::cout << "#Equi-Depth-Histogram("
            << "nx, ny, phi: " << n() << ", " << ", " << phi() 
            << ')'
            << std::endl;
  std::cout << "vx: " << std::endl;
  for(size_t i = 0; i < v().size(); ++i) {
    std::cout << "  " << v()[i] << std::endl;
  }
  return os;
}

std::ostream&
OneDEqDepHist::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_EquiDepth) << ' '
            << 0 // subkind
            << "nx, phi: " << n() << ", " << ", " << phi() 
            << std::endl;
  return os;
} 


} // end namespace


