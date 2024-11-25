#ifndef INFRA_SIMPLE_DICTIONARY_HH
#define INFRA_SIMPLE_DICTIONARY_HH


#include <map>
#include <vector>

/*
 *   SimpleDictionary
 *
 *     maps
 *  T --> uint
 *      und
 *  uint --> T
 */


template<class T>
class SimpleDictionary32 {
  public:
    typedef unsigned int idx_t;
    typedef std::vector<T>    idx2t_t;
    typedef std::map<T, idx_t> t2idx_t;
  public:
    SimpleDictionary32();
  public:
    void     insert(const T& x); 
    idx_t    get_idx(const T& x); // if necessary: INSERTS !!!
    int      lookup(const T& x) const; // returns < 0 if not found;
    bool     contains(const T& x) const;
    const T& get_content(idx_t x) const { return _idx2t[x]; }
  public:
    idx_t size() const { return _t2idx.size(); } // egal
  public:
    void clear();
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    idx2t_t _idx2t;
    t2idx_t _t2idx;
};


template<class T>
SimpleDictionary32<T>::SimpleDictionary32() : _idx2t(), _t2idx() {
}


template<class T>
typename SimpleDictionary32<T>::idx_t
SimpleDictionary32<T>::get_idx(const T& x) {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    const idx_t lNewIdx = _idx2t.size();
    _idx2t.push_back(x);
    _t2idx.insert(std::pair<T,idx_t>(x,lNewIdx));
    return lNewIdx;
  }
  return (*iter).second;
}

template<class T>
void
SimpleDictionary32<T>::insert(const T& x) {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    const idx_t lNewIdx = _idx2t.size();
    _idx2t.push_back(x);
    _t2idx.insert(std::pair<T,idx_t>(x,lNewIdx));
  }
}

template<class T>
int
SimpleDictionary32<T>::lookup(const T& x) const {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    return -1;
  }
  return (*iter).second;
}

template<class T>
bool
SimpleDictionary32<T>::contains(const T& x) const {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  return (_t2idx.end() != iter);
}

template<class T>
void
SimpleDictionary32<T>::clear() {
  _idx2t.clear();
  _t2idx.clear();
}

template<class T>
std::ostream& 
SimpleDictionary32<T>::print(std::ostream& os) const {
  for(idx_t i = 0; i < _idx2t.size(); ++i) {
    os << i << ' '
       << _idx2t[i] << std::endl;
  }
  return os;
}

#endif
