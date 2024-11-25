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
#include "infra/RegularPartitioning2dim.hh"

#include "RegP/RegPEstimator.hh"

#include "CheckError/CheckErrorUtil.hh"
#include "CheckError/TestRandom2dim.hh"
#include "CheckError/TestFixed2dim.hh"
#include "CheckError/TestRegular2dim.hh"

#include "arg.hh"


/*
 * main_regp
 * determine minimum error partitioning
 * just for testimg purposes
 */


void
fCheckRegP(const H2D::Data2dim& aData, const H2D::Cb& aCb, const uint aNx, const uint aNy, bool aPrintXXX) {
  Measure lMeasure;
  lMeasure.start();
  H2D::RegPEstimator lRegP(aData, aNx, aNy, aCb.eps(), aCb.phi(), aCb.q(), aCb.theta());
  lMeasure.stop();

  std::cout << "matrix:" << std::endl;
  std::cout << std::endl;
  lRegP.regp().print(std::cout, 5);
  std::cout << std::endl;
  std::cout << "end matrix" << std::endl;

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._size = lRegP.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._constructionTime = lMeasure.mCpuTime();
  lSummaryLine._kind = H2D::H2D_RegPart;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._regp._nx = aNx;
  lSummaryLine._param._regp._ny = aNy;

  if(aPrintXXX) {
    const uint lNoZero = lRegP.noZero();
    std::cout << "# " << aNx << 'x' << aNy << " ===" << std::endl
              << "# max frequency: " << lRegP.maxEntry() << std::endl
              << "# no zero: " << lNoZero << " fraction " << ((double) lNoZero / (double) (aNx * aNy)) << std::endl
              << "# no non zero: " << ((aNx * aNy) - lNoZero) << " fraction " << ((double) (aNx * aNy - lNoZero) / (double) (aNx * aNy)) << std::endl
              ;
    if(aCb.printRegP()) {
      lRegP.printWithRowColSums(std::cout, 5);
    } else {
      if(aCb.print()) {
        lRegP.print(std::cout, 5);
      }
    }
  }


  fAnalyzeEstimator(aData, lRegP, aCb, lSummaryLine);
  std::cout << lSummaryLine << std::endl;
}

void
fRunRegPart(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  std::cout << "fRunRegPart" << std::endl;
  const uint lBudget = (0 < aCb.budget()) ? aCb.budget() : (aData.total() / 8);
  const uint lBudgetHi = lBudget * 2;
  const uint lBudgetLo = lBudget / 2;
  const uint lStep = std::max<uint>(1, lBudgetLo / 500);

  std::cout << "   budget = " << lBudget << std::endl
            << "       hi = " << lBudgetHi << std::endl
            << "       lo = " << lBudgetLo << std::endl
            << "     step = " << lStep << std::endl
            << std::endl;

  uint lCount = 0;
  for(uint nx = 10; nx <= lBudgetHi; nx += lStep) {
    for(uint ny = std::max<uint>(10, lBudgetLo / nx); (ny * nx) <= lBudgetHi; ny += lStep) {
      ++lCount;
      std::cout << "#NXY   nx,ny = " << nx << ", " << ny << std::endl;
      fCheckRegP(aData, aCb, nx, ny, false);
    }
  }
  std::cout << "    count = " << lCount << std::endl
            << std::endl;
}

void
fRunRegPartFixed(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  fCheckRegP(aData, aCb, aCb.nx(), aCb.ny(), true);
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

  H2D::RegularPartitioning2dim lRegP(1024, 1024, lData);
  std::cout << H2D::gbasename(lCb.filename().c_str())
            << ' ' << lData.total() 
            << ' ' << lData.size()
            << ' ' << lRegP.maxEntry()
            << ' ' << lRegP.noZero()
            << std::endl;

  const bool lLong = false;

  if(!lLong) {
    return 0;
  }
  std::cout << "Statistics:" << std::endl;

  std::cout << "card  = " << lData.size() << std::endl;
  std::cout << "no dv = " << lData.total() << std::endl;

  H2D::rectangle_t lBr;
  lData.getBoundingRectangle(lBr);
  std::cout << "BR = " << lBr << std::endl;


  if(1 >= lCb.theta()) {
    lCb.theta(0.01 * lTotal);
  }

  if(0 >= lCb.nx() || 0 >= lCb.ny()) {
    std::cerr << "warning: no regp run, no nx, ny given." << std::endl;
  } else {
    std::cout << "RegP:" << std::endl;
    fRunRegPartFixed(lData, lCb);
  }

  return 0;
}



