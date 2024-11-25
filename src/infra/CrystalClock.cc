#include "CrystalClock.hh"
#include <iostream>
#include <iomanip>
#include <limits>

extern "C" {
  #include "infra/cmeasure.h"
  // #include "infra/cbind_to_hw_thread.h"
}

double CrystalClock::_freq = -1;

CrystalClock::CrystalClock() : _begin(0), _end(0) {
}

uint64_t
CrystalClock::cycles() const {
  return cycles(begin(), end());
}

uint64_t
CrystalClock::cycles(const uint64_t aBegin, const uint64_t aEnd) {
  uint64_t lRes = 0;
  if(aBegin > aEnd) {
    lRes = (std::numeric_limits<uint64_t>::max() - aBegin) + aEnd;
  } else {
    lRes = aEnd - aBegin;
  }
  return lRes;
}

double
CrystalClock::frequency() {
  if(0 > _freq) {
    _freq = measure_frequency();
  }
  return _freq;
}

void
CrystalClock::init() {
  _freq = measure_frequency();
}

double
CrystalClock::measure_frequency() {
  constexpr bool lTrace = false;
  cmeasure_t lMeas;
  cmeasure_start(&lMeas);
  const uint64_t lBegin = current();
  do {
    cmeasure_stop(&lMeas);
  } while(1.0 > cmeasure_total_s(&lMeas));
  const uint64_t lEnd = current();
  const uint64_t lNoCyc = cycles(lBegin, lEnd);
  const double lFreq = (((double) (lNoCyc)) / cmeasure_total_s(&lMeas));
  if(lTrace) {
    std::cout << "CrystalClock::measure_frequency:" << std::endl;
    std::cout << "   time " << cmeasure_total_s(&lMeas) << " [s]" << std::endl;
    if(lEnd < lBegin) {
      std::cout << "   wrap arround." << std::endl;
    }
    std::cout << "   cycles    :" << lNoCyc << std::endl;
    std::cout << "   frequency :" << lFreq   << std::endl;
  }
  return lFreq;
}

double
CrystalClock::duration_ns() const {
  return duration_s() * ((double) 1000 * 1000 * 1000);
}

double
CrystalClock::duration_us() const {
  return duration_s() * ((double) 1000 * 1000);
}

double
CrystalClock::duration_ms() const {
  return duration_s() * 1000;
}

double
CrystalClock::duration_s()  const {
  return (((double) cycles()) / frequency());
}

double
CrystalClock::duration_ns(const uint64_t aNoCycles) {
    return duration_s(aNoCycles) * ((double) 1000 * 1000 * 1000);

}

double
CrystalClock::duration_us(const uint64_t aNoCycles) {
    return duration_s(aNoCycles) * ((double) 1000 * 1000);
}

double
CrystalClock::duration_ms(const uint64_t aNoCycles) {
    return duration_s(aNoCycles) * ((double) 1000);
}

double
CrystalClock::duration_s(const uint64_t aNoCycles) {
    return (((double) aNoCycles) / frequency());
}

double
CrystalClock::duration_min(const uint64_t aNoCycles) {
  return (duration_s(aNoCycles) / 60);
}

double
CrystalClock::duration_h(const uint64_t aNoCycles) {
  return (duration_s(aNoCycles) / 3600);
}


