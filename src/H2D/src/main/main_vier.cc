#include <iostream>
#include <iomanip>

#include "../infra/tmath.hh"
#include "../infra/q.hh"

typedef unsigned int uint;


double
roundXXX(const double x) {
  return round(x * 1000.0) / 1000.0;
}


template<class Tfloat>
Tfloat
qmiddle(const Tfloat x, const Tfloat y) {
  return mt::sqrtt<Tfloat>(x * y);
}


template<class Tnum>
void
print(Tnum aArr[2][2]) {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      std::cout << std::setw(4) << roundXXX(aArr[i][j]) << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void
read(int aArr[2][2]) {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      std::cin >> aArr[i][j];
      if(std::cin.eof()) {
        return;
      }
    }
  }
}

void
copyDouble(double aArrOut[2][2], const int aArrIn[2][2]) {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      aArrOut[i][j] = (double) aArrIn[i][j];
    }
  }
}  


template<class Tnum>
Tnum
total(const Tnum aArr[2][2]) {
  Tnum lRes = 0;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      lRes += aArr[i][j];
    }
  }
  return lRes;
}

template<class Tnum>
double
fMaxQError(const Tnum aArr[2][2]) {
  double lRes = 0.0;
  double lTotal = (double) total(aArr);
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      const double lTrueVal = aArr[i][j];
      const double lEstVal  = lTotal / 4.0;
      const double lQError  = q::qerrorT1<double>(lTrueVal, lEstVal);
      lRes = std::max<double>(lRes, lQError);
    }
  }

  return lRes;
}

double
fMaxQError(const int aArrOrig[2][2],
           const double aTotal,
           const double aToSubtract,
           const uint   aI,
           const uint   aJ) {
  double lMaxQError = 0.0;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      const double lTrueVal = (double) aArrOrig[i][j];
      const double lEstVal = (i == aI && j == aJ) ? ((aTotal / 4) - aToSubtract) : (aTotal / 4);
      const double lQError = q::qerrorT1<double>(lTrueVal, lEstVal);
      if(lQError > lMaxQError) {
        lMaxQError = lQError;
      }
    }
  }
  return lMaxQError;
}



void
calc(const int  aArr[2][2],
     const bool aDoCutAt1) {
  const int lTotal = total(aArr);
  const double t = (double) lTotal;
  std::cout << "total in old array: " << lTotal << std::endl;
  std::cout << "max q-error in old array: " << fMaxQError(aArr) << std::endl;
  std::cout << std::endl;

  double lMinMaxQError = 9999999.99;
  uint   lBestK;
  uint   lBestL;
  for(uint k = 0; k < 2; ++k) {
    for(uint l = 0; l < 2; ++l) {
      std::cout << "processing k,l: " << k << ", " << l << std::endl;
      // determin min/max in array without aArr[k,l];
      double lMin = +99999.99;
      double lMax = -99999.99;
      for(uint i = 0; i < 2; ++i) {
        for(uint j = 0; j < 2; ++j) {
          if(i == k and j == l) { continue; }
            lMin = std::min<double>(lMin, aArr[i][j]);
            lMax = std::max<double>(lMax, aArr[i][j]);
        }
      }
      std::cout << "min, max: " << lMin << ", " << lMax << std::endl;
      // if((double) aArr[k][l] < lMin) {
      //   lMin = (double) aArr[k][l];
      // }
      // std::cout << "min, max: " << lMin << ", " << lMax << std::endl;

      const double lQMiddle  = qmiddle<double>(lMin, lMax);
      std::cout << "lQMiddle: " << lQMiddle << std::endl;

      // the number we need to subtract from aArr[k][l]
      double lToSubtract = round(lTotal - 4 * lQMiddle);
      std::cout << "to subtract: " << lToSubtract << std::endl;
      if(aDoCutAt1) {
        if(lToSubtract > (double) aArr[k][l]) {
           lToSubtract = (double) aArr[k][l] - 1;
        }
      }
      std::cout << "to subtract: " << lToSubtract << std::endl;

      
      // total after subtraction = cum freq stored in 1x1 tile for input 4x4 tile
      const double lTot = lTotal - lToSubtract;
      std::cout << "lTot: " << lTot << std::endl;

      double lArrNew[2][2];
      copyDouble(lArrNew, aArr);
      lArrNew[k][l] -= lToSubtract;
      std::cout << "array after adjustment: " << std::endl;
      print(lArrNew);
      const double lNewTotal  = total(lArrNew);
      std::cout << "total in new array: " << lNewTotal << std::endl;
      std::cout << "total in new array / 4 : " << (lNewTotal / 4.0) << std::endl;
      const double lMaxQError = fMaxQError(lArrNew);
      std::cout << "max q-error in new array: " << lMaxQError << std::endl;
      const double lMaxQErrorProd = fMaxQError(aArr, lNewTotal, lToSubtract, k, l);
      std::cout << "max q-error produced by new array: " << lMaxQErrorProd 
                << std::endl;
      std::cout << std::endl;

      if(lMaxQErrorProd < lMinMaxQError) {
        lMinMaxQError = lMaxQError;
        lBestK = k;
        lBestL = l;
      }
    }
  }
  std::cout << "min max qerror: " << lMinMaxQError 
            << ", k,l: " << lBestK << ", " << lBestL 
            << "    (vs. old q-error: " << fMaxQError(aArr) << ") "
            << std::endl;

}


int
main() {
  int lArr[2][2];
  while(!std::cin.eof()) {
    read(lArr);
    if(std::cin.eof()) {
      break;
    }
    std::cout << std::endl;
    print(lArr);
    calc(lArr, false);
  }
}

