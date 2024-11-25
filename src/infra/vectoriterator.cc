#include "vectoriterator.hh"

VectorIterator::VectorIterator(const uint_vt& aLim)
	       : _lim(aLim), _vec(), _curr(0), _ok(true) {
  for(uint i = 0; i < lim().size(); ++i) {
    _vec.push_back(0);
  }
}

VectorIterator::~VectorIterator() {}


VectorIterator&
VectorIterator::operator++() {
  if(!ok()) {
    return (*this);
  }
  if((vec(curr()) + 1) < lim(curr())) {
    ++_vec[curr()];
    return (*this);
  }
  while( (vec(curr()) + 1) >= lim(curr()) && (curr() < size()) ) {
    ++_curr;
  }
  if(curr() >= size()) {
    _ok = false;
    return (*this);
  }
  ++_vec[curr()];
  for(uint i = 0; i < _curr; ++i) {
    _vec[i] = 0;
  }
  _curr = 0;
  return (*this);
}


