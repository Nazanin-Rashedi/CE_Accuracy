#include <iostream>
#include <iomanip>


#include "Hilbert.hh"


void
test0() {
  Hilbert lHilbert(4);

  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      const int d = lHilbert.xy2d(i,j);
      std::cout << std::setw(3) << i << ' '
                << std::setw(3) << j << ' '
                << std::setw(3) << d << ' '
                << std::endl;
    }
  }
}


int
main() {
  test0();

  return 0;
}


