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
#include "infra/summaryline.hh"
#include "infra/RegularPartitioning2dim.hh"

#include "ZHist/ZHist.hh"

#include "CheckError/CheckErrorUtil.hh"
#include "CheckError/TestRandom2dim.hh"
#include "CheckError/TestRegular2dim.hh"

#include "arg.hh"

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

// buget zu buckets umrechnen


//  H2D::ZHist lZHist(lData, H2D::ZHist::k_kind, 1, 1, 1, -1, -1, false);
// H2D::ZHist lZHist(aData, H2D::ZHist::k_kind, aCb.nx(), aCb.ny(), aCb.q(), aCb.theta(), aCb.trace());

  std::cout<<"huhu"<< std::endl;

	return 0;
}
