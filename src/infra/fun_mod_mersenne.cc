#include "fun_mod_mersenne.hh"

#include "infra/tmath.hh"

uint32_t 
loop_regular_mod32(const uint32_t p, uint32_t* x, const uint aSize) {
  uint32_t lSum = 0;
  for(uint i = 0; i < aSize; ++i) {
    lSum += x[i] % p;
  }
  return lSum;
}

uint32_t 
loop_mersenne_mod32(const uint32_t p, const uint32_t a, uint32_t* x, const uint aSize) {
  uint32_t lSum = 0;
  for(uint i = 0; i < aSize; ++i) {
    lSum += mt::mod_mersenne<uint32_t>(x[i], p, a);
  }
  return lSum;
}

uint64_t 
loop_regular_mod64(const uint64_t p, uint64_t* x, const uint aSize) {
  uint64_t lSum = 0;
  for(uint i = 0; i < aSize; ++i) {
    lSum += x[i] % p;
  }
  return lSum;
}

uint64_t 
loop_mersenne_mod64(const uint64_t p, const uint64_t a, uint64_t* x, const uint aSize) {
  uint64_t lSum = 0;
  for(uint i = 0; i < aSize; ++i) {
    lSum += mt::mod_mersenne<uint64_t>(x[i], p, a);
  }
  return lSum;
}


