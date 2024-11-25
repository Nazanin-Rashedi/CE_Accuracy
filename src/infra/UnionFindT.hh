#ifndef INFRA_UNION_FIND_TEMPLATE_HH
#define INFRA_UNION_FIND_TEMPLATE_HH

#include <iostream>
#include <iomanip>
#include <string>
#include <assert.h>

template<typename Tuint>
class UnionFindT {
  public:
    typedef Tuint uint_t;
  private:
    UnionFindT(const UnionFindT&) = delete;
    UnionFindT operator=(const UnionFindT&) = delete;
  public:
    UnionFindT(uint_t aNoVar);
    UnionFindT(uint_t* aArr, uint_t aNoVar);
    ~UnionFindT();
  public:
    inline uint_t noVar() const { return _noVar; }
    inline uint_t arr(uint_t i) const { return _arr[i]; }
           bool mUnion(const uint_t x, const uint_t y); // returns true, falls nicht bereits in gleicher Menge
           uint_t mFind(const uint_t x);
    inline bool mEqual(const uint_t x, const uint_t y) { return (mFind(x) == mFind(y)); }
    inline bool isRepresentative(const uint_t x) const { return (_arr[x] == x); }
  public:
    std::ostream& print(std::ostream& os);
    std::ostream& print(std::ostream& os, const std::string* aVarNames);
  private:
    void mLink(const uint_t x, const uint_t y);
  private:
    uint_t  _noVar;
    uint_t* _arr;
    bool    _allocQ;
};


template<typename Tuint>
UnionFindT<Tuint>::UnionFindT(uint_t aNoVar) : _noVar(aNoVar), _arr(0), _allocQ(true) {
  _arr = new uint_t[noVar()];
  for(uint_t i = 0; i < noVar(); ++i) {
    _arr[i] = i;
  }
}

template<typename Tuint>
UnionFindT<Tuint>::UnionFindT(uint_t* aArr, uint_t aNoVar) : _noVar(aNoVar), _arr(aArr), _allocQ(false) {
  for(uint_t i = 0; i < noVar(); ++i) {
    _arr[i] = i;
  }
}


template<typename Tuint>
UnionFindT<Tuint>::~UnionFindT() {
  if(_allocQ) {
    delete[] _arr;
  }
}

template<typename Tuint>
bool
UnionFindT<Tuint>::mUnion(const uint_t x, const uint_t y) {
  assert(x < noVar());
  assert(y < noVar());
  const uint_t xx = mFind(x);
  const uint_t yy = mFind(y);
  if(xx == yy) { return false; }
  mLink(xx, yy);
  return true;
}

template<typename Tuint>
void
UnionFindT<Tuint>::mLink(const uint_t x, const uint_t y) {
  assert(x < noVar());
  assert(y < noVar());
  assert(x != y);
  assert(arr(x) == x);
  assert(arr(y) == y);
  if(x < y) {
    _arr[y] = x;
  } else {
    _arr[x] = y;
  }
}

template<typename Tuint>
Tuint
UnionFindT<Tuint>::mFind(const uint_t x) {
  assert(x < noVar());
  if(arr(x) != x) {
    _arr[x] = mFind(arr(x));
  }
  return arr(x);
}

template<typename Tuint>
std::ostream&
UnionFindT<Tuint>::print(std::ostream& os) {
  for(uint_t x = 0; x < noVar(); ++x) {
    if(mFind(x) == x) {
      os << std::setw(3) << x << ' ';
      for(uint_t y = 0; y < noVar(); ++y) {
        if(x == y) { continue; }
        if(mFind(y) == x) {
          os << ' ' << y;
        }
      }
      os << std::endl;
    }
  }
  return os;
}

template<typename Tuint>
std::ostream&
UnionFindT<Tuint>::print(std::ostream& os, const std::string* aVarNames) {
  for(uint_t x = 0; x < noVar(); ++x) {
    if(mFind(x) == x) {
      os << std::setw(3) << aVarNames[x] << ' ';
      for(uint_t y = 0; y < noVar(); ++y) {
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

template<typename Tuint>
std::ostream& 
operator<<(std::ostream& os, UnionFindT<Tuint>& aUf) {
  return aUf.print(os);
}


#endif


