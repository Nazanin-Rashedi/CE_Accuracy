#ifndef XGBEnrichedEBO_HH
#define XGBEnrichedEBO_HH
#include "EstimatorArea/EstimatorArea.hh"
#include "OneDEqDepHist/OneDEqDepHist.hh"
#include "Sampling/Sample2dim.hh"
#include "XGBoost/XGBEstimator.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/cb.hh"
#include "infra/data2dim.hh"
#include "infra/types.hh"
#include </home/rashedi/xgboost/include/xgboost/c_api.h>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

namespace H2D {

class LWXGB : public XGBEstimator {
public:
  LWXGB(const Data2dim &aData, const Cb &aCb, const uint max_card,
                 const std::string &file_base, const query_vt &queryvec,
                 const query_vt &trainQueryvec);

public:
  virtual void fill_libsvm_trainfiles() const;
  virtual void fill_libsvm_testfiles() const;

private:
  Data2dim _data;
  uint _sampleSize;
  OneDEqDepHist _oneEq;
};

} // namespace H2D

#endif
