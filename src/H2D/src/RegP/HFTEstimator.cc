#include "HFTEstimator.hh"


namespace H2D {

HFTEstimator::HFTEstimator(const int       aLogHi,
                           const int       aLogLo,
                           const uint      aKappa,
                           const uint      aPhi,
                           const double    aQ,
                           const double    aTheta,
                           const bool      aTrace) : EstimatorBase2dim(aQ, aTheta),
                                                     _br(),
                                                     _outlier(),
                                                     _logHi(aLogHi),
                                                     _logLo(aLogLo),
                                                     _kappa(aKappa),
                                                     _phi(aPhi),
                                                     _hftv(),
                                                     _restFrequency(0),
                                                     _trace(aTrace) {
}

HFTEstimator::HFTEstimator(const Data2dim& aData,
                           const int       aLogHi,
                           const int       aLogLo,
                           const uint      aKappa,
                           const uint      aPhi,
                           const double    aQ,
                           const double    aTheta,
                           const bool      aTrace) : EstimatorBase2dim(aQ, aTheta),
                                                     _br(),
                                                     _outlier(),
                                                     _logHi(aLogHi),
                                                     _logLo(aLogLo),
                                                     _kappa(aKappa),
                                                     _phi(aPhi),
                                                     _hftv(),
                                                     _restFrequency(0),
                                                     _trace(aTrace) {
  init(aData);

  // first test [only for longlat.dat]
  if(false) {
    rectangle_t lQueryRectangle(80.0, 30.0, 120.0, 40.0);
    double lTrueVal = aData.countWithin(lQueryRectangle);
    double lEstVal  = estimate(lQueryRectangle);
    std::cout << "query rectangle: " << lQueryRectangle << std::endl;
    std::cout << "true val: " << lTrueVal << std::endl;
    std::cout << "est  val: " << lEstVal  << std::endl;
  }

}

HFTEstimator::~HFTEstimator() {
  for(hft_pvt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    delete (*lIter);
  }
}


void
HFTEstimator::init(const Data2dim& aData) {
  Data2dim lRegular;

  if(10 < phi()) {
    aData.split(lRegular, _outlier, phi());
  } else {
    lRegular = aData;
  }

  lRegular.getBoundingRectangle(_br);
 
  int n = (1 << logHi());
  const double lKappa = (double) kappa();

  RegularPartitioning2dim lRegP(n, n, lRegular);

  for(int k = logHi(); k >= logLo(); --k) {
    HighlyFrequentTile* lHft = new HighlyFrequentTile(br(), n, n);
    lHft->init();
    int lCount = 0;
    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        if(lKappa < lRegP(i,j)) {
          if(false && trace()) {
            std::cout << "added: " << '[' << i << ',' << j << ',' << lRegP(i,j) << ']' << std::endl;
          }
          lHft->step(i, j, lRegP(i,j));
          lRegP.set(i, j, 0);
          ++lCount;
        }
      }
    }
    
    lHft->fin();
    _hftv.push_back(lHft);
   
    if(false && trace()) {
      std::cout << "[0][0]: ";
      _hftv[0]->print(std::cout, 0);
      std::cout << std::endl;
    }

    if(trace()) {
      std::cout << "at step " << k << " inserted " << lCount << std::endl;
      std::cout << "matrix: " << std::endl;
      lRegP.print(std::cout, n, n, 4);
      std::cout << "matrix: " << std::endl;
    } 
    if(k > logLo()) {
      n >>= 1;
      lRegP.shrink2(n,n);
    }
  }

  double lRestFreq = 0;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      lRestFreq += lRegP(i,j);
    }
  }
  _restFrequency = lRestFreq;

  if(false && trace()) {
    std::cout << "[0][0]: ";
    _hftv[0]->print(std::cout, 0);
    std::cout << std::endl;
    std::cout << "complete _hftv[0]: ";
    _hftv[0]->print(std::cout);
    std::cout << std::endl;
    std::cout << "cumulated residual frequency: " << restFreq() << std::endl;
  }
}


double
HFTEstimator::estimate(const rectangle_t& aQueryRectangle) const {
  double lRes = 0;

  if(trace()) {
    std::cout << "HFTEstimator::estimate: hf().size = " << hf().size() << std::endl;
  }

  // estimate intersection with highly frequent tiles
  for(hft_pvt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    const double lEstimate = (*lIter)->estimate(aQueryRectangle);
    lRes += lEstimate;
    if(trace()) {
      std::cout << "   estimate: " << lEstimate << std::endl;
    }
  }

  // estimate rest
  rectangle_t lIsec;
  lIsec.isec(br(), aQueryRectangle);
  const double lRestEstimate = (lIsec.area() / br().area()) * restFreq();
  lRes += lRestEstimate;

  if(trace()) {
    std::cout << "   residual estimate: " << lRestEstimate << std::endl;
  }

  // add outlier frequencies
  lRes += outlierCount(aQueryRectangle);

  return std::max<double>(minEstimate(), lRes);
}




double
HFTEstimator::estimate(const query_t& lQuery) const {
  double lRes = 0;
  rectangle_t aQueryRectangle= lQuery.rectangle();
  if(trace()) {
    std::cout << "HFTEstimator::estimate: hf().size = " << hf().size() << std::endl;
  }

  // estimate intersection with highly frequent tiles
  for(hft_pvt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    const double lEstimate = (*lIter)->estimate(aQueryRectangle);
    lRes += lEstimate;
    if(trace()) {
      std::cout << "   estimate: " << lEstimate << std::endl;
    }
  }

  // estimate rest
  rectangle_t lIsec;
  lIsec.isec(br(), aQueryRectangle);
  const double lRestEstimate = (lIsec.area() / br().area()) * restFreq();
  lRes += lRestEstimate;

  if(trace()) {
    std::cout << "   residual estimate: " << lRestEstimate << std::endl;
  }

  // add outlier frequencies
  lRes += outlierCount(aQueryRectangle);

  return std::max<double>(minEstimate(), lRes);
}


uint
HFTEstimator::outlierCount(const rectangle_t& aQueryRectangle) const {
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(aQueryRectangle.containsHalfOpen(p.x, p.y)) {
      lRes += p.c;
    }
  }
  return lRes;
}

uint
HFTEstimator::size() const {
  uint lRes = 0;
  for(hft_pvt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    lRes += (*lIter)->size();
  }
  return lRes;
}

std::ostream&
HFTEstimator::print(std::ostream& os) const {
  std::cout << "bounding rectangle: " << br() << std::endl
            << "cumulated residual frequency: " << restFreq() << std::endl
            << "hfts: " << std::endl;
  for(hft_pvt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    (*lIter)->print(os);
  }
  return os;
  
}

std::ostream&
HFTEstimator::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_HFT) << ' '
            << 0 // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 


} // end namespace


