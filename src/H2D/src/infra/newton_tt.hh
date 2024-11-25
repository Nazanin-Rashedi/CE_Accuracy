#ifndef INFRA_NEWTON_TT_HH
#define INFRA_NEWTON_TT_HH

#include <iostream>
#include <cmath>

template<class Tfun, class Tder>
double
newton_tt(Tfun& aFun, Tder& aDer, double aInitVal, double aEps, unsigned int* aNoIter) {
  unsigned int lCount = 0;
  double x = aInitVal;
  double y = aFun(x);

  const bool lTrace = false;
  if(lTrace) {
    std::cout << "newton: " << std::endl;
    std::cout << "  x_0 = " << x << std::endl;
    std::cout << "  y_0 = " << y << std::endl;
  }
  while(aEps < std::fabs(y)) {
    x = x - (y / (aDer(x)));
    y = aFun(x);
    ++lCount;
    if(lTrace) {
      std::cout << "  x_" << lCount << " = " << x << std::endl;
      std::cout << "  y_" << lCount << " = " << y << std::endl;
    }
  }
  if(0 != aNoIter) {
    *aNoIter = lCount;
  }
  return x;
}

// Tfun must have double f(double) and double f_der(double)
template<class Tfun>
double
newton_tt(Tfun& f, double aInitVal, double aEps, unsigned int* aNoIter) {
  unsigned int lCount = 0;
  double x = aInitVal;
  double y = f.f(x);

  const bool lTrace = false;
  if(lTrace) {
    std::cout << "newton: " << std::endl;
    std::cout << "  x_0 = " << x << std::endl;
    std::cout << "  y_0 = " << y << std::endl;
  }
  while(aEps < std::fabs(y)) {
    x = x - (y / (f.f_der(x)));
   // x = x - (y /(f.fDerivative(f.n(),f.m(),f.k(),x)));
    y = f.f(x);
    ++lCount;
    if(lTrace) {
      std::cout << "  x_" << lCount << " = " << x << std::endl;
      std::cout << "  y_" << lCount << " = " << y << std::endl;
    }
  }
  if(0 != aNoIter) {
    *aNoIter = lCount;
  }
  return x;
}

#endif
