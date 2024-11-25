#include "Sample2dim.hh"

namespace H2D {


Sample2dim::Sample2dim(const Data2dim& aData,
                       const uint      aSampleSize,
                       const double    aQ,
                       const double    aTheta,
                       const bool      aTrace) 
           : EstimatorBase2dim(aQ, aTheta), 
            _data(), 
            _dataSize(aData.size()),
            _trace(aTrace) {
  // assert(aData.size() / 2 > aSampleSize);  // XXX

  std::set<uint> lIdxSet;
  std::mt19937 lRng;

  // expand aData
  Data2dim lData;
  for(uint i = 0; i < aData.size(); ++i) {
    const xyc_t& lXyc = aData[i];
    for(uint j = 0; j < lXyc.c; ++j) {
      lData.push_back(xyc_t(lXyc.x, lXyc.y, 1));
    }
  }

  if(trace()) {
    std::cout << "Sampling:" << std::endl
              << "   #DV  = " << aData.size() << std::endl
              << "   CARD = " << aData.total() << std::endl
              << "  lData.size() = " << lData.size() << std::endl
              << std::endl;
  }

  if((aData.size() / 2) > aSampleSize) {
    // good case, sufficiently many data points
    while(lIdxSet.size() < aSampleSize) {
      lIdxSet.insert(lRng() % lData.size());
    }

    for(std::set<uint>::const_iterator lIter = lIdxSet.begin();
        lIter != lIdxSet.end();
        ++lIter) {
      _data.push_back(lData[*lIter]);
    }
  } else {
    // very little data points
    for(uint i = 0; (i < lData.size()) && (_data.size() < aSampleSize); ++i) {
      _data.push_back(lData[i]);
    }
  }

 // std::cout<<"*****"<<_data.size()<<std::endl;
  
}

Sample2dim::~Sample2dim() {
}

uint
Sample2dim::size() const {
  return 8 * sampleSize(); // assume 4B per x,y
}


double 
Sample2dim::estimate(const query_t& lQuery) const {
  double lRes = 0;
  lRes = _data.countWithin(lQuery.rectangle());
  lRes = round((double) lRes * ((double) dataSize() / (double) sampleSize()));
  return std::max<double>(minEstimate(), lRes);
}


double
Sample2dim::estimate(const rectangle_t& r) const {
  double lRes = 0;
  lRes = _data.countWithin(r);
  lRes = round((double) lRes * ((double) dataSize() / (double) sampleSize()));
  return std::max<double>(minEstimate(), lRes);
}


std::ostream&
Sample2dim::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_Sampling) << ' '
            << 0 // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 


} // end namespace


