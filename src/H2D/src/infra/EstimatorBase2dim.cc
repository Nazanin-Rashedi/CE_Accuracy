#include "EstimatorBase2dim.hh"

namespace H2D {


uint EstimatorBase2dim::_domValSize  = 4;
uint EstimatorBase2dim::_freqValSize = 4;

EstimatorBase2dim::~EstimatorBase2dim() {}

void EstimatorBase2dim::run_prediction(const char* testFileName){}
void EstimatorBase2dim::train_model(const char* trainFileName , const char* evalFileName){}
void EstimatorBase2dim::fill_libsvm_trainfiles() const{}
void EstimatorBase2dim::fill_libsvm_testfiles() const{}


} // end namespace

