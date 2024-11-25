#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include <assert.h>
#include "tmath.hh"
#include "measure.hh"
extern "C" {
  #include "infra/cmeasure.h"
  #include "cbind_to_hw_thread.h"
}

#include "measure.hh"
#include "fun_mod_mersenne.hh"

template<typename Tuint>
void
test(uint32_t a) {
  int lCountBad = 0;

  const Tuint p = ((Tuint) 1 << a) - 1;
  const Tuint lLim = ((Tuint) 1 << (2 * a)) - 1;
  for(Tuint x = 0; x < lLim ; ++x) {
     Tuint r1 = x % p;
     Tuint r2 = mt::mod_mersenne<Tuint>(x, p, a);
     lCountBad += (r1 != r2);
     if(r1 != r2) {
       std::cout << x << ' ' << a << ' ' << p << ' ' << r1 << ' ' << r2 << std::endl;
     }
  }
  std::cout << "#bad = " << lCountBad << std::endl;
}


template<typename Tuint>
void
testRuntimeMod() {
  const double lNs = 1000.0L * 1000.0L * 1000.0L;
  const int lNLoop = 777777;
  const int lSz = 1000;
  Tuint* x = new Tuint[lSz];

  const Tuint a = 13;                     // exponent of 2
  const Tuint p = ((Tuint) 1 << a) - 1;   // mersenne prime
  const Tuint m = ((Tuint) 1 << (2 * a)) - 1; // mask
  for(int i = 0; i < lSz; ++i) {
    x[i] = rand() % m;
  }
  assert(lSz < (((Tuint) 1 << (2 * a)) - 1));
  {
    Measure lMeasure;
    lMeasure.start();
    Tuint lSum = 0;
    for(int l = 0; l < lNLoop; ++l) {
      for(int i = 0; i < lSz; ++i) {
        lSum += x[i] % p;
      }
    }
    lMeasure.stop();
 
    if(4 == sizeof(Tuint)) {
      std::cout << "% uint32_t" <<  std::endl;
    } else
    if(8 == sizeof(Tuint)) {
      std::cout << "% uint64_t" << std::endl;
    } else {
      assert(0 == 1);
    }
    std::cout << "  tot time: " << (lMeasure.mTotalTime() * 1000.0L) << " [ms]" << std::endl;
    std::cout << "     per x: " << ((lMeasure.mTotalTime() / (lSz * lNLoop)) * lNs) << " [ns] " << std::endl;
    std::cout << "  [[" << lSum << "]]" << std::endl;
  }

  {
    Measure lMeasure;
    lMeasure.start();
    Tuint lSum = 0;
    for(int l = 0; l < lNLoop; ++l) {
      for(int i = 0; i < lSz; ++i) {
        lSum += mt::mod_mersenne<Tuint>(x[i], p, a);
      }
    }
    lMeasure.stop();

    if(4 == sizeof(Tuint)) {
      std::cout << "mmod uint32_t" <<  std::endl;
    } else
    if(8 == sizeof(Tuint)) {
      std::cout << "mmod uint64_t" << std::endl;
    } else {
      assert(0 == 1);
    }
    std::cout << "  tot time: " << (lMeasure.mTotalTime() * 1000.0L) << " [ms]" << std::endl;
    std::cout << "     per x: " << ((lMeasure.mTotalTime() / (lSz * lNLoop)) * lNs) << " [ns] " << std::endl;
    std::cout << "  [[" << lSum << "]]" << std::endl;
  }
  delete[] x;

  std::cout << std::endl;
}

void
testRuntimeMod32() {
  const int lNLoop = 777777;
  const int lSz = 1000;
  uint32_t* x = new uint32_t[lSz];
  
  const uint32_t a = 13;                     // exponent of 2
  const uint32_t p = ((uint32_t) 1 << a) - 1;   // mersenne prime
  const uint32_t m = ((uint32_t) 1 << (2 * a)) - 1; // mask
  for(int i = 0; i < lSz; ++i) {
    x[i] = rand() % m;
  } 

  cmeasure_t lMeasure;
  cmeasure_start(&lMeasure);
  uint32_t lSum = 0;
  for(int l = 0; l < lNLoop; ++l) {
    lSum += loop_regular_mod32(p, x, lSz);
  } 
  cmeasure_stop(&lMeasure);
  const double lTotalReg  = cmeasure_total_ms(&lMeasure);
  const double lSingleReg = ((1000.0 * 1000.0 * lTotalReg) / (lSz * lNLoop));
  std::cout << "regular mod uint32_t" << std::endl;
  std::cout << "  tot time: " << lTotalReg  << " [ms]" << std::endl;
  std::cout << "     per x: " << lSingleReg << " [ns] " << std::endl;
  std::cout << "  [[" << lSum << "]]" << std::endl;

  lSum = 0;

  cmeasure_start(&lMeasure);
  for(int l = 0; l < lNLoop; ++l) {  
    lSum += loop_mersenne_mod32(p, a, x, lSz);
  } 
  cmeasure_stop(&lMeasure);
  const double lTotalMers  = cmeasure_total_ms(&lMeasure); 
  const double lSingleMers = ((1000.0 * 1000.0 * lTotalMers) / (lSz * lNLoop));
  std::cout << "mersenne mod uint32_t" <<  std::endl;
  std::cout << "  tot time: " << lTotalMers  << " [ms]" << std::endl;
  std::cout << "     per x: " << lSingleMers << " [ns] " << std::endl;
  std::cout << "  [[" << lSum << "]]" << std::endl;
  std::cout << std::endl;
  std::cout << "factor: " << (lSingleReg / lSingleMers) << std::endl;
  std::cout << std::endl;
  delete[] x;
}

void
testRuntimeMod64() {
  const int lNLoop = 77777;
  const int lSz = 1000;
  uint64_t* x = new uint64_t[lSz];

  const uint64_t a = 13;                        // exponent of 2
  const uint64_t p = ((uint64_t) 1 << a) - 1;   // mersenne prime
  const uint64_t m = ((uint64_t) 1 << (2 * a)) - 1; // mask
  for(int i = 0; i < lSz; ++i) {
    x[i] = rand() % m;
  }
  cmeasure_t lMeasure;
  cmeasure_start(&lMeasure);
  uint64_t lSum = 0;
  for(int l = 0; l < lNLoop; ++l) {
    lSum += loop_regular_mod64(p, x, lSz);
  }
  cmeasure_stop(&lMeasure);
  const double lTotalReg  = cmeasure_total_ms(&lMeasure);
  const double lSingleReg = ((1000.0 * 1000.0 * lTotalReg) / (lSz * lNLoop));
  std::cout << "regular mod uint64_t" << std::endl;
  std::cout << "  tot time: " << lTotalReg  << " [ms]" << std::endl;
  std::cout << "     per x: " << lSingleReg << " [ns] " << std::endl;
  std::cout << "  [[" << lSum << "]]" << std::endl;

  lSum = 0;

  cmeasure_start(&lMeasure);
  for(int l = 0; l < lNLoop; ++l) {
    lSum += loop_mersenne_mod64(p, a, x, lSz);
  }
  cmeasure_stop(&lMeasure);
  const double lTotalMers  = cmeasure_total_ms(&lMeasure);
  const double lSingleMers = ((1000.0 * 1000.0 * lTotalMers) / (lSz * lNLoop));
  std::cout << "mersenne mod uint64_t" <<  std::endl;
  std::cout << "  tot time: " << lTotalMers  << " [ms]" << std::endl;
  std::cout << "     per x: " << lSingleMers << " [ns] " << std::endl;
  std::cout << "  [[" << lSum << "]]" << std::endl;
  std::cout << std::endl;
  std::cout << "factor: " << (lSingleReg / lSingleMers) << std::endl;
  std::cout << std::endl;
  delete[] x;
}



int
main() {
  // test<uint32_t>(5);
  // test<uint64_t>(5);

  // testRuntimeMod<uint32_t>();
  // testRuntimeMod<uint64_t>();

  cbind_to_hw_thread(4, 1);

  testRuntimeMod32();
  testRuntimeMod64();

  return 0;
}

