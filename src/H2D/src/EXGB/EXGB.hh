#ifndef XGBEnriched_HH
#define XGBEnriched_HH
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

class EXGB : public XGBEstimator {
public:
  EXGB(const Data2dim &aData, const H2D::Cb &aCb, const uint max_card,
              const std::string &file_base, const query_vt &queryvec,
              const query_vt &trainQueryvec);

public:
  virtual void fill_libsvm_trainfiles() const;
  virtual void fill_libsvm_testfiles() const;

private:
  Data2dim _data;
  uint _sampleSize;

  EstimatorArea _areaEst;
  Sample2dim _sampleEst;
};

} // namespace H2D

#endif
