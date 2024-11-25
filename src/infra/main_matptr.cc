#include <iostream>
#include <iomanip>

#include <inttypes.h>

#include "matrix.hh"

typedef unsigned int uint;


void
test0(const uint n) {
  Matrix A(n,n);

  const uint64_t lBegin = (uint64_t) A.getArray();

  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < n; ++j) {
      const uint64_t lAddr = (uint64_t) A.getPtrToElem(i,j);
      std::cout << std::setw(12) << (lAddr - lBegin) << ' ';
    }
    std::cout << std::endl;
  }
}


int
main() {


  test0(4);

  return 0;
}

