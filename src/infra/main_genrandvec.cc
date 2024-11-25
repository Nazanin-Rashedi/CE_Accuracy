#include "infra/GenRandIntVec.hh"
#include "infra/vector_ops.hh"
extern "C" {
  #include "infra/cmeasure.h"
}

#include <fstream>

int
ivec_max(const uint_vt& aVec) {
  uint lRes = 0;
  for(uint i = 0; i < aVec.size(); ++i) {
    if(lRes < aVec[i]) {
      lRes = aVec[i];
    }
  }
  return lRes;
}

void
set_zero(uint_vt& aVec) {
  for(uint i = 0; i < aVec.size(); ++i) {
     aVec[i] = 0;
   }
}

void
fill_freq(uint_vt& aFreq, const uint_vt& aVec) {
  uint lMax = ivec_max(aVec);
  aFreq.resize(lMax+1);
  set_zero(aFreq);
  for(uint i = 0; i < aVec.size(); ++i) {
    ++aFreq[aVec[i]];
  }
}

void
print(const uint_vt& aVec, uint_vt& aFreq, bool aPrintVec) {
  fill_freq(aFreq, aVec);
  if(aPrintVec) {
    std::cout << "aVec: " << aVec << std::endl;
  }
  std::cout << "# frequency vector" << std::endl;
  for(uint i = 0; i < aFreq.size(); ++i) {
    std::cout << i << ' ' << aFreq[i] << std::endl;
  }
}

void
test0() {
  GenRandIntVec lGriv;
  uint_vt       lVec;
  uint_vt       lFreq;
  rng32_t       lRng;

  uint lCard = 20;

  GenRandIntVec::param_t lParam;
  lParam._max  = 10;
  lParam._dist = GenRandIntVec::kKey;

  lGriv.generate(lVec, lCard, lParam, lRng);
  std::cout << "key:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 2;
  lParam._dist = GenRandIntVec::kDiv;
  lGriv.generate(lVec, lCard, lParam, lRng);
  std::cout << "div:" << std::endl;
  print(lVec, lFreq, true);
 
  lParam._max = 20;
  lParam._dist = GenRandIntVec::kUni;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "uni:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 20;
  lParam._dist = GenRandIntVec::kExp;
  lParam._param = 2.0;
  lParam._flags = GenRandIntVec::kShuffle;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "exp:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 20;
  lParam._dist = GenRandIntVec::kNorm;
  lParam._param = 5.0;
  lParam._flags = 0;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "norm:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 20;
  lParam._dist = GenRandIntVec::kZipf;
  lParam._param = 0.5; // 0 < theta <= 1
  lParam._flags = 0;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "zipf:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 20;
  lParam._dist = GenRandIntVec::kSelf;
  lParam._param = 0.2; // 0 < h < 1
  lParam._flags = 0;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "80-20:" << std::endl;
  print(lVec, lFreq, true);

  lParam._max = 20;
  lParam._dist = GenRandIntVec::kPois;
  lParam._param = 10; // 0 < h < 1
  lParam._flags = 0;
  lGriv.generate(lVec, lCard, lParam, lRng); 
  std::cout << "poisson:" << std::endl;
  print(lVec, lFreq, true);

}

void
freq_to_disc(const uint_vt& aVec, uint_vt& aFreq, const char* aFilename) {
  set_zero(aFreq);
  for(uint i = 0; i < aVec.size(); ++i) {
    ++aFreq[aVec[i]];
  }
  std::ofstream lOs(aFilename);
  if(!lOs) {
    std::cout << "can't open file '" << aFilename << "'." << std::endl;
    return;
  }
  for(uint i = 0; i < aFreq.size(); ++i) {
    lOs << i << ' ' << aFreq[i] << std::endl;
  }
}

void
test1() {
  GenRandIntVec lGriv;
  uint_vt       lVec;
  uint_vt       lFreq(1000);
  rng32_t       lRng;
  cmeasure_t    lMeas;
  const uint    lCard = 1000*1000;

  GenRandIntVec::param_t lParam;
  lParam._max   = 1000;
  lParam._flags = 0;

  lParam._dist = GenRandIntVec::kExp;
  lParam._param = 2.0;
  cmeasure_start(&lMeas);
  lGriv.generate(lVec, lCard, lParam, lRng);
  cmeasure_stop(&lMeas);
  cmeasure_print_fin(&lMeas, "runtime exp : %7.2f", "\n");
  freq_to_disc(lVec, lFreq, "z_exp.dat");

  lParam._dist = GenRandIntVec::kNorm;
  lParam._param = 200;
  cmeasure_start(&lMeas);
  lGriv.generate(lVec, lCard, lParam, lRng);
  cmeasure_stop(&lMeas);
  cmeasure_print_fin(&lMeas, "runtime norm: %7.2f", "\n");
  freq_to_disc(lVec, lFreq, "z_norm.dat");

  lParam._dist = GenRandIntVec::kZipf;
  lParam._param = 0.1; // 0 < theta <= 1
  cmeasure_start(&lMeas);
  lGriv.generate(lVec, lCard, lParam, lRng);
  cmeasure_stop(&lMeas);
  cmeasure_print_fin(&lMeas, "runtime zipf: %7.2f", "\n");
  freq_to_disc(lVec, lFreq, "z_zipf.dat");

  lParam._dist = GenRandIntVec::kSelf;
  lParam._param = 0.3; // 0 < h < 1 (ausgeglichen: h = 0.5)
  cmeasure_start(&lMeas);
  lGriv.generate(lVec, lCard, lParam, lRng);
  cmeasure_stop(&lMeas);
  cmeasure_print_fin(&lMeas, "runtime self: %7.2f", "\n");
  freq_to_disc(lVec, lFreq, "z_8020.dat");

  lParam._dist = GenRandIntVec::kPois;
  lParam._param = 500; 
  cmeasure_start(&lMeas);
  lGriv.generate(lVec, lCard, lParam, lRng);
  cmeasure_stop(&lMeas);
  cmeasure_print_fin(&lMeas, "runtime pois: %7.2f", "\n");
  freq_to_disc(lVec, lFreq, "z_pois.dat");
}



int
main(const int argc, const char* argv[]) {
  test0();
  test1();
  return 0;
}

/*
 gnuplot
plot 'z_8020.dat' using 1:2 with lines     title '80-20', \
     'z_exp.dat'  using 1:2 with lines     title 'exp', \
     'z_norm.dat' using 1:2 with lines     title 'norm', \
     'z_pois.dat' using 1:2 with lines     title 'poisson', \
     'z_zipf.dat' using 1:2 with lines     title 'zipf'

plot 'z_exp.dat'  using 1:2 with lines     title 'exp', \
     'z_norm.dat' using 1:2 with lines     title 'norm', \
     'z_zipf.dat' using 1:2 with lines     title 'zipf'

*/


