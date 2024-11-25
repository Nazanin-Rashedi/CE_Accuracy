#include <iostream>
#include <iomanip>

#include "numarray_tt.hh"


void
test0() {
  numarray_tt<int> lArr(5,5);
  for(int i = 0; i < 5; ++i) {
    for(int j = 0; j < 5; ++j) {
       lArr(i,j) = (j*5)+i;
    }
  }
  int x = 0, y = 0;
  x = lArr.max2max(y);
  std::cout << "max: " << x << ", 2nd max: " << y << std::endl;
}

void
test1() {
  numarray_tt<int> lArr9(9,9);
  for(int i = 0; i < 9; ++i) {
    for(int j = 0; j < 9; ++j) {
      lArr9(i,j) = i + j;
    }
  }
  numarray_tt<int> lArr3(3,3);
  lArr3.setToShrink3Of(lArr9);

  lArr9.print(std::cout, 3);
  std::cout << std::endl;
  lArr3.print(std::cout, 3);
  std::cout << std::endl;
  
}


int
main() {
  test0();
  test1();
  return 0;
}

