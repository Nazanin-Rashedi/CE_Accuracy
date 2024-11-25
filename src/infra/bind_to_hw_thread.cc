#include <iostream>
#include <sched.h>
#include "infra/bind_to_hw_thread.hh"


bool
bind_to_hw_thread(const int aHwThreadNo, const bool aMsg) {
  cpu_set_t lMask;
  CPU_ZERO(&lMask);
  CPU_SET(aHwThreadNo, &lMask);
  int lRc = sched_setaffinity(0, sizeof(lMask), &lMask);
  if(0 == lRc) { 
    if(aMsg) {
      std::cerr << "binding thread to hw-thread "
                << aHwThreadNo << " succeeded." << std::endl;
    }
  } else {
    if(aMsg) {
      std::cerr << "binding thread to hw-thread "
                << aHwThreadNo << " failed: return code: " << lRc << std::endl;
    }
  }
  return (0 == lRc);
}

