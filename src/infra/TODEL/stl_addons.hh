#ifndef INFRA_STL_ADD_ONS_HH
#define INFRA_STL_ADD_ONS_HH

#include <algorithm>

template<class Tbulk, class Telement>
bool
contains(const Tbulk& x, const Telement& y) {
  return x.end() != find(x.begin(), x.end(), y);
}

#endif
