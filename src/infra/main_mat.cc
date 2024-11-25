#include <iostream>
#include <iomanip>

#include "../infra/matrix.hh"

void
test0() {
  std::cout << "test 0:" << std::endl;

  double x[] = {4, 2, -2,
                2, 1, -1,
               -2, -1, 10};
  // double x[] = {  4,  2,  1,
  //                 2,  7,  2,
  //                 1,  2,  8 };
  Matrix A(3,3,x);
  Matrix T(3,3);
  Matrix TT(3,3);
  Matrix U(3,3); // inverse of T
  Matrix UU(3,3); // UU'
  Matrix  B(3,3); // AUU'

  T.setToSquareRootUpperTriangular(A);
  TT.setToProductWithFirstTransposed(T, T);
  U.setToInverseOfUpperTriangular(T,0.0000000001);
  UU.setToMMt(U);
  B.setToProductOf(A,UU);

  std::cout << "A = " << std::endl;
  A.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "T = " << std::endl;
  T.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "T'T = " << std::endl;
  TT.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "U = " << std::endl;
  U.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "UU' = " << std::endl;
  UU.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "B = " << std::endl;
  B.print(std::cout, 3);
  std::cout << std::endl;
  
  std::cout << std::endl;
}

void
test1() {
  std::cout << "test 1" << std::endl;
  double X[] = {1, 1, 0,
                3, 0, 4,
                0, 7, 7};
  Matrix Apre(3,3,X);
  Matrix A;
  A.setToMMt(Apre);

  double b[3] = {2, 3, 4};

  Matrix U(3,3);
  Matrix UU(3,3); // U'U
  Matrix Uinv(3,3); // inverse of U
  Matrix UUinv(3,3); // UUinv
  Matrix  Bu(3,3); 

  std::cout << "Apre = " << std::endl;
  Apre.print(std::cout, 3);
  std::cout << std::endl;

  std::cout << "A = " << std::endl;
  A.print(std::cout, 3);
  std::cout << std::endl;

  U.setToSquareRootUpperTriangular(A);
  UU.setToProductWithFirstTransposed(U, U);

  Uinv.setToInverseOfUpperTriangular(U,0.0000000001);
  UUinv.setToProductOf(U, Uinv);

  Bu.setToProductOf(A,UUinv);

  std::cout << "U = " << std::endl;
  U.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << "U'U = " << std::endl;
  UU.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << "U^-1 = " << std::endl;
  Uinv.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << "U^-1U^-1' = " << std::endl;
  UUinv.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << "Bu = " << std::endl;
  Bu.print(std::cout, 9);
  std::cout << std::endl;


  Matrix L(3,3);
  Matrix LL(3,3); // LL'
  double  xl[3] = {0, 0, 0};

  L.setToSquareRootLowerTriangular(A);
  LL.setToProductWithSecondTransposed(L, L);

  std::cout << "L = " << std::endl;
  L.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << "L'L = " << std::endl;
  LL.print(std::cout, 9);
  std::cout << std::endl;

  L.solveSquareRootLowerTriangular(xl, b);
  std::cout << "xl: ";
  Matrix::printVector(xl, 3);
  std::cout << std::endl;

  Matrix Ainv;
  Ainv.inverseSquareRootLowerTriangular(L);
  Matrix AinvA;
  AinvA.setToProductOf(A, Ainv);
  std::cout << "Ainv: " << std::endl;
  Ainv.print(std::cout, 9);
  std::cout << "AinvA: " << std::endl;
  AinvA.print(std::cout, 9);  
  std::cout << std::endl;

  Matrix Ampi(3,3);
  Ampi.setToMoorePenroseInverseOf(A, 0.0000000001);
  Matrix AmpiA(3,3);
  AmpiA.setToProductOf(A, Ampi);
  std::cout << "Ampi:" << std::endl;
  Ampi.print(std::cout, 9);
  std::cout << "AmpiA:" << std::endl;
  AmpiA.print(std::cout, 9);
  std::cout << std::endl;

  std::cout << std::endl;

}


void
test2() {
  double x[] = {0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 0, 0,
                0, 1, 1, 1, 0, 0,
                0, 1, 1, 1, 0, 0,
                0, 1, 1, 1, 0, 0,
                0, 0, 0, 0, 0, 0};
  Matrix A(6, 6, x); 
  std::cout << "noZeroLoRows: " << A.noZeroLoRows() << std::endl
            << "noZeroHiRows: " << A.noZeroHiRows() << std::endl
            << "noZeroLoCols: " << A.noZeroLoCols() << std::endl
            << "noZeroHiCols: " << A.noZeroHiCols() << std::endl
            << std::endl;
}


int
main() {
  double x[] = {1, 2, 4,
                0, 2, 5,
                0, 0, 6};
  Matrix A(3,3, x);

  Matrix B(3,3);
  Matrix C(3,3);

  std::cout << "A = " << std::endl;
  A.print(std::cout, 3);
  std::cout << std::endl;

  B.setToInverseOfUpperTriangular(A, 0.00000000001);

  std::cout << "B = " << std::endl;
  B.print(std::cout, 3);
  std::cout << std::endl;

  C.setToProductOf(A,B);
  C.setZeroSmallerThan(0.00000000001);
  std::cout << "C = " << std::endl;
  C.print(std::cout, 3);
  std::cout << std::endl;

  test0();
  test1();

  test2();
}
