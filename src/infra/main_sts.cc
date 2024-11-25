#include <iostream>

#include "datejd.cc"
#include "stime.cc"
#include "stimestamp.cc"


int
main() {
  STimestamp lLOC;
  STimestamp lUTC;

  lLOC.setCurrentLocal();
  lUTC.setCurrentUTC();

  std::cout << "LOC: " << lLOC << std::endl;
  std::cout << "UTC: " << lUTC << std::endl;

  lLOC.resetMinSec();
  lUTC.resetMinSec();

  std::cout << "LOC: " << lLOC << std::endl;
  std::cout << "UTC: " << lUTC << std::endl;
}
