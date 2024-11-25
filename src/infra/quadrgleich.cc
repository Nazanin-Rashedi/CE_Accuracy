#include <iostream>
#include <iomanip>

#include <cmath>

#include "tmath.hh"

// input: quadratische gleichung der form
// ax^2 + bx + c = 0

inline double
resQuadratisch(const double x, const double a, const double b, const double c) {
  return mt::calcQuadraticEquation<double>(x, a, b, c);
}

inline double
solveQuadratischPos(const double a, const double b, const double c) {
  return mt::solveQuadraticEquationPos<double>(a, b, c);
}

inline double
solveQuadratischNeg(const double a, const double b, const double c) {
  // return (-b - sqrt(b*b - 4*a*c)) / (2 * a);
  return mt::solveQuadraticEquationNeg<double>(a, b, c);
}

void
test0() {
  const double a = 2;
  const double b = 3;
  const double c = 9;
  for(int x = 0; x < 10; ++x) {
    std::cout << std::setw(3) << x << ' '
              << std::setw(12) << resQuadratisch(x, a, b, c) << ' '
              << std::endl;
  }
  const double y = solveQuadratischPos(a, b, c - 99);
  std::cout << "y = " << y << std::endl;
}

void
test1() {

  const double a = 12;
  const double b = 12;

  std::cout << std::setw(5) << "s" << ' '
            << std::setw(12) << "x" << ' '
            << std::setw(12) << "f(x)" << ' '
            << std::endl;
  for(double s = 100; s < 1000; s += 100) {
    const double c =  8 - s;
    const double x1 = solveQuadratischPos(a, b, c);
    const double x2 = solveQuadratischNeg(a, b, c);
    std::cout << std::setw(5) << s << ' '
              << std::setw(12) << x1 << ' '
              << std::setw(12) << x2 << ' '
              << std::setw(12) << resQuadratisch(x1, a, b, c) << ' '
              << std::setw(12) << resQuadratisch(x2, a, b, c) << ' '
              << std::endl;
  }
  std::cout << std::endl;
}


int
main() {
  test0();
  test1();
  return 0;
}


