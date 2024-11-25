#ifndef INFRA_BITVECTOR_K_OF_N_ITERATOR_HH
#define INFRA_BITVECTOR_K_OF_N_ITERATOR_HH

#include "bitvectorsmall.hh"

/*
 * class BitvectorGeneratorKofN
 * generates all bitvectors of size N
 * with exactly K bits set
 */

template<class Tuint>
class BitvectorGeneratorKofN {
  public:
    typedef unsigned int uint;
    typedef BitVectorSmall<Tuint> bv_t;
  public:
    BitvectorGeneratorKofN(uint n, uint k);
    ~BitvectorGeneratorKofN();
  public:
     inline BitvectorGeneratorKofN& operator++() { next(); return (*this); }
     inline bool ok() const { return (bv() < lim()); }
     inline bv_t operator*() const { return _bv; }
  public:
     void init(uint n, uint k);
  public:
     void next();
  public:
    inline uint n() const { return _n; }
    inline uint k() const { return _k; }
    inline bv_t bv() const { return _bv; }
    inline Tuint lim() const { return _lim; }
    inline Tuint calc_lim() const {
	           return (((((Tuint) 1) << k()) - 1) << (n() - k())) + 1;
                 }
  private:
    uint _n;
    uint _k;
    bv_t  _bv;
    Tuint _lim;
  private:
    BitvectorGeneratorKofN(const BitvectorGeneratorKofN&);
    BitvectorGeneratorKofN& operator=(const BitvectorGeneratorKofN&);
};


template<class Tuint>
BitvectorGeneratorKofN<Tuint>::BitvectorGeneratorKofN(uint aN, uint aK) 
	         : _n(aN), _k(aK), _bv(), _lim(0) {
  _lim = calc_lim();
  _bv = (((Tuint) 1) << k()) - 1;
}

template<class Tuint>
void
BitvectorGeneratorKofN<Tuint>::init(uint aN, uint aK) {
  _n   = aN;
  _k   = aK;
  _lim = calc_lim();
  _bv = (((Tuint) 1) << k()) - 1;
}

template<class Tuint>
BitvectorGeneratorKofN<Tuint>::~BitvectorGeneratorKofN() {
}

template<class Tuint>
void
BitvectorGeneratorKofN<Tuint>::next() {
  Tuint x = _bv.bitvector();

  // std::cout << "in: ";
  // bv_t(x).printReverse(std::cout, n());
  // std::cout << std::endl;

  #ifdef __ICC
  const uint lLo = _bit_scan_forward(x);
  #elif  __GNUG__
  const uint lLo = __builtin_ctz(x);
  #endif
  x += (1 << lLo);
  #ifdef __ICC
  const uint lHi = _bit_scan_forward(x);
  #elif __GNUG__
  const uint lHi = __builtin_ctz(x);
  #endif
  x |= (( ((Tuint) 1) << (lHi - lLo - 1)) - 1);
  _bv = x;
}



#endif
