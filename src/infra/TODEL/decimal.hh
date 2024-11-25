#ifndef DECIMAL_V1_HH
#define DECIMAL_V1_HH

#include <stdlib.h>

/*
 * class Decimal
 * is parameterized with
 * class rep_t : type representing a decimal (e.g. int32_t, int 64_t)
 * unsigned int aexp. precision 2 ==> aEXP = 100
 */

template<class rep_t, unsigned int aEXP>
class Decimal {
  public:
    Decimal() : _val((rep_t) 0) {}
    Decimal(const Decimal& x) : _val(x._val) {}
    // direkte verwendung (z.b 2 nachkommastellen dann: x = 125 --> 1.25 in _val
    // bool not used
    Decimal(rep_t x, bool) : _val(x) {}
    // aExp == aEXP, aExp not used
    Decimal(rep_t x, unsigned int aExp) : _val(x * aEXP) {}
  public:
    Decimal  operator-() const { return Decimal(-val(), true); }
    Decimal  operator+(const Decimal& x) const { return Decimal(_val + x._val, true); }
    Decimal  operator-(const Decimal& x) const { return Decimal(_val - x._val, true); }
    Decimal  operator*(const Decimal& x) const { Decimal lRes(*this); lRes *= x; return lRes; }
    Decimal  operator/(const Decimal& x) const { Decimal lRes(*this); lRes /= x; return lRes; }
  public:
    Decimal  operator*(rep_t x) const { Decimal lRes(*this); lRes *= x; return lRes; }
    Decimal  operator/(rep_t x) const { Decimal lRes(*this); lRes /= x; return lRes; }
  public:
    Decimal& operator=(const Decimal& x) { _val = x._val; return (*this); }
    Decimal& operator+=(const Decimal& x) { _val += x._val; return (*this); }
    Decimal& operator-=(const Decimal& x) { _val -= x._val; return (*this); }
    Decimal& operator*=(const Decimal& x) { _val *= x._val; return (*this) /= aEXP; }
    Decimal& operator*=(rep_t x) { _val *= x; return (*this); }
    Decimal& operator/=(const Decimal& x);
    Decimal& operator/=(rep_t);
  public:
    bool operator==(const Decimal& x) const { return (_val == x._val); }
    bool operator!=(const Decimal& x) const { return (_val != x._val); }
    bool operator<(const Decimal& x) const { return (_val < x._val); }
    bool operator<=(const Decimal& x) const { return (_val <= x._val); }
    bool operator>(const Decimal& x) const { return (_val > x._val); }
    bool operator>=(const Decimal& x) const { return (_val >= x._val); }
  public:
    bool read(const char*& x); // after read, x is advanced to the first char after decimal
  public:
    std::ostream& print(std::ostream& os) const;
  public:
    rep_t val() const { return _val; }
    Decimal&  val(rep_t x) { _val = x; return (*this); }
    Decimal&  inc_val(rep_t x) { _val += x; return (*this); }
  private:
    rep_t _val;
};


typedef Decimal<int32_t, 100> decimal32_2_t;
typedef Decimal<int64_t, 100> decimal64_2_t;


template<class rep_t, unsigned int aEXP>
Decimal<rep_t,aEXP>&
Decimal<rep_t,aEXP>::operator/=(rep_t q) {
  bool lNeg = false;
  rep_t lPosVal = val();
  if(0 > val()) {
    lNeg = true;
    lPosVal = -lPosVal;
  }
  if(0 > q) {
    _val = ((lPosVal - (q/2)) / q);
  } else {
    _val = ((lPosVal + (q/2)) / q);
  }
  if(lNeg) {
    _val = -_val;
  }
  return (*this);
}

template<class rep_t, unsigned int aEXP>
Decimal<rep_t,aEXP>&
Decimal<rep_t,aEXP>::operator/=(const Decimal<rep_t,aEXP>& x) {
  if(0 > val()) {
    if(0 > x.val()) {
      // this negative, x negative
      _val =  ( (aEXP * val() + (x.val() / 2)) / x.val());
    } else {
      // this negative, x positive (or 0)
      _val =  ( (aEXP * val() - (x.val() / 2)) / x.val());
    }
  } else {
    if(0 > x.val()) {
      // this positive (or 0), x negative
      _val =  ( (aEXP * val() - (x.val() / 2)) / x.val());
    } else {
      // this positive (or 0), x positive (or 0)
      _val =  ( (aEXP * val() + (x.val() / 2)) / x.val());
    }
  }


  return (*this);
}


template<class rep_t, unsigned int aEXP>
bool
Decimal<rep_t,aEXP>::read(const char*& aInString) {
  char* lEndPtr = 0;
  const char* x = aInString;
  rep_t lVal = strtoll(x, &lEndPtr, 10);
  // std::cout << "lVal = " << lVal << std::endl;
  if(x == lEndPtr) {
    return false;
  }
  lVal *= aEXP;
  // std::cout << "lVal = " << lVal << std::endl;
  x = lEndPtr;
  if('.' == *x) {
    ++x;
    rep_t lFrac = strtol(x, &lEndPtr, 10);
    // std::cout << "lFrac = " << lFrac << std::endl;
    if(x == lEndPtr || lFrac < 0 || lFrac >= 100) {
      return false;
    }
    x = lEndPtr;
    if(0 <= lVal) {
      lVal += lFrac;
      // std::cout << "pos _val = " << _val << std::endl;
    } else {
      lVal -= lFrac;
      // std::cout << "neg _val = " << _val << std::endl;
    }
  }
  // std::cout << "lVal = " << lVal << std::endl;
  _val = lVal;
  // std::cout << "_val = " << _val << std::endl;
  aInString = x;
  return true;
}

template<class rep_t, unsigned int aEXP>
std::ostream&
Decimal<rep_t,aEXP>::print(std::ostream& os) const {
  if(0 == val()) {
    os << "0.00";
    return os;
  }
  bool lNeg = false;
  rep_t lPosVal = val();
  if(0 > val()) {
    lNeg = true;
    lPosVal = -lPosVal;
  }
  const rep_t lInt = (lPosVal / ((rep_t) aEXP));
  const rep_t lFrac = (lPosVal % ((rep_t) aEXP));
  if(lNeg) {
    os << '-';
  }
  os << lInt << '.';
  if(0 == lFrac) { 
    os << "00";
    return os; 
  }
  for(int z = aEXP / 10; lFrac < z; z /= 10) {
    os << '0';
  }
  os << lFrac;
 
  return os;
}


template<class rep_t, unsigned int aEXP>
std::ostream&
operator<<(std::ostream& os, const Decimal<rep_t,aEXP>& x) {
  return x.print(os);
}



#endif
