#include <iostream>
#include <iomanip>


#include "timestamp.hh"


int
main() {
  for(int y = 1968; y < 2000; ++y) {
    for(int m = 1; m < 12; ++m) {
      for(int d = 1; d < 29; ++d) {
        Date lDate(y,m,d);
        std::cout << lDate << ' ' << std::setw(7) << lDate.julianDay() << std::endl;
      }
    }
  }
}
