#include <iostream>
#include <iomanip>
#include "q.hh"
#include "log_counter.hh"

extern "C" {
  #include "cmeasure.h"
}

double
do_count(const int n, const double aBase) {
  std::mt19937 lRng;
  LogCounter lLc(aBase);
  int lCounter = 0;
  cmeasure_t lMeasure;
  cmeasure_start(&lMeasure);
  for(int i = 0; i < n; ++i) {
    lLc.do_inc(lCounter, lRng);
  }
  cmeasure_stop(&lMeasure);
  std::cout << "runtime per increment: " << cmeasure_div_ns(&lMeasure, n) << " [ns]" << std::endl;

  const int    c = (int) std::round(lLc.val(lCounter));
  const double q = q::qerror<double>(n,c);
  std::cout << std::setw(12) << n << "  "
            << std::setw(12) << c << "  "
            << std::setw(12) << q << std::endl;
  return q;
}


void
test1(const double aBase) {
  double lMaxQ = 0;
  for(int n = 100; n < 1000 * 1000; n *= 1.2) {
    const double q = do_count(n, aBase);
    if(lMaxQ < q) {
      lMaxQ = q;
    }
  }
  std::cout << "@ " << aBase << ' ' << lMaxQ << std::endl;
}

int
main() {
  test1(2.0);
  test1(1.5);
  test1(1.3);
  test1(1.1);
  return 0;
}

