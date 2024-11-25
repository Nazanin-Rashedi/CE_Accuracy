#ifndef LOG_CLASSES_HH
#define LOG_CLASSES_HH

#include "infra/glob_infra_standard_includes.hh"
#include <cmath>

inline uint
selectivity_class(const double aSelectivity) {
  return ((uint) std::floor(std::log2(double{1} / aSelectivity)));
}

inline uint
selectivity_class(const double aCardSel, const double aCardTot) {
  return selectivity_class(aCardSel / aCardTot);
}

inline uint
cardinality_class(const double aCardinality) {
  return ((uint) std::floor(std::log2(aCardinality)));
}

inline uint
qerror_class(const double aQError) {
  return std::min<uint>(32, ((uint) std::floor(std::log(aQError) / std::log(std::sqrt(2)))));
}

inline char
qerror_class_as_char(const double aQError) {
  const uint c = qerror_class(aQError);
  char lRes = 'X';
  if(10 > c) {
    lRes = '0' + c;
  } else
  if(16 > c) {
    lRes = 'A' + (c - 10);
  }
  return lRes;
}

inline uint
budget_class(const double aBudget, const double aTotalSize) {
  return ((uint) std::floor(std::log2(aTotalSize / aBudget)));
}


#endif


