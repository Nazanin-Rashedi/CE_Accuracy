#include "matrix.hh"
#include "vp_ops.hh"

Matrix::Matrix() : _noRows(0), _noCols(0), _m(0) {}

Matrix::Matrix(uint n, uint m) : _noRows(n), _noCols(m), _m(0) {
  _m = new double[n*m];
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = 0.0;
    }
  }
}

Matrix::Matrix(uint n, uint m, double a11 ...) : _noRows(n), _noCols(m), _m(0) {
  _m = new double[n*m];
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = 0.0;
    }
  }
  va_list ap;
  va_start(ap, a11);
  (*this)(0,0) = a11;
  // std::cout << (*this)(0,0) << ' ';
  for(uint j = 1; j < m; ++j) {
     (*this)(0,j) = va_arg(ap,double);
     // std::cout << (*this)(0,j) << ' ';
  }
  // std::cout << std::endl;
  for(uint i = 1; i < n; ++i) {
    for(uint j = 0; j < m; ++j) {
      (*this)(i,j) = va_arg(ap,double);
      // std::cout << (*this)(i,j) << ' ';
    }
  }
  // std::cout << std::endl;
  va_end(ap);
}

Matrix::Matrix(uint n, uint m, double* x) : _noRows(n), _noCols(m), _m (0) {
  _m = new double[n*m];
  memcpy(_m, x, sizeof(double) * n * m);
}

Matrix::Matrix(const Matrix& m) : _noRows(m.noRows()), _noCols(m.noCols()), _m(0) {
  _m = new double[noRows()*noCols()];
  assign(m);
}


Matrix::~Matrix() {
  if(0 != _m) {
    delete[] _m;
    _m = 0;
  }
}

Matrix&
Matrix::operator=(const Matrix& m) {
  rescale(m.noRows(), m.noCols());
  assign(m);
  return (*this);
}

void
Matrix::perpose() {
  const uint m = noRows();
  const uint n = noCols();
  // reverse rows
  for(uint i = 0; i < m/2; ++i) {
    for(uint j = 0; j < n; ++j) {
      std::swap<double>((*this)(i,j), (*this)(m - i - 1, j));
    }
  }
  // reverse cols
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n/2; ++j) {
      std::swap<double>((*this)(i,j), (*this)(i, n - j - 1));
    }
  }
}

void
Matrix::copyColumn(const uint aToColumn, const uint aFromColumn) {
  for(uint i = 0; i < noRows(); ++i) {
    (*this)(i, aToColumn) = (*this)(i, aFromColumn);
  }
}


void
Matrix::setToSumOf(const Matrix& m1, const Matrix& m2) {
  if(m1.noRows() != m2.noRows() || m1.noCols() != m2.noCols()) {
    error("incompatible dimensions (A)");
  }
  rescale(m1.noRows(), m1.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m1(i,j) + m2(i,j);
    }
  }
}

void
Matrix::setToSumWithSecondTransposed(const Matrix& m1, const Matrix& m2) {
  assert(m1.noRows() == m2.noCols());
  assert(m1.noCols() == m2.noRows());
  uint m = m1.noRows();
  uint n = m1.noCols();
  resize(m,n);

  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      (*this)(i,j) = m1(i,j) + m2(j,i);
    }
  }
}


void
Matrix::setToDifferenceOf(const Matrix& m1, const Matrix& m2) {
  if(m1.noRows() != m2.noRows() || m1.noCols() != m2.noCols()) {
    error("incompatible dimensions (B)");
  }
  rescale(m1.noRows(), m1.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m1(i,j) - m2(i,j);
    }
  }
}

void
Matrix::setToTransposedOf(const Matrix& M) {
  resize(M.noCols(), M.noRows());
  for(uint i = 0; i < M.noRows(); ++i) {
    for(uint j = 0; j < M.noCols(); ++j) {
      (*this)(j,i) = M(i,j);
    }
  }
}

void
Matrix::setToPerposedOf(const Matrix& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  resize(m, n);
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      (*this)(i,j) = M(m - 1 - i, n - j - 1);
    }
  }
}

void
Matrix::setToProductOf(const Matrix& m1, const Matrix& m2) {
  if(m1.noCols() != m2.noRows()) {
    error("incompatible dimensions (C)");
  }
  rescale(m1.noRows(), m2.noCols());
  for(uint i = 0; i < m1.noRows(); ++i) {
    for(uint j = 0; j < m2.noCols(); ++j) {
      double x = 0.0;
      for(uint k = 0; k < m1.noCols(); ++k) {
        x += m1(i,k) * m2(k,j);
      }
      (*this)(i,j) = x;
    }
  }
}

void
Matrix::setToProductOfXF(const Matrix& m1, const Matrix& m2) {
  if(m1.noCols() != m2.noRows()) {
    error("incompatible dimensions (D)");
  }
  rescale_no_init(m1.noRows(), m2.noCols());
  setNullXF();
  double* lPtrRes = _m;
  double* lPtrM1  = m1._m;
  double* lPtrM2  = m2._m;
  for(uint i = 0; i < m1.noRows(); ++i) {
    for(uint k = 0; k < m1.noCols(); ++k) {
      const double M1_i_k = m1(i,k);
      lPtrRes = &(_m[i*noCols()]);
      lPtrM2  = &(m2._m[k*m2.noCols()]);
      for(uint j = 0; j < m2.noCols(); ++j) {
        // (*this)(i,j) += m1(i,k) * m2(k,j);
        // (*lPtrRes) += m1(i,k) * m2(k,j);
        // (*lPtrRes) += M1_i_k * m2(k,j);
        (*lPtrRes) += M1_i_k  * (*lPtrM2);
        ++lPtrRes;
        ++lPtrM2;
      }
      ++lPtrM1;
    }
  }
}

void
Matrix::setToProductWithDiagAsVector(const Matrix& A, const double* v) {
  rescale(A.noRows(), A.noCols());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < A.noCols(); ++j) {
      (*this)(i,j) = A(i,j) * v[j];
    }
  }
}

void
Matrix::setToProductWithDiagAsVector(const double* v, const Matrix& A) {
  rescale(A.noRows(), A.noCols());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < A.noCols(); ++j) {
      (*this)(i,j) = v[i] * A(i,j);
    }
  }
}

void
Matrix::setToAdiagvAt(const Matrix& M, const double* v) {
  rescale(M.noRows(), M.noRows());
  for(uint i = 0; i < M.noRows(); ++i) {
    for(uint j = i; j < M.noRows(); ++j) {
      double lSum = 0;
      for(uint k = 0; k < M.noCols(); ++k) {
        lSum += M(i,k) * v[k] * M(j,k);
      }
      (*this)(i,j) = (*this)(j,i) = lSum;
    }
  }
}

void
Matrix::setToProductOfMatrixDiag(const Matrix& A, const Matrix& D) {
  rescale(A.noRows(), A.noCols());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < A.noCols(); ++j) {
      (*this)(i,j) = A(i,j) * D(j,0);
    }
  }
}

void
Matrix::setToProductOfDiagMatrix(const Matrix& D, const Matrix& A) {
  rescale(A.noRows(), A.noCols());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < A.noCols(); ++j) {
      (*this)(i,j) = D(j,0) * A(i,j);
    }
  }
}




void
Matrix::setToProductWithFirstTransposed(const Matrix& m1, const Matrix& m2) {
  if(m1.noRows() != m2.noRows()) {
    error("incompatible dimensions (1)");
  }
  rescale(m1.noCols(), m2.noCols());
  for(uint i = 0; i < m1.noCols(); ++i) {
    for(uint j = 0; j < m2.noCols(); ++j) {
      double x = 0.0;
      for(uint k = 0; k < m1.noRows(); ++k) {
        x += m1(k,i) * m2(k,j);
      }
      (*this)(i,j) = x;
    }
  }
}

void
Matrix::setToProductWithSecondTransposed(const Matrix& m1, const Matrix& m2) {
  if(m1.noCols() != m2.noCols()) {
    error("incompatible dimensions (2)");
  }
  rescale(m1.noRows(), m2.noRows());
  for(uint i = 0; i < m1.noRows(); ++i) {
    for(uint j = 0; j < m2.noRows(); ++j) {
      double x = 0.0;
      for(uint k = 0; k < m1.noCols(); ++k) {
        x += m1(i,k) * m2(j,k);
      }
      (*this)(i,j) = x;
    }
  }
}

void
Matrix::setToUUt(const Matrix& U) {
  const uint n = U.noRows();
  assert(n == U.noCols());

  (*this).rescale_no_init(n,n);
  // assert(n == (*this).noRows());
  // assert(n == (*this).noCols());

  double lSum = 0;
  const double* lRow_j = 0;
  const double* lRow_i = U.data();
  for(uint i = 0; i < n; ++i) {
    lRow_j = U.data();
    for(uint j = 0; j <= i; ++j) {
      // lSum = 0;
      // for(uint k = i; k < n; ++k) {
      //   lSum += lRow_i[k] * lRow_j[k];
      // }
      lSum = vp_dotprod8(lRow_i + i, lRow_j + i, n - i);
      (*this)(i,j) = lSum;
      if(i != j) {
        (*this)(j,i) = lSum;
      }
      lRow_j += n;
    }
    lRow_i += n;
  }
}

void
Matrix::setToShrink2Of(const Matrix& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  (*this).resize(m / 2, n / 2);
  for(uint i = 0; i < m; i += 2) {
    for(uint j = 0; j < n; j += 2) {
      (*this)(i >> 1, j >> 1) = M(i,j) + M(i,j+1) + M(i+1,j) + M(i+1,j+1);
    }
  }
}

void
Matrix::setToReverseRows(const Matrix& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  resize(m, n);
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      (*this)(i,j) = M(m - i - 1, j);
    }
  }
}

void
Matrix::setToReverseCols(const Matrix& M) {
  const uint m = M.noRows();
  const uint n = M.noCols();
  resize(m, n);
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      (*this)(i,j) = M(i, n - j - 1);
    }
  }
}


Matrix&
Matrix::setToKroneckerProductOf(const Matrix& A, const Matrix& B) {
  const uint m = A.noRows() * B.noRows();
  const uint n = A.noCols() * B.noCols();
  (*this).resize(m,n);
  for(uint i1 = 0; i1 < A.noRows(); ++i1) {
    for(uint j1 = 0; j1 < A.noCols(); ++j1) {
      const double Aij = A(i1,j1);
      if(0 == Aij) { continue; }
      const uint lRow = i1 * B.noRows();
      const uint lCol = j1 * B.noCols();
      for(uint i2 = 0; i2 < B.noRows(); ++i2) {
        for(uint j2 = 0; j2 < B.noCols(); ++j2) {
          (*this)(lRow + i2, lCol + j2) = Aij * B(i2,j2);
        }
      }
    }
  }

  return (*this); 
}

Matrix&
Matrix::setToDiag(const double x, const uint n) {
  rescale(n,n);
  for(uint i = 0; i < n; ++i) {
     (*this)(i,i) = x;
  }
  return (*this);
}

Matrix&
Matrix::setToDiag(const double* x, const uint n) {
  rescale(n,n);
  for(uint i = 0; i < n; ++i) {
     (*this)(i,i) = x[i];
  }
  return (*this);
}


Matrix&
Matrix::setToCounterDiag(const double x, const uint n) {
  rescale(n,n);
  uint j = n;
  for(uint i = 0; i < n; ++i) {
     (*this)(i,--j) = x;
  }
  return (*this);
}

Matrix&
Matrix::setToCounterDiag(const double* x, const uint n) {
  rescale(n,n);
  uint j = n;
  for(uint i = 0; i < n; ++i) {
     (*this)(i,--j) = x[i];
  }
  return (*this);
}



Matrix&
Matrix::setToAxB(const Matrix& A, const Matrix& B) {
  if(A.noCols() != B.noRows()) {
   error("incompatible dimensions (3)");
  }
  resize(A.noRows(), B.noCols());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < B.noCols(); ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < A.noCols(); ++k) {
        lSum += A(i,k) * B(k,j);
      }
      (*this)(i,j) = lSum;
    }
  }
  return (*this);
}

// (*this) = A^t * B
Matrix&
Matrix::setToAtxB(const Matrix& A, const Matrix& B) {
  if(A.noRows() != B.noRows()) {
    error("incompatible dimensions (4)");
  }
  rescale(A.noCols(), B.noCols());
  for(uint i = 0; i < A.noCols(); ++i) {
    for(uint j = 0; j < B.noCols(); ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < A.noRows(); ++k) {
        lSum += A(k,i) * B(k,j);
      }
      (*this)(i,j) = lSum;
    }
  }
  return (*this);
}

// (*this) = A   * B^t
Matrix&
Matrix::setToAxBt(const Matrix& A, const Matrix& B) {
  if(A.noCols() != B.noCols()) {
    error("incompatible dimensions (5)");
  }
  rescale(A.noRows(), B.noRows());
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < B.noRows(); ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < A.noCols(); ++k) {
        lSum += A(i,k) * B(j,k);
      }
      (*this)(i,j) = lSum;
    }
  }
  return (*this);
}

// (*this) = A^t * B^t
// A MxN A^t NxM
// B OxM B^t MxO
// result: NxO
Matrix&
Matrix::setToAtxBt(const Matrix& A, const Matrix& B) {
  if(A.noRows() != B.noCols()) {
    error("incompatible dimensions (6)");
  }
  rescale(A.noCols(), B.noRows());
  for(uint i = 0; i < A.noCols(); ++i) {
    for(uint j = 0; j < B.noRows(); ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < A.noRows(); ++k) {
        lSum += A(k,i) * B(j,k);
      }
      (*this)(i,j) = lSum;
    }
  }
  return (*this);
}



// (*this) = (A^t * B^t)^t
//
Matrix&
Matrix::setToTransposedOfAtxBt(const Matrix& A, const Matrix& B) {
  if(A.noRows() != B.noCols()) {
    error("incompatible dimensions (7)");
  }
  rescale(B.noRows(), A.noCols());
  for(uint i = 0; i < A.noCols(); ++i) {
    for(uint j = 0; j < B.noRows(); ++j) {
      double x = 0.0;
      for(uint k = 0; k < A.noRows(); ++k) {
        x += A(k,i) * B(j,k);
      }
      (*this)(j,i) = x;
    }
  }
  return (*this);
}


// given upper triangular matrix U
// calculate upper triangular matrix U^-1
// slow
void
Matrix::setToInverseOfUpperTriangular(const Matrix& U, const double eps) {
  rescale(U.noRows(), U.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    (*this)(i,i) = ((double) 1.0) / U(i,i);
  }
  double u_ii_inv = 0;
  double lSum = 0;
  for(int i = noRows() - 2; 0 <= i; --i) {
    u_ii_inv = ((double) 1.0) / U(i,i);
    for(int j = noCols() - 1; i < j; --j) {
      lSum = 0.0;
      for(int k = i+1; k <= j; ++k) {
        lSum += U(i,k) * (*this)(k,j);
      }
      (*this)(i,j) = (-lSum) * u_ii_inv;
    }
  }
  if(0.0 < eps) {
    for(uint i = 0; i < noRows(); ++i) {
      for(uint j = i; j < noCols(); ++j) {
        if(-eps < (*this)(i,j) && (*this)(i,j) < eps) {
          (*this)(i,j) = 0.0;
        }
      }
    }
  }
}


// given some upper triangular matrix U
// calculate lower triangular matrix L
// such that U^{-1} = L^t
// fast
void
Matrix::setToTransposedOfInverseOfUpperTriangular(const Matrix& U, const double eps) {
  rescale(U.noRows(), U.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    (*this)(i,i) = ((double) 1.0) / U(i,i);
  }
  double lSum = 0.0;
  for(int i = noRows() - 2; 0 <= i; --i) {
    const double u_ii_inv = ((double) 1.0) / U(i,i);
    for(int j = noCols() - 1; i < j; --j) {
      // alt 1
      // lSum = 0.0;
      // for(int k = i+1; k <= j; ++k) {
      //   lSum += U(i,k) * (*this)(j,k);
      // }
      // alt 2
      // lSum = vecDotProd(U.data() + i * U.noCols() + i + 1, 
      //                     data() + j * U.noCols() + i + 1,
      //                     j - i);
      // alt 3
      lSum = vp_dotprod8(U.data() + i * U.noCols() + i + 1, 
                           data() + j * U.noCols() + i + 1,
                           j - i);
      (*this)(j,i) = (-lSum) * u_ii_inv;
    }
  }
  if(0.0 < eps) {
    for(uint i = 0; i < noRows(); ++i) {
      for(uint j = i; j < noCols(); ++j) {
        if(eps >= std::fabs((*this)(i,j))) {
          (*this)(i,j) = 0.0;
        }
      }
    }
  }
}

// if matrix m is upper triangular,
// calculate it's g-inverse
void
Matrix::setToGInverseOfUpperTriangular(const Matrix& m, const double eps) {
  rescale(m.noRows(), m.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    if(eps < fabs(m(i,i))) {
      (*this)(i,i) = ((double) 1.0) / m(i,i);
    } else {
      (*this)(i,i) = 0;
    }
  }
  for(int i = noRows() - 2; 0 <= i; --i) {
    const double m_ii_inv = (eps < fabs(m(i,i))) ?  (((double) 1.0) / m(i,i)) : 0.0 ;
    for(int j = noCols() - 1; i < j; --j) {
      double lSum = 0.0;
      for(int k = i+1; k <= j; ++k) {
        lSum += m(i,k) * (*this)(k,j);
      }
      (*this)(i,j) = (-lSum) * m_ii_inv;
    }
  }
  if(0.0 < eps) {
    for(uint i = 0; i < noRows(); ++i) {
      for(uint j = i; j < noCols(); ++j) {
        if(-eps < (*this)(i,j) && (*this)(i,j) < eps) {
          (*this)(i,j) = 0.0;
        }
      }
    }
  }
}

// Cholesky decomposition of a matrix M
// construct upper triangular matrix U
// such that M = U'U
// requires M to be symmetric and positive definite
// see Harville p. 237ff
// slow
void
Matrix::setToSquareRootUpperTriangular(const Matrix& M) {
  assert(M.noRows() == M.noCols());
  const uint n = M.noRows();
  rescale(n, n);
  const double m00 = M(0,0);
  if(0.0 == m00) {
    for(uint j = 0; j < n; ++j) {
      (*this)(0,j) = 0.0;
    }
  } else {
    const double t00 = sqrt(m00);
    const double t00inv = ((double) 1.0) / t00;
    (*this)(0,0) = t00;
    for(uint j = 1; j < n; ++j) {
      (*this)(0,j) = M(0,j) * t00inv;
    }
  }
  for(uint i = 1; i < n - 1; ++i) {
    // t_ii
    {
      double lSum = 0.0;
      for(uint k = 0; k < i; ++k) {
        lSum += (*this)(k,i) * (*this)(k,i);
      }
      (*this)(i,i) = sqrt(M(i,i) - lSum);
    }
    // t_ij
    {
      const double t_ii = (*this)(i,i);
      if(0.0 == t_ii) {
        for(uint j = i + 1; j < n; ++j) {
          (*this)(i,j) = 0.0;
        }
      } else {
        const double t_ii_inv = ((double) 1.0) / t_ii;
        for(uint j = i + 1; j < n; ++j) {
          double lSum = 0.0;
          for(uint k = 0; k < i; ++k) {
            lSum += (*this)(k,i) * (*this)(k,j);
          }
          (*this)(i,j) = (M(i,j) - lSum) * t_ii_inv;
        }
      }
    }
  }
  // t(n-1,n-1) last:
  {
    double lSum = 0.0;
    const uint n_1 = (n - 1);
    for(uint k = 0; k < n_1; ++k) {
      lSum += (*this)(k, n_1) * (*this)(k, n_1);
    }
    (*this)(n_1, n_1) = sqrt(M(n_1, n_1) - lSum);
  }
}

// given some matrix A
// calculates lower triangular matrix L
// such that A = LL^t
// slow
void
Matrix::setToSquareRootLowerTriangular(const Matrix& m) {
  assert(m.noRows() == m.noCols());
  const int n = m.noRows(); // must be equal to m.noCols()
  rescale(m.noRows(), m.noCols());
  // copy m to this
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      (*this)(i,j) = m(i,j);
    }
  }
  // calc lower triangular
  for(int i = 0; i < n; i++) {
    for(int j = i; j < n; j++) {
      double lSum = (*this)(i,j);
      for(int k = i-1 ; k >= 0; k--) {
        lSum -= (*this)(i,k) * (*this)(j,k); 
      }
      if(i == j) {
        if(lSum <= 0.0) {
          // std::cerr.precision(12);
          // std::cerr << "Cholesky failed: " << lSum << std::endl;
          // std::cerr.precision(6);
          throw CholeskyFailed(i, lSum);
        }
        (*this)(i,i) = sqrt(lSum);
      } else {
        (*this)(j,i) = (lSum) / (*this)(i,i);
      }
    }
  }
  // set upper triangular to zero
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      (*this)(i,j) = 0.0;
    }
  }
}

// given some matrix A
// calculates lower triangular matrix L
// such that A = LL^t
// fast
void
Matrix::setToSquareRootLowerTriangular_XF(const Matrix& A, const double aEps) {
  assert(A.noRows() == A.noCols());
  const int n = A.noRows(); // must be equal to m.noCols()
  rescale_no_init(n,n);
  // copy m to this
  memcpy((void*) _m, A._m, n * n * sizeof(double));
  setToSquareRootLowerTriangular_XF(aEps);
}

void
Matrix::setToSquareRootLowerTriangular_XF(const double aEps) {
  assert(noRows() == noCols());
  const int n = noRows(); // must be equal to m.noCols()
  // calc lower triangular
  double lSum = 0;
  double a_ii = 0;
  int i,j;
  for(i = 0; i < n; i++) {
    j = i;
    lSum = 0;
    lSum = vp_dotprod8(data() + i * n, data() + j * n, i);
    lSum = (*this)(i,j) - lSum;
    if(aEps > std::fabs(lSum)) {  
      // std::cerr.precision(12);
      // std::cerr << "Cholesky failed: " << lSum << std::endl;
      // std::cerr.precision(6);
      throw CholeskyFailed(i, lSum);
    }
    (*this)(i,i) = a_ii = sqrt(lSum);
    a_ii = 1 / a_ii;
    for(j = i + 1; j < n; j++) {
      // double lSum = (*this)(i,j);
      lSum = 0;
      // for(int k = i-1 ; k >= 0; k--) {
      //   lSum += (*this)(i,k) * (*this)(j,k);  // row sum
      // }
      lSum = vp_dotprod8(data() + i * n, data() + j * n, i);
      lSum = (*this)(i,j) - lSum;
      // (*this)(j,i) = (lSum) / (*this)(i,i);
      (*this)(j,i) = (lSum) * a_ii;
    }
  }
  // set upper triangular to zero
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      (*this)(i,j) = 0.0;
    }
  }
}


// Cholesky decomposition of a matrix M
// construct upper triangular matrix L
// such that M = LL'
// requires M to be symmetric and positive definite
// see Harville p. 237ff


// let A be a symmetric, positive definite matrix.
// the following member performs:
// solve Ax = b
// for A = L'L, A is lower triangular matrix

void
Matrix::solveSquareRootLowerTriangular(double* x, const double* b) const {
  const int n = noRows();
  for(int i = 0; i < n; ++i) {
    const double m_ii_inv = (((double) 1.0) / (*this)(i,i));
    double lSum = b[i];
    for(int k = i - 1; k >= 0; --k) {
      lSum -= (*this)(i,k) * x[k];
    }
    x[i] = lSum * m_ii_inv;
  }
  for(int i = n-1; i >= 0; --i) {
    const double m_ii_inv = (((double) 1.0) / (*this)(i,i));
    double lSum = x[i];
    for(int k = i + 1; k < n; ++k) {
      lSum -= (*this)(k,i) * x[k];
    }
    x[i] = lSum * m_ii_inv;
  }
}


// some matrix A must have been cholesky decomposed
// into lower triangular matrix L with
// A = LL^t
// a call to inverseSquareRootLowerTriangular(L) will
// put the inverse of A into *this.
// thus it calculates A^{-1} given L with A = LL^t
void 
Matrix::inverseSquareRootLowerTriangular(const Matrix& L) {
  assert(L.noRows() == L.noCols());
  const int n = L.noRows();
  rescale(n, n);
  double sum = 0.0;
  for(int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      sum = ( (i==j) ? 1.0 : 0.0 );
      for(int k = (i - 1); k >= j; --k) {
        sum -= L(i, k) * (*this)(j, k);
      }
      (*this)(j, i) = sum / (L(i, i));
    }
  }
  for(int i = (n - 1); i >= 0; --i) {
    for(int j = 0; j <= i; ++j) {
      sum = ( (i < j) ? 0.0 : (*this)(j, i) );
      for(int k = i + 1; k < n; ++k) {
        sum -= L(k, i) * (*this)(j, k);
      }
      (*this)(i, j) = (*this)(j, i) = sum / (L(i, i));
    }
  }
}

// same as inverseSquareRootLowerTriangular
// but faster
// but not really fast due to second step
void
Matrix::inverseSquareRootLowerTriangular_XF(const Matrix& L) {
  assert(L.noRows() == L.noCols());
  const int n = L.noRows();
  rescale(n, n);
  double lSum = 0.0;
  for(int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      // lSum = 0; 
      // for(int k = (i - 1); k >= j; --k) {
      //   lSum += L(i, k) * (*this)(j, k);
      // }
      lSum = vp_dotprod8(L.data() + i * n + j, data() + j * n + j, i - j);
      lSum = ( (i==j) ? 1.0 : 0.0 ) - lSum;
      (*this)(j, i) = lSum / (L(i, i));
    }
  }
  for(int i = (n - 1); i >= 0; --i) {
    for(int j = 0; j <= i; ++j) {
      lSum = 0;   
      for(int k = i + 1; k < n; ++k) {
        lSum += L(k, i) * (*this)(j, k);
      }
      lSum = ( (i < j) ? 0.0 : (*this)(j, i) ) - lSum;
      (*this)(i, j) = (*this)(j, i) = lSum / (L(i, i));
    }
  }
}



// given lower triangular matrix L
// calculate upper triangular matrix U
// such that L^{-1} = U^t
void
Matrix::setToTransposedOfInverseOfLowerTriangular(const Matrix& L) {
  assert(L.noRows() == L.noCols());
  const int n = L.noRows();
  rescale(n, n);
  double lSum = 0.0;
  for(int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      // lSum = 0; 
      // for(int k = (i - 1); k >= j; --k) {
      //   lSum += A(i, k) * (*this)(j, k);
      // }
      lSum = vp_dotprod8(L.data() + i * n + j, data() + j * n + j, i - j);
      lSum = ( (i==j) ? 1.0 : 0.0 ) - lSum;
      (*this)(j, i) = lSum / (L(i, i));
    }
  }
}

// inverte symmetric positive definite matrix A using cholesky decomposition
void
Matrix::setToInverseOfPositiveDefinite(const Matrix& A, const double aEps) {
  Matrix L;
  L.setToSquareRootLowerTriangular_XF(A, aEps);
  L.setZeroSmallerThan(aEps);
  Matrix Uinv;
  Uinv.setToTransposedOfInverseOfLowerTriangular(L);
  setToUUt(Uinv);
}

bool
Matrix::getLDL(Matrix& L, Matrix& D) const {
  assert(noCols() == noRows());
  // assert(isSymmetric());
  L.resize(noRows(), noCols());
  D.resize(noRows(), noCols());

  const uint n = noRows();

  // set diagonal elements in L to 1
  for(uint i = 0; i < n; ++i) {
    L(i,i) = 1;
  }


  double lSum = 0.0;
  for(uint j = 0; j < n; ++j) {
    for(uint i = j; i < n; ++i) {
      // 1. calculate D_jj
      // 1.1 calculate lSum1:
      lSum = 0.0;
      for(uint k = 0; k < j; ++k) {
        lSum += (L(i,k) * L(j,k) * D(k,k));
      }
      // std::cout << "L " << i << ' ' << j
      //           << "   sum = " << lSum
      //           << std::endl;
      if(i == j) {
        D(j,j) = (*this)(j,j) - lSum;
      } else {
        const double lD1 = ((double) 1.0) / D(j,j);
        L(i,j) = lD1 * ((*this)(i,j) - lSum);
      }
    }
  }

  return true;
}

void
Matrix::setToMMt(const Matrix& M) {
  // M is m x n matrix
  const uint m = M.noRows();
  const uint n = M.noCols();
  rescale(m,m);
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < m; ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < n; ++k) {
        lSum += M(i,k) * M(j,k);
      }
      (*this)(i,j) = lSum;
    }
  }
}

void
Matrix::getAx(double* y, const double* x) const {
  const uint m = noRows();
  const uint n = noCols();
  double lSum = 0;
  for(uint i = 0; i < m; ++i) {
    lSum = 0;
    for(uint j = 0; j < n; ++j) {
      lSum += (*this)(i,j) * x[j];
    }
    y[i] = lSum;
  }
}

void
Matrix::getAx_XF(double* y, const double* x) const {
  const uint m = noRows();
  const uint n = noCols();
/*
  double lSum = 0;
  const double* lCurr = data();
  for(uint i = 0; i < m; ++i) {
    lSum = 0;
    for(uint j = 0; j < n; ++j) {
      lSum += (*lCurr++) * x[j];
    }
    y[i] = lSum;
  }
*/
  const double* lCurr = data();
  for(uint i = 0; i < m; ++i) {
    y[i] = vp_dotprod8(lCurr, x, n);
    lCurr += n;
  }
}

void
Matrix::getAtx(double* y, const double* x) const {
  const uint m = noRows();
  const uint n = noCols();
  double lSum = 0;
  for(uint j = 0; j < n; ++j) {
    lSum = 0;
    for(uint i = 0; i < m; ++i) {
      lSum += (*this)(i,j) * x[i];
    }
    y[j] = lSum;
  }
}


void
Matrix::setToMtM(const Matrix& M) {
  // M is m x n matrix
  const uint m = M.noRows();
  const uint n = M.noCols();
  rescale(n,n);
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      double lSum = 0.0;
      for(uint k = 0; k < m; ++k) {
        lSum += M(k,i) * M(k,j);
      }
      (*this)(i,j) = lSum;
    }
  }
}


void
Matrix::solveUpperTriangular(double* x, const double* b) const {
  assert(noCols() == noRows());
  const uint n = noCols();
  double lSum = 0;
  int i = n - 1;
  x[i] = b[i] / (*this)(i,i);
  --i;
  for(; 0 <= i; --i) {
     lSum = 0;
     for(uint j = i + 1; j < n; ++j) {
        lSum += (*this)(i,j) * x[j];
     }
     x[i] = (b[i] - lSum) / (*this)(i,i);
     lSum += x[i];
  }
}

void
Matrix::solveLowerTriangular(double* x, const double* b) const {
  assert(noCols() == noRows());
  const uint n = noCols();
  double lSum = 0;
  x[0] = b[0] / (*this)(0,0);
  for(uint i = 1; i < n; ++i) {
     lSum = 0;
     for(uint j = 0; j < i; ++j) {
       lSum += (*this)(i,j) * x[j];
     }
     x[i] = (b[i] - lSum) / (*this)(i,i);
  }
}

void
Matrix::solveUpperTriangularT(double* x, const double* b) const {
  assert(noCols() == noRows());
  const uint n = noCols();
  double lSum = 0;
  x[0] = b[0] / (*this)(0,0);
  for(uint i = 1; i < n; ++i) {
     lSum = 0;
     for(uint j = 0; j < i; ++j) {
       lSum += (*this)(j,i) * x[j];
     }
     x[i] = (b[i] - lSum) / (*this)(i,i);
  }
}


void
Matrix::solveLowerTriangularT(double* x, const double* b) const {
  assert(noCols() == noRows());
  const uint n = noCols();
  double lSum = 0;
  int i = n - 1;
  x[i] = b[i] / (*this)(i,i);
  --i;
  for(; 0 <= i; --i) {
     lSum = 0;
     for(uint j = i + 1; j < n; ++j) {
        lSum += (*this)(j,i) * x[j];
     }
     x[i] = (b[i] - lSum) / (*this)(i,i);
     lSum += x[i];
  }
}



void
Matrix::setAllTo(double x) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    (*this)(i,j) = x;
  }
}

void
Matrix::setAllTo(double x, uint aRowStart, uint aRowEnd, uint aColStart, uint aColEnd) {
for(uint i = aRowStart; i < aRowEnd; ++i)
  for(uint j = aColStart; j < aColEnd; ++j) {
    (*this)(i,j) = x;
  }
}

void
Matrix::setZeroSmallerThan(const double eps) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    const double x = (*this)(i,j);
    if(-eps < x && x < eps) {
      (*this)(i,j) = 0.0;
    }
  }
}

void
Matrix::setZeroLessThan(const double aNumber) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    const double x = (*this)(i,j);
    if(x < aNumber) {
      (*this)(i,j) = 0.0;
    }
  }
}

void
Matrix::lnAll() {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    (*this)(i,j) = log((*this)(i,j));
  }
}

void
Matrix::addToAll(const double x) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    (*this)(i,j) += x;
  }
}

void
Matrix::multiplyAllBy(const double x) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    (*this)(i,j) *= x;
  }
}


void
Matrix::divideAllBy(const double x) {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
    (*this)(i,j) /= x;
  }
}

void
Matrix::divideAllBy(double x, const uint aRowStart, const uint aRowEnd, const uint aColStart, const uint aColEnd) {
for(uint i = aRowStart; i < aRowEnd; ++i)
  for(uint j = aColStart; j < aColEnd; ++j) {
    (*this)(i,j) /= x;
  }
}

void
Matrix::logAll() {
  for(uint i = 0; i < noRows(); ++i)
  for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = log((*this)(i,j));
  }
}

void
Matrix::rowNormalize() {
  for(uint i = 0; i < noRows(); ++i) {
    double x = 0.0;
    for(uint j = 0; j < noCols(); ++j) {
      x += (*this)(i,j);
    }
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) /= x;
    }
  }
}

void
Matrix::colNormalize() {
  for(uint j = 0; j < noCols(); ++j) {
    double x = 0.0;
    for(uint i = 0; i < noRows(); ++i) {
      x += (*this)(i,j);
    }
    if(0.0 != x) {
      for(uint i = 0; i < noRows(); ++i) {
        (*this)(i,j) /= x;
      }
    }
  }
}

void
Matrix::setToRowSubmatrix(const Matrix& m, const uint* aRows, uint aNoRows) {
  rescale(aNoRows, m.noCols());
  for(uint i = 0; i < aNoRows; ++i) {
    for(uint j = 0; j < m.noCols(); ++j) {
      (*this)(i,j) = m(aRows[i],j);
    }
  }
}

void
Matrix::setToSubmatrix(const Matrix& M, const uint aNoRows, const uint aNoCols) {
  assert(aNoRows <= M.noRows());
  assert(aNoCols <= M.noCols());
  resize(aNoRows, aNoCols);
  for(uint i = 0; i < aNoRows; ++i) {
    for(uint j = 0; j < aNoCols; ++j) {
      (*this)(i,j) = M(i,j);
    }
  }
}


void
Matrix::setToElementWiseProduct(const Matrix& m1, const Matrix& m2) {
  if(m1.noRows() != m2.noRows() || m1.noCols() != m2.noCols()) {
    error("incompatible dimensions (8)");
  }
  rescale(m1.noRows(), m1.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m1(i,j) * m2(i,j);
    }
  }
}

void
Matrix::setToElementWiseQuotient(const Matrix& m1, const Matrix& m2) {
  if(m1.noRows() != m2.noRows() || m1.noCols() != m2.noCols()) {
    error("incompatible dimensions (9)");
  }
  rescale(m1.noRows(), m1.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m1(i,j) / m2(i,j);
    }
  }
}

int
Matrix::countZeroElements() const {
  int lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(0 == (*this)(i,j)) {
        ++lRes;
      }
    }
  }
  return lRes;
}

int
Matrix::countNonZeroElements() const {
  return ((noRows() * noCols()) - countZeroElements());
}

int
Matrix::countNegElementsInRow(const int aRowNo, const double aEps) const {
  int lRes = 0;
  for(uint j = 0; j < noCols(); ++j) {
    if((*this)(aRowNo, j) <= -aEps) {
      ++lRes;
    }
  }
  return lRes;
}

int
Matrix::countPosElementsInRow(const int aRowNo, const double aEps) const {
  int lRes = 0;
  for(uint j = 0; j < noCols(); ++j) {
    if((*this)(aRowNo, j) >= aEps) {
      ++lRes;
    }
  }
  return lRes;
}




double
Matrix::sumAllElements() const {
  double lRes = 0.0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      lRes += (*this)(i,j);
    }
  }
  return lRes;
}

double
Matrix::sumAllElements(uint aRowStart, uint aRowEnd, uint aColStart, uint aColEnd) const {
  double lRes = 0.0;
  for(uint i = aRowStart; i < aRowEnd; ++i) {
    for(uint j = aColStart; j < aColEnd; ++j) {
      lRes += (*this)(i,j);
    }
  }
  return lRes;
}

double
Matrix::minDiag() const {
  const uint lLim = std::min<uint>(noRows(), noCols());
  double lMin = std::numeric_limits<double>::max();
  for(uint i = 0; i < lLim; ++i) {
    const double x = (*this)(i,i);
    if(x < lMin) {
      lMin = x;
    }
  }
  return lMin;
}

double
Matrix::min() const {
  double lRes = (*this)(0,0);
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = (*this)(i,j);
      if(x < lRes) {
        lRes = x;
      }
    }
  }
  return lRes;
}

double
Matrix::max() const {
  double lRes = (*this)(0,0);
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = (*this)(i,j);
      if(x > lRes) {
        lRes = x;
      }
    }
  }
  return lRes;
}

double
Matrix::absMin() const {
  double lRes = fabs((*this)(0,0));
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = fabs((*this)(i,j));
      if(x < lRes) {
        lRes = x;
      }
    }
  }
  return lRes;
}

double
Matrix::absMax() const {
  double lRes = fabs((*this)(0,0));
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = fabs((*this)(i,j));
      if(x > lRes) {
        lRes = x;
      }
    }
  }
  return lRes;
}

double
Matrix::absMinNonZero(const double aEps) const {
  double lRes = std::numeric_limits<double>::max();
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = fabs((*this)(i,j));
      if((aEps < x) && (x < lRes)) {
        lRes = x;
      }
    }
  }
  return lRes;
}



double
Matrix::prodDiag() const {
  const uint lLim = std::min<uint>(noRows(), noCols());
  double lProd  = 1.0;
  for(uint i = 0; i < lLim; ++i) {
    lProd *= (*this)(i,i);
  }
  return lProd;
}

double
Matrix::minUpperTriag() const {
  double lMin = std::numeric_limits<double>::max();
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i + 1; j < noCols(); ++j) {
      const double x = (*this)(i,j);
      if(x < lMin) {
        lMin = x;
      }
    }
  }
  return lMin;
}

double
Matrix::maxQDeviationFromSymmetry() const {
  assert(noRows() == noCols());
  double lMax = 0.0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < noCols(); ++j) {
      const double lQError = q::qerror<double>((*this)(i,j), (*this)(j,i));
      if(lQError > lMax) {
        lMax = lQError;
      }
    }
  }
  return lMax;
}

double
Matrix::prodQDeviationFromSymmetry() const {
  assert(noRows() == noCols());
  double lProd = 1.0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < noCols(); ++j) {
      const double lQError = q::qerror<double>((*this)(i,j), (*this)(j,i));
      lProd *= lQError;
    }
  }
  return lProd;
}

double
Matrix::prodUpperDivProdLower() const {
  assert(noRows() == noCols());
  double lUpper = 1.0;
  double lLower = 1.0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < noCols(); ++j) {
      lUpper *= (*this)(i,j);
      lLower *= (*this)(j,i);
    }
  }
  return q::qerror<double>(lUpper, lLower);
}

double
Matrix::getTrace() const {
  assert(noRows() == noCols());
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    lRes += (*this)(i,i);
  }
  return lRes;
}


double
Matrix::norm_1() const {
  double lRes = 0;
  for(uint j = 0; j < noCols(); ++j) {
    double lSum = 0;
    for(uint i = 0; i < noRows(); ++i) {
       lSum += std::fabs((*this)(i,j));
    }
    if(lRes < lSum) {
      lRes = lSum;
    }
  }

  return lRes;
}

double
Matrix::norm_infty() const {
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    double lSum = 0;
    for(uint j = 0; j < noCols(); ++j) {
       lSum += std::fabs((*this)(i,j));
    }
    if(lRes < lSum) {
      lRes = lSum;
    }
  }
  return lRes;
}

double
Matrix::norm_frobenius() const {
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      lRes += (*this)(i,j) * (*this)(i,j);
    }
  }
  return std::sqrt(lRes);
}


// Harville p. 59
// ||M|| = (A \cdot B)^{1/2}
// this is the same as norm_frobenius
double
Matrix::norm_sqrt_dot() const {
  double lSum = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noRows(); ++j) {
      const double x = (*this)(i,j);
      lSum += x*x;
    }
  } 
  return std::sqrt(lSum);
  // return std::sqrt(innerMatrixProduct(*this));
}

/*
  A \in \R^{m,n}. then:
                  ||A||_2  \leq  ||A||_F  \leq  \sqrt{n} ||A||_2
  max_{i,j} |A(i,j)|       \leq  ||A||_2  \leq  \sqrt{nm} \max_{i,j} |A(i,j)|
  1/\sqrt{n} ||A||_\infty  \leq  ||A||_2  \leq  \sqrt{m}  ||A||_\infty
  1/\sqrt{m} ||A||_1       \leq  ||A||_2  \leq  \sqrt{n}  ||A||_\1     
                     0     \leq  ||A||_2  \leq  \sqrt{||A||_1 * ||A||_\infty}
*/

// inner (dot) product of two matrices
// A \in R^{m,n}, B \in R^{m,n} then
// A \dot B := tr(A'B) = tr(AB')
// Harville p50:
// A \in R^{m,n}, B \in R^{n,m} then
// tr(AB) = \sum_{i=1}^{m} \sum_{j=1}^{n} a_{i,j} b_{j,i}
// Harville p58
// A \dot B = tr(A'B) = tr(AB')
// Harville p 50
// tr(AB) = tr(B'A')
// tr(AB) = tr(BA), A \in \R^{m,n), B \in \R^{n,m}
double
Matrix::innerMatrixProduct(const Matrix& M) const {
  assert(noRows() == M.noRows());
  assert(noCols() == M.noCols());
  const double m = noRows();
  const double n = noCols();
  double lRes = 0;
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      lRes += (*this)(i,j) * M(i,j);
    }
  }
  return lRes;
}



void
Matrix::rescale(const uint n, const uint m) {
  if(0 == n || 0 == m) {
    std::cerr << "Matrix::rescale: n=0, m=0." << std::endl;
    assert(0 == 1);
  }
  if(0 == _m) {
    _m = new double[n*m];
  } else {
    if(noRows() != n || noCols() != m) {
      if(0 != _m) {
        delete[] _m;
      }
      _m = 0;
      _m = new double[n*m];
    }
  }
  assert(nullptr != _m);
  _noRows = n;
  _noCols = m;
  setNull();
}

void
Matrix::rescale_no_init(const uint n, const uint m) {
  if(0 == n || 0 == m) {
    std::cerr << "Matrix::rescale: n=0, m=0." << std::endl;
    assert(0 == 1);
  }
  if(0 == _m) {
    _m = new double[(uint64_t) n * (uint64_t) m];
  } else {
    if(noRows() != n || noCols() != m) {
      if(0 != _m) {
        delete[] _m;
      }
      _m = 0;
      _m = new double[(uint64_t) n * (uint64_t) m];
    }
  }
  _noRows = n;
  _noCols = m;
}

void
Matrix::shrinkNoInit(const uint n, const uint m) {
  assert((n <= noRows()) && (m <= noCols()));
  _noRows = n;
  _noCols = m;
}

void
Matrix::roundAll() {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = round((*this)(i,j));
    }
  }
}

void
Matrix::setNull() {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = 0.0;
    }
  }
}

void
Matrix::setNullXF() {
  memset((void*) _m, 0, sizeof(double) * noRows() * noCols());
}

bool
Matrix::isIdentity(const double aEps) const {
  assert(0.0 <= aEps);
  double a = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      a = (*this)(i,j);
      if(i == j) {
        if(((1.0 - aEps) > a) || ((1.0 + aEps) < a)) {
          return false;
        }
      } else {
        if((-aEps > a) || (aEps < a)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool
Matrix::isIdentityUpTo(const uint aN, const double aEps) const {
  assert(noRows() >= aN);
  assert(noCols() >= aN);
  double a = 0;
  for(uint i = 0; i < aN; ++i) {
    for(uint j = 0; j < aN; ++j) {
      if(i == j) {
        if(((1.0 - aEps) > a) || ((1.0 + aEps) < a)) {
          return false;
        }
      } else {
        if((-aEps > a) || (aEps < a)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool
Matrix::isSymmetric(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < i; ++j) {
      if(aEps < fabs((*this)(i,j) - (*this)(j,i))) {
        return false;
      }
    }
  }
  return true;
}

// isPersymmetric
bool
Matrix::isMinorSymmetric(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  const uint n = noRows();
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      if(aEps < fabs((*this)(i,j) - (*this)(n - 1 - j, n - 1 - i))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isTransposedOf(const Matrix& M, const double aEps) const {
  // assert(noRows() == M.noCols());
  // assert(noCols() == M.noRows());
  if(noRows() != M.noCols()) {
    return false;
  }
  if(noCols() != M.noRows()) {
    return false;
  }
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(aEps < fabs((*this)(i,j) - M(j,i))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isMinorTransposedOf(const Matrix& M, const double aEps) const {
  // assert(noRows() == M.noCols());
  // assert(noCols() == M.noRows());
  if(noRows() != M.noCols()) {
    return false;
  }
  if(noCols() != M.noRows()) {
    return false;
  }
  const uint m = noRows();
  const uint n = noCols();
  const uint m1 = m - 1;
  const uint n1 = n - 1;
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      if(aEps < fabs((*this)(i,j) - M(n1 - j, m1 - i))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::hasZeroOneDiagonal(const double aEps) const {
  const uint n = std::min<uint>(noRows(), noCols());
  double x = 0;
  double a = 0;
  for(uint i = 0; i < n; ++i) {
    x = (*this)(i,i);
    a = fabs(x);
    if(a <= aEps) {
      continue;
    }
    x -= 1.0;
    a = fabs(x);
    if(a <= aEps) {
     continue;
    }
    return false;
  }
  return true;
}

bool
Matrix::has_diag_1(const double aEps) const {
  const uint n = std::min<uint>(noRows(), noCols());
  for(uint i = 0; i < n; ++i) {
    if(aEps < fabs(1.0 - (*this)(i,i))) {
      return false;
    }
  }
  return true;
}

bool
Matrix::hasSameDiagonalAs(const Matrix& M, const double aEps) const {
  const uint n = std::min<uint>(noRows(), noCols());
  assert(n <= M.noRows());
  assert(n <= M.noCols());
  for(uint i = 0; i < n; ++i) {
    if(aEps < std::fabs((*this)(i,i) - M(i,i))) {
      return false;
    }
  }
  return true;
}

bool
Matrix::hasColSum1(const double aEps) const {
  double lSum = 0;
  for(uint j = 0; j < noCols(); ++j) {
    lSum = 0;
    for(uint i = 0; i < noRows(); ++i) {
      lSum += (*this)(i,j);
    }
    if(aEps < std::fabs(1.0L - lSum)) {
      return false;
    }
  }
  return true;
}

bool
Matrix::hasZeroBlock(const uint aRowStart, const uint aRowEnd,
                     const uint aColStart, const uint aColEnd,
                     const double aEps) const {
  for(uint i = aRowStart; i < aRowEnd; ++i) {
    for(uint j = aColStart; j < aColEnd; ++j) {
      if(!is_equal<double>(0, (*this)(i,j), aEps)) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::hasBlock(const uint aStartRow, const uint aStartCol, const Matrix& A, const double aEps) const {
  for(uint i = 0; i < A.noRows(); ++i) {
    for(uint j = 0; j < A.noCols(); ++j) {
      if(!is_equal<double>(A(i,j), (*this)(aStartRow + i, aStartCol + j), aEps)) {
        return false;
      }
    }
  }
  return true;
}

//  this =  |A 0|
//          |0 B|
bool
Matrix::isBlockMatrixHaupt(const Matrix& A, const Matrix& B,
                           const double aEps) const {
  assert(A.noRows() + B.noRows() == noRows());
  assert(A.noCols() + B.noCols() == noCols());
  const uint lRow = A.noRows();
  const uint lCol = A.noCols();
  return (hasBlock    (   0,      0,      A, aEps) &&
          hasBlock    (lRow,   lCol,      B, aEps) &&
          hasZeroBlock(   0,   lRow,   lCol, noCols(), aEps) &&
          hasZeroBlock(lRow, noRows(),    0, lCol,     aEps));
}

//  this =  |0 A|
//          |B 0|
bool
Matrix::isBlockMatrixNeben(const Matrix& A, const Matrix& B,
                           const double aEps) const {
  assert(A.noRows() + B.noRows() == noRows());
  assert(A.noCols() + B.noCols() == noCols());
  const uint lRow = A.noRows();
  const uint lCol = B.noCols();
  return (hasBlock    (   0,   lCol,      A, aEps) &&
          hasBlock    (lRow,      0,      B, aEps) &&
          hasZeroBlock(   0,      0,      0, lCol,     aEps) &&
          hasZeroBlock(lRow, noRows(), lCol, noCols(), aEps));
}

void
Matrix::getBlock(Matrix& A, 
                 const uint aRowBegin, const uint aRowEnd, 
                 const uint aColBegin, const uint aColEnd) const {
  const uint m = aRowEnd - aRowBegin;
  const uint n = aColEnd - aColBegin;
  A.rescale(m,n);
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      A(i,j) = (*this)(aRowBegin + i, aColBegin + j);
    }
  }
}




void
Matrix::vecSum(double* x, const double* a, const double* b, const uint aSize) {
  for(uint i = 0; i < aSize; ++i) {
    x[i] = a[i] + b[i];
  }
}

double
Matrix::vecDotProd(const double* v1, const double* v2, const uint n) {
  double lRes = 0.0;
  for(uint i = 0; i < n; ++i) {
    lRes += v1[i] * v2[i];
  }
  return lRes;
}

double
Matrix::colColDotProd(const uint c1, const uint c2) const {
  double lRes = 0.0;
  for(uint i = 0; i < noRows(); ++i) {
    lRes += (*this)(i, c1) * (*this)(i, c2);
  }
  return lRes;
}

double
Matrix::rowRowDotProd(const uint r1, const uint r2) const {
  double lRes = 0.0;
  for(uint i = 0; i < noCols(); ++i) {
    lRes += (*this)(r1, i) * (*this)(r2, i);
  }
  return lRes;
}

double
Matrix::colRowDotProd(const uint aRow, const uint aCol) const {
  double lRes = 0.0;
  for(uint i = 0; i < noCols(); ++i) {
    lRes += (*this)(aRow, i) * (*this)(i, aCol);
  }
  return lRes;
}

double
Matrix::colVecDotProd(const double* v, const uint aColNo) const {
  double lRes = 0.0;
  for(uint i = 0; i < noRows(); ++i) {
    lRes += v[i] * (*this)(i,aColNo);
  }
  return lRes;
}

double
Matrix::rowProdVec(const uint aRowNo, const double* v) const {
  double lRes = 0.0;
  for(uint j = 0; j < noCols(); ++j) {
    lRes += (*this)(aRowNo,j) * v[j];
  }
  return lRes;
}

double 
Matrix::selfTransposedProdSelf(uint i, uint j, uint aStopXcl) const {
  double lRes = 0.0;
  for(uint k = 0; k < aStopXcl; ++k) {
    lRes += (*this)(k,i) * (*this)(k,j);
  }
  return lRes;
}

double
Matrix::AB(const Matrix& A, const Matrix& B, uint i, uint j, uint aStopXcl) {
  double lRes = 0.0;
  for(uint l = 0; l < aStopXcl; ++l) {
    lRes += A(i,l) * B(l,j);
  }
  return lRes;
}


double
Matrix::IminusAB(const Matrix& A, const Matrix& B, uint i, uint j, uint aStopXcl) {
  double lRes = 0.0;
  for(uint l = 0; l < aStopXcl; ++l) {
    lRes += A(i,l) * B(l,j);
  }
  return ((i == j) ? (1.0 - lRes) : (-lRes));
}



void
Matrix::vecMPI(double* lRes, const double* v, const uint n) {
  const double lDotProd = vecDotProd(v, v, n);
  if(0.0 == lDotProd) {
    for(uint i = 0; i < n; ++i) {
      lRes[i] = 0.0;
    }
  } else {
    const double x = 1.0 / lDotProd;
    for(uint i = 0; i < n; ++i) {
      lRes[i] = x * v[i];
    }
  }
}

void
Matrix::setToMoorePenroseInverseOf(const Matrix& A, const double aEps, const bool aTrace) {
  if(aTrace) {
    std::cout << "A: " << std::endl;
    A.print(std::cout, 3);
  }
  const uint m = A.noRows();
  const uint n = A.noCols();
  rescale(n,m); // A \in R^{m,n}, A^+ \in R^{n,m}
  Matrix& Ap = (*this); // Moore Penrose Inverse A^+ of A
  // Greville-Algorithm (see book by Schmidt, Trenkler)
  // row-wise calculation of A^+
  // A = [a_0,\ldots,a_n], a_j column vectors of A
  // A_k = [a_0,\ldots,a_k], partial matrix: first $k$ columns
  // k = 0:
  //   calculate first row of A^+_0(0,*) = mpi(a_0)
  const double lDotProd0 = A.colColDotProd(0,0);
  if(aEps > std::fabs(lDotProd0)) {
    for(uint j = 0; j < m; ++j) {
      Ap(0,j) = 0.0;
    }
  } else {
    const double x = 1.0 / lDotProd0;
    for(uint j = 0; j < m; ++j) {
      Ap(0,j) = x * A(j,0);
    }
  }

  if(aTrace) {
    std::cout << "k: 0: " << std::endl;
    std::cout << "A^+: " << std::endl;
    Ap.print(std::cout, 3);
  }

  // k > 0:
  // additional variables:
  // d_k' = a_k' A^+'_{k-1} A^+_{k-1} ; AptAp_k = A^+'_{k-1} A^+_{k-1}
  // c_k  = (I - A_{k-1} A^+{k-1}) a_j
  // b_k' = c_k^+ + ((1 - c_k^+ c_k) / (1 + d_k' a_k)) d_k'
  // A^+_k = | A^+_{k-1} - A^+{k-1} a_k b_k' |
  //         |           b_k'                |
  //
  // d_k':
  /*
     AptAp_k = A^+'_{k-1} A^+_{k-1} 
     AptAp_k(i,j) = sum_{l=1}^{k-1} a^+_{l,i}a^+{l,j} = sum_{l=1}^{k-1} a^+_{l,i}a^+{l,j} + a^+_{k,i}a^+{k,j}
     d_k'(j) = (a_k' A^+_{k-1}' A^+_{k-1})(j)
             = sum_{p=1}^{k-1} a_k(p) (A^+_{k-1}' A^+_{k-1})(p,j)
             = sum_{p=1}^{k-1} A(k,p) (A^+_{k-1}' A^+_{k-1})(p,j)
  */
  double d[m];
  double c[m];
  double cp[m];
  double b[m];
  // b_k stored in Ap;
  for(uint k = 1; k < n; ++k) {
    if(aTrace) {
      std::cout << "-----------------" << std::endl;
      std::cout << "k: " << k << std::endl;

      std::cout << "A^+'A^+: " << std::endl;
      for(uint i = 0; i < m; ++i) {
        for(uint j = 0; j < m; ++j) {
          std::cout << Ap.selfTransposedProdSelf(i,j,k) << ' ';
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }

    // d_k
    for(uint j = 0; j < m; ++j) {
      d[j] = 0.0;
      for(uint i = 0; i < m; ++i) {
        d[j] += A(i,k) * Ap.selfTransposedProdSelf(i,j,k);
      }
    }

    if(aTrace) {
      std::cout << "d: ";
      printVector(d, m);
      std::cout << std::endl << std::endl;
    }

    for(uint i = 0; i < m; ++i) {
      c[i] = 0.0;
      for(uint j = 0; j < m; ++j) {
        c[i] += IminusAB(A, Ap, i, j, k) * A(j,k);
        if(aEps > std::fabs(c[i])) { // -aEpsilon < c[i] && c[i] < aEpsilon 
          c[i] = 0.0;
        }
      }
    }

    if(aTrace) {
      std::cout << "c: ";
      printVector(c, m);
      std::cout << std::endl << std::endl;
    }

    Matrix::vecMPI(cp, c, m);

    if(aTrace) {   
      std::cout << "cp: ";
      printVector(cp, m);
      std::cout << std::endl << std::endl;
    }

    const double cpc = vecDotProd(cp, c, m);
    const double da  = A.colVecDotProd(d, k);

    if(aTrace) {
      std::cout << " <c^+, c> = " << cpc << std::endl;
      std::cout << " <d,a_k>  = " << da << std::endl;
    }

    for(uint i = 0; i < m; ++i) {
      b[i] = cp[i] + ((1 - cpc) / (1.0 + da)) * d[i];
    }

    if(aTrace) {
      std::cout << "b: ";
      printVector(b, m);
      std::cout << std::endl << std::endl;

      std::cout << "A^+: " << std::endl;
      Ap.print(std::cout, 3);
      std::cout << std::endl;
    }

    double Apa[n];

    // Apa = Ap_k a_j   // a_j = j-ter spaltenvektor von A
    //       k,m  m,1 
    for(uint i = 0; i < k; ++i) {
      double x = 0;
      for(uint j = 0; j < m; ++j) {
         x += Ap(i,j) * A(j,k);
      }
      Apa[i] = x;
    }

    if(aTrace) {
      std::cout << "A^+a: ";
      printVector(Apa, k);
      std::cout << std::endl;
    }

    for(uint i = 0; i < k; ++i) {
      for(uint j = 0; j < m; ++j) {
        Ap(i,j) -=  Apa[i] * b[j];
      }
    }

    for(uint j = 0; j < m; ++j) {
      Ap(k,j) = b[j];
    }

    if(aTrace) {
      std::cout << "A^+: " << std::endl;
      Ap.print(std::cout, 3);
      std::cout << std::endl;
    }

  }
} 

void
Matrix::productOf(double* aRes, const Matrix& A, const double* v) {
  for(uint i = 0; i < A.noRows(); ++i) {
    double x = 0.0;
    for(uint j = 0; j < A.noCols(); ++j) {
      x += A(i,j) * v[j];
    }
    aRes[i] = x;
  }
}

void
Matrix::productOfXF(double* aRes, const Matrix& A, const double* v) {
  for(uint i = 0; i < A.noRows(); ++i) {
    aRes[i] = 0;
  }
  for(uint j = 0; j < A.noCols(); ++j) {
    for(uint i = 0; i < A.noRows(); ++i) {
      aRes[i] += A(i,j) * v[j];
    }
  }
}

void
Matrix::productOfAtV(double* aRes, const Matrix& A, const double* v) {
  for(uint i = 0; i < A.noCols(); ++i) {
    double x = 0.0;
    for(uint j = 0; j < A.noRows(); ++j) {
      x += A(j,i) * v[j];
    }
    aRes[i] = x;
  }
}

void
Matrix::productOfAtVXF(double* aRes, const Matrix& A, const double* v) {
  for(uint i = 0; i < A.noCols(); ++i) {
    aRes[i] = 0;
  }
  const double* lA = A.getArray();
  for(uint i = 0; i < A.noRows(); ++i) {
    const double lVi = v[i];
    for(uint j = 0; j < A.noCols(); ++j) {
      // aRes[j] += A(i,j) * lVi;
      aRes[j] += *lA++ * lVi;
    }
  }
}

// only if A is known to be a matrix containing only 0 and 1
void
Matrix::productOfAtVXF01(double* aRes, const Matrix& A, const double* v) {
  for(uint i = 0; i < A.noCols(); ++i) {
    aRes[i] = 0;
  }
  const double* lA = A.getArray();
  double lVi = 0;
  // double x = 0;
  for(uint i = 0; i < A.noRows(); ++i) {
    lVi = v[i];
    for(uint j = 0; j < A.noCols(); ++j) {
      if(0 != (*lA++)) {
        aRes[j] += lVi;
      }
    }
  }
}

std::ostream&
Matrix::printVector(const double* v, const uint n) {
  return printVector(std::cout, v, n);
}

std::ostream&
Matrix::printVector(std::ostream& os, const double*v, const uint n) {
    os << '(';
    for(uint i = 0; i < n; ++i) {
      os << v[i];
      if((n-1) != i ) { os << ' '; }
    }
    os << ')';
    return os;
}

std::ostream&
Matrix::printVector(std::ostream& os, const double*v, const uint n, const int aWidth) {
    os << '(';
    for(uint i = 0; i < n; ++i) {
      os << std::setw(aWidth) << v[i];
      if((n-1) != i ) { os << ' '; }
    }
    os << ')';
    return os;
}


void
Matrix::assign(const Matrix& m) {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m(i,j);
    }
  }
}

int
Matrix::error(const char* aMsg) {
  int i = 0;
  std::cerr << "Matrix::error: " << aMsg << std::endl;
  return (1/i);
}

void
Matrix::initFromVector(const double* x) {
  memcpy(_m, x, sizeof(double) * noRows() * noCols());
}

void
Matrix::initFromStream(std::istream& is) {
  uint m,n;
  is >> m >> n;
  initFromStream(is, m, n);
}

void
Matrix::initFromStream(std::istream& is, const uint n, const uint m) {
  rescale(n,m);
  double x;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < m; ++j) {
       is >> x;
       if(is.eof()) { break; }
       (*this)(i,j) = x;
    }
  }
}

void
Matrix::initRandom(rng32_t& aRng, const uint aMod) {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = (double) (aRng() % aMod);
    }
  }
}

void
Matrix::colSumInclusive() {
  for(uint i = 0; i < noCols(); ++i) {
    double lSum = 0;
    for(uint j = 0; j < noRows(); ++j) {
      lSum += (*this)(j,i);
      (*this)(j,i) = lSum;
    }
  }
}

void
Matrix::rowSumInclusive() {
  for(uint i = 0; i < noRows(); ++i) {
    double lSum = 0;
    for(uint j = 0; j < noCols(); ++j) {
      lSum += (*this)(i,j);
      (*this)(i,j) = lSum;
    }
  }
}

void
Matrix::accumulateInclusive() {
  colSumInclusive();
  rowSumInclusive();
}

void
Matrix::colSumExclusive() {
  for(uint i = 0; i < noCols(); ++i) {
    double lSum = 0;
    double lSumLag = 0;
    for(uint j = 0; j < noRows(); ++j) {
      lSum += (*this)(j,i);
      (*this)(j,i) = lSumLag;
      lSumLag = lSum;
    }
  }
}

void
Matrix::rowSumExclusive() {
  for(uint i = 0; i < noRows(); ++i) {
    double lSum = 0;
    double lSumLag = 0;
    for(uint j = 0; j < noCols(); ++j) {
      lSum += (*this)(i,j);
      (*this)(i,j) = lSumLag;
      lSumLag = lSum;
    }
  }
}


void
Matrix::accumulateExclusive() {
  colSumExclusive();
  rowSumExclusive();
}

void
Matrix::setToElimRows(const Matrix& A, const uint_vt& aRowsToEliminate) {
  const uint n = A.noRows() - aRowsToEliminate.size();
  const uint m = A.noCols();
  resize(n,m);
  uint lCurrIdxArte = 0; // index into aRowsToEliminate
  uint lCurrIdxRow = 0;
  for(uint i = 0; i < A.noRows(); ++i) {
    if((lCurrIdxArte < aRowsToEliminate.size()) && (i == aRowsToEliminate[lCurrIdxArte])) {
      // std::cout << "eliminated: " << i << std::endl;
      ++lCurrIdxArte;
    } else {
      // copy row
      // std::cout << "copy: " << i << " --> " << lCurrIdxRow << std::endl;
      for(uint j = 0; j < m; ++j) {
         (*this)(lCurrIdxRow, j) = A(i,j);
      }
      ++lCurrIdxRow;
    }
  }
}

void
Matrix::setToElimCols(const Matrix& A, const uint_vt& aColsToEliminate) {
  const uint n = A.noRows();
  const uint m = A.noCols() - aColsToEliminate.size();
  resize(n,m);
  uint lCurrIdxActe = 0;
  uint lCurrIdxCol = 0;
  for(uint j = 0; j < A.noCols(); ++j) {
    if((lCurrIdxActe < aColsToEliminate.size()) && (j == aColsToEliminate[lCurrIdxActe])) {
      ++lCurrIdxActe;
    } else {
      // copy column
      for(uint i = 0; i < n; ++i) {
        (*this)(i, lCurrIdxCol) = A(i,j);
      }
      ++lCurrIdxCol;
    }
  }
}

void
Matrix::setToOnlyRows(const Matrix& A, const uint_vt& aRowsToRetain) {
  const uint m = aRowsToRetain.size();
  const uint n = A.noCols();
  resize(m,n);
  uint lCurrRow = 0;
  for(uint lIdx = 0; lIdx < aRowsToRetain.size(); ++lIdx) {
    const uint i = aRowsToRetain[lIdx];
    for(uint j = 0; j < n; ++j) {
      (*this)(lCurrRow, j) = A(i,j);
    }
    ++lCurrRow;
  }
}

void
Matrix::setToOnlyCols(const Matrix& A, const uint_vt& aColsToRetain) {
  const uint m = A.noRows();
  const uint n = aColsToRetain.size();
  resize(m,n);
  uint lCurrCol = 0;
  for(uint lIdx = 0; lIdx < aColsToRetain.size(); ++lIdx) {
    const uint j = aColsToRetain[lIdx];
    for(uint i = 0; i < m; ++i) {
      (*this)(i, lCurrCol) = A(i,j);
    }
    ++lCurrCol;
  }
}

// sets this to hermitial normal form
// requires this to be unit upper triangular
// requires the first non-zero number in each row to be 1
// requires that the column index of the first non-zero number in row j is larger than in row j-1.
// requires this(0,0) to be 1
// after the first row with only zeroes, all subsequent rows have to be zero rows
// returns r, the number of non-zero rows
// aF contains all the factors. it must be initialized to a m x m matrix if this is m x n
uint
Matrix::convertToHermiteNormalFormUUT(Matrix* aF) {
  if(0 != aF) {
    assert(aF->noRows() == aF->noCols());
    assert(aF->noRows() == noRows());
    aF->setAllTo(0);
    for(uint i = 0; i < aF->noRows(); ++i) {
      (*aF)(i,i) = 1;
    }
  }

  uint   lRes = noRows();
  double f = 0; // factor
  uint   j = 1;
  // std::cout << "2hnf: A = " << std::endl;
  // this->print(std::cout, 3) << std::endl;
  for(uint i = 1; i < noRows(); ++i) {
    for(; j < noCols(); ++j) {
      if(0 != (*this)(i,j)) {
        break;
      }
    }
    if(j >= noCols()) {
      // no non-zero element in row i
      i = noRows();
      lRes = i;
      break;
    }
    // for each row up to row i-1: make column j contain only zeroes
    for(uint k = 0; k < i; ++k) {
      if(0 != (*this)(k,j)) {
         // subtract row i from row k with according factor f
         f = -((*this)(k,j));
         for(uint l = j; l < noCols(); ++l) {
           (*this)(k,l) += f * (*this)(i,l);
         }
         // std::cout << "2hnf: f = " << f << ", i = " << i << ", j = " << j << ", A = " << std::endl;
         // this->print(std::cout, 3) << std::endl;
         if(0 != aF) {
           (*aF)(k,i) = f;
         }
      }
    }
  }
  return lRes;
}

bool
Matrix::isZeroRow(const uint i, const double aEps) const {
  for(uint j = 0; j < noCols(); ++j) {
    if(aEps < fabs((*this)(i,j))) {
      return false;
    }
  }
  return true;
}

bool 
Matrix::isZeroCol(const uint j, const double aEps) const {
  for(uint i = 0; i < noRows(); ++i) {
    if(aEps < fabs((*this)(i,j))) {
      return false;
    }
  }
  return true;
}



uint
Matrix::noZeroLoRows() const {
  uint lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(0 != (*this)(i,j)) {
        return lRes;
      }
    }
    ++lRes;
  }
  return lRes;
}

uint
Matrix::noZeroHiRows() const {
  uint lRes = 0;
  for(int i = noRows() - 1; i >= 0; --i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(0 != (*this)(i,j)) {
        return lRes;
      }
    }
    ++lRes;
  }
  return lRes;
}


uint
Matrix::noZeroLoCols() const {
  uint lRes = 0;
  for(uint j = 0; j < noCols(); ++j) {
    for(uint i = 0; i < noRows(); ++i) {
      if(0 != (*this)(i,j)) {
        return lRes;
      }
    }
    ++lRes;
  }
  return lRes;
}


uint
Matrix::noZeroHiCols() const {
  uint lRes = 0;
  for(int j = noCols() - 1; j >= 0; --j) {
    for(uint i = 0; i < noRows(); ++i) {
      if(0 != (*this)(i,j)) {
        return lRes;
      }
    }
    ++lRes;
  }
  return lRes;
}

// returns true, if there is a zero row i for which x[i] == 0
bool
Matrix::collectNonZeroRowCol(uint_vt& aNonZeroRows, 
                             uint_vt& aNonZeroCols, 
                             const double* x,
                             const double  aEps) const {
  aNonZeroRows.clear();
  aNonZeroCols.clear();
  bool lRes = false;

  for(uint i = 0; i < noRows(); ++i) {
    if(isZeroRow(i, aEps)) {
      if(0 != x) {
        lRes |= (0 == x[i]);
      }
    } else {
      aNonZeroRows.push_back(i);
    }
  }

  for(uint j = 0; j < noCols(); ++j) {
    if(!isZeroCol(j, aEps)) {
      aNonZeroCols.push_back(j);
    }
  }
  return lRes;
}

void
Matrix::setToReduction(const Matrix& A, const uint_vt& aRows, const uint_vt& aCols) {
  resize(aRows.size(), aCols.size());
  for(uint i = 0; i < aRows.size(); ++i) {
    for(uint j = 0; j < aCols.size(); ++j) {
      (*this)(i,j) = A(aRows[i], aCols[j]);
    }
  }
}

bool
Matrix::isZero(const double aEps) const {
  const uint n = size();
  for(uint i = 0; i < n; ++i) {
    if(aEps < std::fabs(_m[i])) {
      return false;
    }
  }
  return true;
}

bool
Matrix::isEinheitsmatrix(const double lEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  const uint n = noRows();
  const double lLo = -lEps;
  const double lHi = +lEps;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      const double x = (*this)(i,j) - (double) (i == j);
      if((lLo > x) || (lHi < x)) {
        // std::cout << "B: " << i << ' ' << j << ' ' << x << ' '
        //           << ((*this)(i,j))
        //           << std::endl;
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::equalUpTo(const Matrix& M, const double lEps) const {
  if(noCols() != M.noCols()) { return false; }
  if(noRows() != M.noRows()) { return false; }
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double a = (*this)(i,j);
      const double b = M(i,j);
      if((a > (b + lEps)) || (a < (b - lEps))) {
        return false;
      }
    }
  }
  return true;
}


bool
Matrix::equalUpToFirstKRows(const Matrix& M, const uint k, const double lEps) const {
  if(noCols() != M.noCols()) { return false; }
  if(k > noRows()) { return false; }
  if(k > M.noRows()) { return false; }
  for(uint i = 0; i < k; ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double a = (*this)(i,j);
      const double b = M(i,j);
      if((a > (b + lEps)) || (a < (b - lEps))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isAbsEqual(const Matrix& M, const double lEps) const {
  if(noRows() != M.noRows()) { return false; }
  if(noCols() != M.noCols()) { return false; }
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double a = (*this)(i,j);
      const double b = M(i,j);
      if(lEps < fabs(a - b)) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isUpperTriangular(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  // check everything zero below diagonal
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < i; ++j) {
      if(aEps < fabs((*this)(i,j))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isLowerTriangular(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  // check everything zero above diagonal
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < noCols(); ++j) {
      if(aEps < fabs((*this)(i,j))) {
        return false;
      }
    }
  }
  return true;
}

bool
Matrix::isDiagonal(const double aEps) const {
  return (isUpperTriangular() && isLowerTriangular());
}
 
bool
Matrix::isUnitUpperTriangular(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  bool lRes = true;
  // check everything zero under diagonal
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < i; ++j) {
      if(aEps < fabs((*this)(i,j))) {
        return false;
      }
    }
  }
  // check 1 on diagonal
  double a = 0;
  for(uint i = 0; i < noRows(); ++i) {
    a = fabs((*this)(i,i));
    if(((1.0 - aEps) > a) || ((1.0 + aEps) < a)) {
     return false;
    }
  }
  return lRes;
}

bool
Matrix::isDiag01UpperTriangular(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  bool lRes = true;
  // check everything zero under diagonal
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+1; j < i; ++j) {
      if(aEps < fabs((*this)(i,j))) {
        return false;
      }
    }
  }
  // check 0/1 on diagonal
  double a = 0;
  for(uint i = 0; i < noRows(); ++i) {
    a = fabs((*this)(i,i));
    if(aEps >= a) {
      continue;
    } else
    if(((1.0 - aEps) > a) || ((1.0 + aEps) < a)) {
     return false;
    }
  }
  return lRes;
}

bool
Matrix::isPermOfHermiteNormalForm(const double aEps) const {
  if(noRows() != noCols()) {
    return false;
  }
  if(!isDiag01UpperTriangular(aEps)) {
    return false;
  }
  double x = 0;
  double a = 0;
  for(uint i = 0; i < noRows(); ++i) {
    x = (*this)(i,i);
    a = fabs(x);
    if(a < aEps) {
      continue;
    }
    // now it should be 1.0
    for(uint j = 0; j < i; ++j) {
      if(aEps < fabs((*this)(j,i))) {
       return false;
      }
    }
  }
  return true;
}

// i > j + 1 ==> a_{i,j} = 0 
bool
Matrix::isHessenbergMatrix(const double aEps) const {
  for(uint i = 2; i < noRows(); ++i) {
    const uint lLim = (i - 1);
    for(uint j = 0; j < lLim; ++j) {
      if(!is_equal<double>((*this)(i,j), 0, aEps)) {
        return false;
      }
    }
  }
  return true;
}

// j > i + 1 ==> a_{i,j} = 0 
bool
Matrix::isHessenbergMatrixTransposed(const double aEps) const {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = i+2; j < noCols(); ++j) {
      if(!is_equal<double>((*this)(i,j), 0, aEps)) {
        return false;
      }
    }
  }
  return true;
}

double
Matrix::qerror(const Matrix& M) const {
  assert(noRows() == M.noRows());
  assert(noCols() == M.noCols());
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = M(i,j);
      const double y = (*this)(i,j);
      const double lQError = q::qerrorT3(x,y);
      if(lQError > lRes) {
        lRes = lQError;
      }
    }
  }
  return lRes;
}

double
Matrix::abserror(const Matrix& M) const {
  assert(noRows() == M.noRows());
  assert(noCols() == M.noCols());
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double x = M(i,j);
      const double y = (*this)(i,j);
      const double lAbsError = std::abs(x - y);
      if(lAbsError > lRes) {
        lRes = lAbsError;
      }
    }
  }
  return lRes;
}



Matrix&
Matrix::setToDiscreteCosineOrthonormalMatrix(const uint n) {
  resize(n,n);
  const double lPi = M_PI;
  const double lSqrt[2] = { sqrt((double) 2.0 / (double) n),
	                    sqrt((double) 1.0 / (double) n) };
  const double l2nInv = (double) 1.0 / ((double) 2 * n);
  for(uint i = 0; i < noRows(); ++i) {
    const double ldi = (double) (2 * i + 1);
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = lSqrt[0 == j] * cos(ldi * (double) j * l2nInv * lPi);
    }
  }
  return (*this);
}


// C cosine orthonormal matrix, then
// M = (C^t(C^t M)^t)^t if M is quadratic
// M = (Crow^t(Ccol^t M)^t)^t else
Matrix&
Matrix::setToDiscreteCosineTransformViaC(const Matrix& M) {
  if(M.noRows() == M.noCols()) {
    Matrix C;
    C.setToDiscreteCosineOrthonormalMatrix(M.noRows());
    Matrix A;
    A.setToAtxB(C, M); // A = C^tM
    setToTransposedOfAtxBt(C, A);
  } else {
    Matrix Crow;
    Matrix Ccol;
    Crow.setToDiscreteCosineOrthonormalMatrix(M.noRows());
    Ccol.setToDiscreteCosineOrthonormalMatrix(M.noCols());
    Matrix A;
    A.setToAtxB(Ccol, M);
    setToTransposedOfAtxBt(Crow, A);
  }
  return (*this);
}

// DCT
// R = (C^t(C^t M)^t)^t if M is quadratic
// R = (Crow^t(Ccol^t M)^t)^t else
// iDCT
// R = C(C M^t) if M is quadratic
// R = Ccol(Crow M^t)^t else

Matrix&
Matrix::setToInverseDiscreteCosineTransformViaC(const Matrix& M) {
  resize(M.noRows(), M.noCols());
  if(M.noRows() == M.noCols()) {
    Matrix C;
    C.setToDiscreteCosineOrthonormalMatrix(M.noRows());
    Matrix A;
    A.setToAxBt(C, M);
    setToAxBt(C, A);
  } else {
    Matrix Crow;
    Matrix Ccol;
    Crow.setToDiscreteCosineOrthonormalMatrix(M.noRows());
    Ccol.setToDiscreteCosineOrthonormalMatrix(M.noCols());
    Matrix A;
    A.setToAxBt(Crow, M);
    setToAxBt(Ccol, A);
  }
  return (*this);
}

Matrix&
Matrix::setToDiscreteCosineTransform(const Matrix& m) {
  resize(m.noRows(), m.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m.discreteCosineTransformCoefficient(i, j);
    }
  }
  return (*this);
}

double
Matrix::discreteCosineTransformCoefficient(const uint u, const uint v) const {
  double lSumOuter = 0.0;
  const double lPi = M_PI;
  const double lFactorRow = sqrt(2.0 / noRows()) * dct_k(u);
  const double lFactorCol = sqrt(2.0 / noCols()) * dct_k(v);
  for(uint m = 0; m < noRows(); ++m) {
    double lSumInner = 0.0;
    for(uint n = 0; n < noCols(); ++n) {
      lSumInner += (*this)(m,n) * cos(((2*n + 1) * v * lPi) / (2 * noCols()));
    }
    lSumOuter += lSumInner * cos(((2*m + 1) * u * lPi) / (2 * noRows()));
  }
  return lFactorRow * lFactorCol * lSumOuter;
}


Matrix&
Matrix::setToInverseDiscreteCosineTransform(const Matrix& m) {
  resize(m.noRows(), m.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      (*this)(i,j) = m.inverseDiscreteCosineTransformCoefficient(i, j);
    }
  }
  return (*this);
}

double
Matrix::inverseDiscreteCosineTransformCoefficient(const uint m, const uint n) const {
  double lSumOuter = 0.0;
  const double lPi = M_PI;
  const double lFactorRow = sqrt(2.0 / noRows());
  const double lFactorCol = sqrt(2.0 / noCols());
  for(uint u = 0; u < noRows(); ++u) {
    double lSumInner = 0.0;
    for(uint v = 0; v < noCols(); ++v) {
      lSumInner += dct_k(v) * (*this)(u,v) * cos(((2*n + 1) * v * lPi) / (2 * noCols()));
    }
    lSumOuter += dct_k(u) * lSumInner * cos(((2*m + 1) * u * lPi) / (2 * noRows()));
  }
  return lFactorRow * lFactorCol * lSumOuter;
}

/*
 * apply the following on the discrete cosine transform of some matrix M
 * to estimate the number of elements (\sum M(i,j)) within the rectangle
 * spanned by the indexes given as parameters
 */

double
Matrix::dctEstimateSlow(const uint aRowLo, const uint aRowHi,
                        const uint aColLo, const uint aColHi) const {
  double lSum = 0.0;
  for(uint i = aRowLo; i < aRowHi; ++i) {
    for(uint j = aColLo; j < aColHi; ++j) {
      lSum += Matrix::inverseDiscreteCosineTransformCoefficient(i,j);
    }
  }
  return lSum;
}


double
Matrix::dctEstimate(const uint aRowLo, const uint aRowHi,
                    const uint aColLo, const uint aColHi) const {
  double lSum = 0.0;
  const double lFactorRow = sqrt(2.0 / noRows());
  const double lFactorCol = sqrt(2.0 / noCols());


  for(uint i = 0; i < noRows(); ++i) {
    const double lKi = dct_k(i);
    for(uint j = 0; j < noCols(); ++j) {
      const double lKj = dct_k(j);
      const double lIntegralRow = dctEstimateIntegral(i, noRows(), aRowLo, aRowHi);
      const double lIntegralCol = dctEstimateIntegral(j, noCols(), aColLo, aColHi);
      const double lProd = lKi * lKj * (*this)(i,j) * lIntegralCol * lIntegralRow;
      lSum += lProd;
      // std::cout << lProd << " = " 
      //           << lKi << " * " << lKj 
      //           << " * " << (*this)(i,j) << " * " 
      //           << lIntegralRow << " * " << lIntegralCol
      //           << std::endl;
 
    }
  }
  return (lFactorRow * lFactorCol * lSum); 
}



/*
 * 
 */

double
Matrix::dctEstimateIntegral(const uint aIdx, const uint aMaxIdx, const uint aLo, const uint aHi) const {
  if(0 == aIdx) {
    return (aHi - aLo);
  }

  const double u = (double) aIdx;
  const double lPi = M_PI;
  // const double lHi = ((double) (2 * aHi + 1) * u * lPi) / ((double) (2 * aMaxIdx));
  // const double lLo = ((double) (2 * aLo + 1) * u * lPi) / ((double) (2 * aMaxIdx));
  const double lHi = ((double) (2 * aHi) * u * lPi) / ((double) (2 * aMaxIdx));
  const double lLo = ((double) (2 * aLo) * u * lPi) / ((double) (2 * aMaxIdx));
  const double lRes = (((double) aMaxIdx / (u * lPi)) * (sin(lHi) - sin(lLo)));
  return lRes;
}


// continous, bounds must have been adjusted to represent fractional indices

double
Matrix::dctEstimateContinuous(const double aRowLo, const double aRowHi,
		              const double aColLo, const double aColHi) const {
  double lSum = 0.0;
  const double lFactorRow = sqrt(2.0 / noRows());
  const double lFactorCol = sqrt(2.0 / noCols());


  for(uint i = 0; i < noRows(); ++i) {
    const double lKi = dct_k(i);
    for(uint j = 0; j < noCols(); ++j) {
      const double lKj = dct_k(j);
      const double lIntegralRow = dctEstimateIntegralContinuous(i, noRows(), aRowLo, aRowHi);
      const double lIntegralCol = dctEstimateIntegralContinuous(j, noCols(), aColLo, aColHi);
      const double lProd = lKi * lKj * (*this)(i,j) * lIntegralRow * lIntegralCol;
      lSum += lProd;
      
      // std::cout << "MdctEC: " << i << ", " << j << "  :  "
      //           << lProd << " = "
      //           << lKi << " * " << lKj
      //           << " * " << (*this)(i,j) << " * "
      //           << lIntegralRow << " * " << lIntegralCol
      //           << std::endl;

    }
  }
  return (lFactorRow * lFactorCol * lSum);
}

double
Matrix::dctEstimateIntegralContinuous(const uint aIdx, const uint aMaxIdx, const double aLo, const double aHi) {
  if(0 == aIdx) {
    return (aHi - aLo);
  }

  const double u = (double) aIdx;
  const double lPi = M_PI;
  const double lHi = ((double) (2 * aHi) * u * lPi) / ((double) (2 * aMaxIdx));
  const double lLo = ((double) (2 * aLo) * u * lPi) / ((double) (2 * aMaxIdx));
  const double lRes = (((double) aMaxIdx / (u * lPi)) * (sin(lHi) - sin(lLo)));
  return lRes;
}

// \rho(A) = max_{i \neq j} \frac{|a_i^t a_j|}{||a_i||_2^2}
// a_i: column vectors of A

double
Matrix::oneSidedCoherence() const {
  double lRes = 0;
  double lNormAi = 0;
  double lProdAiAj = 0;
  double lQuot = 0;
  for(uint j = 0; j < noCols(); ++j) {
    // ||a_j||_2^2;
    lNormAi = 0;
    for(uint i = 0; i < noRows(); ++i) {
      lNormAi += (*this)(i,j) * (*this)(i,j);
    }
    // |a_i^t a_k| for all k
    for(uint k = 0; k < noCols(); ++k) {
      if(j == k) {
        continue;
      }
      lProdAiAj = 0;
      for(uint i = 0; i < noRows(); ++i) {
         lProdAiAj += (*this)(i,j) * (*this)(i,k);
      }
      lProdAiAj = fabs(lProdAiAj);
      lQuot = lProdAiAj / lNormAi;
      if(lRes < lQuot) {
        lRes = lQuot;
      }
    }
  }
  return lRes;
}

double
Matrix::twoSidedCoherence() const {
  double lRes = 0;

  return lRes;
}

void
Matrix::initAsRotationMatrix(const double aAlpha) {
  resize(2,2);
  const double lSin = sin(aAlpha);
  const double lCos = cos(aAlpha);
  (*this)(0,0) =  lCos;
  (*this)(0,1) = -lSin;
  (*this)(1,0) =  lSin;
  (*this)(1,1) =  lCos;
}


void
Matrix::qr_mgs(Matrix& Q, Matrix& R) const {
  Matrix A(*this);
  const uint m = A.noRows();
  const uint n = A.noCols();

  assert(m >= n);

  // A:  m x n
  // Q:  m x n
  // R:  n x n
  Q.resize(m,n);
  R.resize(n,n);

  // haette gerne (wie in Demmel, Higham 1993):
  // Q.resize(m,m);
  // R.resize(m,n);

  const bool lTrace = false;
  if(lTrace) {
    std::cout << "qr_mgs: m = " << m << ", n = " << n << std::endl;
  }


  double lSum = 0;
  if(n <= m) {
    std::cout << "qr_mgs: m = " << m << ", n = " << n << std::endl;
    for(uint k = 0; k < n; ++k) {
      if(lTrace) {
        std::cout << "  k = " << k << std::endl;
      }
      // R(k,k) = A.col_norm(k);
      lSum = 0;
      for(uint i = 0; i < m; ++i) {
        lSum += (A(i,k) * A(i,k));
      }
      R(k,k) = sqrt(lSum);
      if(lTrace) {
        std::cout << "  lSum = " << lSum << ", sqrt(lSum) = " << sqrt(lSum) << std::endl;
      }

      // Q(1:m,k) = A(1:m,k) / R(k,k);
      if(lTrace) {
        std::cout << "  Q(*,k) = [";
      }
      for(uint i = 0; i < m; ++i) {
        Q(i,k) = A(i,k) / R(k,k);
        if(lTrace) {
          std::cout << "Q(" << i << ',' << k << ") = " << Q(i,k) << std::endl;
        }
      }
      if(lTrace) {
        std::cout << ']' << std::endl;
      }

      for(uint j = k + 1; j < n; ++j) {
        if(lTrace) {
          std::cout << "  j = " << j << std::endl;
        }
        // R(k,j) = Q(1:m,k)^t A(1:m, j)
        lSum = 0;
        for(uint i = 0; i < m; ++i) {
          lSum += Q(i,k) * A(i,j);
        }
        R(k,j) = lSum;
        if(lTrace) {
          std::cout << "    R(k," << j << ") = " << lSum << std::endl;
        }

        // A(1:m,j) = A(1:m,j) - Q(1:m,k)*R(k,j)
        for(uint i = 0; i < m; ++i) {
          A(i,j) -= Q(i,k) * R(k,j);
          if(lTrace) {
            std::cout << "      A(" << i << ',' << j << ") = " << A(i,j) << std::endl;
          }
        }
      }
    }
  } else {
    assert(0 == 1);
    for(uint k = 0; k < m; ++k) { // eigentlich: m sollte n sein!!!, EXPERIMENTAL
      if(lTrace) {
        std::cout << "  k = " << k << std::endl;
      }
      // R(k,k) = A.col_norm(k);
      lSum = 0;
      for(uint i = 0; i < m; ++i) {
        lSum += (A(i,k) * A(i,k));
      }
      R(k,k) = sqrt(lSum);
      if(lTrace) {
        std::cout << "  lSum = " << lSum << ", sqrt(lSum) = " << sqrt(lSum) << std::endl;
      }

      // Q(1:m,k) = A(1:m,k) / R(k,k);
      if(lTrace) {
        std::cout << "  Q(*,k) = [";
      }
      for(uint i = 0; i < m; ++i) {
        Q(i,k) = A(i,k) / R(k,k);
        if(lTrace) {
          std::cout << ' ' << Q(i,k);
        }
      }
      if(lTrace) {
        std::cout << ']' << std::endl;
      }

      for(uint j = k + 1; j < n; ++j) {
        if(lTrace) {
          std::cout << "  j = " << j << std::endl;
        }

        // R(k,j) = Q(1:m,k)^t A(1:m, j)
        lSum = 0;
        for(uint i = 0; i < m; ++i) {
          lSum += Q(i,k) * A(i,j);
        }
        R(k,j) = lSum;
        if(lTrace) {
          std::cout << "    R(k," << j << ") = " << lSum << std::endl;
        }

        // A(1:m,j) = A(1:m,j) - Q(q:m,k)*R(k,j)
        for(uint i = 0; i < m; ++i) {
          A(i,j) -= Q(i,k) * R(k,j);
          if(lTrace) {
            std::cout << "      A(" << i << ',' << j << ") = " << A(i,j) << std::endl;
          }
        }
      }
    }
  }
}

std::ostream&
Matrix::print(std::ostream& os, const int w, const int aIndent, const format_t aFormat) const {
  switch(aFormat) {
    case kAscii : print(os, w, aIndent); break;
    case kMatlab: printMatlab(os, w); break;
    case kLatex : printLatex(os, w, aIndent); break;
    default     : print(os, w); break;
  }
  return os;
}

std::ostream&
Matrix::print(std::ostream& os, const int w, const int aIndent) const {
  for(uint i = 0; i < noRows(); ++i) {
    if(0 < aIndent) {
      std::cout << std::string(aIndent, ' ');
    }
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (*this)(i,j) << ' ';
    }
    os << std::endl;
  }
  return os;
}

std::ostream&
Matrix::printDown(std::ostream& os, const int w) const {
  const int m = (int) noRows();
  for(int i = (m-1); i >= 0; --i) {
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (*this)(i,j) << ' ';
    }
    os << std::endl;
  }
  return os;
}


std::ostream&
Matrix::printRoundX(std::ostream& os, const int w) const {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (round((*this)(i,j)*10.0) / 10.0) << ' ';
    }
    os << std::endl;
  }
  return os;
}


std::ostream&
Matrix::printRoundXX(std::ostream& os, const int w) const {
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (round((*this)(i,j)*100.0) / 100.0) << ' ';
    }
    os << std::endl;
  }
  return os;
}

std::ostream&
Matrix::printWithRowSum(std::ostream& os, const int w) const {
  double lSum = 0;
  for(uint i = 0; i < noRows(); ++i) {
    lSum = 0;
    for(uint j = 0; j < noCols(); ++j) {
      os << std::setw(w) << (*this)(i,j) << ' ';
      lSum += (*this)(i,j);
    }
    os << "  // " << std::setw(w+1) << lSum << std::endl;
  }
  return os;
}

std::ostream&
Matrix::printWithRowColSum(std::ostream& os, const int w) const {
  printWithRowSum(os, w);
  os << "---" << std::endl;
  for(uint j = 0; j < noCols(); ++j) {
    double lSum = 0;
    for(uint i = 0; i < noRows(); ++i) {
      lSum += (*this)(i,j);
    }
    os << std::setw(w) << lSum << ' ';
  }
  os << std::endl;
  return os;
}

std::ostream&
Matrix::printMatlab(std::ostream& os, const int w, const int aIndent) const {
   os << "[ ";
   for(uint i = 0; i < noRows(); ++i) {
     if(0 < aIndent) {
        std::cout << std::string(aIndent, ' ');
     }
     for(uint j = 0; j < noCols(); ++j) {
       os << std::setw(w) << ((*this)(i,j)) << ' ';
     }
     if(i < (noRows() - 1)) {
       os << ';' << std::endl;
     }
  }
  os << ']';
  return os;
}

std::ostream&
Matrix::printLatex(std::ostream& os, const int aWidth, const int aIndent) const {
  std::string lIndent(aIndent, ' ');
  os << lIndent << "\\begin{pmatrix}" << std::endl;
  for(uint i = 0; i < noRows(); ++i) {
    os << lIndent;
    os << std::setw(aWidth) << (*this)(i,0);
    for(uint j = 1; j < noCols(); ++j) {
      os << " & " << std::setw(aWidth) << (*this)(i,j);
    }
    if((noRows() - 1) > i) {
      os << "  \\\\";
    }
    os << std::endl;
  }
  os << lIndent << "\\end{pmatrix}" << std::endl;
  return os;
}



