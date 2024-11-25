#ifndef INFRA_CBIND_TO_HW_THREAD
#define INFRA_CBIND_TO_HW_THREAD

#ifndef __MACH__
  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE
  #endif
  #include <sched.h>
#endif

int cbind_to_hw_thread(const int aHwThreadNo, const int aMsg);
int cbind_sw_to_hw_thread(const pid_t aSwThreadNo, const int aHwThreadNo, const int aMsg);

#endif

