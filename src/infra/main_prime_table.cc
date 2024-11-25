#include "infra/glob_infra_standard_includes.hh"

#include "infra/tmath.hh"
// #include <iostream>
// #include <iomanip>
// #include <vector>
// #include <stdlib.h>

// #include <inttypes.h>

bool
isPrime(const uint64_t x) {
  uint64_t z = (uint64_t) std::floor(std::sqrt((double) x));
  for(uint64_t i = 2; i <= z; ++i) {
    if(0 == (x % i)) {
      return false;
    }
  }
  return true;
}

void
test0(const uint64_t aEnd, const double aFactor) {
  constexpr bool lTrace = false;
  assert(1 < aFactor);
  std::vector<uint64_t> lPrimes = {
    2,
    3,
    5,
    7,
   11,
   13,
   17,
   19,
   23,
   29,
   31,
   37,
   41,
   43,
   47,
   53,
   59,
   61,
   67,
   71,
   73,
   79,
   83,
   89,
   97,
  };
  for(uint64_t lNum = std::floor(lPrimes[lPrimes.size() - 1] * aFactor); lNum < aEnd; lNum *= aFactor) {
    if(0 == (lNum % 2)) {
      lNum -= 1;
    }
    if constexpr (lTrace) {
      std::cout << " check: " << lNum << std::endl;
    }
    while(!isPrime(lNum)) {
      lNum += 2;
    }
    lPrimes.push_back(lNum);
  }
  if constexpr (lTrace) {
    std::cout << std::endl;
  }
  // calculate max occurring factor between two primes
  double lMaxFactor = 0;
  for(uint i = 20; i < lPrimes.size(); ++i) {
    const double lF = ((double) lPrimes[i]) / ((double) (lPrimes[i-1] + 1));
    lMaxFactor = std::max<double>(lMaxFactor, lF);
  }
  // print table nicely
  std::cout << "// === primes(" 
            << lPrimes.size() << ',' 
            << aFactor << ','
            << mt::roundXXt<double>(lMaxFactor)
            << ") ===" << std::endl;
  std::cout << "const uint64_vt gPrimeTable_1_1 = {" << std::endl;
  for(uint i = 0; i < lPrimes.size(); ++i) {
    std::cout << std::setw(15) << lPrimes[i];
    if((i + 1) < lPrimes.size()) {
      std::cout << ',';
    }
    std::cout << std::endl;
  }
  std::cout << "};" << std::endl;
}

void
print_usage(const int argc, const char* argv[]) {
  std::cout << "usage: " << argv[0] << " <factor>?" << std::endl;
}

int
main(const int argc, const char* argv[]) {
  char* lCharPtr = 0;
  double lFactor = 1.1;
  if(2 == argc) {
    // read min factor between two primes
    lFactor = strtod(argv[1], &lCharPtr);
    if(lCharPtr == argv[1]) {
      print_usage(argc, argv);
      return -1;
    }
  } else
  if(1 != argc) {
    print_usage(argc, argv);
    return -1;
  }
  test0(uint64_t{1} << 40, lFactor);
  return 0;
}
 
