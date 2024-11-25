#include <iostream>
#include <iomanip>
#include <inttypes.h>

#include "print_test_header.hh"
#include "tmath.hh"

void
test0() {
  print_test_header("Factorial");
  for(uint32_t i = 1; i <= 10; ++i) {
    std::cout << std::setw(2) << i << ' '
              << std::setw(12) << mt::fLogFactorial<double>(i)
              << std::setw(12) << mt::expt<double>(mt::fLogFactorial<double>(i))
              << std::endl;
  }
}

void
test1() {
  print_test_header("Catalan");
  for(uint32_t i = 1; i <= 10; ++i) {
    std::cout << std::setw(2) << i << ' '
              << std::setw(12) << mt::fCatalan<double>(i)
              << std::setw(12) << mt::fCatalan2<double>(i)
              << std::endl;
  }
}

void
test2() {
  print_test_header("Yao");
  const uint32_t n = 1000;
  const uint32_t m =  100;
  std::cout << std::setw(3) << "k" << ' '
            << std::setw(12) << "YaoGamma" << ' '
            << std::setw(12) << "Whang" << ' '
            << std::setw(12) << "DihrLower" << ' '
            << std::setw(12) << "DihrUpper" << ' '
            << std::setw(12) << "Bernstein" << ' '
            << std::endl;

  for(uint32_t k = 10; k < n; k += 20) {
    std::cout << std::setw(3) << k << ' '
              << std::setw(12) << mt::roundt(mt::fYaoGamma<double>(k, n, m)) << ' '
              << std::setw(12) << mt::roundt(mt::fWhang<double>(k, n, m)) << ' '
              << std::setw(12) << mt::roundt(mt::fDihrLower<double>(k, n, m)) << ' '
              << std::setw(12) << mt::roundt(mt::fDihrUpper<double>(k, n, m)) << ' '
              << std::setw(12) << mt::roundt(mt::fBernstein<double>(k, n, m)) << ' '
              << std::endl;
  }

}




int
main() {
  test0();
  test1();
  test2();
}

