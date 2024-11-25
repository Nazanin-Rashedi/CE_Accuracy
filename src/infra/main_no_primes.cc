#include "infra/glob_infra_standard_includes.hh"

extern "C" {
  #include "infra/cmeasure.h"
}

#include <cmath>

// #include <gsl_sf_lambert.h>
#include "infra/FukushimaLambertW.hh"

typedef double (*fun_t)(const double);


// number of primes nach primzahlsatz
double
no_primes(const double x) {
  return (x / std::log(x));
}

// ableitung der anzahl der primzahlen 
// ableitung von no_primes
double
no_primes_der(const double x) {
  const double lnx = std::log(x);
  return (lnx - 1) / (lnx * lnx);
}

double
no_primes_between(const double x, const double y) {
  assert(x < y);
  const double lRes1 = no_primes(y) - no_primes(x);
  return lRes1;
}

double
no_primes_sub_y(const double y) {
  return 0;
}

double
lambert_W(const double x) {
  // const double lApprox = 
  //        (                    x 
  //          -                  x * x 
  //          + (3.0L/2.0L)    * x * x * x
  //          - (8.0L/3.0L)    * x * x * x * x
  //          + (125.0L/34.0L) * x * x * x * x * x
  //        );
  // const double lRes1 = Fukushima::LambertW0(x);
  const double lRes2 = Fukushima::LambertWm1(x);
  // lApprox \approx lRes1
  // std::cout << "W (" << x << "): "  << lApprox << ' ' << lRes1 << ' ' << lRes2 << std::endl;
  return lRes2;
}

// tuts nicht
double
no_primes_inverse_W(const double y) {
  return (-y * lambert_W(-1.0L / y));
}

// inverse der anazahl der primzahlen (s.a. lambert W function)
double 
no_primes_inverse_newton(const double y_val, const double x_initial, const double aEps, uint* aCount) {
  double x = x_initial;
  double y = no_primes(x);
  double zaehler, nenner;
  uint lCount = 0;
  while(aEps < std::fabs(y - y_val)) {
    zaehler = y - y_val;
    nenner = no_primes_der(x);
    x = x - (zaehler/nenner);
    y = no_primes(x);
    // std::cout << "  " << lCount << ' ' << x << ' ' << y << std::endl;
    ++lCount;
  } 
  if(nullptr != aCount) {
    *aCount = lCount;
  }
  return x;
}


void
test0() {
  double y = 0;
  double z = 0;
  double zz = 0;
  uint64_t lSumCount = 0;
  uint     lCount = 0;
  uint     lMax = 777;
  for(double x = 5; x <= lMax; x += 1) {
     y = no_primes(x);
     z = no_primes_inverse_newton(y, y * std::log(y), 1e-3, &lCount); // better than
     // z = no_primes_inverse_newton(y, y, 1e-3, &lCount); // this
     zz = no_primes_inverse_W(y);
     std::cout << std::setw(5) << x << ' '
               << std::setw(8) << y << ' '
               << std::setw(8) << zz << ' '
               << std::setw(8) << z << " [" << lCount << ']'
               << std::endl;
     lSumCount += lCount;
  }
  std::cout << "avg count = " << (((double) lSumCount) / ((double) (lMax - 5))) << std::endl;
}

void
test1(const uint n) {
  double lBegin = 5;
  double lEnd   = (double) n;
  double lSum1  = 0;
  cmeasure_t lMeas1;
  cmeasure_start(&lMeas1);
  for(double x = lBegin; x < lEnd; x += 1) {
    lSum1 += no_primes_inverse_newton(x, x * std::log(x), 1e-3, 0);
  }
  cmeasure_stop(&lMeas1);
  const double lNanSec1 = cmeasure_div_ns(&lMeas1, lEnd - lBegin);

  double lSum2 = 0;
  cmeasure_t lMeas2;
  cmeasure_start(&lMeas2);
  for(double x = lBegin; x < lEnd; x += 1) {
    lSum2 += no_primes_inverse_W(x);
  }
  cmeasure_stop(&lMeas2);
  const double lNanSec2 = cmeasure_div_ns(&lMeas2, lEnd - lBegin);


  std::cout << "diff = " << (lSum1 - lSum2) << std::endl;
  std::cout << "runtime [ns]:" << std::endl;
  std::cout.precision(2);
  std::cout << std::fixed;
  std::cout << "  newton  : " << std::setw(12) << lNanSec1 << " [ns]" << std::endl;
  std::cout << "  lambertW: " << std::setw(12) << lNanSec2 << " [ns]" << std::endl;


}


int
main() {
  test0();
  test1(1000*1000);
  return 0;
}

