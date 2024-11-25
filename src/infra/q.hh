#ifndef SRC_INFRA_Q_UTILS_HH
#define SRC_INFRA_Q_UTILS_HH

#include <inttypes.h>
#include <math.h>
#include <limits>

#include "bit_intrinsics.hh"
#include "tmath.hh"

namespace q {


// utils for q-compression:
// uses math templates (mt.hh)
// calcBase calculates the base given the largest number possible to be compressed
//          as well as the number of bits used to hold the q-compressed value
// encode encodes a given value (aFreq) with a given base and a limiting number of bits.
// decode turns the q-compressed value into an estimate of the original value

template<class Tfloat>
Tfloat
calcBase(const Tfloat aNumber, const unsigned int aNoBits) {
  const Tfloat aMaxNoForBits = (Tfloat) ( ((unsigned int) 1 << aNoBits) - 1);
  return mt::powt<Tfloat>(aNumber, ((Tfloat) 1.0 / aMaxNoForBits));
}

template<class Tfloat>
uint32_t 
encode(const Tfloat aFreq, const Tfloat aBase, const uint32_t aLimit) {
   const uint32_t lQ = (0 == aFreq) ?
                       0 :
                       (((uint32_t) mt::floort<Tfloat>(mt::logbaset<Tfloat>(aFreq, aBase))) + 1);
   return (lQ > aLimit) ? aLimit : lQ;
}

template<class Tfloat>
Tfloat   
decode(const uint32_t aCode, const Tfloat aBase) {
  if(0 == aCode) {
    return (Tfloat) 0;
  }
  return mt::powt<Tfloat>(aBase, (Tfloat) (aCode - 1) + (Tfloat) 0.5);
}

template<class Tfloat>
uint32_t
qcompress(const Tfloat aFreq, const Tfloat aBase, const uint32_t aLimit) {
   const uint32_t lQ = (0 == aFreq) ?
                       0 :
                       (((uint32_t) mt::floort<Tfloat>(mt::logbaset<Tfloat>(aFreq, aBase))) + 1);
   return (lQ > aLimit) ? aLimit : lQ;
}

template<class Tfloat>
Tfloat
qdecompress(const uint32_t aCode, const Tfloat aBase) {
  if(0 == aCode) {
    return (Tfloat) 0;
  }
  return mt::powt<Tfloat>(aBase, (Tfloat) (aCode - 1) + (Tfloat) 0.5);
}


// another possibility to do q-compression is the following
// we compress an unsigned integer value into 1 byte (8bit)
// three bits are used to keep the highest three bits
// five bits are used to keep the shift

template<class Tuint>
unsigned int
compress8(const Tuint x) {
  unsigned int l3bit = 0;
  unsigned int lShift = 0;
  if(8 > x) {
    l3bit = x;
    lShift = 0;
  } else {
    lShift = idx_highest_bit_set(x) - 2;
    l3bit  = (x >> lShift) & 0x7;
  }
  // std::cout << std::setw(3) << l3bit << ' '
  //           << std::setw(3) << lShift << ' ';
  return ((l3bit << 5) | lShift);
}


/*
  The q-middle of $2^n$ and $2^{n-1} - 1$ is
  \sqrt{2^n * (2^{n+1} - 1)}
  Thus, this is the perfect estimate.
  since we do not want to compute sqrt, we do the following approximation:
  The above is approximately equal to 
  \sqrt{2^n * 2^{n+1}}
  which is equal to
  \sqrt{2^2n * 2}
  which is
  \sqrt{2} * 2^n
  which in turn is
  2^n + (\sqrt{2} - 1) * 2^n
  the second part can be calculated by a constant shifted by n to the left

  variant 2 implements this.
  q::decompress: 3.1369 ns
         qerror: 1.17647
  variant 1:
  q::decompress: 2.7846 ns 
         qerror: 1.13271

*/

template<class Tuint>
Tuint
decompress8(const unsigned int x) {
  const unsigned int lShift = (x & 0x1F);
  const unsigned int l3bit  = (x >> 5) & 0x7;
  Tuint lRes = (l3bit << lShift);
  // Variante 1: rough approximation of sqrt(2)
  //. if(1 < lShift) {
  //.   lRes |= (1 << (lShift - 2));
  //. }

  // Variante 2: better approximation of sqrt(2)
  // wg. (1 + 1/4 + 1/8 + 1/32 + 1/128 + 1/2^13 + 1/2^16 + 1/2^17 + 1/2^18 
  //      1/2^19 + 1/2^22 + 1/2^23  + 1/2^26 + 1/2^27 + 1/2^30 + 1/2^31) = 1.414213562151412
  // und                                                         sqrt(2) = 1.414213562373095
  // ergibt sich das bitmuster (von links nach rechts, 1 ignoriert)
  // 0110 1010 0000 1001 1110 0110 0110 0110
  // die zeile
   const unsigned int lSqrt2 = ~ - (int) (((sqrt((double) 2.0) - 1.0) * 4) * (1 << 30));
  // ohne den vorspann ergibt sich 
  // 0110 1010 0000 1001 1110 0110 0110 0111 
  // was leicht ueber sqrt(2) liegt. daher der vorspann

  // wir muessen lSqrt2 nur noch an die passende stelle shiften:
  lRes |= (lSqrt2 >> (32 - lShift)) & ((1 << lShift) - 1);

  return lRes;
}


// now the same as a template function
// Tuint: class for input/output of real numbers, in compressed form uint is used
// Tnokeptbits: number of bits stored
// Tnoshiftbits: number of bits used to store shift
// thus, the result has Tnokeptbits + Tnoshiftshift bits in total

template<class Tuint, unsigned int Tnokeptbits, unsigned int Tnoshiftbits>
unsigned int
bqcompress(const Tuint x) {
  unsigned int l3bit = 0;
  unsigned int lShift = 0;
  if((1 << Tnokeptbits) > x) {
    l3bit = x;
    lShift = 0;
  } else {
    lShift = idx_highest_bit_set(x) - Tnokeptbits + 1;
    l3bit  = (x >> lShift) & ((1 << Tnokeptbits) - 1);
  }
  // std::cout << "-c- " 
  //           << std::setw(3) << x << ' '
  //           << std::setw(3) << l3bit << ' '
  //           << std::setw(3) << lShift << ' '
  //           << std::endl;
  return ((l3bit << Tnoshiftbits) | lShift);
}

template<class Tuint, unsigned int Tnokeptbits, unsigned int Tnoshiftbits>
Tuint
bqdecompress(const unsigned int x) {
  const unsigned int lShift = (x & ((1 << Tnoshiftbits) - 1));
  const unsigned int l3bit  = ((x >> Tnoshiftbits) & ((1 << Tnokeptbits) - 1));
  Tuint lRes = (l3bit << lShift);
  const unsigned int lSqrt2 = ~ - (int) (((sqrt((double) 2.0) - 1.0) * 4) *  (1 << 30));
  lRes |= (lSqrt2 >> (32 - lShift)) & ((1 << lShift) - 1);
  // std::cout << "-d- " 
  //           << std::setw(3) << lRes << ' '
  //           << std::setw(3) << l3bit << ' '
  //           << std::setw(3) << lShift << ' '
  //           << std::endl;
  return lRes;
}





// qerror functions

// simple q-error function
template<class Tfloat>
Tfloat
qerror(const Tfloat x, const Tfloat y) {
  if(x == y) { return (Tfloat) 1; }
  if(x >= y) {
    return (x/y);
  }
  return (y/x);
}

template<class Tfloat>
inline Tfloat
qerrorT1(const Tfloat x, const Tfloat y) {
  if(x >= y) {
    return (x/y);
  }
  return (y/x);
}

template<class Tfloat>
inline Tfloat
qerrorT2(const Tfloat x, const Tfloat y) {
  const bool lTest = (x >= y);
  Tfloat lRes[2];
  lRes[1 - lTest] = (x / y);
  lRes[    lTest] = (y / x);
  return lRes[0];
}

template<class Tfloat>
inline Tfloat
qerrorT3(const Tfloat x, const Tfloat y) {
  if(x == y) {
    return (Tfloat) 1;
  } 
  if(x < y) {
    return (y/x);
  }
  return (x/y);
}


template<class Tfloat>
inline Tfloat
p_error(const Tfloat aEstimate, const Tfloat aTrueValue) {
  Tfloat lEstimate = aEstimate;
  Tfloat lTrueValue = aTrueValue;
  if(0.0 == aEstimate) { lEstimate = 1.0; }
  if(0.0 == aTrueValue) { lTrueValue = 1.0; }
  if(lEstimate >= lTrueValue) {
    return (lEstimate / lTrueValue) - 1.0;
  } else {
    return -((lTrueValue / lEstimate) - 1.0);
  }
}

template<class Tfloat>
inline Tfloat
perror(const Tfloat aEstimate, const Tfloat aTrueValue) {
  if(aEstimate == aTrueValue) { return 0; }
  if(aEstimate >= aTrueValue) {
    return (aEstimate / aTrueValue) - 1;
  }
  return 1 - (aTrueValue / aEstimate);
}

template<class Tfloat>
inline Tfloat
q_value(const Tfloat x) {
 if((Tfloat) 0 >= x) {
    return std::numeric_limits<Tfloat>::max();
 }
 return ((1 <= x) ? x : ((Tfloat) 1 / x));
}


template<class Tfloat>
inline Tfloat
q_middle(const Tfloat x, const Tfloat y) {
  if((0 == x) && (0 != y)) {
    return mt::sqrtt<Tfloat>(y);
  } else
  if((0 != x) && (0 == y)) {
    return mt::sqrtt<Tfloat>(x);
  }
  return mt::sqrtt<Tfloat>(x * y);
}

template<typename Tfloat>
inline Tfloat 
q_spread(const Tfloat x, const Tfloat y) {
  if((0 == x) && (0 == y)) {
    return 1;
  } else
  if((0 == x) && (0 != y)) {
    return mt::sqrtt<Tfloat>(y);
  } else
  if((0 != x) && (0 == y)) {
    return mt::sqrtt<Tfloat>(x);
  } else
  if(x < y) {
    return mt::sqrtt<Tfloat>(y / x);
  } 
  return mt::sqrtt<Tfloat>(x / y);
}

template<class Tfloat>
inline Tfloat
q_error(const Tfloat x, const Tfloat y) {
  if(((Tfloat) 0 == x) || ((Tfloat) 0 == y)) {
    return std::numeric_limits<double>::max();
  }

  if((x < 0 && y > 0) || (x > 0 && y < 0)) {
    return std::numeric_limits<double>::max();
  }

  return ((x < y) ? (y/x) : (x/y));
}

template<class Tfloat>
inline Tfloat
q_error_safe(const Tfloat x, const Tfloat y) {
  const Tfloat a = (x == 0) ? 1 : x;
  const Tfloat b = (y == 0) ? 1 : y;
  return ((a < b) ? (b/a) : (a/b));
}

template<class Tfloat>
inline bool
is_theta_q_acceptable(const Tfloat aEst, const Tfloat aTru, const Tfloat aTheta, const Tfloat aQ) {
  return (((aEst <= aTheta) && (aTru <= aTheta)) || (q::qerror<Tfloat>(aEst, aTru) <= aQ));
}



} // end namespace


#endif


