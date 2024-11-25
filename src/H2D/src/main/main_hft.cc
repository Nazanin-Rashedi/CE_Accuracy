#include <iostream>
#include <iomanip>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>

#include "infra/argbase.hh"
#include "infra/aggregate.hh"
#include "infra/matrix.hh"
#include "infra/measure.hh"

#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/cb.hh"
#include "infra/data2dim.hh"
#include "infra/HighlyFrequentTile.hh"
#include "infra/RegularPartitioning2dim.hh"

#include "RegP/RegPEstimator.hh"

#include "CheckError/CheckErrorUtil.hh"
#include "CheckError/TestRandom2dim.hh"
#include "CheckError/TestFixed2dim.hh"
#include "CheckError/TestRegular2dim.hh"

#include "arg.hh"

/*
 * main_hft
 * check implementation of HighlyFrequentValues
 */


void
fCheck(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  H2D::RegPEstimator lRegP(aData, aCb.nx(), aCb.ny(), aCb.eps(), aCb.phi(), aCb.q(), aCb.theta());
  if(aCb.print()) {
    const uint lNoZero = lRegP.noZero();
    std::cout << "# " << aCb.nx() << 'x' << aCb.ny() << " ===" << std::endl
              << "# max frequency: " << lRegP.maxEntry() << std::endl
              << "# no zero: " << lNoZero << " fraction " << ((double) lNoZero / (double) (aCb.nx() * aCb.ny())) << std::endl
              << "# no non zero: " << ((aCb.nx() * aCb.ny()) - lNoZero) 
                                   << " fraction " << ((double) (aCb.nx() * aCb.ny() - lNoZero) / (double) (aCb.nx() * aCb.ny()))
              << std::endl;
    if(aCb.printRegP()) {
      lRegP.printWithRowColSums(std::cout, 5);
    } else {
      if(aCb.print()) {
        lRegP.print(std::cout, 5);
      }
    }
  }

  H2D::rectangle_t lBr;
  aData.getBoundingRectangle(lBr);
  H2D::HighlyFrequentTile lHft(lBr, aCb.nx(), aCb.ny());
  lHft.init();
  for(uint i = 0; i < lRegP.nx(); ++i) {
    for(uint j = 0; j < lRegP.ny(); ++j) {
      if(0 < lRegP.regp()(i,j)) {
        lHft.step(i, j, lRegP.regp()(i,j));
      }
    }
  }
  lHft.fin();

  H2D::EstimateEvaluator lEstEval; // not really needed here
  H2D::TestRandom2dim lTestRandom(aData, lRegP, aCb.nx() * 7, aCb.ny() * 7, lEstEval);

  H2D::rectangle_t r;
  for(uint i = 0; i < 10; ++i) {
    lTestRandom.getRandomRectangle(r);
    const double lEstRegP = lRegP.estimate(r);
    const double lEstHftSlow  = lHft.estimateSlow(r);
    const double lEstHft  = lHft.estimate(r);
    std::cout << r << ' ' << lEstRegP << " =?= " << lEstHftSlow << " =?= " << lEstHft << std::endl;
  }

/* 
  for(uint i = 0; i < 1000000; ++i) {
    lTestRandom.getRandomRectangle(r);
    const double lEstHftSlow  = lHft.estimateSlow(r);
    const double lEstHft  = lHft.estimate(r);
    if(lEstHftSlow != lEstHft) {
      std::cout << "BAD: " << r << ' ' << lEstHftSlow << " =?= " << lEstHft << std::endl;
    }
  }
*/

  lTestRandom.getRandomRectangle(r);
  lTestRandom.getRandomRectangle(r);

  Measure lM1;
  lM1.start();
  double lX = 0;
  for(uint i = 0; i < 100000; ++i) {
    const double lEstHftSlow  = lHft.estimateSlow(r);
    lX += lEstHftSlow;
  }
  lM1.stop();

  Measure lM2;
  lM2.start();
  double lY = 0;
  for(uint i = 0; i < 100000; ++i) {
    const double lEstHft= lHft.estimate(r);
    lY += lEstHft;
  }
  lM2.stop();


  std::cout << "*** " << lX << " *** " << lY << " ***" << std::endl;
  std::cout << "slow: " << lM1.mCpuTime() << std::endl;
  std::cout << "fast: " << lM2.mCpuTime() << std::endl;
}

int
main(const int argc, const char* argv[]) {
  H2D::Cb lCb;
  argdesc_vt lArgDesc;
  construct_arg_desc(lArgDesc);

  if(!parse_args<H2D::Cb>(1, argc, argv, lArgDesc, lCb)) {
    std::cerr << "error while parsing arguments." << std::endl;
    return -1;
  }
  if(lCb.help()) {
    print_usage(std::cout, argv[0], lArgDesc);
    return 0;
  }
  if(0 == lCb.filename().size()) {
    std::cerr << "no file name given." << std::endl;
    return -1;
  }

  H2D::Data2dim lData;
  if(lCb.isHistFile()) {
    lData.readHistFile(lCb.filename());
  } else {
    lData.readValueFile(lCb.filename());
  }
  const double lTotal = (double) lData.total();
  if(100 > lTotal) {
    std::cerr << "data file too small: total = " << lTotal << std::endl;
    return -1;
  }

  if(1 >= lCb.theta()) {
    lCb.theta(0.01 * lTotal);
  }

  fCheck(lData, lCb);

  return 0;
}



