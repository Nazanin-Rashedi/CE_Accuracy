#include <iostream>
#include <iomanip>


#include "unionfind.hh"


int
main() {

  UnionFind aUf(7);

  std::cout << std::boolalpha;
  std::cout << "Initial state: " << std::endl << aUf << std::endl << std::endl;

  std::cout << "mUnion(3, 2): " << aUf.mUnion(3, 2) << std::endl;
  std::cout << "mUnion(1, 3): " << aUf.mUnion(1, 3) << std::endl;
  std::cout << "mUnion(1, 2): " << aUf.mUnion(1, 2) << std::endl;

  std::cout << "Intermediate state: " << std::endl << aUf << std::endl << std::endl;

  std::cout << "mUnion(5, 6): " << aUf.mUnion(5, 6) << std::endl;

  std::cout << "Intermediate state: " << std::endl << aUf << std::endl << std::endl;

  std::cout << "mUnion(3, 6): " << aUf.mUnion(3, 6) << std::endl;

  std::cout << "Final state: " << std::endl << aUf << std::endl << std::endl;

  std::cout << "mEqual(1, 1): " << aUf.mEqual(1, 1) << std::endl;
  std::cout << "mEqual(1, 2): " << aUf.mEqual(1, 2) << std::endl;
  std::cout << "mEqual(1, 3): " << aUf.mEqual(1, 3) << std::endl;
  std::cout << "mEqual(1, 4): " << aUf.mEqual(1, 4) << std::endl;
  std::cout << "mEqual(1, 5): " << aUf.mEqual(1, 5) << std::endl;
  std::cout << "mEqual(1, 6): " << aUf.mEqual(1, 6) << std::endl;

  std::cout << "mEqual(1, 2): " << aUf.mEqual(1, 2) << std::endl;
  std::cout << "mEqual(2, 2): " << aUf.mEqual(2, 2) << std::endl;
  std::cout << "mEqual(3, 2): " << aUf.mEqual(3, 2) << std::endl;
  std::cout << "mEqual(4, 2): " << aUf.mEqual(4, 2) << std::endl;

  std::cout << "mEqual(4, 0): " << aUf.mEqual(4, 0) << std::endl;
  return 0;
}

