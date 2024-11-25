#include <iostream>
#include <iomanip>


#include "Hilbert.hh"


void
test0() {
  std::cout << "=== TEST 0 ===" << std::endl;
  Hilbert lHilbert(4);

  int xr = 0;
  int yr = 0;
  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      const int d = lHilbert.xy2d(i,j);
      lHilbert.d2xy(d, xr, yr);
      std::cout << std::setw(3) << i << ' '
                << std::setw(3) << j << ' '
                << std::setw(3) << d << ' '
                << std::setw(3) << xr << ' '
                << std::setw(3) << yr << ' '
                << std::endl;
    }
  }
}


void
test1() {
  std::cout << "=== TEST 1 ===" << std::endl;

  Hilbert lHilbert(4);
  int x = 0;
  int y = 0;
  for(int d = 0; d < 16; ++d) {
      lHilbert.d2xy(d, x, y);
      const int dr = lHilbert.xy2d(x,y);
      std::cout << std::setw(3) << d << ' '
                << std::setw(3) << x << ' '
                << std::setw(3) << y << ' '
                << std::setw(3) << dr << ' '
                << std::endl;
  }
}

int
test3(int n) {
  // std::cout << "+++ TEST 3 (n = " << n << ") +++" << std::endl;

  int lRes = 0;
  Hilbert lHilbert(n);
  int xr = 0;
  int yr = 0;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      const int d = lHilbert.xy2d(i,j);
      lHilbert.d2xy(d, xr, yr);
      if(xr != i || yr != j) {
        std::cout << "BAD: " << i << ' ' << j << ' ' << d << ' ' << xr << ' ' << yr << std::endl;
        ++lRes;
      }
    }
  }
  // std::cout << "+++ END TEST 3 +++" << std::endl;
  return lRes;
}

int
test4() {
  for(int e = 1; e < 14; ++e) {
    const int n = (1 << e);
    const int lNErr = test3(n);
    std::cout << std::setw(3) << n << "  "
              << std::setw(3) << lNErr
              << std::endl;
  }
  return 0;
}

int
main() {
  test0();
  test1();
  test4();
  return 0;
}


