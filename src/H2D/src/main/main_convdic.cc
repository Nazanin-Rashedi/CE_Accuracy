#include <iostream>
#include <iomanip>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>

#include "infra/argbase.hh"
#include "infra/SimpleOrderedDictionary.hh"

#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/data2dim.hh"

#include "arg.hh"

void
process(const H2D::Cb& aCb) {
  H2D::Data2dim lData;
  if(aCb.isHistFile()) {
    lData.readHistFile(aCb.filename());
  } else {
    lData.readValueFile(aCb.filename());
  }

  std::cerr << "name of file: " << aCb.filename() << std::endl;
  std::cerr << "is hist file: " << aCb.isHistFile() << std::endl;

  std::cerr << "first: " << lData[0] << std::endl;
  std::cerr << "last : " << lData.last() << std::endl;

  SimpleOrderedDictionary<uint, double> lDicX;
  SimpleOrderedDictionary<uint, double> lDicY;
  lDicX.init();
  lDicY.init();
  for(uint i = 0; i < lData.size(); ++i) {
    lDicX.step(lData[i].x);
    lDicY.step(lData[i].y);
  }
  lDicX.fin();
  lDicY.fin();

  H2D::Data2dim lDataDic;
  lDataDic.init();
  for(uint i = 0; i < lData.size(); ++i) {
    lDataDic.step(lDicX.getId(lData[i].x), lDicY.getId(lData[i].y), lData[i].c);
  }  
  lDataDic.fin();

  for(uint i = 0; i < lDataDic.size(); ++i) {
    std::cout << lDataDic[i].x << ' '
              << lDataDic[i].y << ' '
              << lDataDic[i].c << ' '
              << std::endl;
  }

  std::cerr << "size x: " << lDicX.size() << std::endl;
  std::cerr << "size y: " << lDicY.size() << std::endl;
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
  if(0 == lCb.filename().size()) {
    std::cerr << "no file name given." << std::endl;
    return -1;
  }
  if(lCb.help()) {
    print_usage(std::cout, argv[0], lArgDesc);
    return 0;
  }

  process(lCb);

  return 0;
}



