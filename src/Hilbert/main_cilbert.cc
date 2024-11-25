#include <iostream>
#include <iomanip>


//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}



//convert (x,y) to d
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}
 
//convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}
 

void
test0() {
  std::cout << "=== TEST 0 ===" << std::endl;
  const int n = 4;
  int xr = 0;
  int yr = 0;
  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      const int d = xy2d(n,i,j);
      d2xy(n, d, &xr, &yr);
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

  const int n = 4;
  int x = 0;
  int y = 0;
  for(int d = 0; d < 16; ++d) {
      d2xy(n, d, &x, &y);
      const int dr = xy2d(n,x,y);
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
  int xr = 0;
  int yr = 0;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      const int d = xy2d(n, i, j);
      d2xy(n, d, &xr, &yr);
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
  std::cout << "=== TEST 4 ===" << std::endl;
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


