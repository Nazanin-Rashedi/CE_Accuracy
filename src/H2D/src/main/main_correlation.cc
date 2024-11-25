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
#include "infra/cb.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"

#include "arg.hh"
/*
 * main_correlation
 * print max qerror for varying nx,ny assuming independence
 */


void
fCalculateQCorrelations(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  const double lTheta = (double) aData.total() * (double) 0.01;
  double lMax = 0.0;
  for(uint nx = 2; nx < 200; ++nx) {
    for(uint ny = 2; ny < 200; ++ny) {
      H2D::RegularPartitioning2dim lRegP(nx, ny, aData);
      const double lMaxQError = lRegP.max_avi_estimate_qerror(lTheta);
      // std::cout << nx << ' '
      //           << ny << ' '
      //           << H2D::roundX(lMaxQError) << ' '
      //           << std::endl;
      if(lMax < lMaxQError) {
        lMax = lMaxQError;
      }
    }
    std::cout << "... max @nx = " << nx << " : " << lMax << std::endl;
  }
  std::cout << "maximum observed: " << lMax << std::endl;
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
  if(100 > lData.total()) {
    std::cerr << "data file too small." << std::endl;
    return -1;
  }

  fCalculateQCorrelations(lData, lCb);

  return 0;
}



