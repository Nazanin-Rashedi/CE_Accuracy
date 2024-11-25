#include "double_vec_ops.hh"
#include <limits>

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

bool
is_equal(const double_vt& x, const double_vt& y, const double eps) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(!(is_equal<double>(x[i], y[i], eps))) {
      return false;
    }
  }
  return true;
}

double
max_abs_distance(const double_vt& x, const double_vt& y) {
  double lRes = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    lRes = std::max<double>(lRes, std::fabs(x[i] - y[i]));
  }
  return lRes;
}

#ifdef __AVX2__
double
sum_elements(const double_vt& x) {
  double lRes = 0;
  if(8 > x.size()) {
    for(size_t i = 0; i < x.size(); ++i) {
       lRes += x[i];
    }
    return lRes;
  }
  const size_t n = x.size() / 4;
  static const double lZero = 0;
  __m256d lSum = _mm256_broadcast_sd(&lZero);
  const double* y = x.data();
  for(uint i = 0; i < n; ++i) {
    lSum = _mm256_add_pd(lSum, _mm256_loadu_pd(y));
    y += 4;
  }
  const double* r = (const double*) &lSum;
  lRes =  r[0] + r[1] + r[2] + r[3];
  for(uint i = 4*n; i < x.size(); ++i) {
    lRes += x[i];
  }
  return lRes;
}
#else
double
sum_elements(const double_vt& x) {
  double lRes = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    lRes += x[i];
  }
  return lRes;
}
#endif

void
add(double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] += y[i];
  }
}

void
sub(double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] -= y[i];
  }
}

void
invert_sign(double_vt& x) {
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] = -x[i];
  }
}

void
scale_multiplicatively(double_vt& x) {
  const double lSum = sum_elements(x);
  const double lFac = 1.0L / lSum;
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] *= lFac;
  }
}

void
scale_sum_gt_zero(double_vt& x, const double aSum) {
  double lSum = 0;
  uint   lNoGtZero = 0;
  for(uint i = 0; i < x.size(); ++i) {
     if(0 < x[i]) {
       lSum += x[i];
       ++lNoGtZero;
     }
  }
  const double lToAdd = (aSum - lSum) / ((double) lNoGtZero);
  for(uint i = 0; i < x.size(); ++i) {
    if(0 < x[i]) {
       x[i] += lToAdd;
    }
  }
}

void
sum(double_vt& x, const double_vt& a, const double_vt& b) {
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] = a[i] + b[i];
  }
}

void
diff(double_vt& x, const double_vt& a, const double_vt& b) {
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] = a[i] - b[i];
  }
}

bool
is_zero(const double_vt& x, const double aEps) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(std::fabs(x[i]) > aEps) {
      return false;
    }
  }
  return true;
}

bool
is_positive(const double_vt& x) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(0 > x[i]) {
      return false;
    }
  }
  return true;
}

bool
is_negative(const double_vt& x) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(0 < x[i]) {
      return false;
    }
  }
  return true;
}

bool
is_geq_c(const double_vt& x, const double c) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(x[i] < c) {
      return false;
    }
  }
  return true;
} 

bool
fabs_less(const double_vt& x, const double aEps) {
  for(size_t i = 0; i < x.size(); ++i) {
     if(std::fabs(x[i]) > aEps) {
       return false;
     }
  }
  return true;
}

double
sum_fabs(const double_vt& r) {
  double lRes = 0;
  for(size_t i = 0; i < r.size(); ++i) {
    lRes += std::fabs(r[i]);
  }
  return lRes;
}

double
max_fabs(const double_vt& r) {
  double lRes = 0;
  double x = 0;
  for(size_t i = 0; i < r.size(); ++i) {
    x = std::fabs(r[i]);
    if(x > lRes) {
      lRes = x;
    }
  }
  return lRes;
}

double
vec_min(const double_vt& x) {
  double lMin = std::numeric_limits<double>::max();
  for(uint i = 0; i < x.size(); ++i) {
    if(lMin > x[i]) {
      lMin = x[i];
    }
  }
  return lMin;
}

double
vec_max(const double_vt& x) {
  double lMax = -std::numeric_limits<double>::max();
  for(uint i = 0; i < x.size(); ++i) {
    if(lMax < x[i]) {
      lMax = x[i];
    }
  }
  return lMax;
}

size_t
vec_min_idx_min(const double_vt& x) {
  double lMin = std::numeric_limits<double>::max();
  size_t lIdxMin = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    if(lMin > x[i]) {
      lMin = x[i];
      lIdxMin = i;
    }
  }
  return lIdxMin;
}

size_t
vec_max_idx_min(const double_vt& x) {
  double lMax = -std::numeric_limits<double>::max();
  size_t lIdxMax = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    if(lMax < x[i]) {
      lMax = x[i];
      lIdxMax = i;
    }
  }
  return lIdxMax;
}

size_t
vec_min_idx_max(const double_vt& x) {
  double lMin = std::numeric_limits<double>::max();
  size_t lIdxMin = 0;
  for(int64_t i = 0; 0 <= i; --i) {
    if(lMin > x[i]) {
      lMin = x[i];
      lIdxMin = i;
    }
  }
  return lIdxMin;
}

size_t
vec_max_idx_max(const double_vt& x) {
  double lMax = -std::numeric_limits<double>::max();
  size_t lIdxMax = 0;
  for(int64_t i = x.size() - 1; 0 <= i; --i) {
    if(lMax < x[i]) {
      lMax = x[i];
      lIdxMax = i;
    }
  }
  return lIdxMax;
}

bool 
vec_leq(const double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  for(uint i = 0; i < x.size(); ++i) {
    if(x[i] > y[i]) { return false; }
  }
  return true;
}

bool 
vec_geq(const double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  for(uint i = 0; i < x.size(); ++i) {
    if(x[i] < y[i]) { return false; }
  }
  return true;
}

double
norm_1(const double_vt& x) {
  double lRes = 0;
  for(uint i = 0; i < x.size(); ++i) {
    lRes += std::abs(x[i]);
  }
  return lRes;
}
      
double
norm_2(const double_vt& r) {
  double lRes = 0;
  for(size_t i = 0; i < r.size(); ++i) {
    lRes += r[i] * r[i];
  }
  return lRes;
}


bool
vec_equal(const double_vt& x, const double_vt& y, const double aEps) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(std::fabs(x[i] - y[i]) > aEps) {
      return false;
    }
  }
  return true;
}

double
max_fabs_diff(const double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  double lRes  = -1;
  double lDiff =  0;
  for(uint i = 0; i < x.size(); ++i) {
     lDiff = std::fabs(x[i] - y[i]);
     if(lRes < lDiff) {
       lRes = lDiff;
     }
  }
  return lRes;
}

double
min_diff(const double_vt& x, const double_vt& y) {
  assert(x.size() == y.size());
  assert(0 < x.size());
  double lRes = x[0] - y[0];
  for(uint i = 1; i < x.size(); ++i) {
    const double lDiff = x[i] - y[i];
    if(lDiff < lRes) {
      lRes = lDiff;
    }
  }
  return lRes;
}



// x = x + (a - b)
void
addsub(double_vt& x, const double_vt& a, const double_vt& b) {
  for(size_t i = 0; i < x.size(); ++i) {
    x[i] = (a[i] - b[i]);
  }
}


uint 
countSubZero(const double_vt& x) {
  uint lRes = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    lRes += (0 > x[i]);
  }
  return lRes;
}

uint
countEqZero(const double_vt& x, const double aEps) {
  uint lRes = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    lRes += (is_equal<double>(x[i], 0, aEps));
  }
  return lRes;
}

uint 
countGtZero(const double_vt& x) {
  uint lRes = 0;
  for(size_t i = 0; i < x.size(); ++i) {
    lRes += (0 < x[i]);
  }
  return lRes;
}

#ifdef __AVX2__
void 
setZeroSubZero(double_vt& x) {
  const uint n = x.size();
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
  double* y = x.data();
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
void 
setZeroSubZero(double_vt& x) {
  for(size_t i = 0; i < x.size(); ++i) {
    if(0 > x[i]) {
      x[i] = 0;
    }
  }
}
#endif

double 
vec_sum_gt_zero(const double_vt& x, const double aToSub) {
  double lRes = 0;
  double y = 0;
  for(uint i = 0; i < x.size(); ++i) {
    y = x[i] - aToSub;
    if(0 < y) {
      lRes += y;
    }
  }
  return lRes;
}

void
vec_cut_neg(double_vt& x, const double aToSub) {
  double y = 0;
  for(uint i = 0; i < x.size(); ++i) {
    y = x[i] - aToSub;
    if(0 > y) {
      x[i] = 0;
    }
  } 
}

uint
vec_count_gt_zero(const double_vt& x, const double lToSub) {
  uint lRes = 0;
  for(uint i = 0; i < x.size(); ++i) {
    lRes += ((x[i] - lToSub) > 0);
  }
  return lRes;
}

void
vec_set_zero(double_vt& v) {
  const uint n = v.size();
  for(uint i = 0; i < n; ++i) {
    v[i] = 0;
  }
}

void
set_zero(double_vt& v, const double aEps) {
  const uint n = v.size();
  for(uint i = 0; i < n; ++i) {
    if(std::fabs(v[i]) < aEps) {
      v[i] = 0;
    }
  }
}

void
vec_set_to(double_vt& x, const double aVal) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] = aVal;
  }
}

void
vec_add_const(double_vt& x, const double_vt& y, const double c) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] = y[i] + c;
  }
}

void
vec_add_const(double_vt& x, const double c) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] += c;
  }
}

void
vec_x_add_cy(double_vt& x, const double_vt& y, const double c) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] = x[i] + c * y[i];
  }
}

#ifdef __AVX2__
void
vec_x_sub_cy(double_vt& x, const double_vt& y, const double c) {
  const uint n = x.size();
  if(4 > n) {
    for(uint i = 0; i < n; ++i) {
      x[i] = x[i] - c * y[i];
    }
    return;
  }
  const uint m = n / 4;
  __m256d lConst = _mm256_broadcast_sd(&c);
  __m256d lTmp;
        double* a = x.data();
  const double* b = y.data();
  for(uint i = 0; i < m; ++i) {
    lTmp = _mm256_loadu_pd(a);
    _mm256_storeu_pd(a, _mm256_sub_pd(lTmp, _mm256_mul_pd(lConst, _mm256_loadu_pd(b))));
    a += 4;
    b += 4;
  }
  for(uint i = 4*m; i < n; ++i) {
    x[i] = x[i] - c * y[i];
  }
}
#else
void   
vec_x_sub_cy(double_vt& x, const double_vt& y, const double c) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] = x[i] - c * y[i];
  }
}
#endif


void
get_unit_vector(double_vt& v, const uint aDim, const uint aSize) {
  v.resize(aSize);
  for(uint i = 0; i < aSize; ++i) {
    v[i] = 0;
  }
  v[aDim] = 1;
}

double
dotprod(const double_vt& v1, const double_vt& v2) {
  assert(v1.size() == v2.size());
  double lRes = 0;
  for(uint i = 0; i < v1.size(); ++i) {
    lRes += v1[i] * v2[i];
  }
  return lRes;
}

