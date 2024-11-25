#include "WaveletTransformNonStd2dim.hh"

WaveletTransformNonStd2dim::WaveletTransformNonStd2dim() {}
WaveletTransformNonStd2dim::~WaveletTransformNonStd2dim() {}

void
WaveletTransformNonStd2dim::transform(Matrix& W, const Matrix& A) {
  // A: input matrix
  // W: wavelet coefficient matrix
  assert(A.noCols() == A.noRows()); // matrix must be quadratic
  const uint w = A.noCols();
  assert((uint) (1 << idx_highest_bit_set(w)) == w); // matrix must be of dim 2^k x 2^k
  W.rescale(w,w); // make fit
  transformSub(W, A, w);
}

void
WaveletTransformNonStd2dim::transformSub(Matrix& W, const Matrix& A, const uint w) {
  bool lTrace = false;
  const uint n = w/2;
  Matrix  T(n,n);
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      const uint ii = 2 * i;
      const uint jj = 2 * j;
      const double a = A(ii    , jj    );
      const double b = A(ii    , jj + 1);
      const double c = A(ii + 1, jj    );
      const double d = A(ii + 1, jj + 1);
      T(    i,     j) = (a + b + c + d) / 4; // alpha
      W(    i, n + j) = (a - b + c - d) / 4; // beta
      W(n + i,     j) = (a + b - c - d) / 4; // gamma
      W(n + i, n + j) = (a - b - c + d) / 4; // delta
    }
  }
  if(lTrace) {
    std::cout << "computeWaveletSub0: T: " << std::endl;
    T.printDown(std::cout, 8);
  }

  if(n > 1) {
    transformSub(W, T, n);
  } else {
    W(0,0) = T(0,0);
  }

  // RTWAI;
}


void
WaveletTransformNonStd2dim::inverse(Matrix& A, const Matrix& W) {
  // W: input matrix of wavelet coefficients
  // R: result of inversion wavelet transform
  assert(W.noCols() == W.noRows()); // matrix must be quadratic
  const uint w = W.noCols();
  assert((uint) (1 << idx_highest_bit_set(w)) == w); // matrix must be of dim 2^k x 2^k

  A.resize(w,w);
  inverseSub(A, W, w);
}



void
WaveletTransformNonStd2dim::inverseSub(Matrix& A, const Matrix& W, const uint w) {
  const bool lTrace = false;
  const uint n = w/2;
  if(lTrace) {
    std::cout << std::string(W.noRows() - n, ' ') << " enter cIWS0 w = " << w << std::endl;
  }
  Matrix R(w,w);
  if(1 < n) {
    inverseSub(R, W, n);
  } else {
    R(0,0) = W(0,0);
  }

  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
        const uint ii = 2 * i;
        const uint jj = 2 * j;
      const double a = R(    i,     j);
      const double b = W(    i, n + j);
      const double c = W(n + i,     j);
      const double d = W(n + i, n + j);
      if(false) {
        std::cout << "w " << w << " i,j " << i << ',' << j << std::endl
                  << "   a = " << a << std::endl
                  << "   b = " << b << std::endl
                  << "   c = " << c << std::endl
                  << "   d = " << d << std::endl;
      }
      A(ii    , jj    ) = (a + b + c + d); // a
      A(ii    , jj + 1) = (a - b + c - d); // b
      A(ii + 1, jj    ) = (a + b - c - d); // c
      A(ii + 1, jj + 1) = (a - b - c + d); // d
    }
  }
  if(lTrace) {
    std::cout << std::string(W.noRows() - n, ' ') << "computeInverseWaveletSub0: " << std::endl;
    std::cout << "A: " << std::endl;
    A.printDown(std::cout, 8);
    std::cout << std::string(W.noRows() - n, ' ') << " exit cIWS0 " << std::endl;
  }
}

void
WaveletTransformNonStd2dim::extractTopK(coeff_vt& aCv, const Matrix& W, const uint aK) {
  TopK<coeff_t> lTopK(aK);
  aCv.clear();
  for(uint i = 0; i < W.noRows(); ++i) {
    for(uint j = 0; j < W.noCols(); ++j) {
      coeff_t lCoeff(i, j, W(i,j));
      if(0 != lCoeff.c) {
        lTopK.insert(lCoeff);
      }
    }
  }
  while(!lTopK.empty()) {
    coeff_t lCoeff = lTopK.top();
    lTopK.pop();
    aCv.push_back(lCoeff);
    // std::cout << " XX COEFF : " << lCoeff.c << std::endl;
  }
}


double
WaveletTransformNonStd2dim::inverseCoeff(const coeff_vt& C, const uint k, const uint l, const uint n) {
  const bool lTrace = false;
  double lRes = 0;
  for(coeff_vt::const_iterator lIter = C.begin(); lIter != C.end(); ++lIter) {
     const uint   i = (*lIter).i;
     const uint   j = (*lIter).j;
     const double c = (*lIter).c; // wavelet coefficient
     if(lTrace) {
            std::cout << "WTNS2d::inverseCoeff: "  
                      << "  k = " << k << ' '
                      << "  l = " << l << ' '
                      << "  n = " << n << ' ' << std::endl << "     "
                      << "  i = " << i << ' '
                      << "  j = " << j << ' '
                      << "  c = " << c << ' '
                      << std::endl;
     }
     const uint L = getLevel(i,j);
     const uint lMask = ((1 << L) - 1);
     const uint ri = (i & lMask); // rest (i without level bit)
     const uint rj = (j & lMask); // rest (j without level bit)
     const uint w  = (n >> L); // width of support area
     const uint s = idx_highest_bit_set(n) - L; // shift for rest
     const uint lKlo = (ri << s);
     const uint lKhi = lKlo + w; // exclusive
     const uint lLlo = (rj << s);
     const uint lLhi = lLlo + w; // exclusive
     if(lTrace) {
       std::cout << "  L = " << L << std::endl
                 << "  w = " << w << std::endl
                 << "  s = " << s << std::endl
                 << "  i = " << i << std::endl
                 << "  j = " << j << std::endl
                 << "  L = " << L << std::endl
                 << " ri = " << ri << std::endl
                 << " rj = " << rj << std::endl
                 << "klo = " << lKlo << std::endl
                 << "khi = " << lKhi << std::endl
                 << "llo = " << lLlo << std::endl
                 << "lhi = " << lLhi << std::endl
                 ;
     }
     if(    (lKlo <= k && k < lKhi)
         && (lLlo <= l && l < lLhi) ) {
       const uint bi = ((i >> L) & 0x1);
       const uint bj = ((j >> L) & 0x1);
       const uint lQuadrant = (k >= (lKlo + (w >> 1))) | ((l >= (lLlo + (w >> 1))) << 1);
       const uint lSign = wsign(bi, bj, lQuadrant);
       const double lCoeff[2] = {-c, c};
       lRes += lCoeff[lSign];
       if(lTrace) {
          std::cout << " lRes = " << lRes << std::endl;
          std::cout << " bi = " << ri << std::endl
                    << " bj = " << rj << std::endl
                    << "qud = " << lQuadrant << std::endl
                    << "sign= " << lSign << std::endl
                    << std::endl;
       }
     }
  }
  return lRes;
}


double
WaveletTransformNonStd2dim::normalizeCoeff(const uint i, const uint j, const double c) {
  const uint L = getLevel(i,j);
  return (c * ((double) (1 << L)));
}


double
WaveletTransformNonStd2dim::denormalizeCoeff(const uint i, const uint j, const double c) {
  const uint L = getLevel(i,j);
  return (c * ((double) 1.0 / (double) (1 << L)));
}

void
WaveletTransformNonStd2dim::normalize(Matrix& W) {
  for(uint i = 0; i < W.noRows(); ++i) {
    for(uint j = 0; j < W.noCols(); ++j) {
       W(i,j) = normalizeCoeff(i,j,W(i,j));
    }
  }
}

void
WaveletTransformNonStd2dim::denormalize(Matrix& W) {
  for(uint i = 0; i < W.noRows(); ++i) {
    for(uint j = 0; j < W.noCols(); ++j) {
       W(i,j) = denormalizeCoeff(i,j,W(i,j));
    }
  }
}

void
WaveletTransformNonStd2dim::denormalize(coeff_vt& aCoeffs) {
  for(coeff_vt::iterator lIter = aCoeffs.begin(); lIter < aCoeffs.end(); ++lIter) {
    (*lIter).c = denormalizeCoeff((*lIter).i, (*lIter).j, (*lIter).c);
  }
}


