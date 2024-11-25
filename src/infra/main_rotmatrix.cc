#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "matrix.hh"

struct point_t {
  double x;
  double y;

  point_t() : x(0), y(0) {}
  point_t(const double aX, const double aY) : x(aX), y(aY) {}
};

typedef std::vector<point_t> point_vt;

void
print(const point_vt& pv) {
  for(size_t i = 0; i < pv.size(); ++i) {
    std::cout << pv[i].x << ' ' << pv[i].y << std::endl;
  }
}

void
apply(point_vt& aOut, const point_vt& aIn, const Matrix aRot) {
  aOut.clear();
  point_t a;
  point_t b;
  for(size_t i = 0; i < aIn.size(); ++i) {
    b = aIn[i];
    a.x = aRot(0,0) * b.x + aRot(0,1) * b.y;
    a.y = aRot(1,0) * b.x + aRot(1,1) * b.y;
    aOut.push_back(a);
  }
}

void
applyManually45(point_vt& aOut, const point_vt& aIn) {
  aOut.clear();
  point_t a;
  point_t b;
  const double lSqrt2 = sqrt((double) 2.0) / (double) 2.0;
  for(size_t i = 0; i < aIn.size(); ++i) {
    b = aIn[i];
    const double lA = lSqrt2 * b.x;
    const double lB = lSqrt2 * b.y;

    a.x = lA - lB;
    a.y = lA + lB;
    aOut.push_back(a);
  }
}

void
test0() {
  Matrix lRot;
  lRot.initAsRotationMatrix(M_PI/2);

  std::cout << "Rotationsmatrix (90):" << std::endl;
  lRot.print(std::cout, 10);

  point_vt lIn;
  lIn.push_back(point_t(1,1));
  lIn.push_back(point_t(2,2));
  lIn.push_back(point_t(3,3));
  lIn.push_back(point_t(4,4));

  std::cout << "Input:" << std::endl;
  print(lIn);

  point_vt lOut;
  apply(lOut, lIn, lRot);

  std::cout << "Output:" << std::endl;
  print(lOut);
}

void
test1() {
  Matrix lRot;
  lRot.initAsRotationMatrix(M_PI/4);

  std::cout << "Rotationsmatrix (45):" << std::endl;
  lRot.print(std::cout, 10);

  point_vt lIn;
  lIn.push_back(point_t(1,1));
  lIn.push_back(point_t(2,2));
  lIn.push_back(point_t(3,3));
  lIn.push_back(point_t(4,4));

  std::cout << "Input:" << std::endl;
  print(lIn);

  point_vt lOut;
  apply(lOut, lIn, lRot);

  std::cout << "Output:" << std::endl;
  print(lOut);
}

void
test2() {
  std::cout << "rot (45) manually:" << std::endl;

  point_vt lIn;
  lIn.push_back(point_t(1,1));
  lIn.push_back(point_t(2,2));
  lIn.push_back(point_t(3,3));
  lIn.push_back(point_t(4,4));

  std::cout << "Input:" << std::endl;
  print(lIn);

  point_vt lOut;
  applyManually45(lOut, lIn);

  std::cout << "Output:" << std::endl;
  print(lOut);
}



int
main() {

  test0();
  test1();
  test2();

  return 0;
}

