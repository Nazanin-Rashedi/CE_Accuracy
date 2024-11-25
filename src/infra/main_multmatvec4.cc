#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <string.h>

#include "q.hh"
#include "measure.hh"
#include "matrix.hh"
#include "bitvectorsmall.hh"


// multiply Ax
// A \in R^{m,n}, typically m<<n
// x =in R^{n}

typedef Bitvector32 bv_t;
typedef std::vector<uint> uint_vt;

class citer_t { // iterates over C(X)
  public:
    citer_t(bv_t X, uint aN);
  public:
    citer_t& operator++();
    inline bool isValid() const { return _valid; }
    bv_t operator*();
  private:
    bv_t _X;
    bv_t _mask;
    bool _valid;
    bv_t::IteratorSuperset _iter;
};

citer_t::citer_t(bv_t aX, uint aN) : _X(aX), _mask((1 << aN) - 1), _valid(true), _iter() {
  _iter = _X.beginSuper(aN);
}

citer_t&
citer_t::operator++() {
  if(_iter.isValid()) {
    ++_iter;
  } else {
    _valid = false;
  }
  return (*this);
}

bv_t
citer_t::operator*() {
  if(_iter.isValid()) {
    // return ((*_iter) & _mask);
    return (*_iter);
  }
  // return (_X & _mask);
  return _X;
}

struct Abit_t {
  uint  _noBits;
  uint  _size; // number of bitvectors needed
  uint  _nOrm; // n for At*bbx, m for A*x
  uint* _bits;
  Abit_t() : _noBits(0), _size(0), _nOrm(0), _bits(0) {}
  ~Abit_t() { if(0 != _bits) delete[] _bits; }

  Abit_t(const Abit_t&);
  Abit_t& operator=(const Abit_t&);
};

struct Problem {
  private:
    Problem(const Problem&);
    Problem& operator=(const Problem&);
  public:
    Problem(const uint aNoBf);
    ~Problem();
  public:
    inline const Matrix& A() const { return _A; }
    inline       uint get_m() const { return _A.noRows(); }
    inline       uint get_n() const { return _A.noCols(); }
    inline       uint bf() const { return _nobf; }
    inline const double* vec() const { return _vec; }
    inline const uint_vt* idxA() const { return _idxA; }
    inline const uint_vt* idxAt() const { return _idxAt; }
    inline const Abit_t&  bvA4() const { return _bvA4; }
    inline const Abit_t&  bvA8() const { return _bvA8; }
  private:
    uint genKnownPattern();
    void genMatrix(const uint m);
    void genIdx();
    void genBvA4(Abit_t&);
    void genBvA8(Abit_t&);
    void genVec();
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    uint    _nobf;
    uint*   _pat;
    Matrix  _A;
    uint_vt* _idxA;
    uint_vt* _idxAt;
    Abit_t   _bvA4;
    Abit_t   _bvA8;
    double* _vec;
};

Problem::Problem(const uint aNoBf) 
        : _nobf(aNoBf), _pat(0), _A(), 
          _idxA(0),
          _idxAt(0),
          _bvA4(),
          _bvA8(),
          _vec(0) {
  uint x = 1 + aNoBf + ((aNoBf * aNoBf - 1) / 2); 
  _pat = new uint[x];
  uint m = genKnownPattern();
  if(m > x) {
    std::cerr << "EE " << m << " > " << x << std::endl;
    assert(m <= x);
  }
  genMatrix(m);
  _idxA  = new uint_vt[get_m()];
  _idxAt = new uint_vt[get_n()];
  genIdx();
  genBvA4(_bvA4);
  genBvA8(_bvA8);
  _vec = new double[get_n()];
  genVec();
}

Problem::~Problem() {
  delete [] _pat;
  delete [] _idxA;
  delete [] _idxAt;
  delete [] _vec;
}

uint
Problem::genKnownPattern() {
  _pat[0] = 0;
  uint k = 1;
  for(uint i = 0; i < bf(); ++i) {
    _pat[k++] = (1 << i);
  }
  for(uint i = 0; i < bf(); ++i) {
    for(uint j = i + 1; j < bf(); ++j) {
      _pat[k++] = (1 << i) | (1 << j);
    }
  }
  return k;
}

void
Problem::genMatrix(const uint m) {
  const uint n = (1 << bf());
  const uint mask = (n - 1);
  _A.rescale(m, n);
  for(uint i = 0; i < m; ++i) {
    bv_t lBitVector(_pat[i]);
    for(citer_t lIter(lBitVector, bf()); lIter.isValid(); ++lIter) {
      _A(i, (*lIter).bitvector() & mask) = 1;
    }
  }
}

void
Problem::genIdx() {
  const uint m = A().noRows();
  const uint n = A().noCols();
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      if(0 < A()(i,j)) {
        _idxA[i].push_back(j);
        _idxAt[j].push_back(i);
      }
    }
  }
}

void
Problem::genBvA4(Abit_t& aAbit) {
  // first line is ignored
  // viererpaeckchen
  const uint m = get_m();
  const uint n = get_n();
  const uint p = n / 4;

  aAbit._size = n / 4;
  aAbit._nOrm = m;
  aAbit._bits = new uint[aAbit._size * aAbit._nOrm];

  uint lBv = 0;
  uint j = 0;

  uint* lBvPtr = aAbit._bits;  
  for(uint l = 0; l < p; ++l) {
    j = 4 * l;
    for(uint i = 0; i < m; ++i) {
      lBv =  (0 < _A(i, j)) |
            ((0 < _A(i, j + 1)) << 1) |
            ((0 < _A(i, j + 2)) << 2) |
            ((0 < _A(i, j + 3)) << 3);
      *lBvPtr++ = lBv;
    }
  }
}

void
Problem::genBvA8(Abit_t& aAbit) {
  const uint m = get_m();
  const uint n = get_n();
  const uint p = n / 8;

  aAbit._size = n / 8;
  aAbit._nOrm = m;
  aAbit._bits = new uint[aAbit._size * aAbit._nOrm];

  uint lBv = 0;
  uint j = 0;

  uint* lBvPtr = aAbit._bits;  
  for(uint l = 0; l < p; ++l) {
    j = 8 * l;
    for(uint i = 0; i < m; ++i) {
      lBv = ((0 < _A(i , j))     ) |
            ((0 < _A(i , j + 1)) << 1) |
            ((0 < _A(i , j + 2)) << 2) |
            ((0 < _A(i , j + 3)) << 3) |
            ((0 < _A(i , j + 4)) << 4) |
            ((0 < _A(i , j + 5)) << 5) |
            ((0 < _A(i , j + 6)) << 6) |
            ((0 < _A(i , j + 7)) << 7);
      *lBvPtr++ = lBv;
    }
  }
}

void
Problem::genVec() {
  for(uint i = 0; i < get_n(); ++i) {
    _vec[i] = std::abs(std::sin(i+1));
  }
}

std::ostream&
Problem::print(std::ostream& os) const {
  os << "Problem (" << bf() << "): " << get_m() << 'x' << get_n() << std::endl;
  os << "vec: ";
  for(uint i = 0; i < get_m(); ++i) {
    os << ' ' << _vec[i];
  }
  os << std::endl;
  os << "mat:" << std::endl;
  A().print(os, 1);
  os << std::endl;
  return os;
}

typedef void (*fun_t)(double*, const Problem&);

void
fMProd(double* aVecOut, const Problem& aProblem) {
  Matrix::productOf(aVecOut, aProblem.A(), aProblem.vec());
}

void
fMProdXF(double* aVecOut, const Problem& aProblem) {
  Matrix::productOfXF(aVecOut, aProblem.A(), aProblem.vec());
}

void
fIdxProdA(double* aVecOut, const Problem& aProblem) {
  const double* bbx = aProblem.vec();
  const uint m = aProblem.get_m();
  const uint_vt* vA = aProblem.idxA();
  double lSum = 0;
  for(uint i = 0; i < m; ++i) {
    const uint_vt& uv = vA[i];
    lSum = 0;
    for(uint j = 0; j < uv.size(); ++j) {
       lSum += bbx[ uv[j] ];
    }
    aVecOut[i] = lSum;
  }
}

void
fIdxProdAt(double* aVecOut, const Problem& aProblem) {
  const uint m = aProblem.get_m();
  for(uint i = 0; i < m; ++i) {
    aVecOut[i] = 0;
  }

  const uint_vt* vAt = aProblem.idxAt();
  const uint n = aProblem.get_n();
  const double* bbx = aProblem.vec();
  for(uint i = 0; i < n; ++i) {
    const uint_vt& uvt = vAt[i];
    for(uint j = 0; j < uvt.size(); ++j) {
      aVecOut[ uvt[j] ] += bbx[i];
    }
  }
}


void
fProdBitA4(double* aVecOut, const Problem& aProblem) {
  // std::cout << "BitAt4";
  const Abit_t& lAbit = aProblem.bvA4();
  const uint m = lAbit._nOrm; // aProblem.get_m();

  for(uint i = 0; i < m; ++i) {
     aVecOut[i] = 0;
  }

  static double lMat[16];
  double a,b,c,d;
  double ab, cd;
  const uint lLim = lAbit._size;
  const double* x = aProblem.vec();
  const uint*   lBvPtr = lAbit._bits;
  for(uint i = 0; i < lLim; ++i) { 
    a = *x++;
    b = *x++;
    c = *x++;
    d = *x++;
    ab = a + b;
    cd = c + d;
    lMat[ 0] = 0;
    lMat[ 1] = a;
    lMat[ 2] = b;
    lMat[ 3] = ab;
    lMat[ 4] = c;
    lMat[ 5] = a + c;
    lMat[ 6] = b + c;
    lMat[ 7] = ab + c;
    lMat[ 8] = d;
    lMat[ 9] = a + d;
    lMat[10] = b + d;
    lMat[11] = ab + d;
    lMat[12] = c + d;
    lMat[13] = a + cd;
    lMat[14] = b + cd;
    lMat[15] = ab + cd;
    for(uint j = 0; j < m; ++j) {
      aVecOut[j] += lMat[*lBvPtr++];
    }
  }
}

void
fProdBitA8(double* aVecOut, const Problem& aProblem) {
  // std::cout << "BitAt8";
  const Abit_t& lAbit = aProblem.bvA8();
  const uint m = lAbit._nOrm;

  for(uint i = 0; i < m; ++i) {
     aVecOut[i] = 0;
  }

  static double lMat[256];
  double a,b,c,d,z;
  double ab, cd;
  const uint lLim = lAbit._size;
  const double* x = aProblem.vec();
  const uint*   lBvPtr = lAbit._bits;
  for(uint i = 0; i < lLim; ++i) {
    a = *x++;
    b = *x++;
    c = *x++;
    d = *x++;
    ab = a + b;
    cd = c + d;
    lMat[ 0] = 0;
    lMat[ 1] = a;
    lMat[ 2] = b;
    lMat[ 3] = ab;
    lMat[ 4] = c;
    lMat[ 5] = a + c;
    lMat[ 6] = b + c;
    lMat[ 7] = ab + c;
    lMat[ 8] = d;
    lMat[ 9] = a + d;
    lMat[10] = b + d;
    lMat[11] = ab + d;
    lMat[12] = c + d;
    lMat[13] = a + cd;
    lMat[14] = b + cd;
    lMat[15] = ab + cd;
    z = *x++;
    for(uint k = 0; k < 16; ++k) {
       lMat[k + 16] = lMat[k] + z;
    }
    z = *x++;
    for(uint k = 0; k < 32; ++k) {
       lMat[k + 32] = lMat[k] + z;
    }
    z = *x++;
    for(uint k = 0; k < 64; ++k) {
       lMat[k + 64] = lMat[k] + z;
    }
    z = *x++;
    for(uint k = 0; k < 128; ++k) {
       lMat[k + 128] = lMat[k] + z;
    }

    for(uint j = 0; j < m; ++j) {
      aVecOut[j] += lMat[*lBvPtr++];
    }
  }
}


enum prod_t {
  kMA     = 0,
  kMAXF   = 1,
  kIdxA   = 2,
  kIdxAt  = 3,
  kBitAt4 = 4,
  kBitAt8 = 5,
  kNoProd = 6
};

std::string prodname[kNoProd+1] = {
  "MxA",
  "MxAXF",
  "IdxA",
  "IdxAt",
  "BitA4",
  "BitA8",
  "NoProd"
};

std::ostream&
operator<<(std::ostream& os, const prod_t x) {
  os << prodname[x];
  return os;
}

class Test {
  private:
    Test(const Test&);
    Test& operator=(const Test&);
  public:
    Test(const Problem& aProblem, const prod_t aProd, const Test* aTest);
    ~Test();
  public:
    inline prod_t prod() const { return _prod; }
    inline double rt() const { return _rt; }
    inline double err() const { return _err; } 
    inline const double* vec() const { return _vec; }
  private:
    double exec(const Problem& aProblem);
    double calcError(const Test* aTest);
  public:
    std::ostream& print(std::ostream& os) const;
  public:
    prod_t  _prod;
    uint    _m;
    double* _vec;
    double  _rt; 
    double  _err;
  private:
    static fun_t _funs[kNoProd];
};

Test::Test(const Problem& aProblem, const prod_t aProd, const Test* aTest)
     : _prod(aProd), _m(aProblem.get_m()), _vec(0), _rt(0), _err(0) {
  _vec = new double[aProblem.get_m()];
  _rt  = exec(aProblem);
  _err = calcError(aTest);
  if(err() > 0.0) {
    // std::cout << "EE error of " << prod() << ": " << err() << std::endl;
    // assert(0.0 >= _err);
  }
}


Test::~Test() {
  delete[] _vec;
}

double
Test::exec(const Problem& aProblem) {
  const int lNoRuns = ((aProblem.bf() < 10) ? 10000 :  ((aProblem.bf() < 11) ? 500 : 50));
  Measure lMeasure;
  lMeasure.start();
    for(int i = 0; i < lNoRuns; ++i) {
      (_funs[prod()])(_vec, aProblem);
    }
  lMeasure.stop();
  return (lMeasure.mTotalTime() / lNoRuns) * 1000;
}

double
Test::calcError(const Test* aTest) {
  if(0 == aTest) {
    return 0;
  }
  double lMaxErr = -1;
  for(uint i = 0; i < _m; ++i) {
    const double lError = std::abs(_vec[i] - aTest->_vec[i]);
    if(lError > lMaxErr) {
      lMaxErr = lError;
    }
  }
  return lMaxErr;
}

std::ostream&
Test::print(std::ostream& os) const {
  std::cout << "prod(): " << std::setw(6) << prod() << ": vec =";
  for(uint i = 0; i < _m; ++i) {
    os << ' ' << _vec[i];
  }
  return os;
}

fun_t
Test::_funs[kNoProd] = {
  fMProd,
  fMProdXF,
  fIdxProdA,
  fIdxProdAt,
  fProdBitA4,
  fProdBitA8,
};

void
test0(uint aNoBf) {
  Problem lProblem(aNoBf);
  lProblem.print(std::cout);
  Test lRefTest(lProblem, kMA, 0);
  for(uint i = 0; i < kNoProd; ++i) {
    Test lTest(lProblem, (prod_t) i, &lRefTest);
    lTest.print(std::cout) << std::endl;
  }
}

void
test1(uint aNoBf) {
  Problem lProblem(aNoBf);
  Test*   lTests[kNoProd];
  const uint n = lProblem.get_n();
  Test lRefTest(lProblem, kMA, 0);
  for(uint i = 0; i < kNoProd; ++i) {
    lTests[i] = new Test(lProblem, (prod_t) i, &lRefTest);
  }

  if(5 >= aNoBf) {
    for(uint i = 0; i < kNoProd; ++i) {
      std::cout << "vec of " << std::setw(6) << lTests[i]->prod() << ':';
      for(uint j = 0; j < n; ++j) {
        std::cout << ' ' << lTests[i]->vec()[j];
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::setw(2) << "bf" << ' '
            << std::setw(6) << "kind" << ' '
            << std::setw(12) << "runtime" << ' '
            << std::setw(12) << "error" << ' '
            << std::endl;

  std::cout << std::fixed;
  std::cout.precision(6);
  for(uint i = 0; i < kNoProd; ++i) {
    std::cout << std::setw(2)  << aNoBf << ' '
              << std::setw(6)  << lTests[i]->prod() << ' '
              << std::setw(12) << lTests[i]->rt() << ' '
              << std::setw(12) << lTests[i]->err() << ' '
              << std::endl;
  }


  {
    // std::cout << "make sure ";
    // Test lTest(lProblem, kBitAt4, &lRefTest);
    // std::cout << std::endl;
  }

  for(uint i = 0; i < kNoProd; ++i) {
    delete lTests[i];
  }
}

void
test2(const uint aNoBf) {
 Problem lProblem(aNoBf);
  Test*   lTests[kNoProd];
  Test lRefTest(lProblem, kMA, 0);
  for(uint i = 0; i < kNoProd; ++i) {
    lTests[i] = new Test(lProblem, (prod_t) i, &lRefTest);
  }

  std::cout << std::fixed;
  std::cout.precision(6);
  std::cout << ' ' << std::setw(2) << aNoBf;
  for(uint i = 0; i < kNoProd; ++i) {
    std::cout << ' ' << std::setw(10) << lTests[i]->rt();
  }
  std::cout << std::endl;


  {
    // std::cout << "make sure ";
    // Test lTest(lProblem, kBitAt4, &lRefTest);
    // std::cout << std::endl;
  }

  for(uint i = 0; i < kNoProd; ++i) {
    delete lTests[i];
  }
}


int
main() {
  std::cout << "==============================================" << std::endl;
  std::cout << "| Ax (A max entropy design matrix, x vector) |" << std::endl;
  std::cout << "| runtimes of functions in milliseconds [ms] |" << std::endl;
  std::cout << "==============================================" << std::endl;
  // test0(3);
  // test0(6);
  // test0(7);
  // test1(3);
  // test1(6);
  // test1(7);
  // test1(8);
  // test1(9);
  test1(10);
  // test1(11);
  // test1(12);
  // test1(13);
  // test1(14);
  // test1(15);

  std::cout << "#bf" << ' ';
  for(uint i = 0; i < kNoProd; ++i) {
    std::cout << std::setw(10) << ((prod_t) i) << ' ';
  }
  std::cout  << std::endl;

  for(uint i = 3; i < 17; ++i) {
    test2(i);
  }

  return 0;
}

