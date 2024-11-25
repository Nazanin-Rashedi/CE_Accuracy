#include "infra/glob_infra_standard_includes.hh"
#include <vector>
#include "infra/bag_tt.hh"
#include "infra/ShiftedExponentialDistribution.hh"

typedef std::mt19937 rng_t;
typedef ShiftedExponentialDistribution<rng_t> sedist_t;
typedef std::vector<sedist_t>                 sedist_vt;
typedef std::vector<double>                   double_vt;
typedef bag<uint32_t, uint32_t>               bag_t;
typedef std::vector<bag_t>                    bag_vt;

void
test0() {
  const double_vt lLambda = {0.1, 0.5, 1.0, 2.0, 5.0};
  const uint32_t  lNoLambda = lLambda.size();
  const uint32_t  lMod = 1000;
  const uint32_t  lShift = 0;
  sedist_vt       lDist(lNoLambda);
  for(uint i = 0; i < lDist.size(); ++i) {
    lDist[i].init(lShift, lMod, lLambda[i]);
  }

  bag_vt lBag(lNoLambda);

  rng_t  lRng;
  const uint n = 1000*1000;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < lNoLambda; ++j) {
      lBag[j].insert(lDist[j](lRng));
    }
  }

  for(uint i = 0; i < lMod; ++i) {
    std::cout << i;
    for(uint j = 0; j < lNoLambda; ++j) {
      std::cout << ' ' << lBag[j].frequency(i);
    }
    std::cout << std::endl;
  }  
}



int
main() {
  test0();
  return 0;
}

/*
plot ./main_shiftedexpdist > zX

plot 'zX' using 1:2 with lines title '0.1', \
     'zX' using 1:3 with lines title '0.5', \
     'zX' using 1:4 with lines title '1.0', \
     'zX' using 1:5 with lines title '2.0', \
     'zX' using 1:6 with lines title '5.0'


*/

