#ifndef MOSEK_ENV_HH
#define MOSEK_ENV_HH

#include <iostream>
#include "mosek.h"

class MosekEnv {
  public:
    static MosekEnv* instance();
  private:
    MosekEnv();
    MosekEnv(const MosekEnv&);
    MosekEnv& operator=(const MosekEnv&);
    ~MosekEnv();
  public:
    inline MSKenv_t env() { return _mosekEnv; }
  private:
    static MosekEnv* _instance;
    MSKenv_t  _mosekEnv;
};


#endif
