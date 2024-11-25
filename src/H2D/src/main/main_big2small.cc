#include <iostream>
#include <iomanip>

#include <cmath>

#include "../infra/tmath.hh"

typedef unsigned int uint;


double
calculemus(const double n, const double aBase, const double aTheta) {
  const double lRes = mt::logbaset(n / aTheta, aBase);
  return 7.0 + round(lRes);
}

double
gridtreeautosize(const double n, const double aTheta) {
  const double lFreqDiv = (n / aTheta);
  const uint x1 = (uint) ceil(sqrt(lFreqDiv));
  const uint x2 = (uint) (round(mt::logbaset<double>(n, 4.0)));
  uint x = std::min<uint>(x1, x2);
  if(2 > x) {
    x = 2;
  }
  return x;
}


void
run() {
  const int lZ = 16;
  uint64_t N[lZ] = {3, 10, 100, 1000, 10000, 13030, 
                    50000, 100000, 200000, 500000, 
                    1000000, 10000000, 100000000, 
                    1000000000,
                    2000000000,
                    4000000000,};
  std::cout << std::setw(10) << 'n' << ' '
            << std::setw(4)  << "2.0" << ' '
            << std::setw(4)  << "3.0" << ' '
            << std::setw(4)  << "4.0" << ' '
            << std::setw(4)  << "5.0" << ' '
            << std::endl;

  for(uint i = 0; i < lZ; ++i) {
    std::cout << std::setw(10) << N[i] << ' '
              << std::setw(4)  << calculemus(N[i], 2.0, 32) << ' '
              << std::setw(4)  << calculemus(N[i], 3.0, 32) << ' '
              << std::setw(4)  << calculemus(N[i], 4.0, 32) << ' '
              << std::setw(4)  << calculemus(N[i], 5.0, 32) << ' '
              << std::setw(4)  << round(sqrt(N[i] / (16*32))) << ' '
              << std::setw(4)  << gridtreeautosize(N[i], 512) << ' '
              << std::setw(4)  << round(mt::logbaset<double>(N[i] / 2, 10)) << ' '
              << std::endl;
  }
}

int
main() {
  run();
}

