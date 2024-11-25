#ifndef INFRA_NUMARRAY_TT_HH
#define INFRA_NUMARRAY_TT_HH

#include <iostream>
#include <iomanip>
#include <cstdint>

template<typename Tnum>
class numarray_tt {
  public:
    typedef unsigned int uint;
    typedef Tnum  item_t;
  private:
    numarray_tt& operator=(const numarray_tt&);
  public:
    numarray_tt();
    numarray_tt(const uint aNoRows, const uint aNoCols);
    numarray_tt(const numarray_tt&);
    ~numarray_tt();
  public:
    inline uint noRows() const { return _noRows; }
    inline uint noCols() const { return _noCols; }
    inline uint idx(const uint i, const uint j) const { return (i * noCols() + j); }
    inline const item_t& operator()(const uint i, const uint j) const { return _arr[idx(i,j)]; }
    inline       item_t& operator()(const uint i, const uint j)       { return _arr[idx(i,j)]; }
    inline const item_t* arr() const { return _arr; }
  public:
    Tnum sum() const;
    Tnum min() const;
    Tnum max() const;
    Tnum max2max(Tnum& a2ndLargestOut) const; // returns largest, and second largest as parameter
  public:
    uint32_t noGt(const Tnum) const; // number of elements greater than
  public:
    void setToShrink2Of(const numarray_tt<Tnum>& M);
    void setToShrink3Of(const numarray_tt<Tnum>& M);
  public:
    void resize(const uint aNoRows, const uint aNoCols);
  public:
    std::ostream& print(std::ostream& os, const int w) const;
  private:
    item_t* _arr;
    uint    _noRows;
    uint    _noCols;
};



template<typename Tnum>
numarray_tt<Tnum>::numarray_tt() : _arr(0), _noRows(0), _noCols(0) {}


template<typename Tnum>
numarray_tt<Tnum>::numarray_tt(const uint aNx, const uint aNy) : _arr(0), _noRows(0), _noCols(0) {
  resize(aNx, aNy);
}

template<typename Tnum>
numarray_tt<Tnum>::numarray_tt(const numarray_tt<Tnum>& x) : _arr(0), _noRows(0), _noCols(0) {
  resize(x.noRows(), x.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
       (*this)(i,j) = x(i,j);
    }
  }
}

template<typename Tnum>
numarray_tt<Tnum>::~numarray_tt() {
  if(0 != _arr) {
    delete[] _arr;
  }
}

template<typename Tnum>
void
numarray_tt<Tnum>::setToShrink2Of(const numarray_tt<Tnum>& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  (*this).resize(m / 2, n / 2);
  for(uint i = 0; i < m; i += 2) {
    for(uint j = 0; j < n; j += 2) {
      (*this)(i >> 1, j >> 1) = M(i,j) + M(i,j+1) + M(i+1,j) + M(i+1,j+1);
    }
  }
}


template<typename Tnum>
void
numarray_tt<Tnum>::setToShrink3Of(const numarray_tt<Tnum>& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  const uint m3 = m / 3;
  const uint n3 = n / 3;
  (*this).resize(m3, n3);
  for(uint i = 0; i < m3; ++i) {
    for(uint j = 0; j < n3; ++j) {
      const uint k = 3*i;
      const uint l = 3*j;
      (*this)(i, j) = M(k  ,l  ) + M(k  ,l+1) + M(k  ,l+2) +
                      M(k+1,l  ) + M(k+1,l+1) + M(k+1,l+2) +
                      M(k+2,l  ) + M(k+2,l+1) + M(k+2,l+2);
    }
  }
}


template<typename Tnum>
void
numarray_tt<Tnum>::resize(const uint aNoRows, const uint aNoCols) {
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

template<typename Tnum>
Tnum
numarray_tt<Tnum>::sum() const {
  Tnum lRes = (Tnum) 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      lRes += (*this)(i,j);
    }
  }
  return lRes;
}

template<typename Tnum>
Tnum
numarray_tt<Tnum>::min() const {
  Tnum lRes = (Tnum) (*this)(0,0);
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const Tnum x = (*this)(i,j);
      if(x < lRes) {
         lRes = x;
      }
    }
  }
  return lRes;
}

template<typename Tnum>
Tnum
numarray_tt<Tnum>::max() const {
  Tnum lRes = (Tnum) (*this)(0,0);
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const Tnum x = (*this)(i,j);
      if(x > lRes) {
         lRes = x;
      }
    }
  }
  return lRes;
}

template<typename Tnum>
Tnum
numarray_tt<Tnum>::max2max(Tnum& a2ndLargestOut) const {
  Tnum lMax = (*this)(0,0);
  Tnum lMax2 = (*this)(0,1);
  if(lMax < lMax2) {
    Tnum x = lMax;
    lMax  = lMax2;
    lMax2 = x;
  }
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const Tnum x = (*this)(i,j);
      if(x > lMax) {
        lMax2 = lMax;
        lMax  = x;
      } else
      if(x > lMax2) {
        lMax2 = x;
      }
    }
  }
  a2ndLargestOut = lMax2;
  return lMax;
}

template<typename Tnum>
uint32_t
numarray_tt<Tnum>::noGt(const Tnum x) const {
  uint32_t lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(x < (*this)(i,j)) {
         ++lRes;
      }
    }
  }
  return lRes;
}

template<typename Tnum>
std::ostream&
numarray_tt<Tnum>::print(std::ostream& os, const int w) const {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (*this)(i,j) << ' ';
    }
    os << std::endl;
  }
  return os;
}




#endif
