#include <iostream>
#include <iomanip>
#include <cmath>

/*
 *  generate plot to show how integral of cosinus can approximate cosinus value
 */

/*
 plot command and data generated:

# set xrange [0:3.2]
# plot 0, cos(x), 'cosinus.dat' using 1:2 with impulses title 'cos', 'cosinus.dat' using 1:3 with boxes title 'integral'

0.224399 0.974928 0.966766
0.673198 0.781831 0.775286
1.122 0.433884 0.430252
1.5708 6.12323e-17 0
2.0196 -0.433884 -0.430252
2.46839 -0.781831 -0.775286
2.91719 -0.974928 -0.966766


*/


void
generateCosinus(double n) {
  const double lPi = M_PI;
  for(double i = 0; i < n; i += 1) {
    const double x = ((2 * i + 1) / (2 * n)) * lPi;
    std::cout << x << ' ' << cos(x) << std::endl;
  }
  std::cout << std::endl;
}

void
generateIntegralCosinus(double n) {
  const double lPi = M_PI;
  for(double i = 0; i < n; i += 1) {
    const double x = ((2 * i + 1) / (2 * n)) * lPi;
    const double lLo = ((2 * i) / (2 * n)) * lPi;
    const double lHi = ((2 * i + 2) / (2 * n)) * lPi;
    const double lIntegral = ((n) / (lPi)) * (sin(lHi) - sin(lLo));
    std::cout << x << ' ' << cos(x) << ' ' << lIntegral << std::endl;
  }
  std::cout << std::endl;
}


void
generateData(double n) {
  generateCosinus(n);
  generateIntegralCosinus(n);
}



int
main() {
  generateData(7);
  return 0;
}

