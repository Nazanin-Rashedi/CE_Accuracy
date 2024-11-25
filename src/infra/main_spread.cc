#include <iostream>
#include <iomanip>

#include "spread.hh"

int
main() {
  Spread<int> lSpread;

  int i = 0;

  lSpread.init(&i);
  i = 0; lSpread.step(&i); // 2 + 3 + 4
  i = 2; lSpread.step(&i);
  i = 5; lSpread.step(&i);
  i = 9; lSpread.step(&i);
  lSpread.fin();

  lSpread.print(std::cout, 5);
  std::cout << std::endl;


  return 0;
}



