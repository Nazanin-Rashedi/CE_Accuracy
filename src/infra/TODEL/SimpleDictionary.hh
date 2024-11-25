#ifndef GLOB_INFRA_SIMPLE_DICTIONARY_HH
#define GLOB_INFRA_SIMPLE_DICTIONARY_HH

#include "infra/glob_infra_standard_includes.hh"
#include <unordered_map>

template<typename Tuint, typename Tdom>
class SimpleDictionary {
  public:
    typedef Tdom  dom_t;
    typedef Tuint uint_t;
    typedef std::vector<dom_t> dom_vt;
    typedef std::unordered_map<dom_t, uint_t> dom_mt;
  public:
    SimpleDictionary();
  public:
    uint_t insert(const dom_t& aVal);
  public:
    uint_t getIdx(const dom_t& aVal) const;
    const dom_t& getVal(const uint_t aIdx) const;
  public:
    inline size_t size() const { return _vec.size(); }
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    dom_vt _vec;
    dom_mt _map;
};


template<typename Tuint, typename Tdom>
SimpleDictionary<Tuint,Tdom>::SimpleDictionary() : _vec(), _map() {
}

template<typename Tuint, typename Tdom>
Tuint
SimpleDictionary<Tuint, Tdom>::insert(const dom_t& aVal) {
  Tuint lRes = 0;
  typename dom_mt::const_iterator lIter = _map.find(aVal);
  if(_map.end() == lIter) {
     lRes = _vec.size();
     _vec.push_back(aVal);
     _map.insert({aVal, lRes});
  } else {
     lRes = (*lIter).second;
  }
  return lRes;
}


template<typename Tuint, typename Tdom>
Tuint
SimpleDictionary<Tuint, Tdom>::getIdx(const dom_t& aVal) const {
  typename dom_mt::const_iterator lIter = _map.find(aVal);
  return (_map.end() == lIter) ? size() : (*lIter).second;
}

template<typename Tuint, typename Tdom>
const typename SimpleDictionary<Tuint,Tdom>::dom_t&
SimpleDictionary<Tuint, Tdom>::getVal(const uint_t aIdx) const {
  return _vec[aIdx];
}

template<typename Tuint, typename Tdom>
std::ostream&
SimpleDictionary<Tuint, Tdom>::print(std::ostream& os) const {
  for(size_t i = 0; i < _vec.size(); ++i) {
    os << "  " << i << ' ' << _vec[i] << std::endl;
  }
  return os;
}

#endif
