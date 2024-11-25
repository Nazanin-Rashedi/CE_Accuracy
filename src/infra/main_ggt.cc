#include "ggt.hh"


void
test0() {
  for(uint i = 1; i < 10; ++i) {
    for(uint j = 1; j < 10; ++j) {
       std::cout << i << ' ' << j << ' ' << ggt(i,j) << std::endl;
    }
  }
}


int
main() {
  test0();
  return 0;
}

