#ifndef GLOB_INFRA_GGT_HH
#define GLOB_INFRA_GGT_HH

#include "infra/glob_infra_standard_includes.hh"

template<typename Tint>
Tint ggt(Tint a, Tint b) {
  if(0 > a) { a = -a; }
  if(0 > b) { b = -b; }
  if(0 == a) { return b; }
  if(0 == b) { return a; }
  if(a == b) { return a; }
  if(a < b) { std::swap(a,b); }
  Tint r = 0;
  do {
    r = a % b;
    a = b;
    b = r;
  } while (r != 0);
  return a;
}

#endif
