#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdlib.h>

#include <inttypes.h>

bool
isPrime(const uint64_t x) {
  uint64_t z = sqrt(x);
  for(uint64_t i = 2; i <= z; ++i) {
    if(0 == (x % i)) {
      return false;
    }
  }
  return true;
}

void
test0() {
  for(uint64_t i = (2 * 3 * 7 * 43) + 1; i < 2000; ++i) {
    if(isPrime(i)) {
      std::cout << std::setw(5) << i << std::endl;
    }
  }
}

struct factor_t {
  uint64_t _base;
  uint32_t _exp;
  inline uint64_t base() const { return _base; }
  inline uint32_t exp()  const { return _exp;  }
};

typedef std::vector<factor_t> factor_vt;

bool
factorize(factor_vt& aFactor, uint64_t x) {
  uint64_t z = sqrt(x);
  aFactor.clear();
  bool lIsPrime = true;
  for(uint64_t i = 2; i <= z;) {
    if(0 == (x % i)) {
      lIsPrime = false;
      x /= i;
      if((0 == aFactor.size()) || (i != aFactor[aFactor.size() - 1].base())) {
        aFactor.push_back({i, 1});
      } else {
        aFactor[aFactor.size() - 1]._exp += 1;
      }
    } else {
      ++i;
    }
  }
  if(1 != x) {
    aFactor.push_back({x, 1});
  }
  return lIsPrime;
}


void
sieb(const uint64_t n, const int64_t aPrintIdx) {
  const bool lPrintQuotient = true;

  uint64_t lNoPrimesEstimate = std::max<uint64_t>(10, (((double) n) / log(((double) n))) * 1.1);
  std::cout << "~#primes = " << lNoPrimesEstimate << std::endl;
  std::vector<uint64_t> lPrimes;
  lPrimes.reserve(lNoPrimesEstimate);
  lPrimes.push_back(2);
  lPrimes.push_back(3);
  lPrimes.push_back(5);
  lPrimes.push_back(7);
  lPrimes.push_back(11);
  lPrimes.push_back(13);
  lPrimes.push_back(17);
  lPrimes.push_back(19);

  uint64_t lMaxDiff = 0;
  double lMinQuot = 99999.99;
  double lMaxQuot = 0;
  double lMinDidi = 99999.99;
  double lMaxDidi = 0;

  std::cout.precision(10);
  std::cout << std::fixed;

  if(20 < n) {
    for(uint64_t i = 21; i <= n; i += 2) {
      uint64_t lFin = (uint64_t) std::ceil(std::sqrt((double) i)); // eigentlich floor
      bool lIsPrime = true;
      for(uint64_t j = 1; lPrimes[j] <= lFin; ++j) {
        if(0 == (i % lPrimes[j])) {
          lIsPrime = false;
          break;
        }
      }
      if(lIsPrime) {
        lPrimes.push_back(i);
        if(lPrintQuotient) {
          bool lPrint = false;
          const uint64_t p1 = lPrimes[lPrimes.size() - 1];
          const uint64_t p2 = lPrimes[lPrimes.size() - 2];

          const double   lQuotient = ((double) p1) / ((double) p2);
          const uint64_t lDiff     = (double) (p1 - p2);
          const double   lDiffDiv  = (double) lDiff / std::log(p1);
          if(lMaxDiff < lDiff) { lMaxDiff = lDiff; lPrint = true; }
          if(lMaxQuot < lQuotient) { lMaxQuot = lQuotient; }
          if(lMinQuot > lQuotient) { lMinQuot = lQuotient; }
          if(lMaxDidi < lDiffDiv) { lMaxDidi = lDiffDiv; lPrint = true;}
          if(lMinDidi > lDiffDiv) { lMinDidi = lDiffDiv; }
          if(lPrint) {
            std::cout << std::setw(15) << i << ' '
                      << std::setw(10) << lPrimes.size() << ' '
                      << std::setw(10) << (uint64_t) std::round(((double) p1) / std::log(p1)) << ' '
                      << std::setw(13) << lQuotient << ' '
                      << std::setw(13) << lMinQuot << ' '
                      << std::setw(13) << lMaxQuot << ' '
                      << std::setw( 6) << (uint64_t) lDiff << ' '
                      << std::setw(14) << lDiffDiv 
                      << std::setw(14) << lMinDidi << ' '
                      << std::setw(14) << lMaxDidi << ' '
                      << std::setw(14) << (lMaxDidi / std::log(std::log(p1)))
                      << std::endl;
          }
        }
      }
    }
  }
  std::cout << "#primes = " << lPrimes.size() << std::endl;


  std::cout << "primes(" << n << ',' << aPrintIdx << "):" << std::endl;
  if(0 == aPrintIdx) {
    for(uint64_t i = 0; i < lPrimes.size(); ++i) {
      std::cout << lPrimes[i] << std::endl;
    }
  } else
  if(0 < aPrintIdx) {
    for(uint64_t i = aPrintIdx; i < lPrimes.size(); ++i) {
      std::cout << "  " << lPrimes[i] << std::endl;
    }
  } else
  if(0 > aPrintIdx) {
    for(uint64_t i = lPrimes.size() + aPrintIdx; i < lPrimes.size(); ++i) {
      std::cout << lPrimes[i] << std::endl;
    }
  }
}



int
main(const int argc, const char* argv[]) {
  char* lCharPtr = 0;
  if(2 == argc) {
    // simply check whether argument is a prime number
    long int p = strtol(argv[1], &lCharPtr, 10);
    factor_vt lFactor;
    if(argv[1] != lCharPtr) {
      const bool lIsPrime = isPrime(p);
      std::cout << "p = " << p << " is " << (lIsPrime ? "prime" : "not prime") << '.' << std::endl;
      if(!lIsPrime) {
        factorize(lFactor, p);
        std::cout << "p =";
        for(size_t i = 0; i < lFactor.size(); ++i) {
          if(0 < i) {
            std::cout << " *";
          }
          std::cout << ' ' << lFactor[i].base() << '^' << lFactor[i].exp();
        }
        std::cout << std::endl;
      }
      while(!isPrime(++p));
      std::cout << "p = " << p << " is " << (isPrime(p) ? "prime" : "not prime") << '.' << std::endl;
      return 0; 
    } else {
      std::cout << "error while reading argument (must be a number)." << std::endl;
      return -1;
    }
  }
 
/* 
  test0();
  sieb(100,  0);
  sieb(100,  3);
  sieb(100, -3);

  // sieb((1L << 20), -3);
  // sieb((1L << 30), -3);
*/

  sieb((1L << 40), -3);

}

