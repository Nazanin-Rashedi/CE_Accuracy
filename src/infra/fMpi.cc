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
     AptAp_k(i,j) = sum_{l=1}^{k-1} a^+_{l,i}a^+{l,j} 
                  = sum_{l=1}^{k-1} a^+_{l,i}a^+{l,j} + a^+_{k,i}a^+{k,j}
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

    double Apa[m];

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
