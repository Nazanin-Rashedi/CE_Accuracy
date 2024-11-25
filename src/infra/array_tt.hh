#ifndef INFRA_ARRAY_TT_HH
#define INFRA_ARRAY_TT_HH

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>

template<typename Titem>
class array_tt {
  public:
    typedef Titem  item_t;
  public:
    array_tt(const array_tt&);
  private:
    array_tt& operator=(const array_tt&);
  public:
    array_tt();
    array_tt(const size_t aNoRows, const size_t aNoCols);
    ~array_tt();
  public:
    inline size_t noRows() const { return _noRows; }
    inline size_t noCols() const { return _noCols; }
    inline size_t idx(const size_t i, const size_t j) const { return (i * noCols() + j); }
    inline const item_t& operator()(const size_t i, const size_t j) const { return _arr[idx(i,j)]; }
    inline       item_t& operator()(const size_t i, const size_t j)       { return _arr[idx(i,j)]; }
    inline const item_t* arr()     const { return _arr; }
    inline       item_t* arr_nc()  const { return _arr; }
    inline const item_t* data()    const { return _arr; }
    inline       item_t* data_nc() const { return _arr; }
    inline       size_t  byte_size() const { return ((size_t) noRows() * (size_t) noCols() * sizeof(item_t)); }
  public:
    bool operator==(const array_tt& A) const;
  public:
    void resize(const size_t aNoRows, const size_t aNoCols);
    void clear();
    void set_to(const Titem& aItem);
    void mem_init(); // memset 0
  public:
    std::ostream& print(std::ostream& os, 
                        const char*   aSep,
                        const char*   aEndl,
                        const int     aWidth = 0) const;
  private:
    item_t* _arr;
    size_t  _noRows;
    size_t  _noCols;
};


template<typename Titem>
array_tt<Titem>::array_tt(const array_tt& x) : _arr(nullptr), _noRows(0), _noCols(0) {
  assert(0 == 1);
}


template<typename Titem>
array_tt<Titem>::array_tt() : _arr(nullptr), _noRows(0), _noCols(0) {}


template<typename Titem>
array_tt<Titem>::array_tt(const size_t aNx, const size_t aNy) : _arr(nullptr), _noRows(0), _noCols(0) {
  resize(aNx, aNy);
}


template<typename Titem>
array_tt<Titem>::~array_tt() {
  if(0 != _arr) {
    delete[] _arr;
  }
}

template<typename Titem>
bool
array_tt<Titem>::operator==(const array_tt& A) const {
  if(noRows() != A.noRows()) { return false; }
  if(noCols() != A.noCols()) { return false; }
  for(size_t i = 0; i < noRows(); ++i) {
    for(size_t j = 0; j < noCols(); ++j) {
      if((*this)(i,j) != A(i,j)) {
        return false;
      }
    }
  }
  return true;
}

template<typename Titem>
void
array_tt<Titem>::resize(const size_t aNoRows, const size_t aNoCols) {
  if(0 != _arr) {
    if(byte_size() == ((size_t) aNoRows * (size_t) aNoCols * sizeof(Titem))) {
      return;
    } else {
      delete[] _arr;
      _arr = 0;
    }
  }
  _noRows = aNoRows;
  _noCols = aNoCols;
  _arr = new item_t[noRows() * noCols()];
  assert(0 != _arr);
}

template<typename Titem>
void
array_tt<Titem>::clear() {
  _noRows = 0;
  _noCols = 0;
  if(0 != _arr) {
    delete[] _arr;
  }
  _arr = 0;
}

template<typename Titem>
void
array_tt<Titem>::mem_init() {
  std::memset(_arr, 0, byte_size());
}

template<typename Titem>
void 
array_tt<Titem>::set_to(const Titem& aItem) {
  for(size_t i = 0; i < noRows(); ++i) {
    for(size_t j = 0; j < noCols(); ++j) {
      (*this)(i,j) = aItem;
    }
  }
}

template<typename Titem>
std::ostream& 
array_tt<Titem>::print(std::ostream& os, 
                       const char*   aSep,
                       const char*   aEndl,
                       const int     aWidth) const {
  for(size_t i = 0; i < noRows(); ++i) {
    for(size_t j = 0; j < noCols(); ++j) {
       os << std::setw(aWidth) << (*this)(i,j);
       if((j + 1) < noCols()) {
         os << aSep;
       }
    }
    os << aEndl <<  std::endl;
  }
  return os;
}





#endif
