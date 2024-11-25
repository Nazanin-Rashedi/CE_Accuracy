#include "process_query_file.hh"

extern "C" {
#include "infra/cmeasure.h"
}

namespace H2D {

ProcessQueryFile::ProcessQueryFile(const std::string &aDirIn,
                                   const std::string &aDirOut, Cb &aCb)
    : _dir_in(aDirIn), _dir_out(aDirOut), _filebase(), _data(), _query(),
      _esteval(), _construction_time_s(), _avg_query_time_us(),
      _trace(aCb.trace()), _trainQuery() {
  init(aDirIn, aDirOut, aCb);
}

bool ProcessQueryFile::init(const std::string &aDirIn,
                            const std::string &aDirOut, Cb &aCb) {
  // 0. set filebase
  _filebase = aCb.inDir() + '/' + aCb.sds() + '/' + aCb.ds();

  // 1. read data file
  const std::string lFilenameData = filebase() + ".hist";
  if (!read_data_file(lFilenameData)) {
    std::cout << "Can't read data file '" << lFilenameData << "'." << std::endl;
    return false;
  }

  // 2. read query file
  const std::string train_path = aCb.trainQDir()+'/' +
                                 aCb.sds() + '/' + "train_queries/" + aCb.ds() +
                                 ".qu_t100k";
  if(!read_train_query_file(train_path)){
	  std::cout<<"Can't read train query file '"<<train_path<<"';." 
		  <<std::endl;
	  return false;
  }


  //std::ofstream trainQueries(train_path, std::ios::out | std::ios::app);

  //generate_train_query_2000(data(), aCb, aCb.xgb_num_train_queries()); //

  const std::string lFilenameQuery =
      aCb.testQDir()+'/' + aCb.sds() + '/' + aCb.ds() + ".qu_a";
  if (!read_query_file(lFilenameQuery)) {
    std::cout << "Can't read query file '" << lFilenameQuery << "'."
              << std::endl;
    return false;
  }

  // 5. calculate budget and sample size
  const double lTotal = data().total();
  const uint lSampleSize =
      (uint)std::ceil(std::sqrt(lTotal * std::log(lTotal)));
  const uint lBudgetByte = 8 * lSampleSize;
  aCb.budget(lBudgetByte);
  aCb.sampleSize(lSampleSize);
  _esteval.setTotalCard(lTotal);


  // 6. calculate theta
  aCb.theta(std::ceil(std::sqrt(lTotal / std::log(lTotal))));

  // 7. fixed outlier definition
  aCb.phi(100); // outlier definition

  // 8. set members estimate evaluator
  _esteval.push_back(1);
  for (uint i = 1; i <= 5; ++i) {
    _esteval.push_back(i * aCb.theta());
  }

  if (trace()) {
    std::cout << "#points  = " << lTotal << std::endl;
    std::cout << "#nodv    = " << data().size() << std::endl;
    std::cout << "#queries = " << query().size() << std::endl;
    std::cout << "#samples = " << aCb.sampleSize() << std::endl;
    std::cout << "#budget  = " << aCb.budget() << std::endl;
    std::cout << "#theta   = " << aCb.theta() << std::endl;
  }

  return true;
}

bool ProcessQueryFile::run(const Cb &aCb) {
  summaryline_t lSummaryline;
  lSummaryline._card = data().total();
  lSummaryline._noDv = data().size();
  lSummaryline._budget = aCb.budget();
  lSummaryline._filename = aCb.sds() + '/' + aCb.ds();

  return run_all_estimator(lSummaryline, aCb);
}

bool ProcessQueryFile::run_all_estimator(summaryline_t &aSummaryline,
                                         const Cb &aCb) {
  if (aCb.estArea()) {
    run_one_estimator(aSummaryline, H2D::H2D_EST_AREA, aCb);
  }
  if (aCb.regp()) {
    run_one_estimator(aSummaryline, H2D::H2D_RegPart, aCb);
  }
  if (aCb.eqd()) {
    run_one_estimator(aSummaryline, H2D::H2D_EquiDepth, aCb);
  }
  if (aCb.mhist2()) {
    run_one_estimator(aSummaryline, H2D::H2D_MHIST2, aCb);
  }
  if (aCb.qts()) {

    run_one_estimator(aSummaryline, H2D::H2D_QTS, aCb);
  }
  if (aCb.iqts()) {
    run_one_estimator(aSummaryline, H2D::H2D_IQTS, aCb);
  }
  if (aCb.gxtree()) {
    run_one_estimator(aSummaryline, H2D::H2D_GXTREE, aCb);
  }
  if (aCb.sampling()) {
    run_one_estimator(aSummaryline, H2D::H2D_Sampling, aCb);
  }
  if (aCb.xgb()) {
    run_one_estimator(aSummaryline, H2D::H2D_XGB, aCb);
  }
  if (aCb.exgb()) {
    run_one_estimator(aSummaryline, H2D::H2D_EXGB, aCb);
  }

  if (aCb.lwxgb()) {
    run_one_estimator(aSummaryline, H2D::H2D_LWXGB, aCb);
  }

  if (aCb.nxgb()) {
    run_one_estimator(aSummaryline, H2D::H2D_NXGB, aCb);
  }

  if (aCb.nreqd()) {

    run_one_estimator(aSummaryline, H2D::H2D_NREQD, aCb);
  }

  return true;
}

bool ProcessQueryFile::run_one_estimator(summaryline_t &aSummaryline,
                                         const H2D_kind_t aEstKind,
                                         const Cb &aCb) {

  EstimatorBase2dim *lEstimator = new_estimator(aSummaryline, aEstKind, aCb);
  if (nullptr == lEstimator) {
    std::cout << "estimator currently not supported: "
              << h2d_kind_name(aEstKind) << std::endl;
    return false;
  }
  _esteval.init();
  // uint lCount = 0;
  /*
  std::cout<< "................."<<std::endl;
  std::cout << "#samples = " << aCb.sampleSize() << std::endl;
  std::cout << "#budget  = " << aCb.budget() << std::endl;
  std::cout << "#theta   = " << aCb.theta() << std::endl;
  std::cout<< "................."<<std::endl;
*/
  cmeasure_t lMeasNR;
  cmeasure_start(&lMeasNR);
  for (const auto &lQuery : query()) {
    // std::cout << "Query_" << (lQuery.no()) << std::endl;
    const double lEstimate = lEstimator->estimate(
        lQuery); // NR: estimate(lQuery) be jaye lQuery.rectangle()
    _esteval.step(lQuery.card(), lEstimate);

    _esteval.nstep(lQuery.card(), lEstimate);
    //    _esteval.dstep(lQuery.no(),lQuery.card(), lEstimate);
  }
  cmeasure_stop(&lMeasNR);

  _esteval.fin();

  // std::cout << "Estimator " << h2d_kind_name(aEstKind) << std::endl;

  //    std::cout << "result long:" << std::endl;
  //  _esteval.print(std::cout);
  //_esteval.printAllforOneEst(std::cout);
  //
  _esteval.nprint(std::cout);
  // _esteval.dprint(std::cout);
  const uint lTotal = data().total();
  const uint lCardClass = ((uint)std::floor(std::log2(lTotal)));
  const std::string lLineMarker =
      std::string("! qerr-profile ") + aCb.sds() + ' ' + aCb.ds() + ' ' +
      std::to_string(lCardClass) + ' ' + std::to_string(query().size()) + ' ' +
      h2d_kind_name(aEstKind) + ' ' + std::to_string(aEstKind);

  delete lEstimator;
  return true;
}

bool ProcessQueryFile::fin(const Cb &aCb) { return true; }

int ProcessQueryFile::generate_train_query(std::ostream &os,
                                           const H2D::Data2dim &aData,
                                           const H2D::Cb &aCb, uint no_query) {
  // rng32_t lRng;
  typedef std::mt19937 rng32_t;
  std::random_device rd;
  rng32_t lRng(rd());
  std::uniform_int_distribution<uint> lIntDist(0, aData.size() - 1);
  std::uniform_real_distribution<double> lDubDist(0, 0.1);

  H2D::rectangle_t lBr; // bounding rectangle of all data points
  aData.getBoundingRectangle(lBr);

  H2D::rectangle_t lQr; // query rectangle

  double f = 0;
  for (uint i = 0; i < no_query;) {
    const uint lTupleNo = lIntDist(lRng);
    const H2D::xyc_t &t = aData[lTupleNo];
    f = lDubDist(lRng);
    lQr.xlo(t.x + f * (lBr.xlo() - t.x));
    f = lDubDist(lRng);
    lQr.xhi(t.x + f * (lBr.xhi() - t.x));
    f = lDubDist(lRng);
    lQr.ylo(t.y + f * (lBr.ylo() - t.y));
    f = lDubDist(lRng);
    lQr.yhi(t.y + f * (lBr.yhi() - t.y));
    f = lDubDist(lRng);
    const uint lCard = aData.countWithin(lQr);
    if (0 < lCard) {
      ++i;
      os << i << ' ' << lCard << ' ' << lQr << std::endl;
    }
  }
  return 1;
}

bool ProcessQueryFile::read_data_file(const std::string &aFilename) {
  _data.readHistFile(aFilename);
  if (0 == _data.size()) {
    std::cout << "Can't read data file '" << aFilename << "'." << std::endl;
    return false;
  }
  return true;
}
bool ProcessQueryFile::read_train_query_file(const std::string &aFilename) {
  _trainQuery.clear();
  std::ifstream lIs(aFilename);
  if (!lIs) {
    std::cout << "Can't open file '" << aFilename << "'." << std::endl;
    return false;
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
      _trainQuery.push_back(lQuery);
      if (_trainQuery.size() >= 20000) {
        break;
      }
    } else {
      std::cout << "can't read rectangle number " << trainQuery().size()
                << std::endl;
      return false;
    }
  }
  return (20000 == trainQuery().size());
}

int ProcessQueryFile::generate_train_query_2000(const H2D::Data2dim &aData,
                                                const H2D::Cb &aCb,
                                                uint no_query) {
  // rng32_t lRng;
  _trainQuery.clear();
  typedef std::mt19937 rng32_t;
  std::random_device rd;
  rng32_t lRng(rd());
  std::uniform_int_distribution<uint> lIntDist(0, aData.size() - 1);
  std::uniform_real_distribution<double> lDubDist(0, 0.1);

  H2D::rectangle_t lBr; // bounding rectangle of all data points
  aData.getBoundingRectangle(lBr);

  H2D::rectangle_t lQr; // query rectangle

  double f = 0;
  query_t lQuery;
  for (uint i = 0; i < no_query; i++) {
    const uint lTupleNo = lIntDist(lRng);
    const H2D::xyc_t &t = aData[lTupleNo];
    f = lDubDist(lRng);
    lQr.xlo(t.x + f * (lBr.xlo() - t.x));
    f = lDubDist(lRng);
    lQr.xhi(t.x + f * (lBr.xhi() - t.x));
    f = lDubDist(lRng);
    lQr.ylo(t.y + f * (lBr.ylo() - t.y));
    f = lDubDist(lRng);
    lQr.yhi(t.y + f * (lBr.yhi() - t.y));
    f = lDubDist(lRng);
    const uint lCard = aData.countWithin(lQr);
    if (0 < lCard) {
      lQuery._rectangle = lQr;
      lQuery._no = i + 1;
      lQuery._card = lCard;
    }
    // std::cout<<"one query added:" <<lQuery._no<<std::endl;
    _trainQuery.push_back(lQuery);
  }
  return _trainQuery.size();
}

bool ProcessQueryFile::read_query_file(const std::string &aFilename) {
  _query.clear();
  std::ifstream lIs(aFilename);
  if (!lIs) {
    std::cout << "Can't open file '" << aFilename << "'." << std::endl;
    return false;
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
      _query.push_back(lQuery);
    } else {
      std::cout << "can't read rectangle number " << query().size()
                << std::endl;
      return false;
    }
  }
  return (0 < query().size());
}

/*
 * create a new estimator
 * with default parameters (only external parameter is budget/samplesize)
 */

EstimatorBase2dim *ProcessQueryFile::new_estimator(summaryline_t &aSummaryline,
                                                   const H2D_kind_t aEstKind,
                                                   const H2D::Cb &aCb) {
  cmeasure_t lMeas;
  H2D::EstimatorBase2dim *lRes = nullptr;
  const uint lPhi = aCb.phi(); // outlier definition
  const double lQ = 2;         // no relevance
  const uint lTheta = 1;       // minimum returned by every estimator
  aSummaryline._phi = lPhi;
  aSummaryline._theta = lTheta;
  aSummaryline._nout = 0;
  aSummaryline._kind = aEstKind;

  switch (aEstKind) {
  case H2D::H2D_RegPart: {
    const uint n = std::ceil(std::sqrt(aCb.budget()));
    cmeasure_start(&lMeas);
    H2D::RegPEstimator *lRegP =
        new RegPEstimator(data(), n, n, 0.0, lPhi, lQ, lTheta);
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lRegP->size();
    aSummaryline._subkind = 0;
    aSummaryline._nout = lRegP->noOutlier();
    aSummaryline._param._regp._nx = n;
    aSummaryline._param._regp._ny = n;
    aSummaryline._param._regp._maxfreq = lRegP->maxEntry();
    if (trace()) {
      std::cout << "size = " << lRegP->size() << std::endl;
    }
    lRes = lRegP;
  } break;
  case H2D::H2D_EquiDepth: {
    double lDNx = 0;
    const EqDepHist::kind_t lEqdKind = EqDepHist::k_simple;
    if (0 == lEqdKind) {
      lDNx = mt::solveQuadraticEquationPos<double>(9, 9, 8 - (int)aCb.budget());
    } else {
      const double s = 12 + aCb.rx() * aCb.ry();
      lDNx =
          mt::solveQuadraticEquationPos<double>(s, 12, 8 - (int)aCb.budget());
      assert(0 == 1); // do not use this path
    }
    const int lNxHi = (int)mt::ceilt(lDNx);
    uint lNx = 0;
    uint lNy = 0;
    lNx = lNy = lNxHi; // give it more space, its bad anyway
    cmeasure_start(&lMeas);
    H2D::EqDepHist *lEqd =
        new EqDepHist(data(), EqDepHist::k_simple, lNx,
                      lNy,  // nx, ny // TODO: this is too small
                      0, 0, // rx, ry
                      lPhi, lQ, lTheta, aCb.trace());
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lEqd->size();
    aSummaryline._subkind = 0;
    aSummaryline._nout = lEqd->noOutlier();
    aSummaryline._param._eqdh._nx = lNx;
    aSummaryline._param._eqdh._ny = lNy;
    aSummaryline._param._eqdh._rx = 0;
    aSummaryline._param._eqdh._ry = 0;
    if (trace()) {
      std::cout << "size = " << lEqd->size() << std::endl;
    }
    lRes = lEqd;
  } break;

  case H2D::H2D_NREQD: {
    cmeasure_start(&lMeas);
    uint s = (aCb.sampleSize()) / 6;

    uint nX =
        std::ceil(s < 300 ? s : std::min(500.0, 300.0 + std::sqrt(s - 300.0)));

    H2D::OneDEqDepHist *lEqd =
        new OneDEqDepHist(data(), lPhi, lQ, lTheta, nX, aCb.trace());
    cmeasure_stop(&lMeas);

    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lEqd->size();
    aSummaryline._subkind = 0;
    aSummaryline._nout = lEqd->noOutlier();
    if (trace()) {
      std::cout << "size = " << lEqd->size() << std::endl;
    }
    lRes = lEqd;
  } break;

  case H2D::H2D_Sampling: {
    cmeasure_start(&lMeas);
    lRes = new Sample2dim(data(), aCb.sampleSize(), lQ, lTheta, false);
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lRes->size();
    aSummaryline._subkind = 0;
    aSummaryline._param._sampling._noSamples = aCb.sampleSize();
  } break;

  case H2D::H2D_SAMPLALPHA:
    break;

  case H2D::H2D_XGB: {
    cmeasure_start(&lMeas);
    lRes = new XGBEstimator(aCb, data().total(), filebase(), query(),
                            trainQuery());

    lRes->fill_libsvm_trainfiles();
    lRes->train_model((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_train_libsvm.dat")
                          .c_str(),
                      (aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_eval_libsvm.dat")
                          .c_str());
    cmeasure_stop(&lMeas);
    /*
    std::cout << "train model time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    cmeasure_start(&lMeas);

    cmeasure_start(&lMeas);
    lRes->fill_libsvm_testfiles();
    lRes->run_prediction((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                          "_test_libsvm.dat")
                             .c_str());
  /*  cmeasure_stop(&lMeas);
    std::cout << "XGB 1M queries pred time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    // cmeasure_stop(&lMeas);
    // aSummaryline._xgb_predTime = cmeasure_total_s(&lMeas);
    // cmeasure_div_us(&lMeas, query().size());

  } break;
  case H2D::H2D_EXGB: {
    cmeasure_start(&lMeas);
    lRes = new EXGB(data(), aCb,
                           //	aCb.sampleSize(),
                           //	aCb.xgb_num_trees(),
                           //	aCb.xgb_num_train_queries(),
                           //		lTheta,
                           //		lQ,
                           // aCb.budget()
                           //,lPhi,
                           data().total(), filebase(), query(), trainQuery());
    // std::cout<<"sakht"<<std::endl;
    lRes->fill_libsvm_trainfiles();
    lRes->train_model((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_exgb_train_libsvm.dat")
                          .c_str(),
                      (aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_exgb_eval_libsvm.dat")
                          .c_str());

    cmeasure_stop(&lMeas);
    /*std::cout << "exgb train model time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    cmeasure_start(&lMeas);
    lRes->fill_libsvm_testfiles();
    lRes->run_prediction((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                          "_exgb_test_libsvm.dat")
                             .c_str());
    cmeasure_stop(&lMeas);
    /*std::cout << "EXGB 1M query pred time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    // aSummaryline._xgb_predTimePerQuery = cmeasure_div_us(&lMeas,
    // query().size());
    aSummaryline._xgb_predTime = cmeasure_total_s(&lMeas);

  } break;

  case H2D::H2D_NXGB: {

  } break;

  case H2D::H2D_LWXGB: {
    cmeasure_start(&lMeas);
    lRes = new LWXGB(data(), aCb, data().total(), filebase(), query(),
                              trainQuery());

    lRes->fill_libsvm_trainfiles();
    lRes->train_model((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_lwxgb_train_libsvm.dat")
                          .c_str(),
                      (aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                       "_lwxgb_eval_libsvm.dat")
                          .c_str());

    cmeasure_stop(&lMeas);
    /*
    std::cout << "lw-xgb train model time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    cmeasure_start(&lMeas);
    lRes->fill_libsvm_testfiles();
    lRes->run_prediction((aCb.inDir()+"/" + aCb.sds() + "/" + aCb.ds() +
                          "_lwxgb_test_libsvm.dat")
                             .c_str());
    cmeasure_stop(&lMeas);
    /*
    std::cout << "LW-XGB 1M query pred time(s):" << std::endl;
    std::cout << cmeasure_total_s(&lMeas) << std::endl;
    std::cout << "************************************************"
              << std::endl;
*/
    aSummaryline._xgb_predTime = cmeasure_total_s(&lMeas);

  } break;

  case H2D::H2D_QTS: {
    H2D::QTS::kind_t lQTSKind = (H2D::QTS::kind_t)aCb.kind();
    cmeasure_start(&lMeas);
    H2D::QTS *lQts = new H2D::QTS(data(), lQTSKind, aCb.budget(), lPhi, lQ,
                                  lTheta, aCb.trace());
    cmeasure_stop(&lMeas);

    if (trace()) {
      std::cout << "QTS:" << std::endl;
      std::cout << "       q: " << lQts->q() << std::endl;
      std::cout << "   theta: " << lQts->theta() << std::endl;
      std::cout << "    kind: " << lQts->kind() << std::endl;
      std::cout << "   depth: " << lQts->depth() << std::endl;
      std::cout << "#outlier: " << lQts->noOutlier() << std::endl;
    }
    lRes = lQts;
  } break;
  case H2D::H2D_IQTS: {
    H2D::IQTS::kind_t lIQTSKind = (H2D::IQTS::kind_t)aCb.kind();
    // H2D::IQTS::kind_t lIQTSKind = H2D::IQTS::k_lq_card;
    cmeasure_start(&lMeas);
    H2D::IQTS *lIQTS =
        new H2D::IQTS(data(), lIQTSKind, aCb.budget(), lPhi, lQ, lTheta, false);
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lIQTS->size();
    aSummaryline._subkind = lIQTSKind;
    aSummaryline._nout = lIQTS->noOutlier();
    aSummaryline._param._iqts._depth = lIQTS->depth();
    aSummaryline._param._iqts._noNodes = lIQTS->noNodes();
    aSummaryline._param._iqts._noNotRefined = lIQTS->noNotRefined();
    aSummaryline._param._iqts._noLT23 = lIQTS->noLT23();
    aSummaryline._param._iqts._noLT24 = lIQTS->noLT24();
    aSummaryline._param._iqts._noLT2p = lIQTS->noLT2p();

    if (trace()) {
      std::cout << "IQTS:" << std::endl;
      std::cout << "       q: " << lIQTS->q() << std::endl;
      std::cout << "   theta: " << lIQTS->theta() << std::endl;
      std::cout << "    kind: " << lIQTS->kind() << std::endl;
      std::cout << "   depth: " << lIQTS->depth() << std::endl;
      std::cout << "   #node: " << lIQTS->noNodes() << std::endl;
      std::cout << "    size: " << lIQTS->size() << std::endl;
      std::cout << "#outlier: " << lIQTS->noOutlier() << std::endl;
    }
    lRes = lIQTS;
  } break;
  case H2D::H2D_FLEXTREE:
    break;
  case H2D::H2D_GXTREE: {
    H2D::GxTree *lGxt = 0;
    H2D::GxTreeItp *lGxtItp = 0;
    const bool lCheckEncoding = false;
    cmeasure_start(&lMeas);
    lGxt = new H2D::GxTree(data(), GxTree::K_GLMS, aCb.budget(),
                           aCb.leafRefinement(), aCb.lrf(),
                           aCb.minimumNodeTotal(), lPhi, lQ, lTheta, false);
    lGxt->encode(lCheckEncoding);
    // lGxt->printEncodingInfo(std::cout);

    lGxtItp = new H2D::GxTreeItp(lGxt->outlier(), lGxt->encoding(), false);
    lGxt->_gxtitp = lGxtItp;
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lGxt->size();
    aSummaryline._subkind = GxTree::K_GLMS;
    aSummaryline._nout = lGxt->noOutlier();
    aSummaryline._param._gxtree._leafRefinement = aCb.leafRefinement();
    aSummaryline._param._gxtree._lrf = aCb.lrf();
    aSummaryline._param._gxtree._minimumNodeTotal =
        aSummaryline._param._gxtree._depth = lGxt->depth();
    aSummaryline._param._gxtree._noNodes = lGxt->noNodes();
    aSummaryline._param._gxtree._noGNodes = lGxt->noGNodes();
    aSummaryline._param._gxtree._minSplit = lGxt->minSplit();
    aSummaryline._param._gxtree._maxUnsplit = lGxt->maxUnsplit();
    aSummaryline._param._gxtree._encoded = 'E';
    if (trace()) {
      std::cout << std::endl;
      std::cout << "Gx-Tree:" << std::endl;
      std::cout << "       q: " << lGxt->q() << std::endl;
      std::cout << "   theta: " << lGxt->theta() << std::endl;
      std::cout << "    kind: " << lGxt->kind() << std::endl;
      std::cout << "   depth: " << lGxt->depth() << std::endl;
      std::cout << "   #node: " << lGxt->noNodes() << std::endl;
      std::cout << " minleaf: " << lGxt->minLeafCumFreq() << std::endl;
      std::cout << " maxleaf: " << lGxt->maxLeafCumFreq() << std::endl;
      std::cout << "    size: " << lGxt->calcSize() << std::endl;
      std::cout << "    size: " << lGxt->size() << std::endl;
      std::cout << "    size: " << lGxtItp->size() << std::endl;
      std::cout << "#outlier: " << lGxt->noOutlier() << std::endl;
      std::cout << " #points: " << data().size() << std::endl;
      std::cout << "max freq: " << data().maxFrequency() << std::endl;
      std::cout << "tree theta: " << lGxt->theta() << std::endl;
      std::cout << "itp  theta: " << lGxtItp->theta() << std::endl;
      std::cout << std::endl;
    }
    delete lGxt;
    lRes = lGxtItp;
  } break;
  case H2D::H2D_EquiDepth2: {
  } break;
  case H2D::H2D_MHIST2: {
    const uint lNoBuckets =
        std::ceil((double)aCb.budget() / H2D::MHist2Bucket::size());
    // H2D::MHist2::kind_t lKind = H2D::MHist2::k_maxDiffSpread; // best
    //  H2D::MHist2::kind_t lKind = H2D::MHist2::k_maxSpread;     // best
    //  H2D::MHist2::kind_t lKind = H2D::MHist2::k_maxDiffCount;
    //  H2D::MHist2::kind_t lKind = H2D::MHist2::k_maxDiffArea;
    //  H2D::MHist2::kind_t lKind = H2D::MHist2::k_minVariance;
    H2D::MHist2::kind_t lKind = (H2D::MHist2::kind_t)aCb.kind();
    cmeasure_start(&lMeas);
    H2D::MHist2 *lMHist2 = new H2D::MHist2(lNoBuckets, lKind, data());
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);
    aSummaryline._size = lMHist2->size();
    aSummaryline._subkind = lKind;
    aSummaryline._param._mhist._noBuckets = lNoBuckets;
    if (trace()) {
      std::cout << "MHist2:" << std::endl;
      std::cout << "    size = " << lMHist2->size() << std::endl;
      std::cout << "    kind: " << lMHist2->kind() << std::endl;
    }
    lRes = lMHist2;
  } break;
  case H2D::H2D_EST_CONST:
    break;
  case H2D::H2D_EST_AREA: {

    cmeasure_start(&lMeas);
    lRes = new EstimatorArea(data(), lTheta);
    cmeasure_stop(&lMeas);
    aSummaryline._constructionTime = cmeasure_total_s(&lMeas);

  } break;

  case H2D::H2D_INDEP:
    break;
  case H2D::H2D_ZHist:
    break;

  default:
    std::cout << "Bad estimator kind: " << aEstKind << std::endl;
    break;
  }
  return lRes;
}
} // namespace H2D
