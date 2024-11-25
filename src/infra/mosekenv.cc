#include "mosekenv.hh"

// for old version of mosek
// static void MSKAPI mskenvprintstr(void* handle, char str[]) {
//   printf("%s", str);
// }


MosekEnv* MosekEnv::_instance = 0;

MosekEnv::MosekEnv() : _mosekEnv(0) {
  MSKrescodee lRes = MSK_RES_OK;
  // lRes = MSK_makeenv(&_mosekEnv, NULL, NULL, NULL, NULL); // old version of mosek
  lRes = MSK_makeenv(&_mosekEnv, NULL);
  if(MSK_RES_OK == lRes) {
    // lRes = MSK_initenv(_mosekEnv); // old version of mosek
    // lRes = MSK_maketask(_mosekEnv, 0, 1, &_mosekTask);
    // if(MSK_RES_OK != lRes) {
    //   std::cerr << "failed to initialize mosek env." << std::endl;
    //   exit(-1);
    // } else {
    //   // MSK_linkfunctoenvstream(_mosekEnv, MSK_STREAM_LOG, NULL, mskenvprintstr); // in old version of mosek
    //   std::cerr << "failed to initialize mosek env." << std::endl;
    //   exit(-1);
    // }
  } else {
    _mosekEnv = 0;
    std::cerr << "cannot create mosek env." << std::endl;
    exit(-1);
  }
}

MosekEnv::~MosekEnv() {
  if(0 != _mosekEnv) {
    MSK_deleteenv(&_mosekEnv);
    _mosekEnv = 0;
  }
}

MosekEnv*
MosekEnv::instance() {
  if(0 == _instance) {
    _instance = new MosekEnv;
  }
  return _instance;
}


