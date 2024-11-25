#include <iostream>
#include <iomanip>

#include "infra/matrix.hh"

#include "gmsvd.hh"




int
svd(Matrix& A) {
  std::cout << "A: " << std::endl;
  A.print(std::cout, 12);
  std::cout << std::endl;

  GMSVD lSvd(A);

  std::cout << "rank   : " << lSvd.rank() << std::endl
            << "nullity: " << lSvd.nullity()
            << std::endl;

  Matrix U, W, V;
  lSvd.get_U(U);
  lSvd.get_W(W);
  lSvd.get_V(V);
  std::cout << "U: " << std::endl;
  U.print(std::cout, 12);
  std::cout << std::endl;

  std::cout << "W: " << std::endl;
  W.print(std::cout, 12);
  std::cout << std::endl;

  std::cout << "V: " << std::endl;
  V.print(std::cout, 12);
  std::cout << std::endl;

  Matrix UU, VV;
  UU.setToProductWithFirstTransposed(U,U);
  std::cout << "U^T U: " << std::endl;
  UU.print(std::cout, 12);
  std::cout << std::endl;
  VV.setToProductWithFirstTransposed(V,V);
  std::cout << "V^T V: " << std::endl;
  VV.print(std::cout, 12);
  std::cout << std::endl;
  return 0;
}




int
test0() {
  Matrix A(3,2);

  A(0,0) = 1;
  A(1,0) = 1;
  A(2,0) = 1;
  A(0,1) = 1;
  A(1,1) = 2;
  A(2,1) = 3;

  std::cout << "A: " << std::endl;
  A.print(std::cout, 12);
  std::cout << std::endl;

  Matrix b(3,1);

  b(0,0) = 20;
  b(1,0) = 10;
  b(2,0) = 60;

  std::cout << "b: " << std::endl;
  b.print(std::cout, 12);
  std::cout << std::endl;
  Matrix x(2,1);

  GMSVD lSvd(A);
  lSvd.solve(b, x);

  std::cout << "rank   : " << lSvd.rank() << std::endl
            << "nullity: " << lSvd.nullity()
            << std::endl;

  Matrix U, W, V;
  lSvd.get_U(U);
  lSvd.get_W(W);
  lSvd.get_V(V);
  std::cout << "U: " << std::endl;
  U.print(std::cout, 12);
  std::cout << std::endl;

  std::cout << "W: " << std::endl;
  W.print(std::cout, 12);
  std::cout << std::endl;

  std::cout << "V: " << std::endl;
  V.print(std::cout, 12);
  std::cout << std::endl;

  Matrix UU, VV;
  UU.setToProductWithFirstTransposed(U,U);
  std::cout << "U^T U: " << std::endl;
  UU.print(std::cout, 12);
  std::cout << std::endl;
  VV.setToProductWithFirstTransposed(V,V);
  std::cout << "V^T V: " << std::endl;
  VV.print(std::cout, 12);
  std::cout << std::endl;

  std::cout << "x: " << std::endl;
  x.print(std::cout, 12);
  std::cout << std::endl;

  Matrix f(3,1);
  f.setToProductOf(A,x);
  std::cout << "Ax: " << std::endl;
  f.print(std::cout, 12);
  std::cout << std::endl;

  {
    const double a = x(1,0);
    const double b = x(0,0);
    const double d1 = 20.0 - (a * 1 + b);
    const double d2 = 10.0 - (a * 2 + b);
    const double d3 = 60.0 - (a * 3 + b);
    std::cout << "L2^2: " << (d1*d1 + d2*d2 + d3*d3) << std::endl;
  }

  Matrix Mpi;
  lSvd.get_mpi(Mpi);
  std::cout << "MPI from SVD: " << std::endl;
  Mpi.print(std::cout, 12);
  Matrix MpiDirect;
  MpiDirect.setToMoorePenroseInverseOf(A);
  std::cout << "MPI with Greville: " << std::endl;
  MpiDirect.print(std::cout, 12);
  
  return 0;
}

int
test1() {
  Matrix A;


  return 0;
}

int
run(std::istream& is) {
  int m,n;
  is >> m >> n;
  Matrix M;
  M.initFromStream(is, m, n);
  svd(M);

  return 0;
}


int
main() {
  test0();
  test1();

  // run(std::cin);

  return 0;
}

