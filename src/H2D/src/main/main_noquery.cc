#include <iostream>
#include <iomanip>

typedef unsigned int uint;

/*
A000537  Sum of first n cubes; or n-th triangular number squared.
(Formerly M4619 N1972)
The n-th triangular number T(n)=sum_r(1, n)=n(n+1)/2 
satisfies the relations: 
  (i)  T(n) + T(n-1)=n^2 and 
  (ii) T(n) - T(n-1)=n 
from definition, so that 
  n^2*n=n^3={T(n)}^2 - {T(n-1)}^2 and thus summing telescopingly 
  over n we have 
  sum_{ r = 1..n } r^3 = {T(n)}^2 = (1+2+3+...+n)^2 = (n*(n+1)/2)^2.
 */

uint
fNoQuery(uint n) {
  uint x = ((n*(n+1))/2);
  return (x*x);
}

uint
fNoQueries(uint n) {
  uint lRes = 0;
  for(uint xl = 0; xl < n; ++xl) {
    for(uint xh = xl + 1; xh < n; ++xh) {
      for(uint yl = 0; yl < n; ++yl) {
        for(uint yh = yl + 1; yh < n; ++yh) {
          ++lRes;
        }
      }
    }
  }
  return lRes;
}


int
main() {
  for(uint n = 1; n < 100; ++n) {
    std::cout << std::setw(3) << n << ' '
              << std::setw(10) << fNoQueries(n)  << ' '
              << std::setw(10) << fNoQuery(n - 1) << ' '
              << std::endl;
  }
  return 0;
}

