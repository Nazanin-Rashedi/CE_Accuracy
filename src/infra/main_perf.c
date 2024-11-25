#define _GNU_SOURCE

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "infra/cbind_to_hw_thread.h"
#include "infra/perf_wrapper.h"

#define MX_LOG_SIZE 22

uint
fRead(int* aArr, int n) {
  int lSize = (1 << n);
  int lRes = 0;
  int lIdx = aArr[0];
  for(int i = 0; i < lSize; ++i) {
   lIdx = aArr[lIdx];
   lRes ^= lIdx;
  }
  return lRes;
}

void
fWrite(int* aArr, int n) {
  int lSize = (1 << n);
  for(int i = 0; i < lSize; ++i) {
    aArr[i] = i;
  }
  for(uint i = lSize - 1; i > 0; --i) {
    uint lIdx = ((uint) rand()) % i;
    uint x = aArr[i];
    aArr[i] = aArr[lIdx];
    aArr[lIdx] = x;
  }
}

int*
fAlloc(int n) {
  int lSize    = (1 << n);
  int lSizeByte = sizeof(int) * lSize;
  int* lArr = (int*) malloc(lSizeByte);
  fWrite(lArr, n);
  return lArr;
} 
  
void
fFree(int* aArr) {
  free(aArr);
}   

uint
fDoSomething(int* lArr, int n) {
  uint lRes = 0;
  fWrite(lArr, n);
  for(uint i = 0; i < 0; ++i) {
    fWrite(lArr, n);
  }
  for(uint i = 0; i < 10; ++i) {
    lRes = fRead(lArr, n);
  }
  return lRes;
} 



void
test0(const uint aHwThreadNo) {
  printf("=== test0 (raw) ===\n");
  struct perf_event_attr lPerfEventDesc;
  perf_event_clear(&lPerfEventDesc);
  // config one event
  lPerfEventDesc.type   = PERF_TYPE_HARDWARE;
  lPerfEventDesc.size   = sizeof(struct perf_event_attr);
  // lPerfEventDesc.config = PERF_COUNT_HW_INSTRUCTIONS;
  // lPerfEventDesc.config = PERF_COUNT_HW_CPU_CYCLES;
  // lPerfEventDesc.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
  // lPerfEventDesc.config = PERF_COUNT_HW_BRANCH_MISSES;
  lPerfEventDesc.config = PERF_COUNT_HW_CACHE_MISSES;
  lPerfEventDesc.disabled = 1;
  lPerfEventDesc.exclude_kernel = 1;
  lPerfEventDesc.exclude_hv = 1;
  //                                               pid  aHwThreadNo  aFdGroup aFlags
  int lPerfHead = perf_event_open(&lPerfEventDesc,  0, aHwThreadNo,    -1,     0);
  if(-1 == lPerfHead) {
    fprintf(stderr, "Error opening head event %llx\n", lPerfEventDesc.config);
    exit(EXIT_FAILURE);
  }

  perf_event_clear(&lPerfEventDesc);
  // config one event
  lPerfEventDesc.type   = PERF_TYPE_HW_CACHE;
  lPerfEventDesc.size   = sizeof(struct perf_event_attr);
  lPerfEventDesc.config = (PERF_COUNT_HW_CACHE_LL) |
                          (PERF_COUNT_HW_CACHE_OP_READ << 8) |
                          (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
  lPerfEventDesc.disabled = 0;
  lPerfEventDesc.exclude_kernel = 1;
  lPerfEventDesc.exclude_hv = 1;
  //                                                pid  aHwThreadNo  aFdGroup   aFlags
  int lPerfEvent = perf_event_open(&lPerfEventDesc,  0, aHwThreadNo, lPerfHead,     0);
  if(-1 == lPerfEvent) {
    fprintf(stderr, "Error opening 2nd perf event %llx\n", lPerfEventDesc.config);
    exit(EXIT_FAILURE);
  }

  int* lArr = fAlloc(MX_LOG_SIZE);
  int lRes = 0;
  for(int n = 10; n <= MX_LOG_SIZE; ++n) {
    perf_event_reset(lPerfEvent);
    perf_event_reset(lPerfHead);
    perf_event_enable(lPerfHead);
    lRes = fDoSomething(lArr, n);
    perf_event_disable(lPerfHead);
    long long lCountMiss   = perf_event_read(lPerfHead);
    long long lCountAccess = perf_event_read(lPerfEvent);
    printf("Calculated f(%d) = %d with %lld LLC accesses and %lld misses\n", n, lRes, lCountAccess, lCountMiss);
  }
  free(lArr);
  perf_event_close(lPerfEvent);
  perf_event_close(lPerfHead);
}

void
test1(const uint aHwThreadNo) {
  printf("=== test1 (perf_event_LLC) ===\n");
  int lRc = 0;
  struct perf_event_LLC lInfoLLC;
  // lRc = perf_event_open_LLC(&lInfoLLC, aHwThreadNo); // uses PERF_TYPE_HARDWARE
  lRc = perf_event_open_LLC_2(&lInfoLLC, aHwThreadNo);  // uses PERF_TYPE_HW_CACHE
  if(lRc) {
    printf("rc = %d\n", lRc);
    return;
  }
  int* lArr = fAlloc(MX_LOG_SIZE);
  int lRes = 0;
  for(int n = 10; n <= MX_LOG_SIZE; ++n) {
    perf_event_start_LLC(&lInfoLLC);
    lRes = fDoSomething(lArr, n);
    perf_event_stop_LLC(&lInfoLLC);
    printf("Calculated f(%d) = %d with %lld LLC accesses and %lld LLC misses\n", 
            n, lRes, lInfoLLC._count_access, lInfoLLC._count_miss);
  }
  perf_event_close_LLC(&lInfoLLC);
}

void
test2(const uint aHwThreadNo) {
  printf("=== test2 (perf_cache_tlb_read_t) ===\n");
  struct perf_cache_tlb_read_t lPerfCache;
  int lRc = 0;
  lRc = perf_cache_tlb_read_open(&lPerfCache, aHwThreadNo);
  if(lRc) {
    printf("rc = %d\n", lRc);
    // return;
  }
  printf("fds = %d, %d, %d, %d, %d, %d\n",
         lPerfCache._fd_access_llc,
         lPerfCache._fd_miss_llc,
         lPerfCache._fd_access_l1d,
         lPerfCache._fd_miss_l1d,
         lPerfCache._fd_access_dtlb,
         lPerfCache._fd_miss_dtlb);

  int* lArr = fAlloc(MX_LOG_SIZE);
  int lRes = 0;
  for(int n = 10; n <= MX_LOG_SIZE; ++n) {
    perf_cache_tlb_read_start(&lPerfCache);
    lRes = fDoSomething(lArr, n);
    perf_cache_tlb_read_stop(&lPerfCache);
    printf("Calculated f(%d) = %d:\n", n, lRes);
    // printf("%lld LLC accesses, %lld LLC misses, ", lPerfCache._count_access_llc, lPerfCache._count_miss_llc);
    // printf("%lld L1d accesses, %lld L1d misses, ", lPerfCache._count_access_l1d, lPerfCache._count_miss_l1d);
    // printf("%lld dTLB accesses, %lld dTLB misses\n", lPerfCache._count_access_dtlb, lPerfCache._count_miss_dtlb);
    perf_cache_tlb_read_print(&lPerfCache);
  }
  free(lArr);
  perf_cache_tlb_read_close(&lPerfCache);
}

void
test3(const uint aHwThreadNo) {
  printf("=== test3 (perf_cache_rw_t) ===\n");
  struct perf_cache_rw_t lPerfCache;
  int lRc = 0;
  lRc = perf_cache_rw_open(&lPerfCache, aHwThreadNo);
  if(lRc) {
    printf("rc = %d\n", lRc);
    // return;
  }
  printf("fds = %d, %d, %d, %d, %d, %d, %d, %d\n",
         lPerfCache._fd_r_access_llc,
         lPerfCache._fd_r_miss_llc,
         lPerfCache._fd_r_access_l1d,
         lPerfCache._fd_r_miss_l1d,
         lPerfCache._fd_w_access_llc,
         lPerfCache._fd_w_miss_llc,
         lPerfCache._fd_w_access_l1d,
         lPerfCache._fd_w_miss_l1d);

  int* lArr = fAlloc(MX_LOG_SIZE);
  int lRes = 0;
  for(int n = 10; n <= MX_LOG_SIZE; ++n) {
    double lSizeKB = ((double) ((1 << n) * sizeof(int))) / ((double) 1024);
    fWrite(lArr, n); // cache warm up
    fRead(lArr, n);  // cache warm up
    fRead(lArr, n);  // cache warm up
    perf_cache_rw_start(&lPerfCache);
    lRes = fDoSomething(lArr, n);
    perf_cache_rw_stop(&lPerfCache);
    printf("Calculated f(%d) = %d, |A| = %.2f [KB]\n", n, lRes, lSizeKB);
    // printf("%lld LLC accesses, %lld LLC misses, ", lPerfCache._count_access_llc, lPerfCache._count_miss_llc);
    // printf("%lld L1d accesses, %lld L1d misses, ", lPerfCache._count_access_l1d, lPerfCache._count_miss_l1d);
    // printf("%lld dTLB accesses, %lld dTLB misses\n", lPerfCache._count_access_dtlb, lPerfCache._count_miss_dtlb);
    perf_rw_cache_print(&lPerfCache);
    printf("\n");
  }
  free(lArr);
  perf_cache_rw_close(&lPerfCache);
}

int
main() {
  int lHwThreadNo = 3;
  int lRc = 0;

  lRc = cbind_to_hw_thread(lHwThreadNo, 1);
  if(!lRc) {
    return -1;
  }

  // test0(lHwThreadNo);
  // test1(lHwThreadNo);
  // test2(lHwThreadNo);
  test3(lHwThreadNo);

  return 0;
}
