#include "OneDEqDepHist.hh"

namespace H2D {


OneDEqDepHist::OneDEqDepHist(const Data2dim& aData,
                     const uint      aPhi,
                     const double    aQ,
                     const double    aTheta,
		     const uint      aN,
                     const bool      aTrace)
              : EstimatorBase2dim(aQ, aTheta),
                _outlier(), 
                _nx(aN), 
                _phi(aPhi),
                _vx(),
		_vy(),
                _trace(aTrace),
                _totalCard(aData.total()){
  if(trace()) {
    std::cout << "1D Equi-Depth-Histogram: param: " << nx() << ", " << aPhi << std::endl;
  }



  if(2 > nx()) { _nx = 2; }


  if(trace()) {
    std::cout << "1D Equi-Depth-Histogram: nx : " << nx() 
              << ", number of outliers: " << outlier().size() 
              << ", Equi-Depth-Hist::size : " << size()
              << ", Data::size: " << aData.size() 
              << ", Data::total: " << aData.total()
              << std::endl;
  }

  _vx.resize(nx());
  _vy.resize(nx());

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
    ++lCount;

    if(trace()) {
      std::cout << "init : x : k : " << std::setw(4) << k 
                << ", [" << std::setw(6) << lBegin << ", " << std::setw(6) << lEnd << "] "
                << ", cf: " << std::setw(6) << lCumFreqActual 
                << ", cfw: " << std::setw(6) << lCumFreqWish
		<< " aData from " << aData[lBegin].x <<", " << aData[lEnd].x <<std::endl;
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
    ++lCount;
  }

  aData.sorty(0, aData.size());
  lCount = 0; // count entries assigned to vector 
  lBegin = 0; // beginning of current bucket, idx into aData
  lEnd   = 0; // end (exclusive) of current bucket, idx into aData
  lCumFreqActual = 0;
  // first nx - 1 buckets
  for(uint k = 1; (k < nx()) && (lEnd < aData.size()); ++k) {
    lEnd = findEnd(aData, lBegin, aData.size(), lCumFreqWish, lCumFreqActual);

    _vy[lCount]._lo = aData[lBegin].y;
    _vy[lCount]._hi = aData[lEnd-1].y;
    _vy[lCount]._cf = lCumFreqActual;
    ++lCount;
    lBegin = lEnd;
  }

  // last bucket
  lEnd = aData.size();
  lCumFreqActual = 0;
  for(uint i = lBegin; i < lEnd; ++i) {
    lCumFreqActual += aData[i].c;
  }

  if(0 < lCumFreqActual) {
    _vy[lCount]._lo = aData[lBegin].y;
    _vy[lCount]._hi = aData[lEnd - 1].y;
    _vy[lCount]._cf = lCumFreqActual;
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
  //uint lCumFreqLower = 0;

  for(uint i = aBegin; i < aEnd; ++i) {
    lCumFreqUpper += aData[i].c;
    if(aCumFreqWish <= lCumFreqUpper) {
      //if(aBegin == i) { 
        // should never be executed anyway
        aCumFreqOut = lCumFreqUpper;
        return i + 1;
      //}
      /*
      lCumFreqLower = lCumFreqUpper - aData[i].c;
      if(lowerIsCloser(lCumFreqLower, lCumFreqUpper, aCumFreqWish)) {
        aCumFreqOut = lCumFreqLower;
        return i;
      } else {
        aCumFreqOut = lCumFreqUpper;
        return i + 1;
      }*/
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
  
  double lEstimateX = estimateIntervalsX(r) + outlierCountX(r);
  double lEstimatey = estimateIntervalsY(r) + outlierCountY(r);

  double lEstimate = lEstimateX * lEstimatey / _totalCard;

  //lEstimateX += (double) outlierCount(r);

  // return std::max<double>(1.0, lEstimate);
  return std::max<double>(minEstimate(), lEstimate);
}

double
OneDEqDepHist::estimateX(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();

  double lEstimateX = estimateIntervalsX(r) + outlierCountX(r);

  //lEstimateX += (double) outlierCount(r);

  // return std::max<double>(1.0, lEstimate);
  return std::max<double>(minEstimate(), lEstimateX);
}

double
OneDEqDepHist::estimateY(const query_t& lQuery) const {

  rectangle_t r = lQuery.rectangle();

  double lEstimateY = estimateIntervalsY(r) + outlierCountY(r);


  //lEstimateX += (double) outlierCount(r);

  // return std::max<double>(1.0, lEstimate);
  return std::max<double>(minEstimate(), lEstimateY);
}



double
OneDEqDepHist::estimateIntervalsX(const rectangle_t& r) const {
  double lRes = 0.0;
  uint lower = 0;
  uint upper = vx().size();
  
  for(uint i = 0; i < vx().size(); ++i) {
    if(vx()[i].hi() <= r.xlo()) {
     lower = i;	    
     continue;
    }
  //  lower = i;
    if(r.xhi() <= vx()[i].lo()) {
      upper = i; //actually next bucket after r	    
      break;
    }
  }
 /* 
   for (uint i = 0; i < vy().size(); ++i) {
        if (vx()[i].lo() > r.xhi()) {
            upper = i;
            break;
        }
        if (vx()[i].hi() >= r.xlo()) {
            lower = i;
        }
    }*/
  //std::cout<<"lower:"<<vx()[lower].lo()<<" upper:"<<vx()[upper].hi()<<std::endl;
  //std::cout<<"xlo:"<<r.xlo()<<" xhi:"<<r.xhi()<<std::endl;
  lRes = (upper - lower) * (_totalCard / nx()); 
  return lRes;
}



double
OneDEqDepHist::estimateIntervalsY(const rectangle_t& r) const {
  double lRes = 0.0;
  uint lower = 0;
  uint upper = vx().size();
  for(uint i = 0; i < vy().size(); ++i) {
    if(vy()[i].hi() <= r.ylo()) {
      lower = i;	    
      continue;
    }
    //lower = i;
    if(r.yhi() <= vy()[i].lo()) {
      upper = i;
      break;
    }
  }
  lRes = (upper - lower ) * (_totalCard/nx());
  return lRes;
}

uint
OneDEqDepHist::outlierCountX(const rectangle_t& r) const { //TODO:add dim for x or y
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(r._pll.x <= p.x && r._pur.x > p.x) {
      lRes += p.c;
    }
  }
  return lRes;
}

uint
OneDEqDepHist::outlierCountY(const rectangle_t& r) const { //TODO:add dim for x or y
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(r._pll.y <= p.y && r._pur.y > p.y) {
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
  return 2 * nx() * ( 3 * 4 ); //NR Check with Guido
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
            << "nx, ny, phi: " << nx() << ", " << ", " << phi() 
            << ')'
            << std::endl;
  std::cout << "vx: " << std::endl;
  for(size_t i = 0; i < vx().size(); ++i) {
    std::cout << "  " << vx()[i] << std::endl;
    std::cout << "  vy " << i << std::endl;
    std::cout << "      " << vy()[i] << std::endl;
  }
  return os;
}

std::ostream&
OneDEqDepHist::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_EquiDepth) << ' '
            << 0 // subkind
            << "nx, phi: " << nx() << ", " << ", " << phi() 
            << std::endl;
  return os;
} 


} // end namespace


