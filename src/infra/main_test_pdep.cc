#include <iostream>
#include <iomanip>
#include "inttypes.h"

#include "bitvectorsmall.hh"
#include "bit_intrinsics.hh"

#include "zBitSubsets.hh"


void
test00() {
  uint32_t A = 0x000E;

  std::cout << "true non-empty subsets" << std::endl;
  for(BvTrueNonEmptySubsetsPdep<uint32_t> lIter(A); lIter.isValid(); ++lIter) {
    std::cout << std::setw(3) << lIter.idx() << ' ' 
              << Bitvector32(*lIter) << ' ' 
              << Bitvector32(A) 
              << std::endl;
  }
}

void
test01() {
  uint32_t A = 0x000E;

  std::cout << "non-empty subsets" << std::endl;
  for(BvNonEmptySubsetsPdep<uint32_t> lIter(A); lIter.isValid(); ++lIter) {
    std::cout << std::setw(3) << lIter.idx() << ' ' 
              << Bitvector32(*lIter) << ' ' 
              << Bitvector32(A) 
              << std::endl;
  }
}

void
test10() {
  uint32_t A = 0x000E;

  std::cout << "true subsets" << std::endl;
  for(BvTrueSubsetsPdep<uint32_t> lIter(A); lIter.isValid(); ++lIter) {
    std::cout << std::setw(3) << lIter.idx() << ' ' 
              << Bitvector32(*lIter) << ' ' 
              << Bitvector32(A) 
              << std::endl;
  }
}

void
test11() {
  uint32_t A = 0x000E;

  std::cout << "all subsets" << std::endl;
  for(BvAllSubsetsPdep<uint32_t> lIter(A); lIter.isValid(); ++lIter) {
    std::cout << std::setw(3) << lIter.idx() << ' ' 
              << Bitvector32(*lIter) << ' ' 
              << Bitvector32(A) 
              << std::endl;
  }
}


int
main() {
  test00();
  test10();
  test01();
  test11();
  return 0;
}
