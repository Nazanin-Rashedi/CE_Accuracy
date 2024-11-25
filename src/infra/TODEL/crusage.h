#ifndef INFRA_CRUSAGE_H
#define INFRA_CRUSAGE_H

#include <sys/resource.h>
#include <stdio.h>
#include <inttypes.h>

struct crusage_t {
  struct rusage _begin;
  struct rusage _end;
};


void    crusage_start(struct crusage_t* aCru);
void    crusage_stop(struct crusage_t* aCru);
int64_t crusage_sys_us(struct crusage_t* aCru);
int64_t crusage_usr_us(struct crusage_t* aCru);
int64_t crusage_tot_us(struct crusage_t* aCru);

#endif
