#include "HashUniversalH1.hh"


void
test0() {
  rng64_t lRng;
  HashUniversalH1 lH1;
  const uint64_t b = ((uint64_t) 1) << 10;
  for(uint i = 0; i < 10; ++i) {
    lH1.init(b, lRng);
    std::cout << std::setw(12) << lH1.m() << ' '
              << std::setw(12) << lH1.n() << ' '
              << std::setw(12) << lH1.p() << ' '
              << std::setw(12) << lH1.b();
    for(uint j = 0; j < 5; ++j) {
      std::cout << ' ' << std::setw(8) << lH1.hash(j);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int
main() {
  test0();
  return 0;
}

