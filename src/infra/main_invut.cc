#include "glob_infra_standard_includes.hh"
#include "matrix.hh"

extern "C" {
  #include "cmeasure.h"
}

/*
 *   main_invut.cc
 *   there are several possibilities to calculate the inverse of some
 *   matrix A using cholesky decomposition
 *   1) decompose A into upper triangular matrix U with A = U^tU
 *   2) decompose A into lower triangular matrix L with L = LL^t
 *   then invert U or L, i.e., calculate U^{-1} or L^{-1}
 *   then calculate
 *   1) A^{-1} =  U^{-1}   (U^{-1})^t
 *   2) A^{-1} = (L^{-1}^t  L^{-1}
 *   due to the different memory access patterns 
 *   the different procedures work with different efficiency
 *   this small program checks and evaluates some possibilities
 *   the matrix class provides
 *   for cholesky decomposition:
 *   setToSquareRootUpperTriangular(A)    --> U s.t. A = U^t U
 *   setToSquareRootLowerTriangular(A)    --> L s.t. A = L   L^t
 *   setToSquareRootLowerTriangular_XF(A) --> L s.t. A = L   L^t
 *   to directly calculate the inverse given L:
 *   inverseSquareRootLowerTriangular(L)     --> A^-1   // slow
 *   inverseSquareRootLowerTriangular_XF(L)  --> A^-1   // faster but not fast
 *   setToInverseOfUpperTriangular(U)             --> (U^-1)            // slow
 *   setToTransposedOfInverseOfUpperTriangular(U) --> (U^-1)^t = L^-1   // fast
 *   setToTransposedOfInverseOfLowerTriangular(L) --> (L^-1)^t = R^-1   // fast
 *   
 */

// helper class to construct symmetric positive matrices 
void
get_ks_pat(uint_vt& aKsPat, const uint aNoBf, const uint aNoBfKnown) {
  aKsPat.clear();
  assert(aNoBfKnown < aNoBf);
  const uint N = (1 << aNoBf);
  for(uint i = 0; i < N; ++i) {
    if(number_of_bits_set(i) <= aNoBfKnown) {
      aKsPat.push_back(i);
    }
  }
}

// helper class to construct symmetric positive matrices 
void
get_DDt(Matrix& DDt, const uint_vt& aKsPat, const uint aNoBf) {
  const uint n = aNoBf;
  const uint m = aKsPat.size();
  DDt.resize(m,m);
  const uint lMask = (((uint) 1) << n) - 1;
  uint i, j, x;
  DDt.resize(m,m);
  for(uint ii = 0; ii < m; ++ii) {
    i = aKsPat[ii];
    DDt(ii,ii) = (1 << number_of_bits_set<uint>((~i) & lMask));
  }
  for(uint ii = 0; ii < m; ++ii) {
    i = aKsPat[ii];
    for(uint jj = ii + 1; jj < m; ++jj) {
      j = aKsPat[jj];
      x = (1 << number_of_bits_set<uint>((~(i | j)) & lMask));
      DDt(ii,jj) = x;
      DDt(jj,ii) = x;
    }
  }
}

// check whether Ainv is A^{-1}
bool
is_inverse_of(const Matrix& A, const Matrix& Ainv, const double aEps) {
  Matrix I1;
  I1.setToProductOf(A, Ainv);
  Matrix I2;
  I2.setToProductOf(Ainv, A);
  return I1.isIdentity(aEps) && I2.isIdentity(aEps);
}

// check whether deocmposition of A info U^tU is valid
bool
check_U(const Matrix& U, const Matrix& A, const double aEps) {
  Matrix UtU;
  UtU.setToProductWithFirstTransposed(U, U);
  return U.isUpperTriangular(aEps) && UtU.equalUpTo(A, aEps);
}

// check whether deocmposition of A info LL^t is valid
bool
check_L(const Matrix& L, const Matrix& A, const double aEps) {
  Matrix LLt;
  LLt.setToProductWithSecondTransposed(L, L);
  return L.isLowerTriangular(aEps) && LLt.equalUpTo(A, aEps);
}

// check whether Uinv is correctly calculated
// i.e. 
// a) Uinv = U^-1
// b) Uinv is upper triangular (also check U is upper triangular)
// c) Uinv (Uinv)^t = A^{-1}
bool
check_Uinv(const Matrix& U, const Matrix& Uinv, const Matrix& A, const double aEps) {
  Matrix UinvUinvt;
  UinvUinvt.setToProductWithSecondTransposed(Uinv, Uinv);

  const bool lRes =    U.isUpperTriangular(aEps)
                    && Uinv.isUpperTriangular(aEps)
                    && is_inverse_of(U, Uinv, aEps)
                    && is_inverse_of(UinvUinvt, A, aEps);

  if(!lRes) {
    std::cout << "check_Uinv: failure:" << std::endl;
    std::cout << "A = " << std::endl;
    A.print(std::cout, 3) << std::endl;

    std::cout << "U = " << std::endl;
    U.print(std::cout, 8) << std::endl;

    std::cout << "U^-1 = " << std::endl;
    Uinv.print(std::cout, 8) << std::endl;

    std::cout << "U^-1 (U^-1)^t = " << std::endl;
    UinvUinvt.print(std::cout, 8) << std::endl;

    Matrix UUinv;
    UUinv.setToProductOf(U, Uinv);
    std::cout << "UUinv = " << std::endl;
    UUinv.print(std::cout, 8) << std::endl;

    Matrix UinvU;
    UinvU.setToProductOf(Uinv, U);
    std::cout << "UinvU = " << std::endl;
    UinvU.print(std::cout, 8) << std::endl;
  }

  return lRes;
}

// dito for Linv
bool
check_Linv(const Matrix& L, const Matrix& Linv, const  Matrix& A, const double aEps) {
  Matrix X;
  X.setToProductWithFirstTransposed(Linv, Linv);

  const bool lRes =    L.isLowerTriangular(aEps)
                    && Linv.isLowerTriangular(aEps)
                    && is_inverse_of(L, Linv, aEps)
                    && is_inverse_of(X, A, aEps);

  if(!lRes) {
    std::cout << "  check_Linv: failure:" << std::endl;
    std::cout << "A = " << std::endl;
    A.print(std::cout, 3) << std::endl;

    std::cout << "L = " << std::endl;
    L.print(std::cout, 8) << std::endl;

    std::cout << "L^-1 = " << std::endl;
    Linv.print(std::cout, 8) << std::endl;

    std::cout << "(L^-1)^t L^-1 = " << std::endl;
    X.print(std::cout, 8) << std::endl;

  }

  return lRes;
}

// check all procedures and properties
// of cholesky decomposition used to calculate inverses of A
// of all resulting matrices
// of a symmetric positive definite matrix A
bool
check_all(const Matrix& A, const double aEps) {

  // part 1: cholesky decomposition into
  // a) A = UU^t, U upper triangular
  // b) A = L^tL, L lower triangular

  Matrix U1;
  U1.setToSquareRootUpperTriangular(A);
  U1.setZeroSmallerThan(aEps);
  assert(check_U(U1, A, aEps));

  Matrix L1;
  L1.setToSquareRootLowerTriangular(A);
  L1.setZeroSmallerThan(aEps);
  assert(check_L(L1, A, aEps));

  Matrix L2;
  L2.setToSquareRootLowerTriangular_XF(A, aEps);
  L2.setZeroSmallerThan(aEps);
  assert(check_L(L2, A, aEps));

  assert(L1.equalUpTo(L2, aEps));
  assert(L1.isTransposedOf(U1, aEps));
  assert(L2.isTransposedOf(U1, aEps));

  std::cout << "  done part 1 (cholesky decomposition)." << std::endl;

  // part 2: check inversion of lower/upper triangular, and A


  // part 2.a inversion of upper triangular U and lower triangular L
  Matrix U1inv;
  U1inv.setToInverseOfUpperTriangular(U1, aEps);
  U1inv.setZeroSmallerThan(aEps / 10);

  assert(is_inverse_of(U1, U1inv, aEps));
  assert(check_Uinv(U1, U1inv, A, aEps));

  Matrix U2inv;
  U2inv.setToTransposedOfInverseOfLowerTriangular(L1);

  assert(is_inverse_of(U1, U2inv, aEps));
  assert(check_Uinv(U1, U2inv, A, aEps));
  assert(U2inv.equalUpTo(U1inv, aEps));     // U2inv == U1inv

  Matrix L3inv;
  L3inv.setToTransposedOfInverseOfUpperTriangular(U1, aEps);
  L3inv.setZeroSmallerThan(aEps / 10);

  assert(check_Linv(L1, L3inv, A, aEps));
  assert(L3inv.isTransposedOf(U1inv, aEps));  // L3inv = U1inv^t
  assert(is_inverse_of(L1, L3inv, aEps));

  std::cout << "  done part 2 a (inversion of upper/lower triangular matrices)." << std::endl;

  // part 2.b direct calculation of A^{-1}
  Matrix A1inv;
  A1inv.inverseSquareRootLowerTriangular(L1);
  A1inv.setZeroSmallerThan(aEps / 10);

  Matrix A2inv;
  A2inv.inverseSquareRootLowerTriangular_XF(L1);
  A2inv.setZeroSmallerThan(aEps / 10);

  assert(is_inverse_of(A, A1inv, aEps));
  assert(is_inverse_of(A, A2inv, aEps));
  assert(A1inv.equalUpTo(A2inv, aEps));

  std::cout << "  done part 2 b (direct calculation of A^{-1})." << std::endl;
  // part 2.c calculation of A^{-1} using inverses of upper/lower triangular
  Matrix UinvUinvt;
  UinvUinvt.setToUUt(U1inv);
  assert(UinvUinvt.equalUpTo(A1inv, aEps));

  Matrix L3invtL3inv;
  L3invtL3inv.setToProductWithFirstTransposed(L3inv, L3inv);
  assert(L3invtL3inv.equalUpTo(A1inv, aEps));

  std::cout << "  done part 2 c (calculation of A^{-1} via upper/lower triangular inverses)." << std::endl;

  return true;
}

bool
check_ks_pat(const uint_vt& aKsPat, const uint aNoBf, const uint aNoBfKnown) {
  std::cout << "check_ks_pat " << aNoBf << ' ' << aNoBfKnown << ' ' << aKsPat.size() << std::endl;
  Matrix A;
  get_DDt(A, aKsPat, aNoBf);
  assert(check_all(A, 1E-10));
  return true;
}

bool
check_bf(const uint aNoBf) {
  uint_vt lKsPat;
  for(uint lNoBfKnown = 1; lNoBfKnown < aNoBf; ++lNoBfKnown) {
    get_ks_pat(lKsPat, aNoBf, lNoBfKnown);
    assert(check_ks_pat(lKsPat, aNoBf, lNoBfKnown));
  }
  return true;
}

bool
check_all_bf() {
  for(uint lNoBf = 2; lNoBf <= 10; ++lNoBf) {
    assert(check_bf(lNoBf));
  }
  return true;
}


/*
 *  inversion procedures putting things together
 */

void
inverse_trad(Matrix& Ainv, const Matrix& A, const double aEps) {
  Matrix U;
  U.setToSquareRootUpperTriangular(A);
  U.setZeroSmallerThan(aEps);
  Matrix Uinv;
  Uinv.setToInverseOfUpperTriangular(U, aEps);
  Ainv.setToUUt(Uinv);
}

void
inverse_XF(Matrix& Ainv, const Matrix& A, const double aEps) {
  Matrix L;
  L.setToSquareRootLowerTriangular_XF(A, aEps);
  L.setZeroSmallerThan(aEps);
  Matrix Uinv;
  Uinv.setToTransposedOfInverseOfLowerTriangular(L);
  Ainv.setToUUt(Uinv);
}

/*
 *  measure the different procedures that can be involved
 *  in inverting a symmetric positive matrix A
 *  using cholesky decomposition
 */

void
measure_cholesky_decomp(const Matrix& A, const uint n, const uint aNoBfKnown, const double aEps) {
  const uint lNoIter = 1000000 / (n * aNoBfKnown * aNoBfKnown * (1 << n)); 

  // std::cout << "#iter = " << lNoIter << std::endl;
  assert(0 < lNoIter);

  Matrix U1;
  cmeasure_t lM_U1;
  cmeasure_start(&lM_U1);
  for(uint i = 0; i < lNoIter; ++i) {
    U1.setToSquareRootUpperTriangular(A);
    U1.setZeroSmallerThan(aEps);
  }
  cmeasure_stop(&lM_U1);

  Matrix L1;
  cmeasure_t lM_L1;
  cmeasure_start(&lM_L1);
  for(uint i = 0; i < lNoIter; ++i) {
    L1.setToSquareRootLowerTriangular(A);
    L1.setZeroSmallerThan(aEps);
  }
  cmeasure_stop(&lM_L1);

  Matrix L2;
  cmeasure_t lM_L2;
  cmeasure_start(&lM_L2);
  for(uint i = 0; i < lNoIter; ++i) {
    L2.setToSquareRootLowerTriangular_XF(A, aEps);
    L2.setZeroSmallerThan(aEps);
  }
  cmeasure_stop(&lM_L2);

  std::cout << std::setw(2) << n << ' ' 
            << std::setw(2) << aNoBfKnown << " | "
            << std::setw(13) << cmeasure_div_ms(&lM_U1, lNoIter) << ' '
            << std::setw(13) << cmeasure_div_ms(&lM_L1, lNoIter) << ' '
            << std::setw(13) << cmeasure_div_ms(&lM_L2, lNoIter)
            << std::endl;
}

void
measure_inverse_triangular(const Matrix& A, const uint n, const uint aNoBfKnown, const double aEps) {
  const uint lNoIter = 1000000 / (n * aNoBfKnown * aNoBfKnown * (1 << n)); 
 
  // std::cout << "#iter = " << lNoIter << std::endl;
  assert(0 < lNoIter);

  Matrix L;
  L.setToSquareRootLowerTriangular_XF(A, aEps);
  L.setZeroSmallerThan(aEps);
  Matrix U;
  U.setToTransposedOf(L); 

  Matrix U1inv;
  cmeasure_t lM_U1inv;
  cmeasure_start(&lM_U1inv);
  for(uint i = 0; i < lNoIter; ++i) {
    U1inv.setToInverseOfUpperTriangular(U, aEps);
  }
  cmeasure_stop(&lM_U1inv);

  Matrix U2inv;
  cmeasure_t lM_U2inv;
  cmeasure_start(&lM_U2inv);
  for(uint i = 0; i < lNoIter; ++i) {
    U2inv.setToTransposedOfInverseOfLowerTriangular(L);
  }
  cmeasure_stop(&lM_U2inv);

  Matrix L1inv;
  cmeasure_t lM_L1inv;
  cmeasure_start(&lM_L1inv);
  for(uint i = 0; i < lNoIter; ++i) {
    L1inv.setToTransposedOfInverseOfUpperTriangular(U, aEps);
  }
  cmeasure_stop(&lM_L1inv);

  std::cout << std::setw(2) << n << ' ' 
            << std::setw(2) << aNoBfKnown << ' '
            << std::setw(16) << cmeasure_div_ms(&lM_U1inv, lNoIter) << ' '
            << std::setw(16) << cmeasure_div_ms(&lM_U2inv, lNoIter) << ' '
            << std::setw(16) << cmeasure_div_ms(&lM_L1inv, lNoIter)
            << std::endl;

  const bool lWithChecks = true;
  if(lWithChecks) {
    assert(is_inverse_of(U, U1inv, aEps));
    assert(U2inv.equalUpTo(U1inv, aEps));
    assert(L1inv.isTransposedOf(U1inv, aEps));
  }
}

void
measure_inverse_sym_posdef(const Matrix& A, const uint n, const uint aNoBfKnown, const double aEps) {
  const uint lNoIter = 1000000 / (n * aNoBfKnown * aNoBfKnown * (1 << n)); 
 
  // std::cout << "#iter = " << lNoIter << std::endl;
  assert(0 < lNoIter);

  Matrix A1inv;
  cmeasure_t lM_A1inv;
  cmeasure_start(&lM_A1inv);
  for(uint i = 0; i < lNoIter; ++i) {
    inverse_trad(A1inv, A, aEps);
  }
  cmeasure_stop(&lM_A1inv);

  Matrix A2inv;
  cmeasure_t lM_A2inv;
  cmeasure_start(&lM_A2inv);
  for(uint i = 0; i < lNoIter; ++i) {
    inverse_XF(A2inv, A, aEps);
  }
  cmeasure_stop(&lM_A2inv);
 
  std::cout << std::setw(2) << n << ' '
            << std::setw(2) << aNoBfKnown << ' '
            << std::setw(16) << cmeasure_div_ms(&lM_A1inv, lNoIter) << ' '
            << std::setw(16) << cmeasure_div_ms(&lM_A2inv, lNoIter) << ' '
            << std::setw(16) << (cmeasure_div_ms(&lM_A1inv, lNoIter) / cmeasure_div_ms(&lM_A2inv, lNoIter))
            << std::endl;

  const bool lWithChecks = true;
  if(lWithChecks) {
    assert(is_inverse_of(A, A1inv, aEps * 1000));
    assert(is_inverse_of(A, A2inv, aEps * 1000));
    assert(A2inv.equalUpTo(A1inv, aEps * 1000));
  }
}

typedef void (*mfun_t)(const Matrix&, const uint, const uint, const double);

void
run_measurement(mfun_t aMfun) {
  const double lEps = 1E-13;
  uint_vt lKsPat;
  Matrix   A;
  for(uint lNoBf = 2; lNoBf <= 10; ++lNoBf) {
    for(uint lNoBfKnown = 1; lNoBfKnown < lNoBf; ++lNoBfKnown) {
      get_ks_pat(lKsPat, lNoBf, lNoBfKnown);
      get_DDt(A, lKsPat, lNoBf);
      (*aMfun)(A, lNoBf, lNoBfKnown, lEps);
    }
  }
}

void
run_all_measurements(uint lMToDo) {

  std::cout.setf(std::ios::fixed);
  std::cout.precision(6);

  if(0x1 & lMToDo) {
    std::cout << "=== run measurements 1 ===" << std::endl;
    std::cout << " n" << ' ' << "ks" << " | "
              << std::setw(16) << "U chol decomp" << ' '
              << std::setw(16) << "L chol decomp" << ' '
              << std::setw(16) << "L chol decomp XF" << " | "
              // << std::setw(13) << "ratio" << ' '
              // << std::setw(13) << "ratio" << ' '
              // << std::setw(13) << "ratio"
              << std::endl;
    run_measurement(&measure_cholesky_decomp);
  }

  if(0x2 & lMToDo) {
    std::cout << "=== run measurements 2 ===" << std::endl;
    std::cout << " n" << ' ' << "ks" << " | "
              << std::setw(16) << "U inv -> U" << ' '
              << std::setw(16) << "L inv -> U" << ' '
              << std::setw(16) << "U inv -> L" << " | "
              // << std::setw(13) << "ratio" << ' '
              // << std::setw(13) << "ratio" << ' '
              // << std::setw(13) << "ratio"
              << std::endl;
    run_measurement(&measure_inverse_triangular);
  }

  if(0x4 & lMToDo) {
    std::cout << "=== run measurements 3 ===" << std::endl;
    std::cout << " n" << ' ' << "ks" << " | "
              << std::setw(16) << "inverse_trad" << ' '
              << std::setw(16) << "inverse_XF" << ' '
              << std::setw(16) << "ratio"
              << std::endl;

    run_measurement(&measure_inverse_sym_posdef);
  }
}

int
main() {
  // check_all_bf();
  run_all_measurements(0x7);
  return 0;
}

