#include "matrix.hh"


void
test0() {

  Matrix M(5,5);
  M(0,0) = 1.111111111111111e-01;
  M(0,1) = 3.703703703703703e-02;
  M(0,2) = 2.469135802469136e-02;
  M(0,3) = 0e+00                ;
  M(0,4) = 4.938271604938271e-02;
  M(1,0) = 1.111111111111111e-01;
  M(1,1) = 3.703703703703704e-02;
  M(1,2) = 2.469135802469136e-02;
  M(1,3) = 4.938271604938271e-02;
  M(1,4) = 0e+00                ;
  M(2,0) = 0e+00                ;
  M(2,1) = 3.703703703703703e-02;
  M(2,2) = 2.469135802469136e-02;
  M(2,3) = 0e+00                ;
  M(2,4) = 4.938271604938271e-02;
  M(3,0) = 0e+00                ;
  M(3,1) = 0e+00                ;
  M(3,2) = 0e+00                ;
  M(3,3) = 0e+00                ;
  M(3,4) = 0e+00                ;
  M(4,0) = 0e+00                ;
  M(4,1) = 0e+00                ;
  M(4,2) = 0e+00                ;
  M(4,3) = 0e+00                ;
  M(4,4) = 0e+00                ;


  std::cout << "original:" << std::endl;
  M.printWithRowColSum(std::cout, 12);

  double lSum = M.sumAllElements();
  std::cout << "Total: " << lSum << std::endl;

  M.multiplyAllBy(1/lSum);
  std::cout << "normalized:" << std::endl;
  M.printWithRowColSum(std::cout, 12);
}


int
main() {
  test0();
  return 0;
}


