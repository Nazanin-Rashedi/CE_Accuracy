#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>

#include "../infra/tmath.hh"
#include "SamplingAlphaOmegaNewton.hh"

/*
 *
 */



/*
 *  latex table header and footer
 */



struct arg_t {
  uint64_t _card;
  uint64_t _smplsz;
  int64_t  _noqualsmpl;
  double   _eps;
  arg_t() : _card(0), _smplsz(0), _noqualsmpl(-1), _eps(1e-5L) {}
};


int64_t
get_num(const char* x) {
  char* y = 0;
  int64_t z = strtoll(x, &y, 10);
  if(x == y) {
    return 0;
  }
  return z;
}

void
print_usage(const int argc, const char* argv[]) {
  std::cout << "usage: " << argv[0] << " [-eps <eps>] <|R|> <|sample|> [<|noqualsmpl|>]" << std::endl;
}

bool
parse_args(arg_t& aArg, const int argc, const char* argv[]) {
  if((3 > argc) || (6 < argc)) {
    print_usage(argc,argv);
    return false;
  }
  int lCurr = 1;
  char* lCharPtr = 0;
  if(0 == strcmp("-eps", argv[lCurr])) {
    ++lCurr;
    aArg._eps = strtod(argv[lCurr], &lCharPtr);
    if(argv[lCurr] == lCharPtr) {
      return false;
    }
    ++lCurr;
  }
  if(lCurr < argc) {
    aArg._card   = get_num(argv[lCurr++]);
  } else {
    return false;
  }
  if(lCurr < argc) {
    aArg._smplsz = get_num(argv[lCurr++]);
  } else {
    return false;
  }
  aArg._noqualsmpl  = (lCurr < argc ? get_num(argv[lCurr++]) : 0);
  if((0 >= aArg._card) || (0 >= aArg._smplsz)) {
    print_usage(argc,argv);
    return false;
  }
  return true;
}

int
main(const int argc, const char* argv[]) {

  arg_t lArg;

  if(!parse_args(lArg, argc, argv)) {
    print_usage(argc, argv);
    return -1;
  }
  

  if(0 <= lArg._noqualsmpl) {
    SamplingAlphaOmegaNewton lSaoN(lArg._card, lArg._smplsz);
    const double lAlphaN = lSaoN.alpha(lArg._noqualsmpl);
    const double lOmegaN = lSaoN.omega(lArg._noqualsmpl);
    std::cout << "     alpha = " << lAlphaN     << " (" << ((uint64_t) std::round(lAlphaN)) << ')' << std::endl
              << "     omega = " << lOmegaN     << " (" << ((uint64_t) std::round(lOmegaN)) << ')' << std::endl;

  }
  return 0;
}

