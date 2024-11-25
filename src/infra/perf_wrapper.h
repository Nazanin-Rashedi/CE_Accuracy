#ifndef PERF_WRAPPER_H
#define PERF_WRAPPER_H

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>

/*
 * Call sequence:
 *   perf_event_open
 *   perf_event_clear
 *   CONFIGURE PERF EVENT
 *   perf_event_reset
 *   perf_event_enable
 *   DO SOMETHING
 *   perf_event_disable
 *   perf_event_read
 *   perf_event_close
 */

/*
 * aPerfEventDesc: initialized perf event describing structure
 * aPid      =  0: measure current (calling) process/thread
 * aHwThread = -1: if running on any hardware thread (or specified hwtno)
 * aFdGroup  = -1: create new event group with current as head
 * aFlags        :
 * result: file descriptor for the event
 */

/*
 * user counter access is enabled if
 * /proc/sys/kernel/perf_event_paranoid
 * contains 
 * -1 - not paranoid at all
 *  0 - disallow raw tracepoint access for unpriv
 *  1 - disallow cpu events for unpriv
 *  2 - disallow kernel profiling for unpriv
 * to enable counter access:
 * sudo sysctl -w kernel.perf_event_paranoid=-1
 * see 
 * https://superuser.com/questions/980632/run-perf-without-root-rights
 * for details
 * to enable access across boots:
 * create file /etc/sysctl.d/99-mysettings.conf with a single line:
 * kernel.perf_event_paranoid=1
 */


int
perf_event_open(struct perf_event_attr*  aPerfEventDesc,
                pid_t             aPid,
                int               aHwThread,
                int               aFdGroup,
                unsigned long int aFlags);

void perf_event_close(int fd);
void perf_event_clear(struct perf_event_attr*);
void perf_event_disable(int fd);
void perf_event_enable(int fd);
void perf_event_reset(int fd);
long long unsigned int perf_event_read(int fd);

/* 
 * configure for current pid_t measured on aHwThreadNo
 * measure LLC accesses and misses
 */

struct perf_event_LLC {
  int _fd_access; // PERF_COUNT_HW_CACHE_REFERENCES, head of perf event group
  int _fd_miss;   // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_access; // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_miss;   // PERF_COUNT_HW_CACHE_MISSES
};

int  perf_event_open_LLC(struct perf_event_LLC* lPerfLLC, int aHwThreadNo);
int  perf_event_open_LLC_2(struct perf_event_LLC* lPerfLLC, int aHwThreadNo);
void perf_event_start_LLC(struct perf_event_LLC* lPerfLLC);
void perf_event_stop_LLC(struct perf_event_LLC* lPerfLLC);
void perf_event_close_LLC(struct perf_event_LLC* lPerfLLC);

/*
 * For PERF_TYPE_HW_CACHE:
 * config = (perf_hw_cache_id) |
 *          (perf_hw_cache_op_id << 8) |
 *          (perf_hw_cache_op_result_id << 16);
 * perf_hw_cache_id:
 *  PERF_COUNT_HW_CACHE_L1D   L1 data
 *  PERF_COUNT_HW_CACHE_L1I   L1 instruction
 *  PERF_COUNT_HW_CACHE_LL    last level cache
 *  PERF_COUNT_HW_CACHE_DTLB  data TLB
 *  PERF_COUNT_HW_CACHE_ITLB  instruction TLB
 *  PERF_COUNT_HW_CACHE_BPU   branch prediction unit
 *  PERF_COUNT_HW_CACHE_NODE  local memory accesses
 * perf_hw_cache_op_id:
 *  PERF_COUNT_HW_CACHE_OP_READ        read     accesses
 *  PERF_COUNT_HW_CACHE_OP_WRITE       write    accesses
 *  PERF_COUNT_HW_CACHE_OP_PREFETCH    prefetch accesses
 * perf_hw_cache_op_result_id:
 *  PERF_COUNT_HW_CACHE_RESULT_ACCESS  count accesses
 *  PERF_COUNT_HW_CACHE_RESULT_MISS    count misses
 */
struct perf_cache_tlb_read_t {
  int _fd_access_llc;
  int _fd_miss_llc;
  int _fd_access_l1d;
  int _fd_miss_l1d;
  int _fd_access_dtlb;
  int _fd_miss_dtlb;
  long long unsigned int _count_access_llc;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_miss_llc;    // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_access_l1d;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_miss_l1d;    // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_access_dtlb; // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_miss_dtlb;   // PERF_COUNT_HW_CACHE_MISSES
};

int  perf_cache_tlb_read_open(struct perf_cache_tlb_read_t* aPerf, int aHwThreadNo);
void perf_cache_tlb_read_start(struct perf_cache_tlb_read_t* aPerf);
void perf_cache_tlb_read_stop(struct perf_cache_tlb_read_t* aPerf);
void perf_cache_tlb_read_close(struct perf_cache_tlb_read_t* aPerf);
void perf_cache_tlb_read_print(struct perf_cache_tlb_read_t* aPerf);

/*
 * struct perf_cache_rw_t
 * read and write accesses to LLC and L1
 */

struct perf_cache_rw_t {
  int _fd_r_access_llc;
  int _fd_r_miss_llc;
  int _fd_r_access_l1d;
  int _fd_r_miss_l1d;
  int _fd_w_access_llc;
  int _fd_w_miss_llc;
  int _fd_w_access_l1d;
  int _fd_w_miss_l1d;
  long long unsigned int _count_r_access_llc;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_r_miss_llc;    // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_r_access_l1d;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_r_miss_l1d;    // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_w_access_llc;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_w_miss_llc;    // PERF_COUNT_HW_CACHE_MISSES
  long long unsigned int _count_w_access_l1d;  // PERF_COUNT_HW_CACHE_REFERENCES
  long long unsigned int _count_w_miss_l1d;    // PERF_COUNT_HW_CACHE_MISSES
};

int  perf_cache_rw_open(struct perf_cache_rw_t* aPerf, int aHwThreadNo);
void perf_cache_rw_start(struct perf_cache_rw_t* aPerf);
void perf_cache_rw_stop(struct perf_cache_rw_t* aPerf);
void perf_cache_rw_close(struct perf_cache_rw_t* aPerf);
void perf_rw_cache_print(struct perf_cache_rw_t* aPerf);


#endif
