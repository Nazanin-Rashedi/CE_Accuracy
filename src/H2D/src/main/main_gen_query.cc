#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <vector>
#include <random>

#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/data2dim.hh"

extern "C" {
  #include "infra/cmeasure.h"
}


#include "arg.hh"

typedef std::mt19937 rng32_t;



void
generate_query(std::ostream& os, const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  rng32_t lRng;
  std::uniform_int_distribution<uint>    lIntDist(0, aData.size() - 1);
  std::uniform_real_distribution<double> lDubDist(0, 0.1);

  H2D::rectangle_t lBr; // bounding rectangle of all data points
  aData.getBoundingRectangle(lBr);

  H2D::rectangle_t lQr; // query rectangle

  double f = 0;
  for(uint i = 0; i < aCb.no_query();) {
    const uint lTupleNo = lIntDist(lRng);
    const H2D::xyc_t& t = aData[lTupleNo];
    f = lDubDist(lRng);
    lQr.xlo(t.x + f * (lBr.xlo() - t.x));
    f = lDubDist(lRng);
    lQr.xhi(t.x + f * (lBr.xhi() - t.x));
    f = lDubDist(lRng);
    lQr.ylo(t.y + f * (lBr.ylo() - t.y));
    f = lDubDist(lRng);
    lQr.yhi(t.y + f * (lBr.yhi() - t.y));
    f = lDubDist(lRng);
    const uint lCard = aData.countWithin(lQr);
    if(0 < lCard) {
      ++i;
      os << i << ' ' << lCard << ' ' << lQr << std::endl;
    }
  }
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

  if(0 == lData.size()) {
    return -1;
  }

  std::cout << "#  infile = " << lCb.filename() << std::endl;
  std::cout << "# outfile = " << lCb.filename_query() << std::endl;

  cmeasure_t lMeas;
  cmeasure_start(&lMeas);
  if(0 == lCb.filename_query().size()) {
    generate_query(std::cout, lData, lCb);
  } else {
    std::ofstream lOs(lCb.filename_query());
    generate_query(lOs, lData, lCb);
  }
  cmeasure_stop(&lMeas);
  double lNoQ = ((double) lCb.no_query());
  double lNoP = ((double) lData.size());
  std::cout << "# no total points = " << lData.total() << std::endl;
  std::cout << "# no dist  points = " << lData.size() << std::endl;
  std::cout << "# total   runtime = " << cmeasure_total_s(&lMeas) << " [s]" << std::endl;
  std::cout << "# time per query per point: "
            << cmeasure_div_ns(&lMeas, lNoQ * lNoP)
            << " [ns]"
            << std::endl;
}
