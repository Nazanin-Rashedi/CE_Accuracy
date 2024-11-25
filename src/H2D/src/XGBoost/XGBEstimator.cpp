#include "XGBEstimator.hh"

namespace H2D {

XGBEstimator::XGBEstimator(const H2D::Cb &aCb,
                           // const uint num_trees,const uint num_train,
                           const uint max_card, const std::string &file_base,
                           const query_vt &queryvec,
                           const query_vt &trainQueryvec)
    : EstimatorBase2dim(aCb.theta(), aCb.q()), _max_card(max_card),
      _num_trees(aCb.xgb_num_trees()),
      _num_train_queries(aCb.xgb_num_train_queries()), _filebase(file_base),
      _queries(queryvec), _trainQueries(trainQueryvec), _preds(queryvec.size()),
      _silent(), _use_gpu(), booster(), dtrain(), dtest(), deval(), _enriched(),
      _aCb(aCb) {}

XGBEstimator::XGBEstimator(XGBEstimator &oxgb)
    : _max_card(oxgb._max_card), _num_trees(oxgb._num_trees),
      _num_train_queries(oxgb._num_train_queries), _filebase(oxgb._filebase),
      _queries(oxgb._queries), _trainQueries(oxgb._trainQueries),
      _preds(oxgb._preds), _silent(1), _use_gpu(1), booster(), dtrain(),
      dtest(), deval(), _enriched(), _aCb(oxgb._aCb)

{}
XGBEstimator &XGBEstimator::operator=(const XGBEstimator &oxgb) {
  this->_max_card = oxgb._max_card;
  this->_num_trees = oxgb._num_trees;
  this->_num_train_queries = oxgb._num_train_queries;
  this->_filebase = oxgb._filebase;
  this->_queries = oxgb._queries;
  this->_preds = oxgb._preds;
  this->_silent = 1;
  this->_use_gpu = 0;
  this->booster = NULL;
  this->dtrain = NULL;
  this->dtest = NULL;
  return *this;
}
std::ostream &XGBEstimator::print_name_param(std::ostream &os) const {
  return os;
}
uint XGBEstimator::size() const { return 0; }

query_vt XGBEstimator::read_query_file(const std::string &aFilename) {
  _queries.clear();
  std::ifstream lIs(aFilename);
  if (!lIs) {
    std::cout << "Can't open file '" << aFilename << "'." << std::endl;
    return _queries;
  }
  query_t lQuery;
  while (!lIs.eof()) {
    lIs >> lQuery._no;
    if (lIs.eof()) {
      break;
    }
    lIs >> lQuery._card;
    if (lIs.eof()) {
      break;
    }
    if (lQuery._rectangle.read(lIs)) {
      _queries.push_back(lQuery);
    } else {
      std::cout << "can't read rectangle number " << queries().size()
                << std::endl;
    }
  }
  return _queries;
}

void XGBEstimator::fill_libsvm_trainfiles() const {
  std::ofstream train_out_file;
  std::ofstream eval_out_file;
  train_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                      "_train_libsvm.dat");

  for (uint i = 0; i < (num_train_queries() - (num_train_queries() / 5)); i++) {
    const query_t &cq = trainQueries()[i];
    const rectangle_t &cr = cq.rectangle();
    train_out_file << float(std::log(cq.card()) / std::log(max_card()))
                   << " 0:" << cr._pll.x << " 1:" << cr._pur.x
                   << " 2:" << cr._pll.y << " 3:" << cr._pur.y << std::endl;
  }
  train_out_file.close();

  eval_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_eval_libsvm.dat");

  for (uint i = (num_train_queries() - (num_train_queries() / 5));
       i < num_train_queries(); i++) {
    const query_t &cq = trainQueries()[i];
    const rectangle_t &cr = cq.rectangle();

    eval_out_file << float(std::log(cq.card()) / std::log(max_card()))
                  << " 0:" << cr._pll.x << " 1:" << cr._pur.x
                  << " 2:" << cr._pll.y << " 3:" << cr._pur.y << std::endl;
  }
  eval_out_file.close();
}

void XGBEstimator::fill_libsvm_testfiles() const {
  std::ofstream test_out_file;

  test_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_test_libsvm.dat");
  // in order to check estimates for trainset too
  for (uint i = 0; i < queries().size(); i++) {
    test_out_file << float(std::log(queries()[i].card()) / std::log(max_card()))
                  << " 0:" << queries()[i].rectangle()._pll.x
                  << " 1:" << queries()[i].rectangle()._pur.x
                  << " 2:" << queries()[i].rectangle()._pll.y
                  << " 3:" << queries()[i].rectangle()._pur.y << std::endl;
  }
  test_out_file.close();
}

void XGBEstimator::train_model(const char *trainFileName,
                               const char *evalFileName)

{

  safe_xgboost(XGDMatrixCreateFromFile(trainFileName, _silent, &dtrain));
  safe_xgboost(XGDMatrixCreateFromFile(evalFileName, _silent, &deval));

  // create the booster

  DMatrixHandle eval_dmats[2] = {dtrain, deval};

  safe_xgboost(XGBoosterCreate(eval_dmats, 2, &booster));

  safe_xgboost(XGBoosterSetParam(booster, "tree_method",
                                 _use_gpu ? "gpu_hist" : "hist"));
  if (_use_gpu) {
    // set the GPU to use;
    safe_xgboost(XGBoosterSetParam(booster, "gpu_id", "0"));
  } else {
    // avoid evaluating objective and metric
    safe_xgboost(XGBoosterSetParam(booster, "gpu_id", "-1"));
  }

  safe_xgboost(XGBoosterSetParam(booster, "booster", "gbtree"));
  safe_xgboost(XGBoosterSetParam(booster, "objective", "reg:squarederror"));
  safe_xgboost(XGBoosterSetParam(booster, "eta", "0.1"));
  safe_xgboost((XGBoosterSetParam(booster, "seed", "42")))
      safe_xgboost(XGBoosterSetParam(booster, "max_depth", "3"));
  safe_xgboost(XGBoosterSetParam(booster, "verbosity", _silent ? "0" : "1"));
  safe_xgboost(XGBoosterSetParam(booster, "nthread", "10"));

  const char *eval_names[2] = {"train", "eval"};
  const char *eval_result = NULL;
  for (uint i = 0; i < _num_trees; ++i) {
    safe_xgboost(XGBoosterUpdateOneIter(booster, i, dtrain));
    safe_xgboost(XGBoosterEvalOneIter(booster, i, eval_dmats, eval_names, 2,
                                      &eval_result));
    // printf("%s\n", eval_result);
  }

  bst_ulong num_feature = 0;
  safe_xgboost(XGBoosterGetNumFeature(booster, &num_feature));

  safe_xgboost(XGBoosterGetNumFeature(booster, &num_feature));
  bst_ulong out_n_features = 0;
  const char **out_features =
      new const char *[6] { "XL", "XU", "YL", "YU", "E1", "E2" };
  float const *out_scores = NULL;

  char const scoreConfig[] = "{\"importance_type\":\"weight\"}";

  bst_ulong const *outShape;
  safe_xgboost(XGBoosterFeatureScore(booster, scoreConfig, &out_n_features,
                                     &out_features, &out_n_features, &outShape,
                                     &out_scores));

  safe_xgboost(
      XGBoosterSaveModel(booster, (_aCb.outDir()+"/" + _aCb.sds() + "/" +
                                   _aCb.ds() + "_exgb_model.json")
                                      .c_str()));
}

void XGBEstimator::run_prediction(const char *testFileName) {
  safe_xgboost(XGDMatrixCreateFromFile(testFileName, _silent, &dtest));
  float const *pred_card = NULL;

  char const config[] =
      "{\"training\": false, \"type\": 0, "
      "\"iteration_begin\": 0, \"iteration_end\": 0, \"strict_shape\": true}";

  /* Shape of output prediction */
  uint64_t const *out_shape;

  /* Dimension of output prediction */
  uint64_t out_dim;
  /* Pointer to a thread local contigious array, assigned in prediction
   * function. */

  safe_xgboost(XGBoosterPredictFromDMatrix(booster, dtest, config, &out_shape,
                                           &out_dim, &pred_card));

  float const *true_card = NULL;
  bst_ulong out_len = 0;

  safe_xgboost(XGDMatrixGetFloatInfo(dtest, "label", &out_len, &true_card));
  for (int i = 0; i < (int)out_shape[0]; i++) {
    _preds[i] = pred_card[i];
  }
}

double XGBEstimator::estimate(const rectangle_t &r) const {

  std::cout << "Not a suitable estimate method for XGBoost" << std::endl;
  return 0.0;
}

double XGBEstimator::estimate(const query_t &lQuery) const {
  double lRes = 0;
  // return unnormalized card
  lRes = round(std::exp(_preds[lQuery.no() - 1] * (std::log(max_card()))));
  return std::max<double>(minEstimate(), lRes);
}

XGBEstimator::~XGBEstimator() {
  safe_xgboost(XGBoosterFree(booster));
  safe_xgboost(XGDMatrixFree(dtrain));
  safe_xgboost(XGDMatrixFree(dtest));
}

} // namespace H2D
