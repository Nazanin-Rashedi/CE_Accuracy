#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "measure.hh"
#include "q.hh"

template<class Tfloat>
bool
check(int n, const Tfloat* x) {
  bool lRes = true;
  for(int i = 1; i < n; ++i) {
    Tfloat q1 = q::qerrorT1<Tfloat>(x[i-1],x[i]);
    Tfloat q2 = q::qerrorT2<Tfloat>(x[i-1],x[i]);
    // std::cout << "q(" << x[i-1] << ", " << x[i] << ") : " << q1 << "  " << q2;
    if(q1 != q2) {
      // std::cout << " BAD";
      lRes = false;
    }
    // std::cout << std::endl;
  }
  return lRes;
}



template<class Tfloat>
void
test(const int n, const char* aType) {
  Tfloat* x = new Tfloat[n];

  // fill
  for(int i = 0; i < n; ++i) {
    x[i] = (Tfloat) (1 + rand());
  }

  // check
  if(check(n, x)) {
    std::cout << "check: ok." << std::endl;
  } else {
    std::cout << "check: BAD." << std::endl;
  }

  // measure
  Tfloat z1 = 0;
  Measure lMeasure1;
  lMeasure1.start();
  for(int i = 1; i < n; ++i) {
    z1 += q::qerrorT1<Tfloat>(x[i-1],x[i]);
  }
  lMeasure1.stop();
  double lTime1 = (lMeasure1.mTotalTime() / n) * 1000.0 * 1000.0 * 1000.0;

  std::cout << std::setw(8) << "qerrorT1" << ' '
            << std::setw(8) << aType << ' '
            << std::setw(8) << lTime1 << " ns"
            << std::endl;

  Tfloat z2 = 0;
  Measure lMeasure2;
  lMeasure2.start();
  for(int i = 1; i < n; ++i) {
    z1 += q::qerrorT2<Tfloat>(x[i-1],x[i]);
  }
  lMeasure2.stop();
  double lTime2 = (lMeasure2.mTotalTime() / n) * 1000.0 * 1000.0 * 1000.0;


  std::cout << std::setw(8) << "qerrorT2" << ' '
            << std::setw(8) << aType << ' '
            << std::setw(8) << lTime2 << " ns"
            << std::endl;

  delete[] x;
}


int
main() {
  test<float>(1000*1000, "float");
  test<double>(1000*1000, "double");
  test<float>(1000*1000, "float");
  test<double>(1000*1000, "double");
}


