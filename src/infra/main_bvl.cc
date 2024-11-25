#include "BitvectorLong.hh"



void
test0() {
  std::cout << "=== test0 === " << std::endl;
  BitvectorLong lBv(77);

  std::cout << "size = " << lBv.size() << std::endl
            << " cap = " << lBv.capacity() << std::endl
            << "  #e = " << lBv.noEntries() << std::endl
            ;

  lBv.setEvery(10);
  lBv.setEvery( 7);

  lBv.printFwd(std::cout);
  std::cout << std::endl;
  lBv.printBwd(std::cout);
  std::cout << std::endl;
}




int
main() {

  test0();


  return 0;
}

