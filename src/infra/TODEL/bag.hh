#ifndef BAG_HH
#define BAG_HH

#include <iostream>
#include <map>
#include <inttypes.h>

template<class Tval, class Tuint = uint32_t>
class bag {
  public:
    typedef std::map<Tval, Tuint> map_t;
    typedef typename map_t::const_iterator const_iterator;
    typedef typename map_t::iterator iterator;
  public:
    bag();
    ~bag();
  public:
    void insert(const Tval& x, Tuint aNoTimes = 1); // x get's inserted aNoTimes times
  public:
    inline Tuint size() const { return _map.size(); }
    inline Tuint noDistinctValues() const { return _map.size(); }
           uint64_t cardinality() const;
    inline bool contains(const Tval& x) const { return (_map.end() != _map.find(x)); }
           Tuint frequency(const Tval& x) const;
    inline bool empty() const { return _map.empty(); }
    inline void clear() { _map.clear(); }
           Tuint noElemsMoreFrequentThan(const Tuint aThreshold) const;
           Tuint noElemsLessFrequentThan(const Tuint aThreshold) const;
    inline const_iterator begin() const { return _map.cbegin(); }
    inline const_iterator end() const { return _map.cend(); }
    inline const_iterator find(const Tval& x) const { return _map.find(x); }
    inline iterator begin() { return _map.begin(); }
    inline iterator end() { return _map.end(); }
           uint64_t selfjoinsize() const;
  public:
    bag& operator+=(const bag&);
  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& print(std::ostream& os, const int aWidth, const char* aSep) const;
  private:
    map_t _map;
};


template<class Tval, class Tuint>
bag<Tval,Tuint>::bag() : _map() {}

template<class Tval, class Tuint>
bag<Tval,Tuint>::~bag() {}

template<class Tval, class Tuint>
void
bag<Tval,Tuint>::insert(const Tval& x, Tuint aNoTimes) {
  iterator iter = _map.find(x);
  if(_map.end() == iter) {
    _map[x] += aNoTimes;
  } else {
    (*iter).second += aNoTimes;
  }
}

template<class Tval, class Tuint>
uint64_t
bag<Tval,Tuint>::cardinality() const {
  uint64_t lRes = 0;
  for(const_iterator iter = _map.begin(); iter != _map.end(); ++iter) {
    lRes += (uint64_t) (*iter).second;
  }
  return lRes;
}

template<class Tval, class Tuint>
Tuint
bag<Tval,Tuint>::frequency(const Tval& x) const {
  const_iterator iter = _map.find(x);
  if(_map.end() == iter) {
    return 0;
  } else {
    return (*iter).second;
  }
}

template<class Tval, class Tuint>
Tuint
bag<Tval,Tuint>::noElemsMoreFrequentThan(const Tuint aThreshold) const {
  uint lRes = 0;
  for(const_iterator iter = _map.begin(); iter != _map.end(); ++iter) {
    if(aThreshold < (*iter).second) {
      ++lRes;
    }
  }
  return lRes;
}

template<class Tval, class Tuint>
Tuint
bag<Tval,Tuint>::noElemsLessFrequentThan(const Tuint aThreshold) const {
  uint lRes = 0;
  for(const_iterator iter = _map.begin(); iter != _map.end(); ++iter) {
    if(aThreshold > (*iter).second) {
      ++lRes;
    }
  }
  return lRes;
}


template<class Tval, class Tuint>
uint64_t
bag<Tval,Tuint>::selfjoinsize() const {
  uint64_t lRes = 0;
  for(const_iterator lIter = _map.begin(); lIter != _map.end(); ++lIter) {
    lRes += ((*lIter).second * (*lIter).second);
  }
  return lRes;
}

template<class Tval, class Tuint>
bag<Tval, Tuint>& 
bag<Tval,Tuint>::operator+=(const bag& x) {
  for(const_iterator lIter = x.begin(); lIter != x.end(); ++lIter) {
    (*this).insert((*lIter).first, (*lIter).second);
  }
  return (*this);
}

template<class Tval, class Tuint>
std::ostream&
bag<Tval,Tuint>::print(std::ostream& os) const {
  for(const_iterator iter = _map.begin(); iter != _map.end(); ++iter) {
    os << ' ' << (*iter).first << " [" << (*iter).second << ']';
  }
  return os;
}

template<class Tval, class Tuint>
std::ostream&
bag<Tval,Tuint>::print(std::ostream& os, const int aWidth, const char* aSep) const {
  for(const_iterator iter = _map.begin(); iter != _map.end(); ++iter) {
    os << ' ' << (*iter).first << " [" << (*iter).second << ']' << aSep;
  }
  return os;
}


#endif  

