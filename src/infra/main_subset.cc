#include <iostream>
#include <iomanip>

#include "bit_intrinsics.hh"
#include "bit_subsets.hh"


typedef unsigned int uint;

template<class Tgen>
uint
fSubsets(uint n, bool aTrace) {
  uint lCount = 0;
  for(Tgen lIter(n); lIter.isValid(); ++lIter) {
    ++lCount;
    if(aTrace) {
      std::cout << ' ';
      Bitvector32 lBv(*lIter);
      lBv.print(std::cout, n);
    }
  }
  return lCount;
}


void
test0() {

  for(uint n = 0; n < 10; ++n) {
    int lCountTN = 0;
    int lCountTE  = 0;
    int lCountNN  = 0;
    int lCountNE  = 0;
    std::cout << std::setw(2) << n << ":TN: ";
    lCountTN = fSubsets<BvTrueNonEmptySubsets<uint> > (n, true);
    std::cout << std::endl;

    std::cout << std::setw(2) << n << ":NN: ";
    lCountNN = fSubsets<BvNonEmptySubsets<uint> > (n, true);
    std::cout << std::endl;

    std::cout << std::setw(2) << n << ":TE: ";
    lCountTE = fSubsets<BvTrueSubsets<uint> > (n, true);
    std::cout << std::endl;

    std::cout << std::setw(2) << n << ":NE: ";
    lCountNE = fSubsets<BvAllSubsets<uint> > (n, true);
    std::cout << std::endl;

    std::cout << std::setw(2) << n << ' '
              << std::setw(8) << lCountTN << ' '
              << std::setw(8) << lCountNN << ' '
              << std::setw(8) << lCountTE << ' '
              << std::setw(8) << lCountNE << ' '
              << std::endl;
  }
}

void
test1() {
  for(uint i = 0; i < 17; ++i) {
    std::cout << i << "  ";
    for(BvMemberIdx<uint32_t> lX(i); lX.isValid(); ++lX) {
      std::cout << (1 << (*lX)) << ' ';
    }
    std::cout << std::endl;
  }
}



int
main() {
  test0();
  test1();
}


