#include "BitvectorLong.hh"


BitvectorLong::BitvectorLong(const size_t aSize) 
              :  _size(aSize), _capacity(noEntries(aSize) * CBITS), _bv(noEntries(aSize)) {}





std::ostream&
BitvectorLong::printFwd(std::ostream& os) const {
  for(size_t i = 0; i < (noEntries() - 1); ++i) {
    _bv[i].print(os);
  }
  const size_t lRest = (capacity() & MASK);
  if(0LL == lRest) {
    _bv[noEntries() - 1].print(os);
  } else {
    _bv[noEntries() - 1].print(os, lRest);
  }
  return os;
}

std::ostream&
BitvectorLong::printBwd(std::ostream& os) const {
  const size_t lRest = (capacity() & MASK);
  if(0LL == lRest) {
    _bv[noEntries() - 1].printReverse(os);
  } else {
    _bv[noEntries() - 1].printReverse(os, lRest);
  }
  for(int i = (int) noEntries() - 2; 0 <= i; --i) {
    _bv[i].printReverse(os);
  }
  
  return os;
}
