#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include <random>

#include "BitVectorMedium.hh"


typedef BitVectorMedium<uint32_t> Bvm32;


void
print(const Bvm32& aBv) {
  std::cout << "    ";
  for(uint32_t i = 0; i < aBv.noBits(); ++i) {
    std::cout << (i % 10);
  }
  std::cout << std::endl;
  std::cout << "S = " << aBv << std::endl;
  std::cout << "S = ";
  aBv.printSetBitIdx(std::cout);
  std::cout << std::endl;
  std::cout << "cardinality: " << aBv.cardinality() << std::endl;
}

void
print(const Bvm32& x, const Bvm32& y) {
  std::cout << "    ";
  for(uint32_t i = 0; i < x.noBits(); ++i) {
    std::cout << (i % 10);
  }
  std::cout << std::endl;
  std::cout << "x = " << x << std::endl;
  std::cout << "y = " << y << std::endl;
}

void
print(const Bvm32& x, const Bvm32& y, const Bvm32& z) {
  std::cout << "    ";
  for(uint32_t i = 0; i < x.noBits(); ++i) {
    std::cout << (i % 10);
  }
  std::cout << std::endl;
  std::cout << "X = " << x << std::endl;
  std::cout << "Y = " << y << std::endl;
  std::cout << "Z = " << z << std::endl;
}


void
print(const Bvm32& x, const Bvm32& y, const Bvm32& z, const Bvm32& r) {
  std::cout << "    ";
  for(uint32_t i = 0; i < x.noBits(); ++i) {
    std::cout << (i % 10);
  }
  std::cout << std::endl;
  std::cout << "X = " << x << std::endl;
  std::cout << "Y = " << y << std::endl;
  std::cout << "Z = " << z << std::endl;
  std::cout << "R = " << r << std::endl;
}


void
fill(Bvm32& lBv, const uint32_t n) {
  const uint32_t lNoBits = lBv.noBits();
  static std::mt19937   lRng(20130808);
  for(uint32_t i = 0; i < n; ++i) {
    lBv.set(lRng() % lNoBits);
  }
}


void
test0() {
  const uint32_t lSize = 3;
  uint32_t* x = new uint32_t[lSize];
  Bvm32 lBv(x, lSize, true);
  print(lBv);

  const uint32_t lNoBits = lBv.noBits();
  std::cout << "no bits: " << lNoBits << std::endl;
  std::cout << "  shift: " << lBv.shift() << std::endl;
  std::cout << "   mask: " << lBv.mask() << std::endl;

  std::mt19937 lRng(20130808);
  for(uint32_t i = 0; i < 10; ++i) {
     const uint32_t lIdx = (lRng() % lNoBits);
     std::cout << "set bit number " << lIdx << std::endl;
     lBv.set(lIdx);
     print(lBv);
  }

  delete x;
}


void
test1() {
  const uint32_t lSize = 3;
  uint32_t* x = new uint32_t[lSize];
  uint32_t* y = new uint32_t[lSize];
  uint32_t* z = new uint32_t[lSize];
  uint32_t* r = new uint32_t[lSize];
  Bvm32 X(x, lSize, true);
  Bvm32 Y(y, lSize, true);
  Bvm32 Z(z, lSize, true);
  Bvm32 R(r, lSize, true);

  std::cout << "X.isEmpty(): " << X.is_empty() << std::endl;

  X.set(43);
  fill(Y,  5);
  fill(Z,  7);
  fill(R, 13);
  std::cout << "|X| = " << X.cardinality() << ' '
            << "|Y| = " << Y.cardinality() << ' '
            << "|Z| = " << Z.cardinality() << ' '
            << "|R| = " << R.cardinality() << ' '
            << std::endl;
  std::cout << "X.isEmpty(): " << X.is_empty() << std::endl;

  std::cout << "lbs(X) = " << X.idx_lowest_bit_set() << ' '
            << "lbs(Y) = " << Y.idx_lowest_bit_set() << ' '
            << "lbs(Z) = " << Z.idx_lowest_bit_set() << ' '
            << "lbs(R) = " << R.idx_lowest_bit_set() << ' '
            << std::endl;
  std::cout << "hbs(X) = " << X.idx_highest_bit_set() << ' '
            << "hbs(Y) = " << Y.idx_highest_bit_set() << ' '
            << "hbs(Z) = " << Z.idx_highest_bit_set() << ' '
            << "hbs(R) = " << R.idx_highest_bit_set() << ' '
            << std::endl;

  print(X,Y,Z,R);

  std::cout << "isec(X,Y) = " << X.intersects(Y) << ' ' 
            << "isec(X,Z) = " << X.intersects(Z) << ' ' 
            << "isec(Y,Z) = " << Y.intersects(Z) << ' ' 
            << "isec(R,X) = " << R.intersects(X) << ' ' 
            << "isec(R,Y) = " << R.intersects(Y) << ' ' 
            << "isec(R,Z) = " << R.intersects(Z) << ' ' 
            << std::endl;


  fill(X, 19);
  fill(Y, 17);
  fill(Z, 13);
  fill(R, 11);
  print(X,Y,Z,R);

  R.complement();
  std::cout << "R = " << R << " // after complement" << std::endl;
  

  R.union_of(X,Y);
  std::cout << "union_of" << std::endl;
  print(X,Y,R);
  
  
}



int
main() {
  test0();
  test1();
  return 0;
}


