#ifndef SRC_INFRA_BITVECTOR_MEDIUM_HH
#define SRC_INFRA_BITVECTOR_MEDIUM_HH

#include <iostream>
#include <assert.h>

#include "infra/bitvectorsmall.hh"
#include "infra/bit_intrinsics.hh"
#include "infra/bit_subsets.hh"

/*
 * class BitVectorMedium
 * contains array of Tuint (template param),
 * which must be uint32_t or uint64_t
 * the array is given to BitVectorMedium's constructor
 * that is: BitVectorMedium does not allocate/deallocate memory
 */

template<class Tuint>
class BitVectorMedium {
    BitVectorMedium(const BitVectorMedium&) = delete;
    typedef BitVectorSmall<Tuint> bvs_t;
  public:
    BitVectorMedium(Tuint* aData, const uint32_t aNoEntries, const bool aDoInitialize);
    ~BitVectorMedium();
  public:
    inline uint32_t  size() const { return _size; }
    inline uint32_t  noBits() const { return (size() * sizeof(Tuint) * 8); }
    static inline uint32_t shift() { return _shift; }
    static inline uint32_t mask() { return _mask; }
    inline uint32_t& operator[](const uint32_t i) const { return _bv[i]; }
    inline uint32_t& operator[](const uint32_t i)       { return _bv[i]; }
    inline const Tuint* content() const { return _bv; }
  public:
    bool is_empty() const;
    bool is_not_empty() const;
    BitVectorMedium& clear();
    BitVectorMedium& reset(); // synonym for clear
  public:
    uint32_t cardinality() const;
    inline BitVectorMedium& operator++() { return increment(); }
    inline BitVectorMedium& operator~()  { return complement(); }
    BitVectorMedium& operator=(const BitVectorMedium&); // must have been allocated to correct size
    BitVectorMedium& operator|=(const BitVectorMedium&);
    BitVectorMedium& operator&=(const BitVectorMedium&);
    BitVectorMedium& operator^=(const BitVectorMedium&);
    BitVectorMedium& union_with(const BitVectorMedium&);
    BitVectorMedium& union_of(const BitVectorMedium&, const BitVectorMedium&);
    BitVectorMedium& intersect_with(const BitVectorMedium&);
    BitVectorMedium& intersection_of(const BitVectorMedium&, const BitVectorMedium&);
    BitVectorMedium& difference_with(const BitVectorMedium&);
    BitVectorMedium& difference_of(const BitVectorMedium&, const BitVectorMedium&);
    BitVectorMedium& increment();
    BitVectorMedium& complement();
  public:
    uint32_t idx_highest_bit_set() const;
    uint32_t idx_lowest_bit_set() const;
    bool     operator==(const BitVectorMedium&) const;
    bool     operator!=(const BitVectorMedium&) const;
    bool     overlap(const BitVectorMedium&) const;
    bool     intersects(const BitVectorMedium&) const;
    bool     disjoint(const BitVectorMedium&) const;
    bool     contains(const BitVectorMedium&) const;
    bool     not_contains(const BitVectorMedium&) const;
  public:
    // index into array from bit index
    inline uint32_t idx(const uint32_t i) const {  return (i >> _shift); }
    inline uint32_t rmd(const uint32_t i) const {  return (i & _mask); }
    inline BitVectorMedium& set(const uint32_t i) { _bv[idx(i)] |= ((Tuint) 1) << rmd(i); return (*this); }
  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& printSetBitIdx(std::ostream& os) const;
  private:
    Tuint*   _bv;
    uint32_t _size;
  public:
    static const uint32_t _shift;
    static const uint32_t _mask;
};


template<class Tuint>
std::ostream& operator<<(std::ostream& os, const BitVectorMedium<Tuint>& x) { return x.print(os); }

template<class Tuint>
const uint32_t
BitVectorMedium<Tuint>::_shift =    (sizeof(Tuint) + 2)
                                  - ((sizeof(Tuint) >> 2) & 0x1)
                                  - ((sizeof(Tuint) >> 1) & 0x4);

template<class Tuint>
const uint32_t
BitVectorMedium<Tuint>::_mask = ( ((Tuint) 1) << (   (sizeof(Tuint) + 2)
                                                  - ((sizeof(Tuint) >> 2) & 0x1)
                                                  - ((sizeof(Tuint) >> 1) & 0x4)) ) - (Tuint) 1;

template<class Tuint>
BitVectorMedium<Tuint>::BitVectorMedium(Tuint* aData, const uint32_t aNoEntries, const bool aDoInitialize)
                       : _bv(aData), _size(aNoEntries) {
  if(aDoInitialize) {
    clear();
  }
}


template<class Tuint>
BitVectorMedium<Tuint>::~BitVectorMedium() {
}


template<class Tuint>
bool 
BitVectorMedium<Tuint>::is_empty() const {
  bool lRes = true;

  for(uint32_t i = 0; (i < size()) & lRes; ++i) {
    lRes &= ((Tuint) 0) == _bv[i];
  }

  return lRes;
}


template<class Tuint>
bool 
BitVectorMedium<Tuint>::is_not_empty() const {
  return !(is_empty());
}


template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::clear() {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = (Tuint) 0;
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::reset() {
  return clear();
}


template<class Tuint>
uint32_t
BitVectorMedium<Tuint>::cardinality() const {
  uint32_t lRes = 0;
  for(uint32_t i = 0; i < size(); ++i) {
    lRes += number_of_bits_set<Tuint>(_bv[i]);
  }
  return lRes;
}


template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::operator=(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = x._bv[i];
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>&
BitVectorMedium<Tuint>::operator|=(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] |= x._bv[i];
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>&
BitVectorMedium<Tuint>::operator&=(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] &= x._bv[i];
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>&
BitVectorMedium<Tuint>::operator^=(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] ^= x._bv[i];
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::union_with(const BitVectorMedium& x) {
  return ((*this) |= x);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::union_of(const BitVectorMedium& x, const BitVectorMedium& y) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = (x._bv[i] | y._bv[i]);
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::intersect_with(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] &= x._bv[i];
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::intersection_of(const BitVectorMedium& x, const BitVectorMedium& y) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = (x._bv[i] & y._bv[i]);
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::difference_with(const BitVectorMedium& x) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] &= ~(x._bv[i]);
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>& 
BitVectorMedium<Tuint>::difference_of(const BitVectorMedium& x, const BitVectorMedium& y) {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = (x._bv[i] & ~(y._bv[i]));
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>&
BitVectorMedium<Tuint>::increment() {
  for(uint32_t i = 0; i < size(); ++i) {
    ++_bv[i];
    if(0 != _bv[i]) {
      break;
    }
  }
  return (*this);
}

template<class Tuint>
BitVectorMedium<Tuint>&
BitVectorMedium<Tuint>::complement() {
  for(uint32_t i = 0; i < size(); ++i) {
    _bv[i] = ~(_bv[i]);
  }
  return (*this);
}

template<class Tuint>
uint32_t
BitVectorMedium<Tuint>::idx_highest_bit_set() const {
  const uint32_t lNoBitsPerEntry = (sizeof(Tuint) * 8);
  uint32_t lIdx = 0;
  for(int i = size() - 1; i >= 0; ++i) {
    if((Tuint) 0 != _bv[i]) {
      lIdx = i;
      break;
    }
  }
  const uint32_t lIdxRev = (size() - lIdx - 1);
  const uint32_t lRes = (lIdxRev * lNoBitsPerEntry) + ::idx_highest_bit_set(_bv[lIdx]);
  return lRes;
}

template<class Tuint>
uint32_t
BitVectorMedium<Tuint>::idx_lowest_bit_set() const {
  const uint32_t lNoBitsPerEntry = (sizeof(Tuint) * 8);
  uint32_t lIdx = 0;
  for(uint32_t i = 0; i < size(); ++i) {
    if((Tuint) 0 != _bv[i]) {
      lIdx = i;
      break;
    }
  }
  const uint32_t lRes = (lIdx * lNoBitsPerEntry) + ::idx_lowest_bit_set(_bv[lIdx]);
  return lRes;
}

template<class Tuint>
bool
BitVectorMedium<Tuint>::operator==(const BitVectorMedium& x) const {
  assert(size() == x.size());
  bool lRes = true;
  for(uint32_t i = 0; (i < size()) & lRes; ++i) {
    lRes = (_bv[i] == x._bv[i]);
  }
  return lRes;
}

template<class Tuint>
bool
BitVectorMedium<Tuint>::operator!=(const BitVectorMedium& x) const {
  assert(size() == x.size());
  return !((*this) == x);
}


template<class Tuint>
bool
BitVectorMedium<Tuint>::intersects(const BitVectorMedium& x) const {
  assert(size() == x.size());
  bool lRes = true;
  for(uint32_t i = 0; (i < size()) & lRes; ++i) {
    lRes = (((Tuint) 0) == (_bv[i] & x._bv[i]));
  }
  return !lRes;
}

template<class Tuint>
bool
BitVectorMedium<Tuint>::disjoint(const BitVectorMedium& x) const {
  assert(size() == x.size());
  bool lRes = true;
  for(uint32_t i = 0; (i < size()) & lRes; ++i) {
    lRes = (((Tuint) 0) == (_bv[i] & x._bv[i]));
  }
  return lRes;
}

template<class Tuint>
bool
BitVectorMedium<Tuint>::contains(const BitVectorMedium& x) const {
  assert(size() == x.size());
  bool lRes = true;
  for(uint32_t i = 0; (i < size()) & lRes; ++i) {
    lRes = (_bv[i] == (_bv[i] | x._bv[i]));
  }
  return lRes;
}

template<class Tuint>
bool
BitVectorMedium<Tuint>::not_contains(const BitVectorMedium& x) const {
  return !(contains(x));
}


template<class Tuint>
std::ostream&
BitVectorMedium<Tuint>::print(std::ostream& os) const {
  for(uint32_t i = 0; i < size(); ++i) {
    bvs_t lBvs(_bv[i]);
    lBvs.print(os);
  }
  return os;
}

template<class Tuint>
std::ostream&
BitVectorMedium<Tuint>::printSetBitIdx(std::ostream& os) const {
  const uint32_t lNoBitsPerEntry = (sizeof(Tuint) * 8);
  uint32_t lIdxOffset = 0;
  for(uint32_t i = 0; i < size(); ++i) {
    for(BvMemberIdxAsc<Tuint> lIter(_bv[i]); lIter.isValid(); ++lIter) {
      os << (lIdxOffset + (*lIter)) << ' ';
    }
    lIdxOffset += lNoBitsPerEntry;
  }
  return os;
}


#endif
