#ifndef QERROR_HH
#define QERROR_HH
#include <algorithm>
#include <math.h>
#include <limits>
#include <stdlib.h>

typedef unsigned int uint;

inline double
qvalue(const double x) {
  if(0.0 >= x) {
    return std::numeric_limits<double>::max();
  }
  if(1.0 >= x) {
    return (1/x);
  }
  return x;
}

inline double
qmiddle(const double x, const double y) {
  return sqrt(x*y);
}

inline double
qvalue(const double x, const double y) {
  if((0.0 == x) || (0.0 == y)) {
    return std::numeric_limits<double>::max();
  }
  
  if((x < 0 && y > 0) || (x > 0 && y < 0)) {
    return std::numeric_limits<double>::max();
  }
  return std::max<double>(x/y, y/x);
}

inline double
qerror(const double x, const double y) {
  if((0.0 == x) || (0.0 == y)) {
    return std::numeric_limits<double>::max();
  }
  if((x < 0 && y > 0) || (x > 0 && y < 0)) {
    return std::numeric_limits<double>::max();
  }
  return std::max<double>(x/y, y/x);
}

inline double
qerror_01(double x, double y) {
  if(x < 1.0) { x = 1.0; }
  if(y < 1.0) { y = 1.0; }
  return qerror(x,y);
}



inline double
qerrorZeroNeg_1(const double x, const double y) {
  if((0.0 == x) && (0.0 == y)) { return 0.0; }
  if((0.0 < x) && (0.0 > y)) { return std::numeric_limits<double>::max(); }
  if((0.0 > x) && (0.0 < y)) { return std::numeric_limits<double>::max(); }
  const double absquot = fabs(x / y);
  return ( ((1.0 < absquot) ? absquot : (1.0/absquot) ) - 1.0);
}

inline double
qvaluesigned(const double aTrueValue, const double aEstimate) {
  return ((aEstimate >= aTrueValue) ? ((-aEstimate / aTrueValue)) : ((aTrueValue / aEstimate)));
}

inline int
qsign(const double x) {
  return ((x < 1.0) ? -1 : ((x > 1.0) ? +1 : 0));
}


inline double 
pError(const double yk, const double fk) { // yk: tatsaechlicher Wert, fk: Schaetzung
  return ((fk >= yk) ? ((-fk / yk) + 1.0) : ((yk / fk) - 1.0));
}


inline uint
qpivot(const double* x, const uint n) {
  uint lRes = 0;
  for(uint i = 1; i < n; ++i) {
    if(qvalue(fabs(x[i])) < qvalue(fabs(x[lRes]))) {
      lRes = i;
    }
  }
  return lRes;
}

#endif
