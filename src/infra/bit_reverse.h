#ifndef BIT_REVERSE_HH
#define BIT_REVERSE_HH

#include <inttypes.h>

inline uint32_t
reverse32_basic(register uint32_t x) {
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
}

#ifdef WITH_BSWAP

#include <immintrin.h>

inline uint32_t
reverse32_bswap(uint32_t t) {
  uint32_t n = t;
  n = _bswap(n);
  n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
  n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
  n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
  return n;
}
#endif



#endif

