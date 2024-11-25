#include <iostream>
#include <iomanip>
#include <vector>
#include <assert.h>
#include "tmath.hh"
#include "rank_permutation.hh"

void
print(const std::vector<uint>& x) {
  for(unsigned int i = 0; i < x.size(); ++i) {
    std::cout << ' ' << x[i];
  }
}


void
test1(uint n) {
  const uint f = mt::fFactorial<float>(n);
  std::vector<uint> lPi1(n);
  std::vector<uint> lPi2(n);
  std::vector<uint> lPiInv(n);
  for(uint r = 0; r < f; ++r) {
    ftUnrankPermutation<uint>(r, n, lPi1.data());
    lPi2 = lPi1;
    const uint r2 = ftRankPermutation<uint>(n, lPi2.data(), lPiInv.data());
    std::cout << std::setw(2) << r << "   ";
    print(lPi1);
    std::cout << "  " << r2 << std::endl;
    assert(r == r2);
  }
  std::cout << std::endl;
}

int
main() {
  test1(4);
  std::cout << std::endl;

  return 0;
}
