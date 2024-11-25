#include <iostream>
#include <iomanip>

#include "bit_intrinsics.hh"


typedef unsigned int uint;

void
test0() {
  for(uint i = 0; i < 17; ++i) {
    std::cout << std::setw(2) << i             << ' '
              << std::setw(2) << roundQPow2(i) << ' '
              << std::setw(2) << roundAbsPow2(i)
              << std::endl;
  }
}



int
main() {
  test0();
  return 0;
}


