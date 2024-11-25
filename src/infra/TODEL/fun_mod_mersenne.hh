#ifndef FUN_MOD_MERSENNE_HH
#define FUN_MOD_MERSENNE_HH

#include "infra/glob_infra_standard_includes.hh"

// outsourced loops

uint32_t loop_regular_mod32(const uint32_t p, uint32_t* x, const uint aSize);
uint32_t loop_mersenne_mod32(const uint32_t p, const uint32_t a, uint32_t* x, const uint aSize);

uint64_t loop_regular_mod64(const uint64_t p, uint64_t* x, const uint aSize);
uint64_t loop_mersenne_mod64(const uint64_t p, const uint64_t a, uint64_t* x, const uint aSize);

#endif
