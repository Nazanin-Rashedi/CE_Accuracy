#include "perf_wrapper.h"

int
perf_event_open(struct perf_event_attr*  aPerfEvent,
                pid_t             aPid,
                int               aHwThread,
                int               aFdGroup,
                unsigned long int aFlags) {
  aFlags |= PERF_FLAG_FD_CLOEXEC;
  int lRes = 0;
  lRes = syscall(__NR_perf_event_open, aPerfEvent, aPid, aHwThread, aFdGroup, aFlags);
  if(0 > lRes) {
    printf("perf_event_open: errno = %d: %s\n", errno, strerror(errno));
  }
  return lRes;
}

void
perf_event_close(int fd) {
  close(fd);
}

void 
perf_event_clear(struct perf_event_attr* aPerfEvent) {
  memset(aPerfEvent, 0, sizeof(struct perf_event_attr));
}

void 
perf_event_disable(int fd) {
  if(-1 != fd) {
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  }
}

void 
perf_event_enable(int fd) {
  if(-1 != fd) {
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
  }
}

void 
perf_event_reset(int fd) {
  if(-1 != fd) {
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  }
}

long long unsigned int
perf_event_read(int fd) {
  if(-1 == fd) { return 0; }
  long long unsigned int lRes;
  read(fd, &lRes, sizeof(long long unsigned int));
  return lRes;
}

void
perf_event_desc_init_a(struct perf_event_attr* aPerfEventDesc,
                       uint32_t aPerfType,
                       uint64_t aPerfConfig,
                       int      aDisable) {
  perf_event_clear(aPerfEventDesc);
  aPerfEventDesc->type   = aPerfType;
  aPerfEventDesc->size   = sizeof(struct perf_event_attr);
  aPerfEventDesc->config = aPerfConfig;
  if(aDisable) {
    aPerfEventDesc->disabled = 1;
  } else {
    aPerfEventDesc->disabled = 0;
  }
  // aPerfEventDesc->pinned = 1;
  aPerfEventDesc->exclude_user = 0;
  aPerfEventDesc->exclude_kernel = 1;
  aPerfEventDesc->exclude_hv = 1;
}

/*
#define perf_event_desc_init_a(aPerfEventDesc, aPerfType, aPerfConfig, aDisable) \
  perf_event_clear(aPerfEventDesc); \
  (aPerfEventDesc)->type   = aPerfType; \
  (aPerfEventDesc)->size   = sizeof(struct perf_event_attr); \
  (aPerfEventDesc)->config = aPerfConfig; \
  if(aDisable) { \
    (aPerfEventDesc)->disabled = 1; \
  } else { \
    (aPerfEventDesc)->disabled = 0; \
  } \
  (aPerfEventDesc)->exclude_user = 0; \
  (aPerfEventDesc)->exclude_kernel = 1; \
  (aPerfEventDesc)->exclude_hv = 1; \
*/


int
perf_event_open_LLC(struct perf_event_LLC* aPerfLLC, const int aHwThreadNo) {
  aPerfLLC->_fd_access = -1;
  aPerfLLC->_fd_miss   = -1;
  struct perf_event_attr lPerfEventDesc;
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES, 1);
  // count LLC accesses // head of event group
  int lPerfHead = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, -1, 0);
  if(-1 == lPerfHead) {
    printf("Warning: Error opening event %lld (access LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  aPerfLLC->_fd_access = lPerfHead;
  // count LLC misses
  perf_event_desc_init_a(&lPerfEventDesc,
                         PERF_TYPE_HARDWARE,
                         PERF_COUNT_HW_CACHE_MISSES,
                         0);
  int lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (miss LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  aPerfLLC->_fd_miss = lPerfEvent;
  aPerfLLC->_count_access = 0;
  aPerfLLC->_count_miss   = 0;
  return 0;
}

int
perf_event_open_LLC_2(struct perf_event_LLC* aPerfLLC, const int aHwThreadNo) {
  aPerfLLC->_fd_access = -1;
  aPerfLLC->_fd_miss   = -1;

  uint64_t lConfigA = 0; // access counter config
  uint64_t lConfigM = 0; // miss   counter config

  struct perf_event_attr lPerfEventDesc;

  /* 
   * config LLC
   */

  // (1) count LLC accesses 
  //     (head of event group)

  lConfigA =  PERF_COUNT_HW_CACHE_LL |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM =  PERF_COUNT_HW_CACHE_LL |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  int lPerfHead = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, -1, 0);
  if(-1 == lPerfHead) {
    printf("Warning: Error opening event %lld (access LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  aPerfLLC->_fd_access = lPerfHead;

  // (2) count LLC misses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 0);
  int lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (miss LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  aPerfLLC->_fd_miss = lPerfEvent;
  aPerfLLC->_count_access = 0;
  aPerfLLC->_count_miss   = 0;
  return 0;
}


void 
perf_event_start_LLC(struct perf_event_LLC* aPerfLLC) {
  perf_event_reset(aPerfLLC->_fd_access);
  perf_event_reset(aPerfLLC->_fd_miss);
  perf_event_enable(aPerfLLC->_fd_access);
}

void
perf_event_stop_LLC(struct perf_event_LLC* aPerfLLC) {
  perf_event_disable(aPerfLLC->_fd_access);
  aPerfLLC->_count_access = perf_event_read(aPerfLLC->_fd_access);
  aPerfLLC->_count_miss   = perf_event_read(aPerfLLC->_fd_miss);
}

void
perf_event_close_LLC(struct perf_event_LLC* lPerfLLC) {
  perf_event_close(lPerfLLC->_fd_miss);
  perf_event_close(lPerfLLC->_fd_access);
}


int
perf_cache_tlb_read_open(struct perf_cache_tlb_read_t* x, int aHwThreadNo) {
  x->_fd_access_llc  = -1;
  x->_fd_miss_llc    = -1;
  x->_fd_access_l1d  = -1;
  x->_fd_miss_l1d    = -1;
  x->_fd_access_dtlb = -1;
  x->_fd_miss_dtlb   = -1;

  uint64_t lConfigA = 0; // access counter config
  uint64_t lConfigM = 0; // miss   counter config
  int      lPerfEvent = 0; // FD of perf event

  struct perf_event_attr lPerfEventDesc;

  /* 
   * config LLC
   */

  // (1) count LLC accesses 
  //     (head of event group)

  // 
  lConfigA =  PERF_COUNT_HW_CACHE_LL |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM =  PERF_COUNT_HW_CACHE_LL |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  // perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES, 1);
  int lPerfHead = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, -1, 0);
  if(-1 == lPerfHead) {
    printf("Warning: Error opening event %lld (access LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  x->_fd_access_llc = lPerfHead;

  // (2) count LLC misses

  // perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 0);
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 1);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (miss LLC).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_miss_llc = lPerfEvent;
  x->_count_access_llc = 0;
  x->_count_miss_llc   = 0;

  /* 
   * config L1
   */

  lConfigA = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  // (3) count L1 accesses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (access L1d).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_access_l1d = lPerfEvent;

  // (4) count L1 misses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (miss L1d).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_miss_l1d = lPerfEvent;
  x->_count_access_l1d = 0;
  x->_count_miss_l1d   = 0;

  /*
   * config dTLB
   */

  lConfigA = (PERF_COUNT_HW_CACHE_DTLB) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_DTLB) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  // (5) count dTLB accesses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (access dTLB).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_access_dtlb = lPerfEvent;

  // (6) count dTLB misses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (miss dTLB).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_miss_dtlb = lPerfEvent;
  x->_count_access_dtlb = 0;
  x->_count_miss_dtlb   = 0;

  return 0;
}

void
perf_cache_tlb_read_start(struct perf_cache_tlb_read_t* aPerfCache) {
  perf_event_reset(aPerfCache->_fd_access_llc);
  perf_event_reset(aPerfCache->_fd_miss_llc);
  perf_event_reset(aPerfCache->_fd_access_l1d);
  perf_event_reset(aPerfCache->_fd_miss_l1d);
  perf_event_reset(aPerfCache->_fd_access_dtlb);
  perf_event_reset(aPerfCache->_fd_miss_dtlb);

  perf_event_enable(aPerfCache->_fd_access_llc);
} 
  
void
perf_cache_tlb_read_stop(struct perf_cache_tlb_read_t* aPerfCache) {
  perf_event_disable(aPerfCache->_fd_access_llc);
  aPerfCache->_count_access_llc  = perf_event_read(aPerfCache->_fd_access_llc);
  aPerfCache->_count_miss_llc    = perf_event_read(aPerfCache->_fd_miss_llc);
  aPerfCache->_count_access_l1d  = perf_event_read(aPerfCache->_fd_access_l1d);
  aPerfCache->_count_miss_l1d    = perf_event_read(aPerfCache->_fd_miss_l1d);
  aPerfCache->_count_access_dtlb = perf_event_read(aPerfCache->_fd_access_dtlb);
  // aPerfCache->_count_miss_dtlb   = perf_event_read(aPerfCache->_fd_miss_dtlb);
}   
    
void
perf_cache_tlb_read_close(struct perf_cache_tlb_read_t* aPerfCache) {
  perf_event_close(aPerfCache->_fd_miss_dtlb);
  perf_event_close(aPerfCache->_fd_access_dtlb);
  perf_event_close(aPerfCache->_fd_miss_l1d);
  perf_event_close(aPerfCache->_fd_access_l1d);
  perf_event_close(aPerfCache->_fd_miss_llc);
  perf_event_close(aPerfCache->_fd_access_llc);
}
  
void
perf_cache_tlb_read_print(struct perf_cache_tlb_read_t* aPerfCache) {
  printf("read  L1d: %lld, %lld\n", aPerfCache->_count_access_l1d,  aPerfCache->_count_miss_l1d);
  printf("read  LLC: %lld, %lld\n", aPerfCache->_count_access_llc,  aPerfCache->_count_miss_llc);
  printf("read dTLB: %lld, %lld\n", aPerfCache->_count_access_dtlb, aPerfCache->_count_miss_dtlb);
}

/*
 *  struct perf_rw_cache_t
 */

int
perf_cache_rw_open(struct perf_cache_rw_t* x, int aHwThreadNo) {
  x->_fd_r_access_llc  = -1;
  x->_fd_r_miss_llc    = -1;
  x->_fd_r_access_l1d  = -1;
  x->_fd_r_miss_l1d    = -1;
  x->_fd_w_access_llc  = -1;
  x->_fd_w_miss_llc    = -1;
  x->_fd_w_access_l1d  = -1;
  x->_fd_w_miss_l1d    = -1;
  struct perf_event_attr lPerfEventDesc;

  uint64_t lConfigA = 0;
  uint64_t lConfigM = 0;

  // int lPerfHead = 0; // this implementation does not use groups
  const int lPerfHead = -1;
  int lPerfEvent = 0;

  /*
   * count LLC read access and miss
   * (LLC read accesses: head of event group)
   */

  // possibly unused
  lConfigA = (PERF_COUNT_HW_CACHE_LL) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_LL) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
  
  // (1) LLC read accesses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  // perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, -1, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (read access LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  x->_fd_r_access_llc = lPerfEvent;

  // (2) LLC read misses
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 1);
  // perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES, 0);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (read miss LLC).\n", lPerfEventDesc.config);
    return -1;
  }
  x->_fd_r_miss_llc = lPerfEvent;
  x->_count_r_access_llc = 0;
  x->_count_r_miss_llc   = 0;

  /*
   * count L1d read accesses and misses
   */

  lConfigA = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_READ << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
  
  // (3) count L1d read accesses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1); 
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (read access L1d).\n", lPerfEventDesc.config);
    return -1;
  }
  x->_fd_r_access_l1d = lPerfEvent;

  // (4) count L1d read misses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (read miss L1d).\n", lPerfEventDesc.config);
    return -1;
  }
  x->_fd_r_miss_l1d = lPerfEvent;

  x->_count_r_access_l1d = 0;
  x->_count_r_miss_l1d   = 0;

  /* 
   * count LLC write accesses and misses
   */

  lConfigA = (PERF_COUNT_HW_CACHE_LL) |
             (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_LL) |
             (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  // (5) count LLC write accesses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (write access LLC).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_w_access_llc = lPerfEvent;

  // (6) count LLC write misses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (write miss LLC).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_w_miss_llc = lPerfEvent;
  x->_count_w_access_llc = 0;
  x->_count_w_miss_llc   = 0;

  /* 
   * count L1d write accesses and misses
   */

  lConfigA = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lConfigM = (PERF_COUNT_HW_CACHE_L1D) |
             (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
             (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

  // (7) count L1d write accesses

  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigA, 1);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (write access L1d).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_w_access_l1d = lPerfEvent;
 
  // (8) count L1d write misses
/*
  perf_event_desc_init_a(&lPerfEventDesc, PERF_TYPE_HW_CACHE, lConfigM, 0);
  lPerfEvent = perf_event_open(&lPerfEventDesc, 0, aHwThreadNo, lPerfHead, 0);
  if(-1 == lPerfEvent) {
    printf("Warning: Error opening event %lld (write miss L1d).\n", lPerfEventDesc.config);
    // return -1;
  }
  x->_fd_w_miss_l1d = lPerfEvent;
*/

  x->_count_w_access_l1d = 0;
  x->_count_w_miss_l1d   = 0;

  return 0;
}

void
perf_cache_rw_start(struct perf_cache_rw_t* aPerfCache) {
  perf_event_reset(aPerfCache->_fd_r_access_llc);
  perf_event_reset(aPerfCache->_fd_r_miss_llc);
  perf_event_reset(aPerfCache->_fd_r_access_l1d);
  perf_event_reset(aPerfCache->_fd_r_miss_l1d);
  perf_event_reset(aPerfCache->_fd_w_access_llc);
  perf_event_reset(aPerfCache->_fd_w_miss_llc);
  perf_event_reset(aPerfCache->_fd_w_access_l1d);
  perf_event_reset(aPerfCache->_fd_w_miss_l1d);

  perf_event_enable(aPerfCache->_fd_r_access_llc);
  perf_event_enable(aPerfCache->_fd_r_miss_llc);
  perf_event_enable(aPerfCache->_fd_r_access_l1d);
  perf_event_enable(aPerfCache->_fd_r_miss_l1d);
  perf_event_enable(aPerfCache->_fd_w_access_llc);
  perf_event_enable(aPerfCache->_fd_w_miss_llc);
  perf_event_enable(aPerfCache->_fd_w_access_l1d);
  perf_event_enable(aPerfCache->_fd_w_miss_l1d);
} 
  
void
perf_cache_rw_stop(struct perf_cache_rw_t* aPerfCache) {
  perf_event_disable(aPerfCache->_fd_r_access_llc);
  perf_event_disable(aPerfCache->_fd_r_miss_llc);
  perf_event_disable(aPerfCache->_fd_r_access_l1d);
  perf_event_disable(aPerfCache->_fd_r_miss_l1d);
  perf_event_disable(aPerfCache->_fd_w_access_llc);
  perf_event_disable(aPerfCache->_fd_w_miss_llc);
  perf_event_disable(aPerfCache->_fd_w_access_l1d);
  perf_event_disable(aPerfCache->_fd_w_miss_l1d);

  aPerfCache->_count_r_access_llc = perf_event_read(aPerfCache->_fd_r_access_llc);
  aPerfCache->_count_r_miss_llc   = perf_event_read(aPerfCache->_fd_r_miss_llc);
  aPerfCache->_count_r_access_l1d = perf_event_read(aPerfCache->_fd_r_access_l1d);
  aPerfCache->_count_r_miss_l1d   = perf_event_read(aPerfCache->_fd_r_miss_l1d);
  aPerfCache->_count_w_access_llc = perf_event_read(aPerfCache->_fd_w_access_llc);
  aPerfCache->_count_w_miss_llc   = perf_event_read(aPerfCache->_fd_w_miss_llc);
  aPerfCache->_count_w_access_l1d = perf_event_read(aPerfCache->_fd_w_access_l1d);
  aPerfCache->_count_w_miss_l1d   = perf_event_read(aPerfCache->_fd_w_miss_l1d);
}   
    
void
perf_cache_rw_close(struct perf_cache_rw_t* aPerfCache) {
  perf_event_close(aPerfCache->_fd_w_miss_l1d);
  perf_event_close(aPerfCache->_fd_w_access_l1d);
  perf_event_close(aPerfCache->_fd_w_miss_llc);
  perf_event_close(aPerfCache->_fd_w_access_llc);
  perf_event_close(aPerfCache->_fd_r_miss_l1d);
  perf_event_close(aPerfCache->_fd_r_access_l1d);
  perf_event_close(aPerfCache->_fd_r_miss_llc);
  perf_event_close(aPerfCache->_fd_r_access_llc);
}
  
void
perf_rw_cache_print(struct perf_cache_rw_t* aPerfCache) {
  printf("L1d: access/miss: rd: %lld, %lld wr: %lld, %lld\n", 
          aPerfCache->_count_r_access_l1d,  aPerfCache->_count_r_miss_l1d,
          aPerfCache->_count_w_access_l1d,  aPerfCache->_count_w_miss_l1d);
  printf("L3u: access/miss: rd: %lld, %lld wr: %lld, %lld\n", 
          aPerfCache->_count_r_access_llc,  aPerfCache->_count_r_miss_llc,
          aPerfCache->_count_w_access_llc,  aPerfCache->_count_w_miss_llc);
}


