#include <iostream>
#include <iomanip>

#include "q.hh"

typedef unsigned int uint;

void
test0() {
  for(uint lDelta = 32; lDelta < 100; lDelta += 32) {
    for(uint lKappa = 10; lKappa <= 16; ++lKappa) {
      double lBase = q::calcBase<double>(lKappa * lDelta, 4);
      std::cout << std::setw(4) << lDelta << ' '
                << std::setw(4) << lKappa << ' '
                << std::setw(12) << lBase << ' '
                << std::setw(12) << sqrt(lBase) << ' '
                << std::endl;
    }
  }
}



int
main() {
  test0();
  return 0;
}

