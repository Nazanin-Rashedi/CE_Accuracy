#include <iostream>
#include <iomanip>

#include "vectoriterator.hh"

typedef unsigned int uint;

int
main() {
  VectorIterator::uint_vt lVt;
  lVt.push_back(1);
  lVt.push_back(7);
  lVt.push_back(1);
  for(VectorIterator lIt(lVt); lIt.ok(); ++lIt) {
    for(uint i = 1; i <= lIt.size(); ++i) {
     std::cout << lIt.vec(lIt.size() - i) << ' ';
    }
    std::cout << std::endl;
  }
  return 0;
}
	
