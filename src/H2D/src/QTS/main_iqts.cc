#include <iostream>
#include <iomanip>
#include "IQTS.hh"

#include "infra/bitvectorsmall.hh"
#include "infra/matrix.hh"

void
test0() {
  std::cout << "*** TEST 0 ***" << std::endl;

  for(uint i = 1; i < 10; ++i) {
    for(uint j = i; j < 10; ++j) {
       uint64_t lCode = H2D::IQTS::Node::encode(i, j, 3);
       double   lDeco = H2D::IQTS::Node::decode(lCode, j, 3);
       std::cout << std::setw(2) << i << ' '
                 << std::setw(2) << j << ' '
                 << std::setw(4) << lCode << ' '
                 << std::setw(4) << lDeco << ' '
                 << std::endl;
    }
  }
}


void
test1(int a, int b, int c, int d) {
  std::cout << "*** TEST 1 ***" << std::endl;
  double f00 = a;
  double f01 = b;
  double f10 = c;
  double f11 = d;
  double lTotal = f00 + f01 + f10 + f11;

  uint64_t lCode = H2D::IQTS::Node::encode2x2(f00, f01, f10, f11, lTotal, 6, 5);
  Bitvector32 lCodeBv((uint32_t) lCode);

  double f00d = 0;
  double f01d = 0;
  double f10d = 0;
  double f11d = 0;
  H2D::IQTS::Node::decode2x2(lCode, lTotal, 6, 5, f00d, f01d, f10d, f11d);

  std::cout << std::setw(6) << f00 << ' '
            << std::setw(6) << f01 << ' '
            << std::setw(6) << f10 << ' '
            << std::setw(6) << f11 << ' '
            <<                 lCodeBv << ' '
            << std::setw(6) << f00d << ' '
            << std::setw(6) << f01d << ' '
            << std::setw(6) << f10d << ' '
            << std::setw(6) << f11d << ' '
            << std::endl;
}

void
test2() {
  std::cout << "*** TEST 2 ***" << std::endl;

  Bitvector64 lBv;

  Matrix M(2,2);
  M(0,0) = 7;
  M(0,1) = 13;
  M(1,0) = 17;
  M(1,1) = 19;

  std::cout << "M = " << std::endl;
  M.print(std::cout, 3);
  
  uint64_t lCode = H2D::IQTS::Node::encode2x2(M, 0, 0, 3, 2);
  lBv = lCode;
  std::cout << "lCode: " << std::endl << lBv << std::endl;

  Matrix D(2,2);
  double lTotal = M.sumAllElements();
  H2D::IQTS::Node::decode2x2(lCode, lTotal, 3, 2, D, 0, 0);

  std::cout << "D = " << std::endl;
  D.print(std::cout, 3);

  M(0,0) = 10;
  M(0,1) = 18;
  M(1,0) = 42;
  M(1,1) = 50;

  std::cout << "M = " << std::endl;
  M.print(std::cout, 3);

  lCode = H2D::IQTS::Node::encode2x2(M, 0, 0, 6, 5);
  lBv = lCode;
  std::cout << "lCode: " << std::endl << lBv << std::endl;

  lTotal = M.sumAllElements();
  lBv = lCode;
  std::cout << "lCode: " << std::endl << lBv << std::endl;

  H2D::IQTS::Node::decode2x2(lCode, lTotal, 6, 5, D, 0, 0);

  std::cout << "D = " << std::endl;
  D.print(std::cout, 3);

}


void
test3() {
  std::cout << "*** TEST 3 ***" << std::endl;

  Matrix M(4,4);
  double lTotal = 0;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      M(i,j) = 4*i+j;
      lTotal += M(i,j);
    }
  }

  std::cout << "M:" << std::endl;
  M.print(std::cout, 3);
  const uint64_t lCode = H2D::IQTS::Node::encode4x4(M, 0, 0, 6, 5, 4, 3);

  Matrix D(4,4);
  H2D::IQTS::Node::decode4x4(lCode, lTotal, D, 0, 0, 6, 5, 4, 3);

  std::cout << "D:" << std::endl;
  D.print(std::cout, 3);
  std::cout << std::endl;

}

void
test4() {
  std::cout << "*** TEST 4 ***" << std::endl;

  Matrix M(4,4);
  Matrix D(4,4);

  double lTotal = 0;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      M(i,j) = 4*i*j +j;
      lTotal += M(i,j);
    }
  }

  std::cout << "M:" << std::endl;
  M.print(std::cout, 3);

  uint64_t lCode = H2D::IQTS::Node::encode23LT(M, 0, 0);
  H2D::IQTS::Node::decode23LT(lCode, lTotal, D, 0, 0);

  std::cout << "D:" << std::endl;
  D.print(std::cout, 3);
  std::cout << std::endl;

}

void
test5() {
  std::cout << "*** TEST 5 ***" << std::endl;
  Matrix M(4,4);
  Matrix D(4,4);

  M(0,0) = 0;
  M(0,1) = 1;
  M(0,2) = 2;
  M(0,3) = 3;
  
  M(1,0) =  8;
  M(1,1) =  9;
  M(1,2) = 10;
  M(1,3) = 11;

  M(2,0) = 16;
  M(2,1) = 17;
  M(2,2) = 18;
  M(2,3) = 19;

  M(3,0) = 24;
  M(3,1) = 25;
  M(3,2) = 26;
  M(3,3) = 27;

  std::cout << "M:" << std::endl;
  M.print(std::cout, 3);

  const double   lTotal = M.sumAllElements();
  const uint64_t lCode = H2D::IQTS::Node::encode4x4(M, 0, 0, 4, 3, 2, 1);
  H2D::IQTS::Node::decode4x4(lCode, lTotal, D, 0, 0, 4, 3, 2, 1);

  std::cout << "D:" << std::endl;
  D.print(std::cout, 3);
  std::cout << std::endl;

}

void
test8() {
  std::cout << "*** TEST 8 ***" << std::endl;

  Matrix M(8,8);
  Matrix D(8,8);

  double lTotal = 0;
  for(uint i = 0; i < 8; ++i) {
    for(uint j = 0; j < 8; ++j) {
      M(i,j) = 8*i +j;
      lTotal += M(i,j);
    }
  }

  std::cout << "M:" << std::endl;
  M.print(std::cout, 3);


  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      for(uint k = 0; k < 2; ++k) {
        for(uint l = 0; l < 2; ++l) {
          if(i == k && j ==l) { continue; }
          D.setNull();
          const uint lQEnc = (i << 3) | (j << 2) | (k << 1) | (l << 0);
          std::cout << "--------- lQEnc: ";
          Bitvector32 lBvEnc(lQEnc);
          lBvEnc.printReverse(std::cout, 4);
          std::cout << " ---------" << std::endl;
          const uint64_t lCode = H2D::IQTS::Node::encode24LT(M, lQEnc);
          H2D::IQTS::Node::decode24LT(lCode, lTotal, D);
          std::cout << "D:" << std::endl;
          D.print(std::cout, 6);
          std::cout << std::endl;

          std::cout << "error L2 = " << H2D::IQTS::Node::errorL2_8x8(M, D) << std::endl;
          std::cout << "error LQ = " << H2D::IQTS::Node::errorLQ_8x8(M, D, 3) << std::endl;
        }
      }
    }
  }

  return;
}


void test_nr(){

//	H2D::QTS* qts = new H2D::QTS()
}



int
main() {
  // test0();
  // test1(13, 17, 19, 23);
  // test1(6, 7, 88, 999);
  // test1(10, 100, 1000, 10000);
  // test2();
  // test3();
  // test4();
  // test5();
//  test8();
test_nr();
  return 0;
}
