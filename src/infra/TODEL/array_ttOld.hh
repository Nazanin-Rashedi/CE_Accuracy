#ifndef INFRA_ARRAY_TT_HH
#define INFRA_ARRAY_TT_HH

#include <iostream>
#include <iomanip>


template<typename Titem>
class array_tt {
  public:
    typedef unsigned int uint;
    typedef Titem  item_t;
  public:
    array_tt(const array_tt&);
  private:
    array_tt& operator=(const array_tt&);
  public:
    array_tt();
    array_tt(const uint aNoRows, const uint aNoCols);
    ~array_tt();
  public:
    inline uint noRows() const { return _noRows; }
    inline uint noCols() const { return _noCols; }
    inline uint idx(const uint i, const uint j) const { return (i * noCols() + j); }
    inline const item_t& operator()(const uint i, const uint j) const { return _arr[idx(i,j)]; }
    inline       item_t& operator()(const uint i, const uint j)       { return _arr[idx(i,j)]; }
    inline const item_t* arr() const { return _arr; }
  public:
    void resize(const uint aNoRows, const uint aNoCols);
    void clear();
    void set_to(const Titem& aItem);
  public:
    std::ostream& print(std::ostream& os, 
                        const char*   aSep,
                        const char*   aEndl,
                        const int     aWidth = 0) const;
  private:
    item_t* _arr;
    uint    _noRows;
    uint    _noCols;
};


template<typename Titem>
array_tt<Titem>::array_tt(const array_tt& x) : _arr(0), _noRows(0), _noCols(0) {
}


template<typename Titem>
array_tt<Titem>::array_tt() : _arr(0), _noRows(0), _noCols(0) {}


template<typename Titem>
array_tt<Titem>::array_tt(const uint aNx, const uint aNy) : _arr(0), _noRows(0), _noCols(0) {
  resize(aNx, aNy);
}


template<typename Titem>
array_tt<Titem>::~array_tt() {
  if(0 != _arr) {
    delete[] _arr;
  }
}

template<typename Titem>
void
array_tt<Titem>::resize(const uint aNoRows, const uint aNoCols) {
  if(0 != _arr) {
    if((noRows() == aNoRows) && (noCols() == aNoCols)) {
      return;
    } else {
      delete[] _arr;
      _arr = 0;
    }
  }
  _noRows = aNoRows;
  _noCols = aNoCols;
  _arr = new item_t[noRows() * noCols()];
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
array_tt<Titem>::set_to(const Titem& aItem) {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
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
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
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
