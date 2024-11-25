#include <iostream>
#include <iomanip>

#include <stdlib.h>

#include "types.hh"
#include "../infra/bitvectorsmall.hh"

typedef unsigned int uint;

typedef BitVectorSmall<uint64_t> bv_t;

void
test0() {
  uint     lCount = 0;
  QC2d16x4 lData;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      lData.set(i,j,lCount);
      ++lCount;
      uint32_t lShift = QC2d16x4::shift(i,j);
      bv_t lBv(lData._data);
      std::cout << i << ' ' << j << ' '
                << std::setw(2) << lShift << ' '
                << lBv
                << std::endl;
    }
  }

  bv_t lBv(lData._data);
  std::cout << "Bv: ";
  lBv.print(std::cout);
  std::cout << std::endl;

  const double lBase = q::calcBase<double>(16*32, 4);
  std::cout << "base = " << lBase << std::endl;

  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      std::cout << i << ' ' << j << ' ' 
                << std::setw(2) << lData.get(i, j) << ' '
                << std::setw(2) << lData(i, j)     << ' '
                << std::setw(8) << lData.getVal(i, j, 2) << ' '
                << std::setw(8) << lData.getVal(i, j, lBase) << ' '
                << std::endl;
    }
  }
}

void
test1() {
  const double lBase = q::calcBase<double>(16*32, 4);
  std::cout << "base = " << lBase << std::endl;

  QC2d16x4 lData;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      const double   lValue = (rand() % 513);
      const uint32_t lValueCompressed = q::qcompress<double>(lValue, lBase, 16);
      const double   lValueDecompressed = q::qdecompress<double>(lValueCompressed, lBase);
      lData.setVal(i, j, lValue, lBase);
      bv_t lBv(lData._data);
      std::cout << i << ' ' << j << ' '
                << std::setw(4) << lValue << ' '
                << std::setw(2) << lValueCompressed << ' '
                << std::setw(8) << lValueDecompressed << ' '
                << std::endl;
    }
  }

  bv_t lBv(lData._data);
  std::cout << "Bv: ";
  lBv.print(std::cout);
  std::cout << std::endl;


  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
      std::cout << i << ' ' << j << ' ' 
                << std::setw(8) << lData.getVal(i, j, lBase) << ' '
                << std::endl;
    }
  }
}



int
main() {
  test0();
  test1();
  return 0;
}
