void
Matrix::setToInverseOfUpperTriangular(const Matrix& m, const double eps) {
  rescale(m.noRows(), m.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    (*this)(i,i) = ((double) 1.0) / m(i,i);
  }
  for(int i = noRows() - 2; 0 <= i; --i) {
    const double m_ii_inv = ((double) 1.0) / m(i,i);
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


void
Matrix::setToTransposedOfInverseOfUpperTriangular(const Matrix& m, const double eps) {
  rescale(m.noRows(), m.noCols());
  for(uint i = 0; i < noRows(); ++i) {
    (*this)(i,i) = ((double) 1.0) / m(i,i);
  }
  double lSum = 0.0;
  for(int i = noRows() - 2; 0 <= i; --i) {
    const double m_ii_inv = ((double) 1.0) / m(i,i);
    for(int j = noCols() - 1; i < j; --j) {
      // alt 1
      // lSum = 0.0;
      // for(int k = i+1; k <= j; ++k) {
      //   lSum += m(i,k) * (*this)(j,k);
      // }
      // alt 2
      // lSum = vecDotProd(m.data() + i * m.noCols() + i + 1, 
      //                     data() + j * m.noCols() + i + 1,
      //                     j - i);
      // alt 3
      lSum = vp_dotprod8(m.data() + i * m.noCols() + i + 1, 
                           data() + j * m.noCols() + i + 1,
                           j - i);
      (*this)(j,i) = (-lSum) * m_ii_inv;
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

void
Matrix::setToSquareRootLowerTriangular_XF(const Matrix& A, const double aEps) {
  assert(A.noRows() == A.noCols());
  const int n = A.noRows(); // must be equal to m.noCols()
  rescale_no_init(n,n);
  // copy m to this
  memcpy((void*) _m, A._m, n * n * sizeof(double));
  // calc lower triangular
  double lSum = 0;
  for(int i = 0; i < n; i++) {
    for(int j = i; j < n; j++) {
      // double lSum = (*this)(i,j);
      lSum = 0;
      // for(int k = i-1 ; k >= 0; k--) {
      //   lSum += (*this)(i,k) * (*this)(j,k);  // row sum
      // }
      lSum = vp_dotprod8(data() + i * n, data() + j * n, i);
      lSum = (*this)(i,j) - lSum;
      if(i == j) {
        if(aEps > std::fabs(lSum)) {  
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


// Cholesky decomposition of a matrix M
// construct upper triangular matrix L
// such that M = LL'
// requires M to be symmetric and positive definite
// see Harville p. 237ff
// let A be a symmetric, positive definite matrix.
// the following member performs:
// solve Ax = b
// for A = L'L, A is lower triangular matrix
// some matrix A must have been cholesky decomposed
// with result put into M.
// a call to inverseSquareRootLowerTriangular will
// put the inverse of A into this.
void 
Matrix::inverseSquareRootLowerTriangular(const Matrix& M) {
  assert(M.noRows() == M.noCols());
  const int n = M.noRows();
  rescale(n, n);
  double sum = 0.0;
  for(int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      sum = ( (i==j) ? 1.0 : 0.0 );
      for(int k = (i - 1); k >= j; --k) {
        sum -= M(i, k) * (*this)(j, k);
      }
      (*this)(j, i) = sum / (M(i, i));
    }
  }
  for(int i = (n - 1); i >= 0; --i) {
    for(int j = 0; j <= i; ++j) {
      sum = ( (i < j) ? 0.0 : (*this)(j, i) );
      for(int k = i + 1; k < n; ++k) {
        sum -= M(k, i) * (*this)(j, k);
      }
      (*this)(i, j) = (*this)(j, i) = sum / (M(i, i));
    }
  }
}

void
Matrix::setToInverseOfLowerTriangular(const Matrix& A) {
  assert(A.noRows() == A.noCols());
  const int n = A.noRows();
  rescale(n, n);
  double lSum = 0.0;
  for(int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      // lSum = 0; 
      // for(int k = (i - 1); k >= j; --k) {
      //   lSum += A(i, k) * (*this)(j, k);
      // }
      lSum = vp_dotprod8(A.data() + i * n + j, data() + j * n + j, i - j);
      lSum = ( (i==j) ? 1.0 : 0.0 ) - lSum;
      (*this)(j, i) = lSum / (A(i, i));
    }
  }
  for(int i = (n - 1); i >= 0; --i) {
    for(int j = 0; j <= i; ++j) {
      lSum = 0;   
      for(int k = i + 1; k < n; ++k) {
        lSum += A(k, i) * (*this)(j, k);
      }
      lSum = ( (i < j) ? 0.0 : (*this)(j, i) ) - lSum;
      (*this)(i, j) = (*this)(j, i) = lSum / (A(i, i));
    }
  }
}
