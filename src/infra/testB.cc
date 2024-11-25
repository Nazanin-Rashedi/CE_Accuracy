#include <iostream>
#include "bitvectorsmall.hh"


void
test0() {
  Bitvector32 A(0xA);
  Bitvector32 B(0xB);

  std::cout << "0xA = "; A.print(std::cout, 4) << std::endl;
  std::cout << "0xB = "; B.print(std::cout, 4) << std::endl;
}


int
main() {
  test0();
  return 0;
}
  
