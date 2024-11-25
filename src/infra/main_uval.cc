#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <vector>

#include "infra/measure.hh"
#include "infra/uval.hh"

const double NS = (1024.0L * 1024.0L * 1024.0L);

enum instr_et {
  kNoOp    =  0,
  kAddI32  =  1,
  kSubI32  =  2,
  kMulI32  =  3,
  kDivI32  =  4,
  kModI32  =  5,
  kNoInstr =  6
};

struct instr_t {
  instr_et  _opcode;
  int       _begin0; // dest
  int       _begin1; // src1
  int       _begin2; // src2
  int       _anz;    // number of elements to process
  int       _gap0;   // distance between dest uvals
  int       _gap1;   // distance between src1 uvals
  int       _gap2;   // distance between src2 uvals
  int       _idx0;   // for uval_t arrays
  int       _idx1;   // for uval_t arrays
  int       _idx2;   // for uval_t arrays
};

void
fAddI32_v0(const instr_t& aInstr, uval_t* aBase) {
  uint32_t n = aInstr._anz + 1;
        uval_t*  a = aBase + aInstr._begin0;
  const uval_t*  b = aBase + aInstr._begin1;
  const uval_t*  c = aBase + aInstr._begin2;
  uint32_t o0 = aInstr._idx0;
  uint32_t o1 = aInstr._idx1;
  uint32_t o2 = aInstr._idx2;
  uint32_t s0 = aInstr._gap0;
  uint32_t s1 = aInstr._gap1;
  uint32_t s2 = aInstr._gap2;
  while(--n) {
    a->_i32[o0] = b->_i32[o1]  + c->_i32[o2];
    a += s0;
    b += s1;
    c += s2;
  }
}

void
fAddI32_v1(const instr_t& aInstr, uval_t* aBase) {
  uint32_t n = aInstr._anz + 1;
        int* a = ((int*) (aBase + aInstr._begin0)) + aInstr._idx0;
  const int* b = ((int*) (aBase + aInstr._begin1)) + aInstr._idx1;
  const int* c = ((int*) (aBase + aInstr._begin2)) + aInstr._idx2;
  uint32_t s0 = (sizeof(uval_t) / sizeof(int32_t)) * aInstr._gap0;
  uint32_t s1 = (sizeof(uval_t) / sizeof(int32_t)) * aInstr._gap1;
  uint32_t s2 = (sizeof(uval_t) / sizeof(int32_t)) * aInstr._gap2;
  while(--n) {
    *a = *b + *c;
    a += s0;
    b += s1;
    c += s2;
  }
}

void
fAddI32_C(const instr_t& aInstr, uval_t* aBase) {
  uint32_t n = aInstr._anz + 1;
        int* a = ((int*) (aBase + aInstr._begin0));
  const int* b = ((int*) (aBase + aInstr._begin1));
  const int* c = ((int*) (aBase + aInstr._begin2));
  while(--n) {
    *a = *b + *c;
    ++a; ++b; ++c;
    *a = *b + *c;
    ++a; ++b; ++c;
  }
}

void
fAddI32_X(const instr_t& aInstr, uval_t* aBaseA, const uval_t* aBaseB, const uval_t* aBaseC) {
  uint32_t n = aInstr._anz + 1;
        int* a = (int*) (aBaseA + aInstr._begin0);
  const int* b = (int*) (aBaseB + aInstr._begin1);
  const int* c = (int*) (aBaseC + aInstr._begin2);
  while(--n) {
    *a = *b + *c;
    ++a; ++b; ++c;
    *a = *b + *c;
    ++a; ++b; ++c;
  }
}

void
print_i32(std::ostream& os, const uval_t* aUv, const uint32_t aIdx, const uint32_t aNoRows, const uint32_t aNoCols, const int aWidth) {
  const uval_t* x = aUv;
  for(uint32_t i = 0; i < aNoRows; ++i) {
    for(uint32_t j = 0; j < aNoCols; ++j) {
      os << std::setw(aWidth) << x->_i32[aIdx] << ' ';
      ++x;
    }
    os << std::endl;
  }
  os << std::endl;
}

void
test0() {
  std::cout << "sizeof(uval_t) = " << sizeof(uval_t) << std::endl;
}

void
test1() {
  uval_t* lUvals = new uval_t[10*5];
  uval_t* x = lUvals;
  for(uint i = 0; i < 10; ++i) {
    for(uint j = 0; j < 5; ++j) {
      x->_i32[0] = i+j;
      ++x;
    }
  }

  print_i32(std::cout, lUvals, 0, 10, 5, 3);

  instr_t lInstr;
  lInstr._opcode = kAddI32;
  lInstr._anz    = 10;
  lInstr._begin0 = 0;
  lInstr._begin1 = 1;
  lInstr._begin2 = 2;
  lInstr._gap0   = 5;
  lInstr._gap1   = 5;
  lInstr._gap2   = 5;
  lInstr._idx0   = 0;
  lInstr._idx1   = 0;
  lInstr._idx2   = 0;

  fAddI32_v1(lInstr, lUvals);

  print_i32(std::cout, lUvals, 0, 10, 5, 3);

  delete[] lUvals;
}

double
fMeasureArrayPlus() {
  int  n = 1024*1024;
  int* a = new int[n];
  int* b = new int[n];
  int* c = new int[n];

  for(int i = 0; i < n; ++i) {
    a[i] = 0;
    b[i] = i % 17;
    c[i] = i % 13;
  }

  Measure lMeasure;
  lMeasure.start();
  for(int i = 0; i < n; ++i) {
    a[i] = b[i] + c[i];
  }
  lMeasure.stop();
  delete[] c;
  delete[] b;
  delete[] a;
  return (lMeasure.mTotalTime() / (double) n) * NS;
}

void
fMeasureAdd_v_0_1(double& aRtV0, double& aRtV1, const int aNoCols) {
  assert(5 < aNoCols);
  int k =  512;
  int m = 2048;
  int n = k*m;

  std::vector<uval_t*> lRel;

  // init: m chunks of size k*aNoCols each (i.e. row store)
  // row: [0]: a, [3]: b, [5]: c
  for(int i = 0; i < m; ++i) {
    uval_t* x = new uval_t[k * aNoCols];
    uval_t* y = x;
    memset(x, 0, k * sizeof(uval_t));
    for(int j = 0; j < k; ++j) {
      y[3]._i32[1] = i;
      y[5]._i32[1] = j;
      y += aNoCols;
    }
    lRel.push_back(x);
  }

  instr_t lInstr;
  lInstr._opcode = kAddI32;
  lInstr._begin0 = 0;
  lInstr._begin1 = 3;
  lInstr._begin2 = 5;
  lInstr._anz    = k;
  lInstr._gap0   = aNoCols;
  lInstr._gap1   = aNoCols;
  lInstr._gap2   = aNoCols;
  lInstr._idx0   = 1;
  lInstr._idx1   = 1;
  lInstr._idx2   = 1;

  Measure lMeasure0;
  lMeasure0.start();
  for(size_t i = 0; i < lRel.size(); ++i) {
    fAddI32_v0(lInstr, lRel[i]);
  }
  lMeasure0.stop();
  aRtV0 = (lMeasure0.mTotalTime() / ((double) n)) * NS;
  
  Measure lMeasure1;
  lMeasure1.start();
  for(size_t i = 0; i < lRel.size(); ++i) {
    fAddI32_v1(lInstr, lRel[i]);
  }
  lMeasure1.stop();
  aRtV1 = (lMeasure1.mTotalTime() / ((double) n)) * NS;
  
  for(size_t i = 0; i < lRel.size(); ++i) {
    delete[] lRel[i];
  }
  
}

double
fMeasureAdd_X() {
  const int n = 1024*1024;
  const int m = n / 2;

  uval_t* a = new uval_t[m];
  uval_t* b = new uval_t[m];
  uval_t* c = new uval_t[m];
  for(int i = 0; i < m; ++i) {
    a[i]._i32[0] = 0;
    a[i]._i32[1] = 0;
    b[i]._i32[0] = i % 11;
    b[i]._i32[1] = i % 13;
    c[i]._i32[0] = i % 17;
    c[i]._i32[1] = i % 19;
  }
 
  instr_t lInstr;
  lInstr._anz = m; 
  lInstr._begin0 = 0;
  lInstr._begin1 = 0;
  lInstr._begin2 = 0;
  lInstr._gap0 = 0;
  lInstr._gap1 = 0;
  lInstr._gap2 = 0;
  lInstr._idx0 = 0;
  lInstr._idx1 = 0;
  lInstr._idx2 = 0;

  Measure lMeasure;
  lMeasure.start();
  fAddI32_X(lInstr, a, b, c);
  lMeasure.stop();

  delete[] c;
  delete[] b;
  delete[] a;

  return (lMeasure.mTotalTime() / ((double) n)) * NS;
}

double
fMeasureAdd_C(double& aRtX) {
  const int n = 1024*1024;
  const int k = 512;      // matrix size in number of uvals
  const int m = (k / 3);  // number of uvals per column in matrix
  const int mm = (m * 2); // number of rows in matrix 
  const int l = n / mm;   // number of full matrices
  const int r = n % mm;   // number of elements in last matrix

  struct entry_t {
    uval_t* _uv;
    int     _no;

    entry_t() : _uv(0), _no(0) {}
    entry_t(uval_t* x, const int aNo) : _uv(x), _no(aNo) {}
  };

  std::vector<entry_t> lRel;
  for(int i = 0; i < l; ++i) {
    uval_t* x = new uval_t[k];
    lRel.push_back({x, m});
    for(int j = 0; j < m; ++j) {
      x[j]._i32[0] = 0;
      x[j]._i32[1] = 0;
      x[j+m]._i32[0] = j;
      x[j+m]._i32[1] = j+1;
      x[j+2*m]._i32[0] = j+2;
      x[j+2*m]._i32[1] = j+3;
    }
  }
  if(0 < r) {
    uval_t* x = new uval_t[k];
    lRel.push_back({x, r});
    for(int j = 0; j < m; ++j) {
      x[j]._i32[0] = 0;
      x[j]._i32[1] = 0;
      x[j+m]._i32[0] = j;
      x[j+m]._i32[1] = j+1;
      x[j+2*m]._i32[0] = j+2;
      x[j+2*m]._i32[1] = j+3;
    }
  }


  instr_t lInstr;
  lInstr._opcode = kAddI32;
  lInstr._anz    = m;
  lInstr._begin0 = 0;
  lInstr._begin1 = m;
  lInstr._begin2 = m * 2;
  lInstr._idx0   = 0; // unused
  lInstr._idx1   = 0; // unused
  lInstr._idx2   = 0; // unused

  Measure lMeasure;
  lMeasure.start();
  for(size_t i = 0; i < lRel.size(); ++i) {
    lInstr._anz = lRel[i]._no;
    fAddI32_C(lInstr, lRel[i]._uv);
  }
  lMeasure.stop();

  Measure lMeasureRtX;
  lMeasureRtX.start();
  for(size_t i = 0; i < lRel.size(); ++i) {
    lInstr._anz = lRel[i]._no;
    fAddI32_X(lInstr, lRel[i]._uv, lRel[i]._uv, lRel[i]._uv);
  }
  lMeasureRtX.stop();

  aRtX = (lMeasureRtX.mTotalTime() / ((double) n)) * NS;
  

  // mfree
  for(size_t i = 0; i < lRel.size(); ++i) {
    delete[] lRel[i]._uv;
  }

  return (lMeasure.mTotalTime() / ((double) n)) * NS;
}

void
test2() {
  const double lRtArr = fMeasureArrayPlus();
        double lRtAdd0 = 0;
        double lRtAdd1 = 0;
  const double lRtAddX = fMeasureAdd_X();
        double lRtAddCx = 0;
  const double lRtAddC = fMeasureAdd_C(lRtAddCx);
  std::cout << "array  : -- : " << std::setw(10) << lRtArr  << std::endl;
  std::cout << "add x  : -- : " << std::setw(10) << lRtAddX << std::endl;
  std::cout << "add c  : -- : " << std::setw(10) << lRtAddC << std::endl;
  std::cout << "add cx : -- : " << std::setw(10) << lRtAddCx << std::endl;
  for(int lNoCols = 8; lNoCols < 50; lNoCols += 2) {
    fMeasureAdd_v_0_1(lRtAdd0, lRtAdd1, lNoCols);
    std::cout << "add v0 : " << std::setw(2) << lNoCols << " : " << std::setw(10) << lRtAdd0 << std::endl;
    std::cout << "add v1 : " << std::setw(2) << lNoCols << " : " << std::setw(10) << lRtAdd1 << std::endl;
  }
  
}


int
main() {
  test0();
  test1();
  test2();
  std::cout << "sizeof(uval_t) = " << sizeof(uval_t) << std::endl;
  return 0;
}



