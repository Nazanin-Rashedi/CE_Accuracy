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
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"

#include "arg.hh"

/*
 * main_total
 * returns the total of the histogram
 */


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
    lData.readHistFile(lCb.filename(), lCb.lowLim());
  } else {
    lData.readValueFile(lCb.filename(), lCb.lowLim());
  }


  std::cout << lData.total() << std::endl;

  return 0;
}



