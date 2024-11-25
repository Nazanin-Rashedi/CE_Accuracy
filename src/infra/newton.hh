#ifndef INFRA_NEWTON
#define INFRA_NEWTON

// #include <iostream>
#include <cmath>

typedef double (*defun_t)(double);

double newton(defun_t aFun, defun_t aDer, double aInitVal, double aEps, unsigned int* aNoIter);


#endif
