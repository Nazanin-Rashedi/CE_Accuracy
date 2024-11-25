#ifndef GLOB_INFRA_VP_OPS_HH
#define GLOB_INFRA_VP_OPS_HH

#ifdef __AVX2__
  #include <immintrin.h>
#else
  #warning no AVX2
  #ifdef __SSE2__
    #include <emmintrin.h>
  #else
    #warning no SSE
  #endif
#endif

// x,y two vectors, calculate <x,y>
// n vector length with n % 4 = 0

#ifdef __AVX2__
inline double
vp_dotprod4(const double* x, const double* y, const int n) {
  static const double lZero = 0;
  __m256d lSum = _mm256_broadcast_sd(&lZero);
  for(int i = 0; i < n; i += 4) {
    // lSum = _mm256_fmadd_pd(_mm256_load_pd(x), _mm256_load_pd(y), lSum);
    lSum = _mm256_fmadd_pd(_mm256_loadu_pd(x), _mm256_loadu_pd(y), lSum);
    x += 4;
    y += 4;
  }
  // double r[4] __attribute__((aligned (32)));
  double* r = (double*) &lSum;
  return r[0] + r[1] + r[2] + r[3];
}
#else
#endif

// __m256d _mm256_broadcast_pd (__m128d const * mem_addr)
// __m256d _mm256_loadu_pd (double const * mem_addr)  // unaligned load
// __m256d _mm256_load_pd (double const * mem_addr)   // aligned load
// void    _mm256_store_pd (double * mem_addr, __m256d a) // aligned store
// __m256d _mm256_broadcast_sd (double const * mem_addr)
#endif

#ifdef __AVX2__
inline double
vp_dotprod(const double* x, const double* y, const int n) {
  static const double lZero = 0;
  const int n1 = n & 0x3; 
  double lRes = 0;
  for(int i = 0; i < n1; ++i) {
    lRes += x[i] * y[i];
  }
  if(n1 < n) {
    x += n1;
    y += n1;
    __m256d lSum = _mm256_broadcast_sd(&lZero);
    for(int i = n1; i < n; i += 4) {
      lSum = _mm256_fmadd_pd(_mm256_loadu_pd(x), _mm256_loadu_pd(y), lSum);
      x += 4;
      y += 4;
    }
    double* r = (double*) &lSum;
    lRes += r[0] + r[1] + r[2] + r[3];
  }
  return lRes;
}
#else
#endif

#ifdef __AVX2__
// fastest
inline double
vp_dotprod8(const double* x, const double* y, const int n) {
  static const double lZero = 0;
  const int n1 = n & 0x7;
  double lRes = 0;
  for(int i = 0; i < n1; ++i) {
    lRes += x[i] * y[i];
  }
  if(n1 < n) {
    x += n1;
    y += n1;
    __m256d lSum1 = _mm256_broadcast_sd(&lZero);
    __m256d lSum2 = _mm256_broadcast_sd(&lZero);
    for(int i = n1; i < n; i += 8) {
      lSum1 = _mm256_fmadd_pd(_mm256_loadu_pd(x), _mm256_loadu_pd(y), lSum1);
      lSum2 = _mm256_fmadd_pd(_mm256_loadu_pd(x+4), _mm256_loadu_pd(y+4), lSum2);
      x += 8;
      y += 8;
    }
    double* r1 = (double*) &lSum1;
    double* r2 = (double*) &lSum2;
    lRes +=   r1[0] + r1[1] + r1[2] + r1[3]
            + r2[0] + r2[1] + r2[2] + r2[3];
  }
  return lRes;
}
#else
  #ifdef __SSE2__
    #define XXX
    #ifdef XXX
    inline double
    vp_dotprod8(const double* x, const double* y, const int n) {
      double lRes = 0;
      static const double lZero = 0;
      int i = 0;
      if((3 < n) && (0 == ((long unsigned int) x & 0x3F)) && (0 == ((long unsigned int) y & 0x3F))) {
        __m128d lSum1 = _mm_load_pd1(&lZero);
        __m128d lSum2 = _mm_load_pd1(&lZero);
        __m128d lProd1;
        __m128d lProd2;
        for(; (i + 4) < n; i += 4, x+= 4, y +=4) {
          lProd1 = _mm_mul_pd(_mm_load_pd(x), _mm_load_pd(y));
          lProd2 = _mm_mul_pd(_mm_load_pd(x+2), _mm_load_pd(y+2));
          lSum1 = _mm_add_pd(lSum1, lProd1);
          lSum2 = _mm_add_pd(lSum2, lProd2);
          // x += 4;
          // y += 4;
        }
        // std::cout << "sum 1 = " << lSum1[0] << ' ' << lSum1[1] << std::endl;
        // std::cout << "sum 2 = " << lSum2[0] << ' ' << lSum2[1] << std::endl;
        lSum1 = _mm_add_pd(lSum1, lSum2);
        lRes += lSum1[0] + lSum1[1];
        // std::cout << "sum12 = " << lRes << std::endl;
      }
      for(; i < n; ++i, ++x, ++y) {
        lRes += (*x) * (*y);
        // std::cout << i << ' ' << lRes << ' ' << (*x) << ' ' << (*y) << std::endl;
      }
      return lRes;
    }
    // __m128d _mm_load_pd1 (double const* mem_addr)
    // __m128d _mm_load_pd (double const* mem_addr)
    // __m128d _mm_mul_pd (__m128d a, __m128d b)
    // __m128d _mm_add_pd (__m128d a, __m128d b)
    #else
    inline double
    vp_dotprod8(const double* x, const double* y, const int n) {
      double lRes = 0;
      for(int i = 0; i < n; ++i) {
        lRes += x[i] * y[i];
      }
      return lRes;
    }
    #endif
  #else
    inline double
    vp_dotprod8(const double* x, const double* y, const int n) {
      double lRes = 0;
      for(int i = 0; i < n; ++i) {
        lRes += x[i] * y[i];
      }
      return lRes;
    }
  #endif
#endif

// adding two vectors, len < 4 || len % 4 = 0, nonoverlapping, unaligned
#ifdef __AVX2__
  inline void
  vp_add_pow2_al(double* x, const double* y, const int aLen) {
    double* lEnd = x + aLen;
    if(4 > aLen) {
      while(x < lEnd) {
        *x++ += *y++;
      }
    } else {
      while(x < lEnd) {
        _mm256_storeu_pd(x, _mm256_add_pd(_mm256_loadu_pd(x), _mm256_loadu_pd(y)));
        x += 4;
        y += 4;
      }
    }
  }
#else
  #ifdef __SSE2__
  #warning no AVX2
  inline void
  vp_add_pow2_al(double* x, const double* y, const int aLen) {
    for(int i = 0; i < aLen; ++i) {
      x[i] += y[i];
    }
  }
  #else
  #warning no SSE2
  inline void
  vp_add_pow2_al(double* x, const double* y, const int aLen) {
    for(int i = 0; i < aLen; ++i) {
      x[i] += y[i];
    }
  }
  #endif
#endif


#ifdef __AVX2__
inline void
vp_setzero_subzero(double* x, const uint n) {
  if(4 > n) {
    for(uint i = 0; i < n; ++i) {
      if(0 > x[i]) {
        x[i] = 0;
      }
    }
    return;
  }
  const uint m = n / 4;
  static const double lZeroS = 0;
  __m256d lZero = _mm256_broadcast_sd(&lZeroS);
  double* y = x;
  __m256d z;
  for(uint i = 0; i < m; ++i) {
    z = _mm256_loadu_pd(y);
    _mm256_storeu_pd(y, _mm256_and_pd(_mm256_cmp_pd(lZero, z, _CMP_LE_OS), z));
    y += 4;    
  }
  for(uint i = m * 4; i < n; ++i) {
    if(0 > x[i]) {
      x[i] = 0;
    }
  }
}
#else
inline void
vp_setzero_subzero(double* x, const int n) {
  assert(0 == 1);
}
#endif

// todo
// x[i] = f * x[i]
#ifdef __AVX2__
  inline void 
  vp_mul_all(double* x, const uint n, const double f) {
    for(uint i = 0; i < n; ++i) {
      x[i] *= f;
    }
  }
#else
  #ifdef __SSE2__
  #warning no AVX2
  inline void 
  vp_mul_all(double* x, const uint n, const double f) {
    for(uint i = 0; i < n; ++i) {
      x[i] *= f;
    }
  }
  #else
  #warning no AVX2, no SSE2
  inline void 
  vp_mul_all(double* x, const uint n, const double f) {
    for(uint i = 0; i < n; ++i) {
      x[i] *= f;
    }
  }
  #endif
#endif


