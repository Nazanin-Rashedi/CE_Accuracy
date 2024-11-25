#include "gmsvd.hh"
#include "../../src3/nr/nr3.h"
#include "../../src3/nr/svd.h"


GMSVD::GMSVD(const Matrix& A, double aEpsilon) : _epsilon(aEpsilon), _svd(0) {
   MatDoub lA(A.noRows(), A.noCols());
   // std::cout << "N: " << A.noRows()
   //           << ", M: " << A.noCols()
   //           << std::endl;
   for(uint i = 0; i < A.noRows(); ++i) {
     for(uint j = 0; j < A.noCols(); ++j) {
       lA[i][j] = A(i,j);
     }
   }
  _svd = new SVD(lA);
}


GMSVD::~GMSVD() {
  delete _svd;
}

void 
GMSVD::solve(const Matrix& b, Matrix& x /* out */, double aThreshold) {
  VecDoub lB(b.noRows());
  VecDoub lX(x.noRows());
  // std::cout << "GMSVD::solve: N(b): " << b.noRows()
  //           << ", N(x): " << x.noRows()
  //           << std::endl;
  for(uint i = 0; i < b.noRows(); ++i) {
    lB[i] = b(i,0);
  }
  svd()->solve(lB, lX, aThreshold);
  for(uint i = 0; i < x.noRows(); ++i) {
    x(i,0) = lX[i];
  }
  // std::cout << "GMSVD::solve: rank: " << rank()
  //           << ", nullity: " << nullity()
  //           << std::endl;
  std::cout << "x: ";
  for(uint i = 0; i < x.noRows(); ++i) {
    std::cout << lX[i] << ", ";
  }
  std::cout << std::endl;
}

int 
GMSVD::rank(double aThreshold) {
  int lRes = 0;
  lRes = svd()->rank(aThreshold);
  return lRes;
}

int 
GMSVD::nullity(double aThreshold) {
  int lRes = 0;
  lRes = svd()->nullity(aThreshold);
  return lRes;
}

void
GMSVD::get_U(Matrix& U) const {
  const int m = svd()->m; 
  const int n = svd()->n;
  U.rescale(m,n);
  for(int i = 0; i < m; ++i)
  for(int j = 0; j < n; ++j) {
    U(i,j) = svd()->u[i][j];
  }
}

void
GMSVD::get_W(Matrix& W) const {
  const int n = svd()->n;
  W.rescale(n,1);
  for(int i = 0; i < n; ++i) {
    W(i,0) = svd()->w[i];
  }
}

double
GMSVD::norm() const {
  return svd()->w[0];
}

void
GMSVD::get_Winv(Matrix& W) const {
  const int n = svd()->n;
  W.rescale(n,1);
  for(int i = 0; i < n; ++i) {
    W(i,0) = (svd()->w[i] <= eps()) ? 0 : ((double) 1.0 / svd()->w[i]);
  }
}

void
GMSVD::get_V(Matrix& V) const {
  const int n = svd()->n;
  V.rescale(n,n);
  for(int i = 0; i < n; ++i) 
  for(int j = 0; j < n; ++j) {
    V(i,j) = svd()->v[i][j];
  }
}

void
GMSVD::get_mpi(Matrix& M) const {
  Matrix V;
  get_V(V);
  Matrix W;
  get_Winv(W);
  Matrix U;
  get_U(U);
  Matrix VW;
  VW.setToProductOfMatrixDiag(V, W);
  M.setToProductWithSecondTransposed(VW, U);
}


