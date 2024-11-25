#ifndef INFRA_UNION_FIND_TT_HH
#define INFRA_UNION_FIND_TT_HH
#pragma once

#include "infra/glob_infra_standard_includes.hh"
#include <set>
#include <map>

/*
 * TODO
 */

template<class Telem>
class union_find_tt {
  public:
    typedef Telem                elem_t;
    typedef std::set<Telem>      set_t;
    typedef std::vector<Telem>   vec_t;
    typedef std::map<Telem,uint> map_t;
  private:
    union_find_tt(const UnionFind&);
    union_find_tt& operator=(const UnionFind&);
  public:
    union_find_tt();
    ~union_find_tt();
  public:
    void insert(const elem_t& aElem);
    void prepare(); // call after all inserts are done, then, mUnion can be called
  public:
    bool mUnion(const elem_t& aElem1, const elem_t& aElem2);
  public:
    inline uint size() const { return _noVar; }
    inline uint at(uint aIdx) const { return _elem[aIdx]; }
    inline bool mEqualIdx(const uint x, const uint y) { return (mFind(x) == mFind(y)); }
    inline bool isRepresentativeIdx(const uint x) const { return (_arr[x] == x); }
  private:
    bool mUnionIdx(const uint x, const uint y); // returns true, falls nicht bereits in gleicher Menge
    uint mFindIdx(const uint x);                // returns index
  public:
    std::ostream& print(std::ostream& os);
  private:
    void mLink(const uint x, const uint y);
  private:
    elem_st _elem_set;
    elem_vt _elem_vec;
    uint_vt _arr;
};

template<class Telem>
std::ostream& operator<<(std::ostream& os, const union_find_tt<Telem>& aUf) {
  return aUf.print(os);
}

std::ostream&
UnionFind::print(std::ostream& os, const std::string* aVarNames) {
  for(uint x = 0; x < size(); ++x) {
    if(mFind(x) == x) {
      os << std::setw(3) << at(i) << ' ';
      for(uint y = 0; y < size(); ++y) {
        if(x == y) { continue; }
        if(mFind(y) == x) {
          os << ' ' << aVarNames[y];
        }
      }
      os << std::endl;
    }
  }
  return os;
}


#endif
