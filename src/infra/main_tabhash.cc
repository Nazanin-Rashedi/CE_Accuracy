#include <iostream>
#include <iomanip>

#include <random>
#include "TabulationHash.hh"
#include "bitvectorsmall.hh"

void
test0() {
  std::mt19937 lMt;
  TabulationHash<std::mt19937, 8> lHt(5, lMt);
  typedef typename TabulationHash<std::mt19937, 8>::byte_t byte_t;

  for(uint32_t i = 0; i < 32; ++i) {
    uint32_t h = lHt.hash((byte_t*) &i, 4);
    std::cout << std::setw(4) << i << ' '
              << Bitvector32(h) << ' '
              << std::setw(4) << Bitvector32(h).cardinality() << ' '
              << std::endl;
  }
}


int
main() {
  test0();
  return 0;
}
