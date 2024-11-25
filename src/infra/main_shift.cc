#include <iostream>
#include <iomanip>


#include "bitvectorsmall.hh"
#include "bit_intrinsics.hh"


void
test0() {
             // 0000 0010 0010 0101
  uint32_t x = 0x0225;
  for(uint i = 0; i < 11; ++i) {
    uint32_t y = cyclic_shift_left(x, i, 11);
    std::cout << std::setw(2) << i << ' ' ;
    Bitvector32(y).print(std::cout, 11);
    std::cout << std::endl;
  }
}

void
test1() {
             // 0000 0010 0010 0101
  uint32_t c = 12;
  uint32_t x = 0x0D0;
  for(uint i = 0; i < c; ++i) {
    uint32_t y = cyclic_shift_left(x, i, c);
    std::cout << std::setw(2) << i << ' ' ;
    Bitvector32(y).print(std::cout, c + 5);
    std::cout << std::endl;
  }
}


int
main() {
  test0();
  test1();
  return 0;
}

