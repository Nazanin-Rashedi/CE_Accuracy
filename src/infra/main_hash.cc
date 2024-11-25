#include <iostream>
#include <iomanip>

#include <inttypes.h>

#include "hasht.hh"



void
test0() {
  for(uint64_t x = 0; x < 10; ++x) {
    uint64_t h = ht::murmurhash(x);
    std::cout << std::setw(3) << x << "  "
              << std::setw(14) << h << std::endl;
  }
}


void
test1(uint64_t aLim) {
  std::cout << std::endl;
  std::cout << "==============" << std::endl;
  std::cout << "=== test 1 ===" << std::endl;
  std::cout << "==============" << std::endl;

  uint64_t lFreqHash[64];
  uint64_t lFreqId[64];
  for(int i = 0; i < 64; ++i) {
    lFreqId[i] = 0;
    lFreqHash[i] = 0;
  }

  for(uint64_t x = 0; x < aLim; ++x) {
    uint64_t y = x;
    uint64_t h = ht::murmurhash(x);
    for(int i = 0; i < 64; ++i) {
      lFreqHash[i] += (h & 0x1);
      h >>= 1;
      lFreqId[i] += (y & 0x1);
      y >>= 1;
    }
  }
  for(int i = 0; i < 64; ++i) {
     std::cout << std::setw(2) << i << ' '
               << std::setw(10) << lFreqHash[i] << ' '
               << std::setw(10) << lFreqId[i]
               << std::endl;
  }
}

int
main() {
  test0();
  test1(1000*1000*10);
  return 0;
}

