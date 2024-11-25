#include <iostream>
#include <iomanip>
#include <inttypes.h>

#include "CrystalClock.hh"

// #include "hasht.hh"

extern "C" {
  // #include "infra/cbind_to_hw_thread.h"
}

/*
 * get crystal clock frequency
 * x86_64, ArmV8 (e.g. apollo, pi3macmini1)
 * g++ -I.. main_test_rdtsc.cc /home/moer/obj/${HOST}/infra/cmeasure.o
 * geht auch:
 * make hdi
 * ${HOME}/obj/${HOST}/test/main_test_rdtsc 4
 *
 * calculation with cpuid: 
 *  Nominal TSC frequency = ( CPUID.15H.ECX[31:0] * CPUID.15H.EBX[31:0] ) ÷ CPUID.15H.EAX[31:0]
 * (Intel Architectures Software Developer's Manual 20.7.3, p 20-140)
 * pluto: estimated: frequency of crystal clock: 2.59403e+09
 */

void
test0() {
  std::cout << "=== test0 ===" << std::endl;
  CrystalClock::init();
  std::cout << "frequency of crystal clock: " << CrystalClock::frequency() << std::endl;
  CrystalClock lClock;
  lClock.start();
  do {
    lClock.stop();
  } while(lClock.cycles() < (1000*1000));
  std::cout << "duration of 1 mil cycles:" << std::endl;
  std::cout << "duration [ns] = " << lClock.duration_ns() << std::endl;
  std::cout << "duration [us] = " << lClock.duration_us() << std::endl;
  std::cout << "duration [ms] = " << lClock.duration_ms() << std::endl;
  std::cout << "duration [ s] = " << lClock.duration_s()  << std::endl;
}

void
test1() {
  CrystalClock lClock;
  std::cout << "Empty:" << std::endl;
  for(uint i = 0; i < 10; ++i) {
    lClock.start();
    lClock.stop();
    std::cout << ' ' << lClock.cycles();
  }
  std::cout << std::endl;

/*
  std::cout << "mumur32:" << std::endl;
  uint32_t h32 = 7834729;
  for(uint i = 0; i < 10; ++i) {
    lClock.start();
    h32 = ht::murmur_hash<uint32_t>(h32);
    lClock.stop();
    std::cout << ' ' << lClock.cycles();
  }
  std::cout << std::endl;

  std::cout << "mumur64:" << std::endl;
  uint32_t h64 = 7834729;
  for(uint i = 0; i < 10; ++i) {
    lClock.start();
    h64 = ht::murmur_hash<uint64_t>(h64);
    lClock.stop();
    std::cout << ' ' << lClock.cycles();
  }
  std::cout << std::endl;
*/

}

void
convert_to_time(uint64_t aNoCycles) {
  std::cout << "no cycles: " << std::setw(12) << aNoCycles << std::endl;
  std::cout << "    in  ns: " << std::setw(12) << CrystalClock::duration_ns(aNoCycles) << std::endl;
  std::cout << "    in  us: " << std::setw(12) << CrystalClock::duration_us(aNoCycles) << std::endl;
  std::cout << "    in  ms: " << std::setw(12) << CrystalClock::duration_ns(aNoCycles) << std::endl;
  std::cout << "    in   s: " << std::setw(12) << CrystalClock::duration_s(aNoCycles) << std::endl;
  std::cout << "    in min: " << std::setw(12) << CrystalClock::duration_min(aNoCycles) << std::endl;
  std::cout << "    in   h: " << std::setw(12) << CrystalClock::duration_h(aNoCycles) << std::endl;
}

int
main(const int argc, const char* argv[]) {
  // const int lHwtNo = 0;
  // cbind_to_hw_thread(lHwtNo, 1);
  test0();
  test1();
  if(2 == argc) {
    char*    lCharPtr = 0;
    uint64_t lArg = strtol(argv[1], &lCharPtr, 10);
    convert_to_time(lArg);
  }
  return 0;
}

