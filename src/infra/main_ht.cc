#include <iostream>
#include <iomanip>

#include "ht.hh"


int
main() {
  for(uint32_t i = 0; i < 100; ++i) {
    const uint32_t hui = ht::fibhash(i);
    const double   hdb = ht::fibhash01(i);
    std::cout << std::setw(4) << i << ' '
              << std::setw(12) << hui << ' '
              << std::setw(12) << hdb << ' '
              << std::endl;
     
  }
}
