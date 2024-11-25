#include <iostream>
#include <iomanip>
#include <fstream>

#include "matrix.hh"
#include "bit_intrinsics.hh"
#include "print_test_header.hh"
#include "bitvectorsmall.hh"
#include "WaveletTransformNonStd2dim.hh"


void
computeWaveletSub(const Matrix& I, Matrix& W, const uint w) {
  bool lTrace = false;
  const uint n = w/2;
  Matrix  A(n,n);
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      const uint ii = 2 * i;
      const uint jj = 2 * j;
      const double a = I(ii    , jj    );
      const double b = I(ii    , jj + 1);
      const double c = I(ii + 1, jj    );
      const double d = I(ii + 1, jj + 1);
      A(    i,     j) = (a + b + c + d) / 4; // alpha
      W(    i, n + j) = (a - b + c - d) / 4; // beta
      W(n + i,     j) = (a + b - c - d) / 4; // gamma
      W(n + i, n + j) = (a - b - c + d) / 4; // delta
    }
  }
  if(lTrace) {
    std::cout << "computeWaveletSub0: A: " << std::endl;
    A.printDown(std::cout, 8);
  }

  if(n > 1) {
    computeWaveletSub(A, W, n);
  } else {
    W(0,0) = A(0,0);
  }
}

void
computeWavelet(const Matrix& I, Matrix& W) {
  // I: input matrix
  // W: wavelet coefficient matrix
  assert(I.noCols() == I.noRows()); // matrix must be quadratic
  const uint w = I.noCols();
  assert((1 << idx_highest_bit_set(w)) == w); // matrix must be of dim 2^k x 2^k
  W.rescale(w,w); // make fit
  computeWaveletSub(I, W, w);
}


void
computeInverseWaveletSub(const Matrix& W, Matrix& R, const uint w) {
  const bool lTrace = false;
  const uint n = w/2;
  if(lTrace) {
    std::cout << std::string(W.noRows() - n, ' ') << " enter cIWS0 w = " << w << std::endl;
  }
  Matrix A(w,w);
  if(1 < n) {
    computeInverseWaveletSub(W, A, n);
  } else {
    A(0,0) = W(0,0);
  }

  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
        const uint ii = 2 * i;
        const uint jj = 2 * j;
/*
        const double a = I(ii    , jj    );
        const double b = I(ii    , jj + 1);
        const double c = I(ii + 1, jj    );
        const double d = I(ii + 1, jj + 1);
        A(    i,     j) = (a + b + c + d) / 4; // alpha
        W(    i, n + j) = (a - b + c - d) / 4; // beta
        W(n + i,     j) = (a + b - c - d) / 4; // gamma
        W(n + i, n + j) = (a - b - c + d) / 4; // delta
*/
      const double a = A(    i,     j);
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
      R(ii    , jj    ) = (a + b + c + d); // a
      R(ii    , jj + 1) = (a - b + c - d); // b
      R(ii + 1, jj    ) = (a + b - c - d); // c
      R(ii + 1, jj + 1) = (a - b - c + d); // d
    }
  }
  if(lTrace) {
    std::cout << std::string(W.noRows() - n, ' ') << "computeInverseWaveletSub0: " << std::endl;
    std::cout << "R: " << std::endl;
    R.printDown(std::cout, 8);
    std::cout << std::string(W.noRows() - n, ' ') << " exit cIWS0 " << std::endl;
  }
}


void
computeInverseWavelet(const Matrix& W, Matrix& R) {
  // W: input matrix of wavelet coefficients
  // R: result of inversion wavelet transform
  assert(W.noCols() == W.noRows()); // matrix must be quadratic
  const uint w = W.noCols();
  assert((1 << idx_highest_bit_set(w)) == w); // matrix must be of dim 2^k x 2^k

  R.resize(w,w);
  computeInverseWaveletSub(W, R, w);
}


uint
getLevel(const uint i) {
  if(0 == i) {
    return 0;
  }
  return idx_highest_bit_set(i) + 1;
}

/*
 * sign(i,j, aSignCase) 
 * let w(i,j) be a wavelet coeffcient. then let
 * bi: level bit of wavelet index i in bit number 0
 * bj: level bit of wavelet index j in bit number 0
 * sign case: (level) quadrant (00, 01, 10, 11) of number to be reconstructed from wavelet coefficients
 * then: sign(i, j, aSignCase) return 1 if '+' and 0 if '-' is the wavelet sign
 */

uint
wsign(const uint bi, const uint bj, const uint aQuadrant) {
  const uint a = aQuadrant;
  const uint b = (bj << 1) | bi;
  return ( !(a & b) | (3 == (a & b)) );
}


void
PrintSignCase() {
  std::cout << "------------------------" << std::endl;
  for(uint lSignCase = 1; lSignCase < 4; ++lSignCase) {
    for(uint i = 0; i < 2; ++i) {
      for(uint j = 0; j < 2; ++j) {
        const uint a = lSignCase;
        const uint b = (j << 1) | i; // ACHTUNG: NICHT (i << 1) | j !!!
        const uint lIsNeg = !(a & b) | (a == b);
        Bitvector32 lBvSc(a);
        lBvSc.printReverse(std::cout, 2);
        std::cout << " | ";
        Bitvector32 lBvIJ(b);
        lBvIJ.printReverse(std::cout, 2);
        std::cout << " | " << i << j << "  |   " 
                  << (a & b) << " | "
                  << ( !(a & b) | (3 == (a & b)) ) << " | "   // ergibt korrekte spalte 1 == +, 0 == -
                  << wsign(i, j, lSignCase) << " | " 
                  << ( !( !(a & b) | (3 == (a & b)) )  ) << " | "  // ergibt korrekte spalte 1 == -, 0 == +
                  << (a ^ b) << " | "
                  << (a == b) << " | "
                  << (a != b) << " | "
                  << ((0 == lIsNeg) ? '+' : '-')
                  << std::endl;
      }
    }
    std::cout << "------------------------" << std::endl;
  }
}


void
testSupport(const uint n) {
  print_test_header(" TEST SUPPORT ");
  uint w[n][n];
  uint l[n][n];
  uint sign[n][n];

  static const char lSignChar[3] = {'+', '-', '.'};

  // i and j are the indices of some wavelet coefficient W(i,j)
  // ii and jj are the indices of the lower left corner of the support area
  // w[i][j] is the width of the support area
  // sign[k][l] is the sign of the support area, two if outside support area, i.e.
  //            if we reconstruct A(k = ii+c, l = jj+c) for 0 <= c < w[i][j] then
  //            we must add W(i,j) with sign sign[k][l] to the result
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
       // W(i,j) is assumed to be the Wavelet coefficient
       // -- whose support area needs to be determined
       // -- whose sign to reconstruct some A(k,l) needs to be determined

       // first, we reset all signs to '.'
       for(uint k = 0; k < n; ++k) {
         for(uint l = 0; l < n; ++l) {
           sign[k][l] = 2;
         }
       }

       // first, we get the level of the wavelet coefficient
       const uint li = getLevel(i);
       const uint lj = getLevel(j);
       l[i][j]  = std::max<uint>(li, lj);

       // the rectangle '[(lXlo, lYlo), (lXhi, lYhi)]' will
       // be the support area of W(i,j)
       uint lXlo = 0;
       uint lXhi = 0;
       uint lYlo = 0;
       uint lYhi = 0;

       // the bit at the level position is reset in the
       // the rest of indices i and j, called ri and rj.

       uint ri = 0; // rest (highest bit set to zero)
       uint rj = 0; // rest (highest bit set to zero)

       uint ii = 0; // ursprungsindex
       uint jj = 0; // ursprungsindex

       // the quadrant of the wavelet coefficient determines
       // the case for the sign:
       // wavelet coefficient is from quadrant qq
       // determins the sign of the quartered support area (qsa
       //   qq   qsa
       //
       //   01   +-
       //        +-
       //
       //   10   --
       //        ++
       //
       //   11   -+
       //        +-
       //
       uint bi; // level bit of i
       uint bj; // level bit of j
       if(0 == l[i][j]) {
         lXlo = 0; lXhi = n;
         lYlo = 0; lYhi = n;
         ri = 0; rj = 0;
         w[i][j] = n;
         // all signs '+'
         for(uint k = 0; k < n; ++k) {
           for(uint l = 0; l < n; ++l) {
               sign[k][l] = 0;
           }
         }
       } else {
         if(1 == l[i][j]) {
           lXlo = 0; lXhi = n;
           lYlo = 0; lYhi = n;
           ri = 0; rj = 0;
           w[i][j] = n;
           bi = (i & 0x1);
           bj = (j & 0x1);
         } else {
           ri = i & ((1 << (l[i][j]-1)) - 1);
           rj = j & ((1 << (l[i][j]-1)) - 1);
           w[i][j] = (n >> (l[i][j] - 1));
           // ii = (ri * w[i][j]);
           // jj = (rj * w[i][j]);
           ii = (ri << (idx_highest_bit_set(n) - l[i][j] + 1));
           jj = (rj << (idx_highest_bit_set(n) - l[i][j] + 1));
           lXlo = jj;
           lYlo = ii;
           lXhi = lXlo + w[i][j] - 1;
           lYhi = lYlo + w[i][j] - 1;
           bi = ((i >> (l[i][j] - 1)) & 0x1);
           bj = ((j >> (l[i][j] - 1)) & 0x1);
         }
         for(uint k = 0; k < n; ++k) {
           for(uint l = 0; l < n; ++l) {
             if(    ((lYlo <= k) && (k <= lYhi))
                 && ((lXlo <= l) && (l <= lXhi)) ) {
               const uint qk = (k >= (lYlo + (w[i][j] >> 1)));
               const uint ql = (l >= (lXlo + (w[i][j] >> 1)));
               const uint lSubQuadrant =   (qk) | (ql << 1);
               sign[k][l] = 1 - wsign(bi, bj, lSubQuadrant);
             } else {
               sign[k][l] = 2;
             }
           }
         }
       }
       std::cout << '(' << std::setw(2) << i << ',' << std::setw(2) << j << ')' << std::endl
                 << " level " << li << ',' << lj << ',' << l[i][j] << std::endl
                 << " rest  " << ri << ',' << rj  << std::endl
                 << " iijj " << ii << ',' << jj  << std::endl
                 << " width  " << w[i][j]  << std::endl
                 << " support " 
                 << '[' 
                 << '(' << std::setw(2) << lXlo << ',' << std::setw(2) << lXhi << ')'
                 << '(' << std::setw(2) << lYlo << ',' << std::setw(2) << lYhi << ')'
                 << ']' 
                 << std::endl;
       std::cout << "sign: " << std::endl;
       for(uint k = 0; k < n; ++k) {
         std::cout << std::setw(2) << (n - k - 1) << "  ";
         for(uint l = 0; l < n; ++l) {
           std::cout << lSignChar[sign[n - k - 1][l]];
         }
         std::cout << std::endl;
       }

    }
  }

  std::cout << "l: " << std::endl;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
       std::cout << std::setw(2) << l[n-i-1][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "w: " << std::endl;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
       std::cout << std::setw(2) << w[n-i-1][j] << ' ';
    }
    std::cout << std::endl;
  }

}


// get original A(k,l) from matrix W of wavelet coefficients
// n == W.noRows() == W.noCols()

double
inverseCoeff(const Matrix& W, const uint k, const uint l, const uint n, bool aTrace = false) {
  bool lTrace = aTrace;
  double lRes = 0;

  uint w = 0; // width
  uint L = 0; // level
  uint lSign = 0; // sign

  static const char lSignChar[3] = {'+', '-', '.'};


  if(lTrace) {
    std::cout << "inverseCoeff: "
              << "  k = " << k << ' '
              << "  l = " << l << ' '
              << "  n = " << n << ' '
              << std::endl;
  }

  // i and j are the indices of some wavelet coefficient W(i,j)
  // ii and jj are the indices of the lower left corner of the support area
  // w[i][j] is the width of the support area
  // sign[k][l] is the sign of the support area, two if outside support area, i.e.
  //            if we reconstruct A(k = ii+c, l = jj+c) for 0 <= c < w[i][j] then
  //            we must add W(i,j) with sign sign[k][l] to the result
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
       // W(i,j) is assumed to be the Wavelet coefficient
       // -- whose support area needs to be determined
       // -- whose sign to reconstruct some A(k,l) needs to be determined

       // first, we get the level of the wavelet coefficient
       const uint li = getLevel(i);
       const uint lj = getLevel(j);
       const uint L  = std::max<uint>(li, lj);
       
       // the rectangle '[(lXlo, lYlo), (lXhi, lYhi)]' will
       // be the support area of W(i,j)
       uint lXlo = 0;
       uint lXhi = 0;
       uint lYlo = 0;
       uint lYhi = 0;

       // the bit at the level position is reset in the
       // the rest of indices i and j, called ri and rj.

       uint ri = 0; // rest (highest bit set to zero)
       uint rj = 0; // rest (highest bit set to zero)

       uint ii = 0; // ursprungsindex
       uint jj = 0; // ursprungsindex

       // the quadrant of the wavelet coefficient determines
       // the case for the sign:
       // wavelet coefficient is from quadrant qq
       // determins the sign of the quartered support area (qsa
       //   qq   qsa
       //
       //   01   +-
       //        +-
       //
       //   10   --
       //        ++
       //
       //   11   -+
       //        +-
       //
       uint bi; // level bit of i
       uint bj; // level bit of j

       uint lQuadrant = 0; // (sub) quadrant (0,0),(0,1),(1,0),(1,1) of k,l

       bool lInside = false;
       if(0 == L) {
         lXlo = 0; lXhi = n;
         lYlo = 0; lYhi = n;
         ri = 0; rj = 0;
         w = n;
         lSign = 0;
         lRes += W(0,0);
         lInside = true;
         if(lTrace) {
           std::cout << "added (00): " << W(0,0) << std::endl;
         }
       } else {
         if(1 == L) {
           lXlo = 0; lXhi = n;
           lYlo = 0; lYhi = n;
           ri = 0; rj = 0;
           w = n;
           bi = (i & 0x1);
           bj = (j & 0x1);
         } else {
           ri = i & ((1 << (L-1)) - 1);
           rj = j & ((1 << (L-1)) - 1);
           w = (n >> (L - 1));
           ii = (ri << (idx_highest_bit_set(n) - L + 1));
           jj = (rj << (idx_highest_bit_set(n) - L + 1));
           lXlo = jj;
           lYlo = ii;
           lXhi = lXlo + w - 1;
           lYhi = lYlo + w - 1;
           bi = ((i >> (L - 1)) & 0x1);
           bj = ((j >> (L - 1)) & 0x1);
         }
         if(    ((lYlo <= k) && (k <= lYhi))
             && ((lXlo <= l) && (l <= lXhi)) ) {
           const uint qk = (k >= (lYlo + (w >> 1)));
           const uint ql = (l >= (lXlo + (w >> 1)));
           lQuadrant =   (qk) | (ql << 1);
           lSign = 1 - wsign(bi, bj, lQuadrant);
           double C[2] = {W(i,j), -W(i,j)};
           lRes += C[lSign];
           lInside = true;
           if(lTrace) {
              std::cout << "added: " << C[lSign] << " , res = " << lRes << std::endl;
           }
         } else {
           lSign = 2;
           lInside = false;
         }
       }

       if(lTrace && lInside) {
         std::cout << (lInside ? "inside: " : "outside: ") 
                       << '(' << i << ',' << j << ')' 
                       << " W(i,j) = " << std::setw(8) << W(i,j) 
                       << " sign = " << lSign
                       << " res = " << lRes
                       << std::endl
                   << "  L = " << L << std::endl
                   << "  w = " << w << std::endl
                   << "l2n = " << idx_highest_bit_set(n) << std::endl
                   << "  i = " << i << std::endl
                   << "  j = " << j << std::endl
                   << "  L = " << L << std::endl
                   << " ri = " << ri << std::endl
                   << " rj = " << rj << std::endl
                   << "klo = " << lYlo << std::endl
                   << "khi = " << lYhi << std::endl
                   << "llo = " << lXlo << std::endl
                   << "lhi = " << lXhi << std::endl
                   << " bi = " << ri << std::endl
                   << " bj = " << rj << std::endl
                   << "qud = " << lQuadrant << std::endl
                   << "sign= " << lSign << std::endl
                   << std::endl;
       }
    }
  }

  return lRes;
}




void
process(const Matrix& I, 
        const double aEpsMin = 0, const double aEpsMax = 0, const double aEpsStep = 0,
        bool aPrintMatrix = true) {


  if(aPrintMatrix) {
    std::cout << "I: " << std::endl;
    I.printDown(std::cout, 8);
  }


  WaveletTransformNonStd2dim lWt;

  Matrix W;
  computeWavelet(I,W);
  if(aPrintMatrix) {
    std::cout << "W: " << std::endl;
    W.printDown(std::cout, 8);
  }

  Matrix W2;
  lWt.transform(W2, I);
  if(aPrintMatrix) {
    std::cout << "W2: " << std::endl;
    W2.printDown(std::cout, 8);
  }

  if(!W.equalUpTo(W2, 0.01)) {
    std::cout << "BAD BAD: W and W2 differ" << std::endl;
    exit(-1);
  }

  Matrix WI;
  computeInverseWavelet(W, WI);

  if(aPrintMatrix) {
    std::cout << "WI: " << std::endl;
    WI.printDown(std::cout, 8);
  }

  if(!I.equalUpTo(WI, 0.1)) {
    std::cerr << "BAD BAD: computeWaveletTransform \\circ computeInverseWavelet != I" << std::endl;
    exit(-1);
  }

  Matrix WI2;
  lWt.inverse(WI2, W2);
  if(aPrintMatrix) {
    std::cout << "WI2: " << std::endl;
    WI2.printDown(std::cout, 8);
  }

  if(!I.equalUpTo(WI2, 0.01)) {
    std::cerr << "BAD BAD: WaveletTransformNonStd2dim::inverse" << std::endl;
    exit(-1);
  }


  // this part of the code should be deleted
  if(false && 0 < aEpsStep) {
    std::cout << std::setw(8) << "w * w" << ' '
              << std::setw(8) << "epsilon" << ' '
              << std::setw(8) << "no coeff" << ' '
              << std::setw(8) << "q-error"  << ' '
              << std::setw(9) << "abs-error"  << ' '
              << std::setw(9) << "exp(absE)"  << ' '
              << std::endl;
    // same on truncated matrix
    for(double lEps = aEpsMin; lEps <= aEpsMax; lEps += aEpsStep) {
      W.setZeroSmallerThan(lEps);
      Matrix WIT;
      // std::cout << "truncated wavelet coefficients: " << std::endl;
      // W.printDown(std::cout, 8);
      computeInverseWavelet(W, WIT);
      // std::cout << "WI on truncated W: " << std::endl;
      // WIT.printDown(std::cout, 6);
      const double lQError = I.qerror(WIT);
      const double lAbsError = I.abserror(WIT);
      std::cout << std::setw(8) << (W.noCols()*W.noCols()) << ' '
                << std::setw(8) << lEps << ' '
                << std::setw(8) << W.countNonZeroElements() << ' '
                << std::setw(8) << lQError << ' '
                << std::setw(8) << lAbsError << ' '
                << std::setw(8) << exp(lAbsError) << ' '
                << std::endl;
    }
  }


  const uint n = I.noRows();

  // check inverseCoeff
  bool lCheckInverseCoeff = true;
  if(lCheckInverseCoeff) {
    for(uint k = 0; k < I.noRows(); ++k) {
      for(uint l = 0; l < I.noCols(); ++l) {
        const double a = I(k,l);
        const double b = inverseCoeff(W, k, l, n);
        if(a != b) {
           std::cout << "original = " << a << " != " << b << " = inverseCoeff" << std::endl;
           exit(-1);
        }
      }
    }
    std::cout << "Check inverseCoeff: " << "o.k." << std::endl; 
  }

  typedef WaveletTransformNonStd2dim::coeff_t  wcoeff_t;
  typedef WaveletTransformNonStd2dim::coeff_vt wcoeff_vt;

  wcoeff_vt lCoeffs;
  lCoeffs.clear();
  for(uint i = 0; i < W2.noRows(); ++i) {
    for(uint j = 0; j < W2.noCols(); ++j) {
      lCoeffs.push_back(wcoeff_t(i, j, W2(i,j)));
    }
  }


  bool lCheckWtInverseCoeff = true;
  if(lCheckWtInverseCoeff) {
    bool lOk = true;
    for(uint k = 0; k < I.noRows(); ++k) {
      for(uint l = 0; l < I.noCols(); ++l) {
        const double a = I(k,l);
        const double b = lWt.inverseCoeff(lCoeffs, k, l, n);
        if(a != b) {
           std::cout << "original = " << a << " != " << b << " = inverseCoeff" << std::endl;
           const double x = inverseCoeff(W, k, l, n, true);
           std::cout << "     out = " << x << std::endl;
           std::cout << "    orig = " << a << std::endl;
           lOk = false;
        }
      }
    }
    std::cout << "Check lWt.inverseCoeff: ";
    if(lOk) {
      std::cout << "o.k." << std::endl;
    } else {
      std::cout << "BAD BAD" << std::endl;
    }
  }

  return;

  // now check for different numbers of coefficients retained:
  std::cout << "# error check" << std::endl;
  for(uint lToRetain = n; lToRetain <= (n*n); ++lToRetain) {
    lCoeffs.clear();
    double lMaxQError = 0;
    lWt.extractTopK(lCoeffs, W, lToRetain);
    for(uint k = 0; k < I.noRows(); ++k) {
      for(uint l = 0; l < I.noCols(); ++l) {
        const double a = std::max<double>(1, I(k,l));
        const double b = std::max<double>(1, lWt.inverseCoeff(lCoeffs, k, l, n));
        const double lQError = q::qerror(a,b);
        if(lQError > lMaxQError) {
          lMaxQError = lQError;
          // std::cout << "ab: " << a << ' ' << b << std::endl;
        }
      }
    }
    std::cout << lCoeffs.size() << ' ' 
              << ((double) lCoeffs.size() / (double) (n*n)) << ' '
              << lMaxQError << ' ' 
              << lToRetain << ' ' 
              << std::endl;
  }
  
}


/*
 ./a.out zM? 
 produziert zM?.res

 set logscale y
 plot 'zM4.res' using 2:4, 'zM4.res' using 2:6, 'zM4.res' using 2:8, 'zM4.res' using 2:10
 plot 'zM5.res' using 2:4, 'zM5.res' using 2:6, 'zM5.res' using 2:8
 plot 'zM6.res' using 2:4, 'zM6.res' using 2:6, 'zM6.res' using 2:8
 plot 'zM7.res' using 2:4, 'zM7.res' using 2:6, 'zM7.res' using 2:8
 plot 'zM8.res' using 2:4, 'zM8.res' using 2:6, 'zM8.res' using 2:8
 plot 'zM9.res' using 2:4, 'zM9.res' using 2:6, 'zM9.res' using 2:8

 plot 'zM4.res' using 3:4, 'zM4.res' using 5:6, 'zM4.res' using 7:8
 plot 'zM5.res' using 3:4, 'zM5.res' using 5:6, 'zM5.res' using 7:8
 plot 'zM6.res' using 3:4, 'zM6.res' using 5:6, 'zM6.res' using 7:8
 plot 'zM7.res' using 3:4, 'zM7.res' using 5:6, 'zM7.res' using 7:8
 plot 'zM8.res' using 3:4, 'zM8.res' using 5:6, 'zM8.res' using 7:8
 plot 'zM9.res' using 3:4, 'zM9.res' using 5:6, 'zM9.res' using 7:8


set logscale y

set title '16x16'
plot 'zM4.res' using 2:4 title 'regular', 'zM4.res' using 2:6 title 'normalized', 'zM4.res' using 2:8 title 'ln', 'zM4.res' using 2:10 title 'ln norm', 'zM4.res' using 2:12 title 'cumfreq'


set title '32x32'
plot 'zM5.res' using 3:4 title 'regular', 'zM5.res' using 5:6 title 'normalized', 'zM5.res' using 7:8 title 'ln', 'zM5.res' using 9:10 title 'ln norm', 'zM5.res' using 11:12 title 'cumfreq'

set title '64x64'
plot 'zM6.res' using 3:4 title 'regular', 'zM6.res' using 5:6 title 'normalized', 'zM6.res' using 7:8 title 'ln', 'zM6.res' using 9:10 title 'ln norm', 'zM6.res' using 11:12 title 'cumfreq'

set title '128x128'
plot 'zM7.res' using 3:4 title 'regular', 'zM7.res' using 5:6 title 'normalized', 'zM7.res' using 7:8 title 'ln', 'zM7.res' using 9:10 title 'ln norm', 'zM7.res' using 11:12 title 'cumfreq'

set title '256x256'
plot 'zM8.res' using 3:4 title 'regular', 'zM8.res' using 5:6 title 'normalized', 'zM8.res' using 7:8 title 'ln', 'zM8.res' using 9:10 title 'ln norm', 'zM8.res' using 11:12 title 'cumfreq'

set title '512x512'
plot 'zM9.res' using 3:4 title 'regular', 'zM9.res' using 5:6 title 'normalized', 'zM9.res' using 7:8 title 'ln', 'zM9.res' using 9:10 title 'ln norm', 'zM9.res' using 11:12 title 'cumfreq'

*/

void
fCheckError(const Matrix& A, const char* aFileNameIn, const double aAlpha, const double aTheta,
            std::ostream& os) {
  const double lTheta = 0;
  const double lAlpha = 4;

  typedef WaveletTransformNonStd2dim::coeff_vt coeff_vt;
  WaveletTransformNonStd2dim           lWt;
  coeff_vt lCoeffsW;  // regular wavelet transform
  coeff_vt lCoeffsWN; // regular wavelet transform, normalized
  coeff_vt lCoeffsV;  // ln(A(i,j)+alpha)
  coeff_vt lCoeffsVN;  // ln(A(i,j)+alpha), normalized
  coeff_vt lCoeffsU;  // cumulated

  Matrix W;     // unnormalized wavelet coefficients
  lWt.transform(W, A);
  Matrix WN(W); // normalized
  WaveletTransformNonStd2dim::normalize(WN);

  Matrix B(A);
  B.addToAll(lAlpha);
  B.lnAll();
  Matrix V;
  lWt.transform(V, B);
  Matrix VN(V);
  WaveletTransformNonStd2dim::normalize(VN);

  Matrix C(A);
  C.accumulateInclusive();
  Matrix U;
  lWt.transform(U, C);

  const uint n = A.noRows();
 
  std::cout << "# error check " << aFileNameIn << std::endl;
  for(uint lToRetain = 1; lToRetain <= (n*n); ++lToRetain) {
    if(3000 < lToRetain) { break; }
    lCoeffsW.clear();
    lCoeffsWN.clear();
    lCoeffsV.clear();
    lCoeffsVN.clear();
    lCoeffsU.clear();
    double lMaxQErrorW = 1;
    double lMaxQErrorWN = 1;
    double lMaxQErrorV = 1;
    double lMaxQErrorVN = 1;
    double lMaxQErrorU = 1;
    lWt.extractTopK(lCoeffsW, W, lToRetain);
    lWt.extractTopK(lCoeffsWN, WN, lToRetain);
    lWt.denormalize(lCoeffsWN);
    lWt.extractTopK(lCoeffsV, V, lToRetain);
    lWt.extractTopK(lCoeffsVN, VN, lToRetain);
    lWt.denormalize(lCoeffsVN);
    lWt.extractTopK(lCoeffsU, U, lToRetain);
    for(uint k = 0; k < A.noRows(); ++k) {
      for(uint l = 0; l < A.noCols(); ++l) {
        const double a  = std::max<double>(1, A(k,l));
        const double b  = std::max<double>(1, lWt.inverseCoeff(lCoeffsW, k, l, n));
        const double bn = std::max<double>(1, lWt.inverseCoeff(lCoeffsWN, k, l, n));
        const double c  = std::max<double>(1, exp(lWt.inverseCoeff(lCoeffsV, k, l, n)) - lAlpha);
        const double cn = std::max<double>(1, exp(lWt.inverseCoeff(lCoeffsVN, k, l, n)) - lAlpha);
        const double d  = std::max<double>(1, C(k,l));
        const double e  = std::max<double>(1, lWt.inverseCoeff(lCoeffsU, k, l, n));
        const double lQErrorW  = q::qerror(a,b);
        const double lQErrorWN = q::qerror(a,bn);
        const double lQErrorV  = q::qerror(a,c);
        const double lQErrorVN = q::qerror(a,cn);
        const double lQErrorU  = q::qerror(d,e);
        if(   ((a > lTheta) || (b > lTheta))
           && (lQErrorW > lMaxQErrorW)) {
          lMaxQErrorW = lQErrorW;
        }
        if(   ((a > lTheta) || (bn > lTheta))
           && (lQErrorWN > lMaxQErrorWN)) {
          lMaxQErrorWN = lQErrorWN;
        }
        if(   ((a > lTheta) || (c > lTheta))
           && (lQErrorV > lMaxQErrorV)) {
          lMaxQErrorV = lQErrorV;
        }
        if(   ((a > lTheta) || (c > lTheta))
           && (lQErrorVN > lMaxQErrorVN)) {
          lMaxQErrorVN = lQErrorVN;
        }
        if(   ((d > lTheta) || (e > lTheta))
           && (lQErrorU > lMaxQErrorU)) {
          lMaxQErrorU = lQErrorU;
        }
      }
    }
    os << lToRetain << ' '
       << ((double) lToRetain / (double) (n*n)) << ' '
       << lCoeffsW.size() << ' '
       << lMaxQErrorW << ' '
       << lCoeffsWN.size() << ' '
       << lMaxQErrorWN << ' '
       << lCoeffsV.size() << ' '
       << lMaxQErrorV << ' '
       << lCoeffsVN.size() << ' '
       << lMaxQErrorVN << ' '
       << lCoeffsU.size() << ' '
       << lMaxQErrorU << ' '
       << std::endl;
  }
}


void
test0() {
  Matrix I(4, 4,
           1.0,  2.0,  2.0, 4.0,
           3.0,  4.0,  6.0, 8.0,
           3.0,  6.0,  4.0, 8.0,
           9.0, 12.0, 12.0, 16.0
          );
  process(I, 0.5, 2.5, 0.5);
}

void
test1() {
  Matrix I(8,8,
             47.0,   96.0, 1178.0,    0.0,    8.0,    0.0,  536.0,    1.0,
             63.0,   94.0,   70.0,   51.0,  385.0,  129.0,   49.0,    1.0,
             38.0,  194.0,  524.0,  350.0,  317.0,    2.0,    2.0,    3.0,
            262.0,   30.0,   10.0,  108.0,   33.0,   44.0,   16.0,   16.0,
             28.0,   30.0,    7.0,   28.0,   33.0,  245.0,    0.0,   12.0,
              1.0,   71.0,   41.0,   51.0,   61.0,  393.0,    2.0,    1.0,
              0.0,   35.0,   21.0, 1578.0,  784.0,  296.0,   28.0,   10.0,
            167.0,  174.0,  887.0, 1511.0,  238.0, 1160.0,  471.0,    0.0
          );
  process(I, 1.0, 5.0, 0.5);
}


void
process2(const Matrix& M, const double aEpsMin, const double aEpsMax, const double aEpsStep) {
  const bool lIsSmall = (8 > M.noRows());
  if(lIsSmall) {
    process(M, aEpsMin, aEpsMax, aEpsStep, true);
  } else {
    process(M, aEpsMin, aEpsMax, aEpsStep, false);
  }
}

void
test2(const char* aFileName) {
  std::ifstream lIs(aFileName);
  if(!lIs) {
    std::cerr << "can't open '" << aFileName << '\'' << std::endl;
    return;
  }
  Matrix M;
  M.initFromStream(lIs);

  std::cout << "processing '" << aFileName << '\'' << std::endl;
  process2(M, 0.1, 2.0, 0.1);

  Matrix Ma(M);
  Ma.accumulateInclusive();
  std::cout << "processing '" << aFileName << "\' cumulated" << std::endl;
  process2(Ma, 10.0, 100.0, 10.0);

  Matrix Mb(M);
  Mb.addToAll(20);  
  std::cout << "processing '" << aFileName << "\' plus alpha (20)" << std::endl;
  process2(Mb, 1.0, 10.0, 1.0);

  Matrix Mc(Mb);
  std::cout << "processing '" << aFileName << "\' log of plus alpha (20)" << std::endl;
  process2(Mc, 0.5, 2.0, 0.1);
}

void
test3(const char* aFileName, const double aAlpha, const double aTheta) {
 std::ifstream lIs(aFileName);
  if(!lIs) {
    std::cerr << "can't open '" << aFileName << '\'' << std::endl;
    return;
  }
  Matrix A;
  A.initFromStream(lIs);

  char lBuff[100];
  snprintf(lBuff, 99, "%s.res", aFileName);

  std::ofstream lOs(lBuff);
  fCheckError(A, aFileName, aAlpha, aTheta, lOs);
}


int
main(const int argc, const char* argv[]) {
  if(2 > argc) {
    test0();
    test1();
  }
  for(uint i = 1; i < argc; ++i) {
    // test2(argv[i]);
    test3(argv[i], 10, 130);
  }
  // testSupport( 4);
  // testSupport( 8);
  // testSupport(16);
  // PrintSignCase();
  return 0;
}


