#include <algorithm>
#include <cmath>
#include <random>
#include "infra/glob_infra_standard_includes.hh"
#include "infra/zipf_distribution.hh"

void
test0() {
  zipf_distribution<uint,double> lDist(1000, 0.5);
  uint_vt lFrq(1000);
  uint v = 0;
  std::mt19937 lRng;
  for(uint i = 0; i < 1000*1000; ++i) {
    v = lDist(lRng) - 1;
    // std::cout << "v_" << i << " = " << v << std::endl;
    assert(v < lFrq.size());
    ++lFrq[v];
  }
  for(uint i = 0; i < 1000; ++i) {
    std::cout << i << ' ' << lFrq[i] << std::endl;
  }
}

int
main() {
  test0();
  return 0;
}


