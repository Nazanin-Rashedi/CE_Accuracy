#ifndef GLOB_INFRA_DICTIONARY_CHAINED_HH
#define GLOB_INFRA_DICTIONARY_CHAINED_HH

#include "infra/glob_infra_standard_includes.hh"
#include <unordered_map>

template<typename Tdom>
class DictionaryChained {
  public:
    struct id_t {
       uint32_t _id_val : 24;
       uint32_t _id_dic :  8;
       id_t() : _id_val(0), _id_dic(255) {}
       inline bool is_valid() const { return (251 > _id_dic); }
       inline uint8_t  id_dic() const { return _id_dic; }
       inline uint32_t id_val() const { return _id_val; }
    };
  public:
    typedef Tdom  dom_t;
    typedef std::vector<dom_t> dom_vt;
    typedef std::unordered_map<dom_t,uint32_t> dom_mt;
    typedef DictionaryChained<dom_t>  dic_t;
  private:
    DictionaryChained(const DictionaryChained&) = delete;
    DictionaryChained& operator=(const DictionaryChained&) = delete;
  public:
    DictionaryChained();
    DictionaryChained(dic_t* aSub);
  public:
          id_t   insert(const dom_t& aVal);
          id_t   find(const dom_t& aVal) const;
    const dom_t* get_val(const id_t aId) const;
  public:
    inline size_t size() const { return _vec.size(); }
    inline uint8_t id() const { return _id; }
  private:
    inline const dic_t* sub() const { return _sub; }
    inline       dic_t* sub()       { return _sub; }
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    dom_vt  _vec;
    dom_mt  _map;
    dic_t*  _sub;
    uint8_t _id;
};


template<typename Tdom>
DictionaryChained<Tdom>::DictionaryChained() : _vec(), _map(), _sub(nullptr), _id(0) {
}

template<typename Tdom>
DictionaryChained<Tdom>::DictionaryChained(dic_t* aSub) : _vec(), _map(), _sub(aSub), _id(aSub->id() + 1) {
  assert(250 > id());
}

template<typename Tdom>
typename DictionaryChained<Tdom>::id_t
DictionaryChained<Tdom>::insert(const dom_t& aVal) {
  id_t lRes = find(aVal);
  if(lRes.valid()) {
    return lRes;
  }
  typename dom_mt::const_iterator lIter = _map.find(aVal);
  if(_map.end() == lIter) {
     lRes._id_val = _vec.size();
     lRes._id_dic = id();
     _vec.push_back(aVal);
     _map.insert({aVal, lRes});
  } else {
     lRes = (*lIter).second;
  }
  return lRes;
}

template<typename Tdom>
typename DictionaryChained<Tdom>::id_t
DictionaryChained<Tdom>::find(const dom_t& aVal) const {
  id_t lRes;
  if(nullptr != sub()) {
    lRes = sub()->find(aVal);
    if(lRes.is_valid()) {
      return lRes;
    }
  }
  typename dom_mt::const_iterator lIter = _map.find(aVal);
  if(_map.cend() == lIter) {
    return lRes;
  }
  lRes._id_dic = id();
  lRes._id_val = (*lIter).second;
  return lRes; 
}

template<typename Tdom>
const typename DictionaryChained<Tdom>::dom_t*
DictionaryChained<Tdom>::get_val(const id_t aId) const {
  if(id() == aId.id_dic()) {
    return &(_vec[aId.id_val()]);
  }
  if(nullptr != sub()) {
    return sub()->get_val(aId);
  }
  return nullptr;
}

template<typename Tdom>
std::ostream&
DictionaryChained<Tdom>::print(std::ostream& os) const {
  for(size_t i = 0; i < _vec.size(); ++i) {
    os << "  " << i << ' ' << _vec[i] << std::endl;
  }
  return os;
}

#endif
