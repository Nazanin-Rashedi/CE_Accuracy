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

