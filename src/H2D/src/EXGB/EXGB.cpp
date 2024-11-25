#include "EXGB.hh"

namespace H2D {

EXGB::EXGB(const Data2dim &aData, const H2D::Cb &aCb,
                         const uint max_card, const std::string &file_base,
                         const query_vt &queryvec,
                         const query_vt &trainQueryvec)
    : XGBEstimator(aCb, max_card, file_base, queryvec, trainQueryvec),
      _data(aData), _sampleSize(aCb.sampleSize()), _areaEst(_data, theta()),
      _sampleEst(_data, _sampleSize, q(), theta(), false) {
  setEnriched(1);
}

void EXGB::fill_libsvm_trainfiles() const {

  std::ofstream train_out_file;

  std::ofstream eval_out_file;
  train_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                      "_exgb_train_libsvm.dat");

  for (uint i = 0; i < (num_train_queries() - (num_train_queries() / 5)); i++) {
    train_out_file << float(std::log(trainQueries()[i].card())) /
                          float(std::log(max_card()))
                   << " 0:" << trainQueries()[i].rectangle()._pll.x
                   << " 1:" << trainQueries()[i].rectangle()._pur.x
                   << " 2:" << trainQueries()[i].rectangle()._pll.y
                   << " 3:" << trainQueries()[i].rectangle()._pur.y
                   << " 4:" << std::log(_areaEst.estimate(trainQueries()[i]))
                   << " 5:" << std::log(_sampleEst.estimate(trainQueries()[i]))
                   << std::endl;
  }

  train_out_file.close();

  eval_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_exgb_eval_libsvm.dat");

  for (uint i = (num_train_queries() - (num_train_queries() / 5));
       i < num_train_queries(); i++) {
    eval_out_file << float(std::log(trainQueries()[i].card())) /
                         float(std::log(max_card()))
                  << " 0:" << trainQueries()[i].rectangle()._pll.x
                  << " 1:" << trainQueries()[i].rectangle()._pur.x
                  << " 2:" << trainQueries()[i].rectangle()._pll.y
                  << " 3:" << trainQueries()[i].rectangle()._pur.y
                  << " 4:" << std::log(_areaEst.estimate(trainQueries()[i]))
                  << " 5:" << std::log(_sampleEst.estimate(trainQueries()[i]))
                  << std::endl;
  }

  eval_out_file.close();
}

void EXGB::fill_libsvm_testfiles() const {
  std::ofstream test_out_file;

  test_out_file.open(_aCb.inDir()+"/" + _aCb.sds() + "/" + _aCb.ds() +
                     "_exgb_test_libsvm.dat");
  for (uint i = 0; i < queries().size(); i++) {
    test_out_file << float(std::log(queries()[i].card())) /
                         float(std::log(max_card()))
                  << " 0:" << queries()[i].rectangle()._pll.x
                  << " 1:" << queries()[i].rectangle()._pur.x
                  << " 2:" << queries()[i].rectangle()._pll.y
                  << " 3:" << queries()[i].rectangle()._pur.y
                  << " 4:" << std::log(_areaEst.estimate(queries()[i]))
                  << " 5:" << std::log(_sampleEst.estimate(queries()[i]))
                  << std::endl;
  }
  test_out_file.close();
}

} // namespace H2D
