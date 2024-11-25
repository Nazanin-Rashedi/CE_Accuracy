#include <iostream>
#include <iomanip>
#include <format>
#include <vector>

#include <stdlib.h>


extern "C" {
  #include "cbind_to_hw_thread.h"
  #include "perf_wrapper.h"
}

#include "CrystalClock.hh"


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
  for(int i = lSize - 1; i > 0; --i) {
    uint lIdx = ((uint) random()) % i;
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


/*
 * struct cache_tlb_t
 * holds measurements of a single run of some algorithm
 */

struct cache_tlb_t {
  static constexpr double M = 1024 * 1024;
  double _size_mb;    // size of matrix [MB]
  double _runtime_ms; // in ms
  uint   _no_iter;
  double _l3_access;
  double _l3_miss;
  double _l1_access;
  double _l1_miss;
  double _dtlb_access;
  double _dtlb_miss;
  cache_tlb_t() = default;
  inline void clear() { _runtime_ms = 0;
                        _no_iter = 0;
                        _l3_access = 0;
                        _l3_miss = 0;
                        _l1_access = 0;
                        _l1_miss = 0;
                        _dtlb_access = 0;
                        _dtlb_miss = 0;
                      }
  inline double size_mb() const { return _size_mb; }
  inline double runtime_ms() const { return _runtime_ms; }
  inline double runtime_s()  const { return (_runtime_ms / 1000); }
  inline uint   no_iter() const { return _no_iter; }
  inline double l3_access() const { return _l3_access; }
  inline double l3_miss() const { return _l3_miss; }
  inline double l1_access() const { return _l1_access; }
  inline double l1_miss() const { return _l1_miss; }
  inline double dtlb_access() const { return _dtlb_access; }
  inline double dtlb_miss() const { return _dtlb_miss; }
  inline double thruput(const double aBaseMB) const { return (aBaseMB / runtime_s()); } // [MB/s]
  inline double l3_miss_rate() const { return 100 * (l3_miss() / l3_access()); }
  inline double l3_read_ratio(const double aBaseMB) const { return (((l3_access() * 64) / M) / aBaseMB); }
  inline double l3_miss_ratio(const double aBaseMB) const { return (((l3_miss() * 64) / M) / aBaseMB); }
  inline double mm_bandwidth() const { return (((l3_miss() * 64) / M) / runtime_s()); }
  inline double l1_miss_rate() const { return 100 * (l1_miss() / l1_access()); }
  inline double l1_read_ratio(const double aBaseMB) const { return (((l1_access() * 64) / M) / aBaseMB); }
  inline double l1_miss_ratio(const double aBaseMB) const { return (((l1_miss() * 64) / M) / aBaseMB); }
  inline double dtlb_miss_rate() const { return 100 * (dtlb_miss() / dtlb_access()); }
  std::ostream& print(std::ostream& os) const;
};

using cache_tlb_vt = std::vector<cache_tlb_t>;

std::ostream&
cache_tlb_t::print(std::ostream& os) const {
  os << " " << std::format("{:6.2f}", runtime_s()) << " [s]"
     << " " << std::format("{:6.2f}", thruput(size_mb())) << " [MB/s]"
     << " " << std::format("{:6.2f}", l3_miss_rate())   << " %"
     << " " << std::format("{:6.2f}", l3_miss_ratio(size_mb())) << " MR"
     << " " << std::format("{:6.2f}", l3_read_ratio(size_mb())) << " RR"
     << " " << std::format("{:6.1f}", mm_bandwidth()) << " BW";
  return os;
}

std::ostream&
operator<<(std::ostream& os, const cache_tlb_t& x) {
  return x.print(os);
}

using cache_tlb_vt  = std::vector<cache_tlb_t>;
using cache_tlb_vvt = std::vector<cache_tlb_vt>;

void
test0(perf_cache_tlb_read_t* aPerfCache) {
  printf("=== test2 ===\n");
  CrystalClock lClock;
  cache_tlb_t lNumbers;
  int    lRes = 0;
  uint   lNoIter = 3;
  size_t m = 1 << 16;
  size_t n = 1 << 16;
  size_t lSizeMb = ((double) ((n * m) / 8)) / (1024 * 1024);
  int* lArr = fAlloc(MX_LOG_SIZE);
  for(int n = 10; n <= MX_LOG_SIZE; ++n) {
    std::cout << "round " << n << std::endl;
    perf_cache_tlb_read_start(aPerfCache);
    lClock.start();
    for(uint k = 0; k < lNoIter; ++k) {
      lRes = fDoSomething(lArr, n);
    }
    lClock.stop();
    const double lTransposeTimeMs = lClock.duration_ms();
    perf_cache_tlb_read_stop(aPerfCache);
    std::cout << "result calculated: " << lRes << std::endl;
    /*
    // print in C manually:
    printf("calculated f(%d) = %d:\n", n, lRes);
    printf("%lld LLC accesses, %lld LLC misses, ", aPerfCache->_count_access_llc, aPerfCache->_count_miss_llc);
    printf("%lld L1d accesses, %lld L1d misses, ", aPerfCache->_count_access_l1d, aPerfCache->_count_miss_l1d);
    printf("%lld dTLB accesses, %lld dTLB misses\n", aPerfCache->_count_access_dtlb, aPerfCache->_count_miss_dtlb);
    */
    // print in C using printing routine:
    perf_cache_tlb_read_print(aPerfCache);
    // copy nach cache_tlb_t:
    lNumbers._size_mb = lSizeMb; // muss natuerlich richtige zahl sein
    lNumbers._no_iter = lNoIter; // nur falls man es sich merken will, wird nicht wirklich gebraucht
    // wichtig:
    lNumbers._runtime_ms  = lTransposeTimeMs;
    lNumbers._no_iter     = lNoIter;
    lNumbers._l3_access   = ((double) aPerfCache->_count_access_llc)  / lNoIter;
    lNumbers._l3_miss     = ((double) aPerfCache->_count_miss_llc)    / lNoIter;
    lNumbers._l1_access   = ((double) aPerfCache->_count_access_l1d)  / lNoIter;
    lNumbers._l1_miss     = ((double) aPerfCache->_count_miss_l1d)    / lNoIter;
    lNumbers._dtlb_access = ((double) aPerfCache->_count_access_dtlb) / lNoIter;
    lNumbers._dtlb_miss   = ((double) aPerfCache->_count_miss_dtlb)   / lNoIter;
    std::cout << "numbers:" << std::endl;
    lNumbers.print(std::cout) << std::endl;
    std::cout << "====" << std::endl;
  }
}

struct cache_rw_t {
  static constexpr double M = 1024 * 1024;
  double _size_mb;    // size of matrix [MB]
  double _runtime_ms; // in ms
  uint   _no_iter;
  double _l3_r_access;
  double _l3_r_miss;
  double _l1_r_access;
  double _l1_r_miss; 
  double _l3_w_access;
  double _l3_w_miss;
  double _l1_w_access;
  double _l1_w_miss; 
  cache_rw_t() = default;
  inline void clear() { _runtime_ms = 0;
                        _no_iter = 0; 
                        _l3_r_access = 0;
                        _l3_r_miss = 0;
                        _l1_r_access = 0;
                        _l1_r_miss = 0;
                        _l3_w_access = 0;
                        _l3_w_miss = 0;
                        _l1_w_access = 0;
                        _l1_w_miss = 0;
                      }
  inline double size_mb() const { return _size_mb; }
  inline double runtime_ms() const { return _runtime_ms; }
  inline double runtime_s()  const { return (_runtime_ms / 1000); }
  inline uint   no_iter() const { return _no_iter; }
  inline double l3_r_access() const { return _l3_r_access; }
  inline double l3_r_miss() const { return _l3_r_miss; }
  inline double l1_r_access() const { return _l1_r_access; }
  inline double l1_r_miss() const { return _l1_r_miss; }
  inline double l3_w_access() const { return _l3_w_access; }
  inline double l3_w_miss() const { return _l3_w_miss; }
  inline double l1_w_access() const { return _l1_w_access; }
  inline double l1_w_miss() const { return _l1_w_miss; }
  inline double l3_r_miss_rate() const { return 100 * (l3_r_miss() / l3_r_access()); }
  inline double l1_r_miss_rate() const { return 100 * (l1_r_miss() / l1_r_access()); }
  inline double l3_w_miss_rate() const { return 100 * (l3_w_miss() / l3_w_access()); }
  inline double l1_w_miss_rate() const { return 100 * (l1_w_miss() / l1_w_access()); }

  inline double mm_r_bandwidth() const { return (((l3_r_miss() * 64) / M) / runtime_s()); }
  inline double mm_w_bandwidth() const { return (((l3_w_miss() * 64) / M) / runtime_s()); }

  inline double l3_read_ratio(const double aBaseMB) const { return (((l3_r_access() * 64) / M) / aBaseMB); }
  inline double l1_read_ratio(const double aBaseMB) const { return (((l1_r_access() * 64) / M) / aBaseMB); }
  inline double l3_write_ratio(const double aBaseMB) const { return (((l3_w_access() * 64) / M) / aBaseMB); }
  inline double l1_write_ratio(const double aBaseMB) const { return (((l1_w_access() * 64) / M) / aBaseMB); }

  inline double thruput(const double aBaseMB) const { return (aBaseMB / runtime_s()); } // [MB/s]
  std::ostream& print(std::ostream& os) const;
};

using cache_rw_vt = std::vector<cache_rw_t>;

std::ostream&
cache_rw_t::print(std::ostream& os) const {
  os << " " << std::format("{:6.2f}", runtime_s()) << " [s]"
     << " " << std::format("{:6.2f}", thruput(size_mb())) << " [MB/s]"
     << " " << std::format("{:6.2f}", l1_r_miss_rate())   << " [l1r%]"
     << " " << std::format("{:6.2f}", l1_w_miss_rate())   << " [l1w%]"
     << " " << std::format("{:6.2f}", l3_r_miss_rate())   << " [l3r%]"
     << " " << std::format("{:6.2f}", l3_w_miss_rate())   << " [l3w%]"
     << " " << std::format("{:6.1f}", mm_r_bandwidth())   << " rdBW"
     << " " << std::format("{:6.1f}", mm_w_bandwidth())   << " wrBW";
  return os;
}

std::ostream&
operator<<(std::ostream& os, const cache_rw_t& x) {
  return x.print(os);
}

using cache_rw_vt  = std::vector<cache_rw_t>;
using cache_rw_vvt = std::vector<cache_rw_vt>;


void
test1(perf_cache_rw_t* aPerfCache) {
  printf("=== test1 ===\n");
  CrystalClock lClock;
  cache_rw_t lNumbers;
  int    lRes = 0;
  uint   lNoIter = 3;
  size_t m = 1 << 16;
  size_t n = 1 << 16;
  size_t lSizeMb = ((double) ((n * m) / 8)) / (1024 * 1024);
  int* lArr = fAlloc(MX_LOG_SIZE);
  for(int n = 10; n < MX_LOG_SIZE; ++n) {
    std::cout << "round " << n << std::endl;
    perf_cache_rw_start(aPerfCache);
    lClock.start();
    for(uint k = 0; k < lNoIter; ++k) {
      lRes = fDoSomething(lArr, n);
    }
    lClock.stop(); 
    const double lTransposeTimeMs = lClock.duration_ms();
    perf_cache_rw_stop(aPerfCache);
    std::cout << "result calculated: " << lRes << std::endl;
    /*
    // print in C manually:
    printf("calculated f(%d) = %d:\n", n, lRes);
    printf("%lld LLC accesses, %lld LLC misses, ", aPerfCache->_count_access_llc, aPerfCache->_count_miss_llc);
    printf("%lld L1d accesses, %lld L1d misses, ", aPerfCache->_count_access_l1d, aPerfCache->_count_miss_l1d);
    printf("%lld dTLB accesses, %lld dTLB misses\n", aPerfCache->_count_access_dtlb, aPerfCache->_count_miss_dtlb);
    */
    // print in C using printing routine:
    perf_rw_cache_print(aPerfCache);
    // copy nach cache_tlb_t:
    lNumbers._size_mb = lSizeMb; // muss natuerlich richtige zahl sein
    lNumbers._no_iter = lNoIter; // nur falls man es sich merken will, wird nicht wirklich gebraucht
    // wichtig:
    lNumbers._runtime_ms  = lTransposeTimeMs;
    lNumbers._no_iter     = lNoIter;
    lNumbers._l3_r_access   = ((double) aPerfCache->_count_r_access_llc)  / lNoIter;
    lNumbers._l3_r_miss     = ((double) aPerfCache->_count_r_miss_llc)    / lNoIter;
    lNumbers._l1_r_access   = ((double) aPerfCache->_count_r_access_l1d)  / lNoIter;
    lNumbers._l1_r_miss     = ((double) aPerfCache->_count_r_miss_l1d)    / lNoIter;
    lNumbers._l3_w_access   = ((double) aPerfCache->_count_w_access_llc)  / lNoIter;
    lNumbers._l3_w_miss     = ((double) aPerfCache->_count_w_miss_llc)    / lNoIter;
    lNumbers._l1_w_access   = ((double) aPerfCache->_count_w_access_l1d)  / lNoIter;
    lNumbers._l1_w_miss     = ((double) aPerfCache->_count_w_miss_l1d)    / lNoIter;
    std::cout << "numbers:" << std::endl;
    lNumbers.print(std::cout) << std::endl;
    std::cout << "====" << std::endl;
  }
}

int
main() {
  int lHwThreadNo = 4;
  cbind_to_hw_thread(lHwThreadNo, 1);

  int lRc = 0; // return code

  // prepare and open performance counters for cache observations
  struct perf_cache_tlb_read_t lPerfCache;
  lRc = perf_cache_tlb_read_open(&lPerfCache, lHwThreadNo); // PMU core dependent
  if(lRc) { return -1; }
  // close performance counters for cache observations
  perf_cache_tlb_read_close(&lPerfCache);

  test0(&lPerfCache);

  struct perf_cache_rw_t lPerfRwCache;
  lRc = perf_cache_rw_open(&lPerfRwCache, lHwThreadNo);
  if(lRc) { return -1; }
  test1(&lPerfRwCache);
  perf_cache_rw_close(&lPerfRwCache);

  return 0;
}

