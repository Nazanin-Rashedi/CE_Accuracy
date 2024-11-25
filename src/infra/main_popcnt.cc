#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include "bitvectorsmall.hh"
#include "bit_intrinsics.hh"


void
test0() {
  uint64_t x = 0;
  uint64_t lOne = 1;
  for(uint i = 0; i < 64; ++i) {
    x = lOne << i;
    std::cout << Bitvector64(x) << ' ' << number_of_bits_set(x) << std::endl;
  }
}


int
main() {
  test0();
  return 0;
}

