#include <iostream>
#include <iomanip>
#include <random>


void
fGenerateRandom2dim(const int n) {
  std::mt19937 lRng(1823749525);
  for(int i = 0; i < n; ++i) {
    const int x1 = lRng() % 900;
    const int x2 = lRng() % 900;
    std::cout << x1 << ' ' << x2 << std::endl;
  }
}

int
main() {
  fGenerateRandom2dim(100* 1000);
}

//  10.000 ==> max 1.9
// 100.000 ==> max 1.2
