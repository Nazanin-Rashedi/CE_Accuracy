#include "crusage.h"

#ifndef US
#define US ((int64_t) 1000LL * 1000LL)
#endif

void 
crusage_start(struct crusage_t* aCru) {
  int lRc = getrusage(RUSAGE_SELF, &(aCru->_begin));
  if(0 != lRc) {
    printf("ERROR: crusage_start: getrusage returned %d.\n", lRc);
  }
}

void 
crusage_stop(struct crusage_t* aCru) {
  int lRc = getrusage(RUSAGE_SELF, &(aCru->_end));
  if(0 != lRc) {
    printf("ERROR: crusage_stop: getrusage returned %d.\n", lRc);
  }
}


int64_t
crusage_sys_us(struct crusage_t* aCru) {
  struct timeval* b = &(aCru->_begin.ru_stime);
  struct timeval* e   = &(aCru->_end.ru_stime);
  int64_t lRes = 0;
  lRes  =  (int64_t) (e->tv_sec  - b->tv_sec) * US;
  lRes +=  (int64_t) (e->tv_usec - b->tv_usec);
  return lRes;
}

int64_t
crusage_usr_us(struct crusage_t* aCru) {
  struct timeval* b = &(aCru->_begin.ru_utime);
  struct timeval* e   = &(aCru->_end.ru_utime);
  int64_t lRes = 0;
  lRes  =  (int64_t) (e->tv_sec  - b->tv_sec) * US;
  lRes +=  (int64_t) (e->tv_usec - b->tv_usec);
  return lRes;
}

int64_t
crusage_tot_us(struct crusage_t* aCru) {
  return crusage_sys_us(aCru) + crusage_usr_us(aCru);
}


