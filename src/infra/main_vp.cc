#include "glob_infra_standard_includes.hh"
#include <stdlib.h>
#include <cmath>
#include "vp_ops.hh"

extern "C" {
  #include "cmeasure.h"
}

void
fill_vector(double* x, const uint n) {
  for(uint i = 0; i < n; ++i) {
    // x[i] = 1;
    x[i] = 1 + i;
  }
}

double
dotprod(double* x, double* y, const uint n) {
  double lRes = 0;
  for(size_t i = 0; i < n; ++i) {
    lRes += x[i] * y[i];
  }
  return lRes;
}


void
check(const uint n) {
  int lRc = 0;
  double* x;
  lRc = posix_memalign((void**) &x, 32, 2 * n * sizeof(double));
  assert(0 == lRc);
  double* y;
  lRc = posix_memalign((void**) &y, 32, 2 * n * sizeof(double));
  assert(0 == lRc);

  // std::cout << "x = 0x" << std::hex << ((long unsigned int) x) << std::dec << std::endl;

  fill_vector(x, 2*n);
  fill_vector(y, 2*n);

  cmeasure_t lM1;
  cmeasure_start(&lM1);
  const double r1 = dotprod(x,y,n);
  cmeasure_stop(&lM1);

  cmeasure_t lM2;
  cmeasure_start(&lM2);
  const double r2 = vp_dotprod8(x, y, n);
  cmeasure_stop(&lM2);


  if(n < 200000) {
    if(std::fabs(r1 - r2) > 1E-17) {
      std::cout << "# " << n << ": " << r1 << " =?= " << r2 << ": diff = " << (r1 - r2) << std::endl;
    }
    assert(std::fabs(r1 - r2) < 1E-17);
  }

  std::cout << std::setw(8) << n << ' '
            << std::setw(8) << cmeasure_total_ns(&lM1) << ' '
            << std::setw(8) << cmeasure_total_ns(&lM2) << ' '
            << std::setw(8) << ((double) cmeasure_total_ns(&lM1) / (double) cmeasure_total_ns(&lM2)) << "  "
            // << std::setw(8) << r1 << ' '
            // << std::setw(8) << r2 << ' '
            << std::endl;

  const bool lCheckUnaligned = false;
  if(lCheckUnaligned) {
    if(100 < n) {
      for(uint i = 3; i < 100; ++i) {
        const double r1 = dotprod(&x[i], &y[i], n - i - 2);
        const double r2 = vp_dotprod8(&x[i], &y[i], n - i - 2);
        if(r1 != r2) {
          std::cout << "#ua " << r1 << " =?= " << r2 << std::endl;
        }
        assert(r1 == r2);
      }
      std::cout << "checked unaligned." << std::endl;
    }
  }
  
  free(x);
  free(y);

}

void
test0() {
  for(uint i = 1; i < 100; ++i) {
    check(i);
  }
}

void
test1() {
  for(uint i = 1000; i < 1000*1000; i *= 2) {
    check(i);
  }
}

int
main() {
  const bool lPrintTable = false;
  if(lPrintTable) {
    std::cout << "table:" << std::endl;
    for(uint i = 1; i < 17; ++i) {
      std::cout << std::setw(3) << i << ' ' 
                << std::setw(6) << (i*i)
                << std::setw(9) << ( (i * (i+1) * (2 * i + 1)) / 6 )
                << std::endl;
    }
  }

  std::cout << "end table." << std::endl;

  test0();
  test1();
  return 0;
}

 
