#include "newton.hh"


double
newton(defun_t aFun, defun_t aDer, double aInitVal, double aEps, unsigned int* aNoIter) {
  unsigned int lCount = 0;
  double x = aInitVal;
  double y = (*aFun)(x);
  // std::cout << "newton: " << std::endl;
  // std::cout << "  x_0 = " << x << std::endl;
  // std::cout << "  y_0 = " << y << std::endl;
  while(aEps < std::fabs(y)) {
    x = x - (y / ((*aDer)(x)));
    y = (*aFun)(x);
    ++lCount;
    // std::cout << "  x_" << lCount << " = " << x << std::endl;
    // std::cout << "  y_" << lCount << " = " << y << std::endl;
  }
  if(0 != aNoIter) {
    *aNoIter = lCount;
  }
  return x;
}


