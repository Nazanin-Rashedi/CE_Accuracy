#include "LWXGB.hh"

namespace H2D {

LWXGB::LWXGB(const Data2dim &aData, const Cb &aCb,
                               const uint max_card,
                               const std::string &file_base,
                               const query_vt &queryvec,
                               const query_vt &trainQueryvec)
    : XGBEstimator(aCb, // aCb.xgb_num_trees(),aCb.xgb_num_train_queries(),
                   max_card, file_base, queryvec, trainQueryvec),
      _data(aData), _sampleSize(aCb.sampleSize()),

      _oneEq(OneDEqDepHist(_data, _aCb.phi(), q(), theta(), _aCb.sampleSize(),
                           false)) {}

void LWXGB::fill_libsvm_trainfiles() const {
  //  EstimatorArea* areaEst = new EstimatorArea(_data,theta());
  // H2D::QTS1D::kind_t lQTS1Kind = H2D::QTS1D::k_card;
  // H2D::QTS1D lQts1(data(), lQTS1Kind, aCb.budget(),aCb.phi(), false,0);
  // H2D::QTS1D lQts2(data(), lQTS1Kind, aCb.budget(),aCb.phi(), false,1);
  //  Sample2dim* sampleEst = new Sample2dim(_data, _sampleSize, q(), theta(),
  //  false); Sampling_NRAlphaOmega* sampleEst = new
  //  Sampling_NRAlphaOmega(_data, _sampleSize, q(), theta(), false,1e-5);
  std::ofstream train_out_file;
  std::ofstream eval_out_file;
  train_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                      "_lwxgb_train_libsvm.dat");
  const double total_data = _data.total();
  const double log_max_card = std::log(max_card());
  uint minSelEst;
  uint eboSelEst;
  uint indepEst;
  double minSel;
  double maxSel;
  double eboSel;
  double indep;
  //#pragma omp parallel for
  for (uint i = 0; i < (num_train_queries() - (num_train_queries() / 5)); i++) {

    const query_t &current_query = trainQueries()[i];
    const rectangle_t &current_rect = current_query.rectangle();

    double xSel = 1.0 * _oneEq.estimateX(current_query) / total_data;
    double ySel = 1.0 * _oneEq.estimateY(current_query) / total_data;
    double log_card_ratio = std::log(current_query.card()) / log_max_card;

    // Avoid division and sqrt inside the loop
    minSel = std::min(xSel, ySel);
    maxSel = std::max(xSel, ySel);
    eboSel = 1.0 * total_data * minSel * std::sqrt(maxSel);
    indep = xSel * ySel * total_data;

    // Compute the remaining variables
    minSelEst = std::ceil(minSel * total_data);
    eboSelEst = std::ceil(eboSel);
    indepEst = std::ceil(indep);

    //        #pragma omp critical
    train_out_file << log_card_ratio << " 0:" << current_rect._pll.x
                   << " 1:" << current_rect._pur.x
                   << " 2:" << current_rect._pll.y
                   << " 3:" << current_rect._pur.y << " 4:" << minSelEst
                   << " 5:" << eboSelEst << " 6:" << indepEst << std::endl;
  }

  train_out_file.close();

  eval_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_lwxgb_eval_libsvm.dat");

  //  #pragma omp parallel for

  for (uint i = (num_train_queries() - (num_train_queries() / 5));
       i < num_train_queries(); i++) {

    const query_t &current_query = trainQueries()[i];
    const rectangle_t &current_rect = current_query.rectangle();

    double xSel = 1.0 * _oneEq.estimateX(current_query) / total_data;
    double ySel = 1.0 * _oneEq.estimateY(current_query) / total_data;
    double log_card_ratio = std::log(current_query.card()) / log_max_card;

    // Avoid division and sqrt inside the loop

    minSel = std::min(xSel, ySel);
    maxSel = std::max(xSel, ySel);
    eboSel = 1.0 * total_data * minSel * std::sqrt(maxSel);
    indep = xSel * ySel * total_data;

    // Compute the remaining variables
    minSelEst = std::ceil(minSel * total_data);
    eboSelEst = std::ceil(eboSel);
    indepEst = std::ceil(indep);

    //    #pragma omp critical
    eval_out_file << log_card_ratio << " 0:" << current_rect._pll.x
                  << " 1:" << current_rect._pur.x
                  << " 2:" << current_rect._pll.y
                  << " 3:" << current_rect._pur.y << " 4:" << minSelEst
                  << " 5:" << eboSelEst << " 6:" << indepEst << std::endl;
  }

  eval_out_file.close();
}

/*
void XGBEnrichedEBO::fill_libsvm_testfiles() const
{
    std::ofstream test_out_file;


    test_out_file.open("/home/rashedi/H2D/in/" + _aCb.sds() + "/" + _aCb.ds() +
"_eboxgb_test_libsvm.dat");
  //in order to check estimates for trainset too
  //for(uint i= num_train_queries() ; i < (num_train_queries()+10000)  ; i++) {
//queries().size()
  //Sample2dim* sampleEst = new Sample2dim(_data, _sampleSize, q(), theta(),
false); double xSel; double ySel; uint minSelEst; uint eboSelEst; uint indepEst;
  // for(uint i= num_train_queries() ; i < queries().size()  ; i++) {
    for(uint i= 0 ; i < queries().size()  ; i++) {
    xSel = (_oneEq.estimateX(queries()[i]))/_data.total();
    ySel = (_oneEq.estimateY(queries()[i]))/_data.total();
    minSelEst = std::ceil(std::min(xSel,ySel)*(_data.total()));
    eboSelEst =
std::ceil((_data.total())*(std::min(xSel,ySel))*(std::sqrt(std::max(xSel,ySel))));
    indepEst = std::ceil((xSel * ySel * _data.total()));
    test_out_file <<
float(std::log(queries()[i].card()))/float(std::log(max_card())) << " 0:"
    << queries()[i].rectangle()._pll.x<<" 1:"
    << queries()[i].rectangle()._pur.x<<" 2:"
    << queries()[i].rectangle()._pll.y<<" 3:"
    << queries()[i].rectangle()._pur.y<<" 4:"
    << minSelEst << " 5:"
    << eboSelEst << " 6:"
    << indepEst <<std::endl;

 }
  test_out_file.close();

}
*/

void LWXGB::fill_libsvm_testfiles() const {
  std::ofstream test_out_file;
  test_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_lwxgb_test_libsvm.dat");

  if (!test_out_file.is_open()) {
    std::cerr << "Error opening output file!" << std::endl;
    return;
  }

  const uint num_test_queries = queries().size();
  const double total_data = _data.total();
  const double log_max_card = std::log(max_card());

  //    #pragma omp parallel for
  for (uint i = 0; i < num_test_queries; ++i) {
    const query_t &current_query = queries()[i];
    const rectangle_t &current_rect = current_query.rectangle();

    double xSel = 1.0 * _oneEq.estimateX(current_query) / total_data;
    double ySel = 1.0 * _oneEq.estimateY(current_query) / total_data;
    double log_card_ratio = std::log(current_query.card()) / log_max_card;

    // Avoid division and sqrt inside the loop
    //
    double minSel = std::min(xSel, ySel);
    double maxSel = std::max(xSel, ySel);
    double eboSel = 1.0 * total_data * minSel * std::sqrt(maxSel);
    double indep = xSel * ySel * total_data;

    // Compute the remaining variables
    uint minSelEst = std::ceil(minSel * total_data);
    uint eboSelEst = std::ceil(eboSel);
    uint indepEst = std::ceil(indep);

    //      #pragma omp critical
    test_out_file << log_card_ratio << " 0:" << current_rect._pll.x
                  << " 1:" << current_rect._pur.x
                  << " 2:" << current_rect._pll.y
                  << " 3:" << current_rect._pur.y << " 4:" << minSelEst
                  << " 5:" << eboSelEst << " 6:" << indepEst << std::endl;
  }

  test_out_file.close();
}
} // namespace H2D
