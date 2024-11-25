#include <iostream>
#include <iomanip>
#include "tmath.hh"



void
growth(const double aBegin, const double aEnd, const double aStepFac) {
  for(double x = aBegin; x < aEnd; x *= aStepFac) {
    std::cout << x << ' ' 
              << x << ' ' 
              << mt::log2t<double>(x) << ' '
              << (150 * mt::log2t<double>(x)) << ' '
              << (15000 * mt::log2t<double>(x))
              << std::endl;
  }
}

int
main() {
  growth(1, 1000*1000, 1.2);
  return 0;
}

