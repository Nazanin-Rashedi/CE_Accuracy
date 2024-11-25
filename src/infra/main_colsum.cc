#include <iostream>
#include <iomanip>

#include "matrix.hh"


void
test0() {
  Matrix M(4, 5, 1.0, 1.0, 1.0, 1.0, 1.0,
                 1.0, 1.0, 1.0, 1.0, 1.0,
                 1.0, 1.0, 1.0, 1.0, 1.0,
                 1.0, 1.0, 1.0, 1.0, 1.0);
  std::cout << "M" << std::endl;
  M.print(std::cout, 3);

  Matrix I(M);
  std::cout << "I" << std::endl;
  I.accumulateInclusive();
  I.print(std::cout, 3);

  Matrix E(M);
  E.accumulateExclusive();
  std::cout << "E" << std::endl;
  E.print(std::cout, 3);
}


int
main() {
  test0();
  return 0;
}

