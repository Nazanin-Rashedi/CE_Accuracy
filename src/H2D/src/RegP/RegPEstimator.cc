#include "RegPEstimator.hh"

namespace H2D {

RegPEstimator::RegPEstimator(const Data2dim& aData,
                             const uint      aNx,
                             const uint      aNy,
                             const double    aEpsilon,
                             const uint      aPhi,
                             const double    aQ,
                             const double    aTheta)
              : EstimatorBase2dim(aQ,aTheta), _regp(), _outlier(), _nx(aNx), _ny(aNy), _epsilon(aEpsilon), _phi(aPhi) {
  if(0 < aPhi) {
    Data2dim lRegular;
    aData.split(lRegular, _outlier, aPhi);
    _regp.initFromData2dim(aNx, aNy, lRegular);
  } else {
    _regp.initFromData2dim(aNx, aNy, aData);
  }
 
//  std::cout << "RegPEstimator: number of outliers: " << outlier().size() << std::endl;
 
}


RegPEstimator::~RegPEstimator() {}

double
RegPEstimator::estimate(const rectangle_t& r) const {
  double lEstimate = 0;
  if(0.0 < epsilon()) {
    lEstimate = regp().estimateIgnore(r, epsilon());
  } else {
    lEstimate = regp().estimate(r);
  }
  lEstimate += (double) outlierCount(r);
  return std::max<double>(minEstimate(), lEstimate);
}



double
RegPEstimator::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}


uint
RegPEstimator::outlierCount(const rectangle_t& r) const {
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
RegPEstimator::size() const {
  return nx() * ny(); // one byte should suffice due to q-compression
}

std::ostream& 
RegPEstimator::print(std::ostream& os, const int aFieldWidth) const {
  os << nx() << ' ' << ny() << std::endl;
  return regp().print(os, aFieldWidth);
}



std::ostream& 
RegPEstimator::printWithRowColSums(std::ostream& os, const int aFieldWidth) const {
  return regp().printWithRowColSums(os, aFieldWidth);
}

std::ostream&
RegPEstimator::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_RegPart) << ' '
            << 0 // subkind
            << '('
            << nx() << ',' << ny() 
            << ')'
            << std::endl;
  return os;
} 


} // end namespace


