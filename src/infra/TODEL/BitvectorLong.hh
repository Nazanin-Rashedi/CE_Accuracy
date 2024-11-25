#ifndef SRC_INFRA_BITVECTOR_LONG_HH
#define SRC_INFRA_BITVECTOR_LONG_HH

#include <iostream>
#include <iomanip>
#include <vector>
#include <inttypes.h>
#include <assert.h>
#include <limits>

#include "bitvectorsmall.hh"

class BitvectorLong {
  public:
    static constexpr size_t CBITS  =  (sizeof(uint64_t) * 8);
    static constexpr size_t CBITS1 = ((sizeof(uint64_t) * 8) - 1);
    static constexpr size_t MASK   = 0x3F;
    static constexpr size_t SHIFT  = 6;
    typedef Bitvector64       bv_t;
    typedef std::vector<bv_t> bv_vt;
  public:
    BitvectorLong() : _size(0), _capacity(0), _bv() {}
    BitvectorLong(const size_t aSize); // size in number of bits
  public:
    inline BitvectorLong& resize(const size_t aSize);   // size in number of bits
    inline BitvectorLong& clear();                      // set all bits to 0
  public:
    inline size_t size() const { return _size; }          // in number of bits
    inline size_t capacity() const { return _capacity; }  // in number of bits
    inline size_t noEntries() const { return _bv.size(); } // in number of 64-bit elements
  public:
    static inline size_t noEntries(const size_t aSize) { return ((aSize + CBITS1) / CBITS); }
  public:
    inline BitvectorLong& set  (const size_t aBitIdx);
    inline BitvectorLong& clear(const size_t aBitIdx); 
    inline BitvectorLong& setEvery(const size_t n, const size_t aOffset = 0); // inclusive 0
    inline BitvectorLong& setEveryX(const size_t n, const size_t aNoTimes, const size_t aOffset = 0);
  public:
    inline const bv_vt& bv() const { return _bv; }
    inline       bv_vt& bv()       { return _bv; }
  public:
    inline BitvectorLong& operator=(const BitvectorLong&);  // set to          argument
    inline BitvectorLong& operator|=(const BitvectorLong&); // set to and with argument
    inline BitvectorLong& operator&=(const BitvectorLong&); // set to  or with argument
    inline BitvectorLong& operator^=(const BitvectorLong&); // set to xor with argument
    inline BitvectorLong& complementOf(const BitvectorLong&); // set to          complement
    inline BitvectorLong& operator+=(const BitvectorLong&); // Addition
    inline BitvectorLong& operator-=(const BitvectorLong&); // Subtraktion
  public:
    std::ostream& printFwd(std::ostream& os) const; 
    std::ostream& printBwd(std::ostream& os) const; 
  private:
    size_t _size;     // in number of bits
    size_t _capacity; // in number of bits
    bv_vt  _bv;       // the bits
};


BitvectorLong&
BitvectorLong::resize(const size_t aSize) {
  _size = aSize;
  if(_size <= _capacity) {
    return (*this);
  }
  _capacity = noEntries(aSize);
  _bv.resize(noEntries());
  return (*this);
}

BitvectorLong&
BitvectorLong::clear() {
  for(size_t i = 0; i < _bv.size(); ++i) {
    _bv[i] = 0LL;
  }
  return (*this);
}

BitvectorLong&
BitvectorLong::set(const size_t aBitIdx) {
  _bv[aBitIdx >> SHIFT] |= (1LL << aBitIdx);
  return (*this);
}

BitvectorLong&
BitvectorLong::setEvery(const size_t aBitIdx, const size_t aOffset) {
  assert(0 < aBitIdx);
  size_t lBitIdx = aOffset;
  while(lBitIdx < size()) {
    set(lBitIdx);
    lBitIdx += aBitIdx;
  }
  return (*this);
}

BitvectorLong&
BitvectorLong::setEveryX(const size_t aBitIdx, const size_t aNoTimes, const size_t aOffset) {
  assert(0 < aBitIdx);
  size_t lBitIdx = aOffset;
  size_t lCount  = 0LL;
  while((lBitIdx < size()) && (lCount <= aNoTimes)) {
    set(lBitIdx);
    lBitIdx += aBitIdx;
    ++lCount;
  }
  return (*this);
}

BitvectorLong&
BitvectorLong::operator=(const BitvectorLong& x) {
  _bv.resize(x.size());
  return (*this);
}

BitvectorLong& 
BitvectorLong::operator|=(const BitvectorLong& x) {
  assert(size() == x.size());
  for(size_t i = 0; i < noEntries(); ++i) {
    _bv[i] |= x._bv[i];
  }
  return (*this);
}

BitvectorLong& 
BitvectorLong::operator&=(const BitvectorLong& x) {
  assert(size() == x.size());
  for(size_t i = 0; i < noEntries(); ++i) {
    _bv[i] &= x._bv[i];
  }
  return (*this);
}

BitvectorLong& 
BitvectorLong::operator^=(const BitvectorLong& x) {
  assert(size() == x.size());
  for(size_t i = 0; i < noEntries(); ++i) {
    _bv[i] ^= x._bv[i];
  }
  return (*this);
}

BitvectorLong& 
BitvectorLong::complementOf(const BitvectorLong& x) {
  assert(size() == x.size());
  for(size_t i = 0; i < noEntries(); ++i) {
    _bv[i] = ~(x._bv[i].bitvector());
  }
  return (*this);
}

BitvectorLong& 
BitvectorLong::operator+=(const BitvectorLong& x) {
  assert(size() == x.size());
  assert(0 == 1); // NYI
  uint64_t lOverflow = 0LL;
  for(size_t i = 0; i < noEntries(); ++i) {
    const uint64_t lSum = _bv[i].bitvector() + x._bv[i].bitvector() + lOverflow;
    _bv[i]    = lSum;
    if(std::numeric_limits<uint64_t>::max() == _bv[i].bitvector()) {
      lOverflow = ((0 < lOverflow) | (0 < x._bv[i].bitvector()));
    } else {
      lOverflow = (((std::numeric_limits<uint64_t>::max() - _bv[i].bitvector())  - lOverflow) < x._bv[i].bitvector());
    }
  }
  return (*this);
}

BitvectorLong& 
BitvectorLong::operator-=(const BitvectorLong& x) {
  assert(size() == x.size());
  assert(0 == 1); // NYI
  return (*this);
}



#endif
