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
 * main_select
 * selection operator with count(*) stacked
 * rectangle:
 * [(142.105,-27.977),(161.053,-13.636)]
 */


bool
fReadRectangle(H2D::rectangle_t& aRectangle) {
  char c1 = '$', c2 = '$', c3 = '$', c4 = '$', c5 = '$', c6 = '$', c7 = '$';
  double lXmin = 7777, lXmax = 7777, lYmin = 7777, lYmax = 7777;

  std::cin >> c1 // '['
           >> c2 // '('
           ;
  if(std::cin.eof() || '[' != c1 || '(' != c2) {
    return false;
  }

  std::cin >> lXmin >> c1 >> lYmin >> c2 >> c3 >> c4
           >> lXmax >> c5 >> lYmax >> c6 >> c7;
  if(std::cin.eof() || ')' != c6 || ']' != c7) {
   return false;
  }

  aRectangle._pll.x = lXmin;
  aRectangle._pll.y = lYmin;
  aRectangle._pur.x = lXmax;
  aRectangle._pur.y = lYmax;

  return true;
  
}


bool
fRunSelect(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  std::cout << "query rectangle: ";
  std::cout.flush();

  if(std::cin.eof()) {
    return false;
  }
  H2D::rectangle_t lRectangle;
  if(fReadRectangle(lRectangle)) {
    const uint lCount = aData.countWithin(lRectangle);
    std::cout << "result for " << lRectangle 
              << " is " << lCount 
              << std::endl;

  } else {
    return false;
  }

  return true;
}


void
fRun(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  while(std::cin && !std::cin.eof()) {
    if(!fRunSelect(aData, aCb)) {
      break;
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

  fRun(lData, lCb);

  return 0;
}



