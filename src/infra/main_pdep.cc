#include <iostream>
#include <iomanip>
#include <assert.h>

#include "bitvectorsmall.hh"
#include "bit_intrinsics.hh"

#include "measure.hh"

/*
compile with
g++ -mbmi2 main_pdep.cc
g++ main_pdep.cc
*/

template<typename Tuint>
bool
test0(const uint aNoBits, const uint aMask, const bool aTrace) {
  bool lOk = true;
  for(Tuint i = 0; i < ((Tuint) 1 << aNoBits); ++i) {
     const Tuint x = i;
     const Tuint y = bit_distribute(x, aMask);
     const Tuint z = bit_gather<Tuint>(y, aMask);
     if(aTrace) {
       BitVectorSmall<Tuint> lBvX(x);
       BitVectorSmall<Tuint> lBvY(y);
       BitVectorSmall<Tuint> lBvZ(z);
       std::cout << lBvX << " " << lBvY << " " << lBvZ << std::endl;
     }
     if(x != z) {
       lOk = false;
       BitVectorSmall<Tuint> lBvX(x);
       BitVectorSmall<Tuint> lBvY(y);
       BitVectorSmall<Tuint> lBvZ(z);
       std::cout << "BAD: " << lBvX << " " << lBvY << " " << lBvZ << std::endl;
     }
  }
  if(aTrace) {
    std::cout << std::endl;
  }
  assert(lOk);
  return lOk;
}


void
test1() {
  const double lNS = (1000.0 * 1000.0 * 1000.0);
  const uint lNoBits = 20;
  Measure lMeasure;
  lMeasure.start();
  // test0<uint32_t>(10, 0x30707300, false);
  test0<uint32_t>(lNoBits, 0x75757375, false);
  lMeasure.stop();
  #ifdef __BMI2__
  std::cout << "runtime (1x gather, 1x distr) BMI2: ";
  #else
  std::cout << "runtime (1x gather, 1x distr) hand: ";
  #endif
  std::cout << ((lMeasure.mTotalTime() * lNS) / (1 << lNoBits)) << " [ns]" << std::endl;
}

int
main() {
  test0<uint32_t>(3, 0x7,        true);
  test0<uint32_t>(3, 0x01010100, true);
  test0<uint32_t>(3, 0x10010100, true);
  test0<uint32_t>(3, 0x03001000, true);
  test0<uint32_t>(7, 0x03031300, true);
  test1();
  return 0;
};


