#include <iostream>
#include <iomanip>

#include "measure.hh"
#include "reservoir.hh"

typedef unsigned int uint;



int
test(Reservoir<int>& aReservoir) {

  {
    Measure lMeasure;
    lMeasure.start();
    for(int i = 0; i < 100000000; ++i) {
      aReservoir.push_back(&i);
    }
    lMeasure.stop();
    std::cout << "insert: " << lMeasure << std::endl;
  }

  {
    int lSum = 0;
    Measure lMeasure;
    lMeasure.start();
    for(uint i = 0; i < aReservoir.cardinality(); ++i) {
      lSum = (*(aReservoir[i]));
    }
    lMeasure.end();
    std::cout << "access[i]: " << lMeasure << std::endl;
    std::cout << "           " << lSum << std::endl;
  }

  {
    int lSum = 0;
    Measure lMeasure;
    lMeasure.start();
    for(Reservoir<int>::iterator iter = aReservoir.begin(); iter.ok(); ++iter) {
      lSum = (*(*iter));
    }
    lMeasure.end();
    std::cout << "iter  acc: " << lMeasure << std::endl;
    std::cout << "           " << lSum << std::endl;
  }
  return 0;
}


int
main() {

  Reservoir<int> lReservoir(10);

  std::cout << "Test1:" << std::endl;
  test(lReservoir);
  lReservoir.clear();

  std::cout << "Test2:" << std::endl;
  test(lReservoir);


  return 0;
}

