#ifndef INFRA_BITVECTOR_GENERATOR_K_OF_N_ITERATOR_HH
#define INFRA_BITVECTOR_GENERATOR_K_OF_N_ITERATOR_HH

#include "bit_intrinsics.hh"

/*
 * class BvGenKofN
 * generates all bitvectors of size N
 * with exactly K bits set
 */

template<class Tuint>
class BvGenKofN {
  public:
    typedef unsigned int uint;
    typedef Tuint        bv_t;
  public:
    BvGenKofN(const uint n, const uint k);
    ~BvGenKofN();
  public:
     inline BvGenKofN& operator++() { next(); return (*this); }
     inline bool ok() const { return (bv() < lim()); }
     inline bv_t operator*() const { return _bv; }
  public:
     void next();
  public:
    inline uint n() const { return _n; }
    inline uint k() const { return _k; }
    inline bv_t bv() const { return _bv; }
    inline bv_t lim() const { return _lim; }
    inline bv_t calc_lim() const {
	           return (((((Tuint) 1) << k()) - 1) << (n() - k())) + 1;
                }
  private:
          bv_t _bv;
          bv_t _lim;
    const uint _n;
    const uint _k;
  private:
    BvGenKofN(const BvGenKofN&);
    BvGenKofN& operator=(const BvGenKofN&);
};


template<class Tuint>
BvGenKofN<Tuint>::BvGenKofN(const uint aN, const uint aK) 
	         : _bv(), _lim(0),
                   _n(aN), _k(aK) {
  _lim = calc_lim();
  _bv = (((bv_t) 1) << k()) - 1;
}

template<class Tuint>
BvGenKofN<Tuint>::~BvGenKofN() {
}

template<class Tuint>
void
BvGenKofN<Tuint>::next() {
  bv_t x = _bv;
  const uint lLo = idx_lowest_bit_set<bv_t>(x);
  x += (1 << lLo);
  const uint lHi = idx_lowest_bit_set<bv_t>(x);
  x |= (( ((Tuint) 1) << (lHi - lLo - 1)) - 1);
  _bv = x;
}



#endif
