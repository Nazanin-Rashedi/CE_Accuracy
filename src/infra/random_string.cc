#include "random_string.hh"


char*
fRandomString(char* x, const size_t n) {
  static std::mt19937 lRng;
  for(size_t i = 0; i < n; ++i) {
    x[i] = 'A' + (lRng() % 20);
  }
  return x;
}


char*
fRandomString01(char* x, const size_t n, const int p) {
  static std::mt19937 lRng;
  for(size_t i = 0; i < n; ++i) {
    if(0 == (lRng() % p)) {
      x[i] = '1';
    } else {
      x[i] = '0';
    }
  }
  return x;
}

char*
fUint2Char(char* x, const unsigned int n, const unsigned int aNumber) {
  const unsigned int N = std::min<unsigned int>(n, 32);
  Bitvector32 lBv(aNumber);
  for(unsigned int i = 0; i < N; ++i) {
    x[i] = (lBv.test(i) ? '1' : '0');
  }
  return x;
}

