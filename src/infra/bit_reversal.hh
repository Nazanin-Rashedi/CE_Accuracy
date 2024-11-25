#ifndef GLOB_INFRA_BIT_REVERSAL_HH
#define GLOB_INFRA_BIT_REVERSAL_HH


#include "infra/bit_subsets.hh"


template<typename Tuint>
Tuint
bit_reverse(const Tuint x) {
  const uint32_t lSize = 8 * sizeof(Tuint) - 1;
  Tuint r = 0;
  for(BvMemberIdxAsc<Tuint> lIter(x); lIter.isValid(); ++lIter) {
    r |= ((Tuint) 1) << (lSize - (*lIter));
  }
  return r;
}



#endif
