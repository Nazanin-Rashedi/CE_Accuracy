#include <stdlib.h>
#include <sched.h>        // sched_setaffinity
#include <unistd.h>       // getpid, sysconf

#include "system.hh"
#include "vector_ops.hh"

extern "C" {
  #include "cbind_to_hw_thread.h"
}

using namespace GM;


void
test0(const int aHwThreadNo) {
  System lSystem(aHwThreadNo);

  lSystem.print(std::cout);
  std::cout << std::endl;

  lSystem.print_cpuid(std::cout);
  std::cout << std::endl;

  // lSystem.sysinfo_numcpu();

  std::cout << "sw2hw binding:" << lSystem.sw2hwbinding() << std::endl;

  for(uint i = 0; i < lSystem.no_threads(); ++i) {
    if(0 == lSystem.get_clockticks(i)) {
      break;
    }
    std::cout << "hw thread " << i << " clockticks: " << lSystem.clockticks() << std::endl;
  }
}



int
main(const int argc, const char* argv[]) {
  int lHwThreadNo = 0; // hw thread no
  if(2 == argc) {
    char* lCharPtr = 0;
    lHwThreadNo = strtol(argv[1], &lCharPtr, 10);
    if(argv[1] != lCharPtr) {
      if(0 <= lHwThreadNo) {
        int lRc = cbind_to_hw_thread(lHwThreadNo, 0);
        if(0 != lRc) {
          std::cerr << "binding thread to " << lHwThreadNo << " failed: return code: " << lRc << std::endl;
        } else {
          std::cerr << "binding thread to " << lHwThreadNo << " succeeded." << std::endl;
        }
      } else {
        // try all hw-threads
      }
    }
  }
  test0(lHwThreadNo);
  return 0;
}

