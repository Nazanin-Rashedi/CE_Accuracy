#include "glob_infra_standard_includes.hh"

#include "matrix.hh"
#include "double_vec_ops.hh"

extern "C" {
  #include "cmeasure.h"
}

void
init_matrix(const Matrix& A) {
  const uint m = A.noRows();
  const uint n = A.noCols();
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      A(i,j) = 1 + sin(i * j);
    }
  } 
}

void
init_vec(double_vt& x) {
  const uint n = x.size();
  for(uint i = 0; i < n; ++i) {
    x[i] = 1 + sin(3 + i);
  }
}
  
void
check(const uint m, const uint n, const bool aTrace) {
  Matrix A(m,n);
  init_matrix(A);

  if(aTrace) {
    std::cout << "A = " << std::endl;
    A.print(std::cout, 3) << std::endl;
  }

  double_vt x(n);
  double_vt b1(m);
  double_vt b2(m);
  cmeasure_t lMeas1;

  init_vec(x);

  cmeasure_start(&lMeas1);
  A.getAx(b1.data(), x.data());
  cmeasure_stop(&lMeas1);

  cmeasure_t lMeas2;
  cmeasure_start(&lMeas2);
  A.getAx_XF(b2.data(), x.data());
  cmeasure_stop(&lMeas2);

  assert(is_equal(b1, b2, 0));
  
  std::cout << m << ',' << n << ": " 
            << cmeasure_total_ms(&lMeas1) << " / "
            << cmeasure_total_ms(&lMeas2) << " = "
            << (cmeasure_total_ms(&lMeas1) / cmeasure_total_ms(&lMeas2))
            << std::endl;
}

void
test0() {
  for(uint i = 8; i <= 14; ++i) {
    for(uint j = 8; j <= 14; ++j) {
      const uint m = (1 << i);
      const uint n = (1 << j);
      check(m, n, m < 10 && n < 10);
    }
  }
}

int
main() {
  test0();
  return 0;
}

