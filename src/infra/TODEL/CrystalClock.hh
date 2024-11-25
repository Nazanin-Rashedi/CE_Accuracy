#ifndef CRYSTAL_CLOCK_HH
#define CRYSTAL_CLOCK_HH
#pragma once

#include <inttypes.h>

#ifdef __x86_64__
  #include <x86intrin.h>
#endif

/*
 *  CrystalClock
 *  measures execution time in clock ticks using rdtsc on X86 and cntvtl_el0 on ARM
 *  host  frequency
 *  pluto      2.594  GHz (measured)
 *  apollo     3.592  GHz (measured)
 *  pi3-hiwi1  2.496  GHz (from cpuid)
 *  pi3-hiwi2  2.4192 GHz (from cpuid)
 *  pi3-hiwi1: gm_cpuid:
 *   zCpuid.pi3_hiwi1: EAX   TSC_CCC_DEN: 2         // denominator of the TSC/core crystal clock ratio 
 *   zCpuid.pi3_hiwi1: EBX   TSC_CCC_NUM: 208       // numerator of the TSC/core crystal clock ratio 
 *   zCpuid.pi3_hiwi1: ECX   TSC_CCC_NOM: 24000000  // nominal frequency of the core crystal clock in Hz 
 *   CPUID.15H.ECX[31:0] * CPUID.15H.EBX[31:0] ) ÷ CPUID.15H.EAX[31:0]
 *   (208 * 24000000) / 2 = 2.496.000.000
 *  pi3-hiwi2: gm_cpuid:
 *   (126 * 38400000) / 2 = 2.419.200.000
 */


class CrystalClock {
  public:
    CrystalClock();
  public:
    inline void start() { _begin = current(); }
    inline void stop()  { _end   = current(); }
  public:
    inline uint64_t begin()  const { return _begin; }
    inline uint64_t end()    const { return _end; }
    inline bool     wrap()   const { return (begin() > end()); }
           uint64_t cycles() const; // number of cycles between begin and end, assumes max one wrap around
  public:
    // return duration between begin/end; may require call to measure_frequency
    double duration_ns() const; // nanoseconds
    double duration_us() const; // microseconds
    double duration_ms() const; // milliseconds
    double duration_s()  const; // seconds
  public:
    static inline uint64_t current();
    static        uint64_t cycles(const uint64_t aBegin, const uint64_t aEnd); // assumes max one wrap around
    static        double    duration_ns(const uint64_t aNoCycles);
    static        double    duration_us(const uint64_t aNoCycles);
    static        double    duration_ms(const uint64_t aNoCycles);
    static        double    duration_s(const uint64_t aNoCycles);
    static        double    duration_min(const uint64_t aNoCycles);
    static        double    duration_h(const uint64_t aNoCycles);
  public:
    static void init(); // enforces frequency measurement
    static double frequency(); // first call takes one second
  private:
    uint64_t _begin;
    uint64_t _end;
  private:
    static double measure_frequency();
  private:
    static double _freq;
};

uint64_t
CrystalClock::current() {
  uint64_t lCyc = 0;
  #ifdef __x86_64__
    lCyc =  __rdtsc();
  #elif (__ARM_ARCH == 8)
    asm volatile("mrs %0, cntvct_el0" : "=r" (lCyc));
  #else 
    #error CrystalClock: unsupported architecture
  #endif
  return lCyc;
}

#endif
