#ifndef INFRA_SIMPLE_ORDERED_DICTIONARY_HH
#define INFRA_SIMPLE_ORDERED_DICTIONARY_HH

#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>
#include <inttypes.h>

template<class Tuint, class Tdom>
class SimpleOrderedDictionary {
  public:
    typedef std::set<Tdom>        set_t;
    typedef std::map<Tdom, Tuint> map_t;
    typedef std::vector<Tdom>     vec_t;
  public:
    SimpleOrderedDictionary();
  public:
    void init();
    void step(const Tdom&);
    void fin();
  public:
    inline Tuint getId(const Tdom& x) const;
    inline const Tdom& getVal(const Tuint i) const { return _vec[i]; }
  public:
    Tuint size() const { return _map.size(); }
  private:
    set_t _set;
    map_t _map;
    vec_t _vec;
};

template<class Tuint, class Tdom>
SimpleOrderedDictionary<Tuint,Tdom>::SimpleOrderedDictionary() : _set(), _map(), _vec() {
}


template<class Tuint, class Tdom>
void
SimpleOrderedDictionary<Tuint,Tdom>::init() {
  _set.clear();
  _map.clear();
  _vec.clear();
}

template<class Tuint, class Tdom>
void
SimpleOrderedDictionary<Tuint,Tdom>::step(const Tdom& x) {
  if(_set.end() == _set.find(x)) {
    _set.insert(x);
  }
}

template<class Tuint, class Tdom>
void
SimpleOrderedDictionary<Tuint,Tdom>::fin() {
  _vec.clear();
  Tuint i = 0;
  for(typename set_t::const_iterator iter = _set.begin(); iter != _set.end(); ++iter) {
    _map.insert(std::pair<Tdom,Tuint>((*iter), i++));
    _vec.push_back(*iter);
  }
  _set.clear();
}

template<class Tuint, class Tdom>
Tuint
SimpleOrderedDictionary<Tuint,Tdom>::getId(const Tdom& x) const {
  typename map_t::const_iterator lIter = _map.find(x);
  if(_map.end() == lIter) {
    return 0;
  }
  return (*lIter).second;
}

#endif

