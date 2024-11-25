#ifndef SRC_INFRA_RANDOM_STRING_HH
#define SRC_INFRA_RANDOM_STRING_HH

#include <stdio.h>
#include <random>
#include "infra/bitvectorsmall.hh"

// generate a random string of length n into char array x
// of [A-Z]
char* fRandomString(char* x, const size_t n);

// generate a random string of length n into char array x
// containing 0 and 1, 1 with probability 1/p
char* fRandomString01(char* x, const size_t n, const int p);

// for a given (random) number aNumber, put its [01] representation
// into string x (use first n bits only)

char* fUint2Char(char* x, const unsigned int n, const unsigned int aNumber);

#endif
