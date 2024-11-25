#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

// #include <gsl/gsl_linalg.h>

#include "matrix.hh"

typedef std::vector<double> double_vt;

double_vt&
diff(double_vt& x, const double_vt& y, const double_vt& z) {
  assert(y.size() <= z.size());
  x.resize(y.size());
  for(uint i = 0; i < y.size(); ++i) {
    x[i] = y[i] - z[i];
  }
  return x;
}

std::ostream&
operator<<(std::ostream& os, const double_vt& x) {
  os << '[' << x[0];
  
  for(uint i = 1; i < x.size(); ++i) {
    os << ' ' << x[i];
  }
  os << ']';
  return os;
}

bool
readRaw(std::istream& aIs, Matrix& A, double_vt& b) {
  A.initFromStream(aIs);
  const uint m = A.noRows();
  // const uint n = A.noCols();
  for(uint i = 0; i < m; ++i) {
    double x = 0;
    aIs >> x;
    b.push_back(x);
  }
  return true;
}

void
fQR(const Matrix& A, const double_vt& b) {

  const uint m = A.noRows();
  const uint n = A.noCols();

  std::cout << "A = " << std::endl;
  A.print(std::cout, 3);

  std::cout << "b = " << b << std::endl;

  Matrix Q;
  Matrix R;
  A.qr_mgs(Q,R);
  std::cout << "Q = " << std::endl;
  Q.print(std::cout, 3);
  std::cout << "R = " << std::endl;
  R.print(std::cout, 3);

  Matrix QR;
  QR.setToProductOf(Q,R);
  std::cout << "QR = " << std::endl;
  QR.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "q-error(A, QR) = " << QR.qerror(A) << std::endl;

  Matrix QQt;
  QQt.setToAxBt(Q,Q);
  std::cout << "QQ' = " << std::endl;
  QQt.print(std::cout, 3);

  Matrix QtQ;
  std::cout << "Q'Q = " << std::endl;
  QtQ.setToAtxB(Q,Q);
  QtQ.print(std::cout, 3);


  Matrix R1;
  R1.setToSubmatrix(R, m, m);
  std::cout << "R1 = " << std::endl;
  R1.print(std::cout, 3);

  Matrix R1inv;
  R1inv.setToMoorePenroseInverseOf(R1);
  std::cout << "R1^{-1} = " << std::endl;
  R1inv.print(std::cout, 3);

  Matrix R1R1inv;
  R1R1inv.setToAxB(R1, R1inv);
  std::cout << "R1 R1^{-1} = " << std::endl;
  R1R1inv.print(std::cout, 3);

  Matrix R1invR1;
  R1invR1.setToAxB(R1inv, R1);
  std::cout << "R1^{-1} R1 = " << std::endl;
  R1invR1.print(std::cout, 3);

  double_vt x0(n), z0(n), b0(m), d0(m);
 
  std::cout << std::endl;
  std::cout << "Solution by QR-Decomposition" << std::endl;
  std::cout << std::endl;
  Matrix::productOfAtV(z0.data(), Q, b.data());
  Matrix::productOf(x0.data(), R1inv, z0.data());
  Matrix::productOf(b0.data(), A, x0.data());
  diff(d0, b, b0);

  std::cout << "x0 = " << x0 << std::endl;
  std::cout << "b0 = " << b0 << std::endl;
  std::cout << "d0 = " << d0 << std::endl;


  // 
  std::cout << std::endl;
  std::cout << "Solution by Mixed QR-Decomposition/Moore-Penrose_Inverse" << std::endl;
  std::cout << std::endl;

  Matrix Rinv;
  Rinv.setToMoorePenroseInverseOf(R);
  std::cout << "R^+ = " << std::endl;
  Rinv.print(std::cout, 7);
  std::cout << std::endl;

  double_vt z1(n), x1(n), b1(m), d1(m);
  Matrix::productOfAtV(z1.data(), Q, b.data());
  Matrix::productOf(x1.data(), Rinv, z1.data());
  Matrix::productOf(b1.data(), A, x1.data());
  diff(d1, b, b1);
 
  // std::cout << "z1 = " << z1 << std::endl;
  std::cout << "x1 = " << x1 << std::endl;
  std::cout << "b1 = " << b1 << std::endl;
  std::cout << "d1 = " << d1 << std::endl; 

  // MPI

  std::cout << std::endl;
  std::cout << "Solution by Moore-Penrose-Inverse" << std::endl;
  std::cout << std::endl;

  Matrix Ainv;
  Ainv.setToMoorePenroseInverseOf(A);
  std::cout << "A^+ = " << std::endl;
  Ainv.print(std::cout, 7);

  double_vt x2(n), b2(m), d2(m);
  Matrix::productOf(x2.data(), Ainv, b.data());
  Matrix::productOf(b2.data(), A,    x2.data());
  diff(d2, b, b2);
  std::cout << "x2 = A^+b, b2 = AA^+b: solution exists iff b = b2)" << std::endl;
  std::cout << "x2 = " << x2 << std::endl;
  std::cout << "b2 = " << b2 << std::endl;
  std::cout << "d2 = " << d2 << std::endl;


  const double_vt Ainvb = x2;

  Matrix AinvA;
  AinvA.setToProductOf(Ainv, A);
  std::cout << "A^+A = " << std::endl;
  AinvA.print(std::cout, 8);

  Matrix I_AinvA; // (I - AinvA)
  I_AinvA.resize(AinvA.noRows(), AinvA.noCols());
  for(uint i = 0; i < AinvA.noRows(); ++i) {
    for(uint j = 0; j < AinvA.noCols(); ++j) {
      if(i == j) {
        I_AinvA(i,j) = 1.0 - AinvA(i,j);
      } else {
        I_AinvA(i,j) = -AinvA(i,j);
      }
    }
  }
  std::cout << "(I - A^+A) = " << std::endl;
  I_AinvA.print(std::cout, 8);
  
  double lI_AinvAMin = I_AinvA.absMinNonZero(0.00000001);
  std::cout << "min(I-A^+A) = " << lI_AinvAMin << std::endl;
  if(0.0001 < lI_AinvAMin) {
    for(uint i = 0; i < I_AinvA.noRows(); ++i) {
      for(uint j = 0; j < I_AinvA.noCols(); ++j) {
        I_AinvA(i,j) /= lI_AinvAMin;
      }
    }
  }
  I_AinvA.setZeroSmallerThan(1.0E-15);
  std::cout << "normalized(I - A^+A) = " << std::endl;
  I_AinvA.print(std::cout, 8);
 
  Matrix AAinv;
  AAinv.setToProductOf(A, Ainv);
  std::cout << "AA^+ = " << std::endl;
  AAinv.setZeroSmallerThan(1.0E-15);
  AAinv.print(std::cout, 3);
 
}


/*
 A   \in R^{m,n}, m <= n
 A^t \in R^{n,m}, n >= m

 Q   \in R^{m,n}
 R   \in R^{n,n}

*/

void
fQRT(const Matrix& A, const double_vt& b) {
  const uint m = A.noRows();
  const uint n = A.noCols();

  std::cout << "A = " << std::endl;
  A.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "b = " << b << std::endl;
  std::cout << std::endl;

  Matrix At;
  At.setToTransposedOf(A);
  std::cout << "A' = " << std::endl;
  At.print(std::cout, 3);
  std::cout << std::endl;

  Matrix Q;
  Matrix R;

  At.qr_mgs(Q,R);
  std::cout << "Q(" << Q.noRows() << 'x' << Q.noCols() << ") = " << std::endl;
  Q.print(std::cout, 9);
  std::cout << std::endl;


  std::cout << "R(" << R.noRows() << 'x' << R.noCols() << ") = " << std::endl;
  R.print(std::cout, 9);
  std::cout << std::endl;

  Matrix QR;
  QR.setToProductOf(Q,R);
  std::cout << "QR = " << std::endl;
  QR.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "q-error(At, QR) = " << QR.qerror(At) << std::endl;

  Matrix QQt;
  QQt.setToAxBt(Q,Q);
  std::cout << "QQ' = " << std::endl;
  QQt.print(std::cout, 3);
  std::cout << std::endl;

  Matrix QtQ;
  std::cout << "Q'Q = " << std::endl;
  QtQ.setToAtxB(Q,Q);
  QtQ.print(std::cout, 3);
  std::cout << std::endl;




  Matrix R1;
  R1.setToSubmatrix(R, m, m);
  std::cout << "R1 = " << std::endl;
  R1.print(std::cout, 9);


  Matrix R1inv;
  R1inv.setToMoorePenroseInverseOf(R1);
  std::cout << "R1^{-1} = " << std::endl;
  R1inv.print(std::cout, 9);

  Matrix X;
  X.setToProductOf(R1, R1inv);
  std::cout << "R1R^{-1} = " << std::endl;
  X.print(std::cout, 9);
  X.setToProductOf(R1inv, R1);
  std::cout << "R^{-1}R1 = " << std::endl;
  X.print(std::cout, 9);

  Matrix R_T; // R^{-T} =(?) (R^{-1})^{t}
  R_T.setToTransposedOf(R1inv);
  std::cout << "R_T = " << std::endl;
  R_T.print(std::cout, 9);

  
  double_vt y1(m);
  Matrix::productOf(y1.data(), R_T, b.data());
 
  std::cout << "y1 = " << y1 << std::endl; 

  double_vt y(n);
  for(uint i = 0; i < m; ++i) {
    y[i] = y1[i];
  }
  for(uint i = m; i < n; ++i) {
    y[i] = 0;
  }

  double_vt x(n);
  double_vt bb(m);

  for(double yn = 0; yn <= 1.0; yn += 0.1) {
    y[n-1] = yn;
    Matrix::productOf(x.data(), Q, y.data());
    std::cout << " y = " << y << std::endl;
    std::cout << " x = " << x << std::endl;

    Matrix::productOf(bb.data(), A, x.data());
    std::cout << "bb = " << bb << std::endl;
    std::cout << " b = " << b  << std::endl;
  }
}


int
main(const int argc, const char* argv[]) {
  if(2 != argc) {
    std::cerr << "main_qr <filename>" << std::endl;
    return -1;
  }
  std::string lFilename(argv[1]);
  std::ifstream lIs(lFilename);
  if(!lIs) {
    std::cerr << "can't open file '" << lFilename << "'." << std::endl;
    return -2;
  }

  Matrix    A;
  double_vt b;

  readRaw(lIs, A, b);

  if(A.noRows() >= A.noCols()) {
    fQR(A, b);
  } else {
    fQRT(A, b);
  }

  return 0;
}


/*
A \in R^{m,n} m >= n
Q \in R^{m,m} 
R \in R^{n,n}

A = [a_1 A_2] \in R^{m,n}, m > n

1. spalte von
[a_1 A_2] = [q_1 Q_2] [r11 R12
                       0   R22]

q1 and Q2 must satisfy
[ q1^t
  Q2^t] [q1 Q1] = [q1^t q1, q1^t Q2   =  [1  0
                   Q1^t q1, Q2^t Q1]      0  I]

r11 = sqrt(<a1,a1>)
q1  = (1/r11) a1


*/








