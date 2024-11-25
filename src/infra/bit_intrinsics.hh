#ifndef SRC_INFRA_BIT_INTRINSICS_HH
#define SRC_INFRA_BIT_INTRINSICS_HH

#include <inttypes.h>
#include <limits.h>
#include <limits>

#ifdef __x86_64
#include <immintrin.h>
#endif

/*
 * return the index of the highest bit set
 */

template<class Tuint>
inline uint32_t
idx_highest_bit_set(const Tuint x);


template<>
inline uint32_t
idx_highest_bit_set<uint32_t>(const uint32_t x) {
#if __ICC
  return (_bit_scan_reverse(x));
#elif __GNUG__
  return (31 - __builtin_clz(x));
#elif _MSC_VER
  // microsoft compiler
  #pragma intrinsic(_BitScanReverse)
  unsigned long lIdx;
  unsigned char lIsNonZero;
  lIsNonZero = _BitScanReverse(&lIdx, x);
  if(!lIsNonZero) {
  lIdx = 32;
  return lIdx;
#endif
}

template<>
inline uint32_t
idx_highest_bit_set<uint64_t>(const uint64_t x) {
#if __ICC
  return (63 - __builtin_clzll(x)); // gibt kein _bit_scan_reverse64
#elif __GNUG__
  return (63 - __builtin_clzll(x));
#elif _MSC_VER
  // microsoft compiler
  #pragma intrinsic(_BitScanReverse64)
  unsigned long lIdx;
  unsigned char lIsNonZero;
  lIsNonZero = _BitScanReverse64(&lIdx, x);
  if(!lIsNonZero) {
  lIdx = 64;
  return lIdx;

#endif
}

/*
 * return the index of the lowest bit set
 */

template<class Tuint>
inline uint32_t
idx_lowest_bit_set(const Tuint x);

template<>
inline uint32_t
idx_lowest_bit_set<uint8_t>(const uint8_t x) {
#if __ICC
  return (_bit_scan_forward((uint32_t) x));
#elif __GNUG__
  return (__builtin_ctz((uint32_t) x));
#endif
}

template<>
inline uint32_t
idx_lowest_bit_set<uint16_t>(const uint16_t x) {
#if __ICC
  return (_bit_scan_forward(x));
#elif __GNUG__
  return (__builtin_ctz(x));
#endif
}


template<>
inline uint32_t
idx_lowest_bit_set<uint32_t>(const uint32_t x) {
#if __ICC
  return (_bit_scan_forward(x));
#elif __GNUG__
  return (__builtin_ctz(x));
#endif
}


template<>
inline uint32_t
idx_lowest_bit_set<uint64_t>(const uint64_t x) {
#if __ICC
  return (__builtin_ctzll(x)); // gibt kein _bit_scan_forward64
#elif __GNUG__
  return (__builtin_ctzll(x));
#endif
}

/*
 *  return the lowest bit set (no index, the bit!)
 *  get least significan bit set to '1'
 * (x & (-x)) oder blsi
 * (x & (x ^ (x - 1))); // tuts auch, aber zu teuer
 */

template<typename Tuint>
inline Tuint get_lsb_set(const Tuint x);

template<>
inline uint32_t
get_lsb_set(const uint32_t x) {
  #ifdef __BMI__
    return _blsi_u32(x);
  #else
    return (x & (-x));
  #endif
}

template<> 
inline uint64_t
get_lsb_set(const uint64_t x) {
  #ifdef __BMI__
    return _blsi_u64(x);
  #else
    return (x & (-x));
  #endif
}


/*
 *  reset the least sigificant bit which is '1' to '0'
 */

template<typename Tuint>
inline Tuint reset_lsb_set(const Tuint x);

template<>
inline uint32_t
reset_lsb_set(const uint32_t x) {
  #ifdef __BMI__
    return _blsr_u32(x);
  #else
    return (x & (x - 1));
  #endif
}

template<>
inline uint64_t
reset_lsb_set(const uint64_t x) {
  #ifdef __BMI__
    return _blsr_u64(x);
  #else
    return (x & (x - 1));
  #endif
}

/*
 * set  '0' bits to '1' from lsb to first bit being '1'
 * (x ^ (x - 1))
 * lsb..msb
 * 00010001 becomes
 * 11110001 
 * if the first bit is set, this returns 0x1:
 * 101100 becomes
 * 100000
 */

template<typename Tuint>
inline Tuint set_trailing_zeros(const Tuint x);

template<>
inline uint32_t
set_trailing_zeros(const uint32_t x) {
  #ifdef __BMI__
    return _blsmsk_u32(x);
  #else
    return (x ^ (x - 1));
  #endif
}

template<>
inline uint64_t
set_trailing_zeros(const uint64_t x) {
  #ifdef __BMI__
    return _blsmsk_u64(x);
  #else
    return (x ^ (x - 1));
  #endif
}


/*
 *  test in x whether bit number i is set
 *  _bittest gibt es nicht in gcc
 */

template<typename Tuint>
inline bool test_bit(const Tuint x, const Tuint i);

template<>
inline bool
test_bit(const uint32_t x, const uint32_t i) {
   return ((x >> i) & 0x1);
}

template<>
inline bool
test_bit(const uint64_t x, const uint64_t i) {
  return ((x >> i) & 0x1);
}

/*
 *  get_bit at index i -> 0,1
 */

template<typename Tuint>
inline Tuint
get_bit(const Tuint x, const uint32_t i) {
  return ((x >> i) & 0x1);
}


/*
 * return the number of bits set
 */

template<class Tuint>
inline uint32_t 
number_of_bits_set(const Tuint);


template<>
inline uint32_t
number_of_bits_set<uint32_t>(const uint32_t x) {
#if __ICC
  return (_popcnt32(x));
#elif __GNUG__
  return (__builtin_popcount(x));
#endif
}


template<>
inline uint32_t
number_of_bits_set<uint64_t>(const uint64_t x) {
#if __ICC
  return (_popcnt64(x));
#elif __GNUG__
  return (__builtin_popcountll(x));
#endif
}


/*
 * return true if X \subseteq Y
 */

template<class Tuint>
bool
isSubsetOf(const Tuint X, const Tuint Y) {
  return (Y == (X | Y));
}

/*
 * returns true if X \supseteq Y
 */

template<class Tuint>
bool
isSupersetOf(const Tuint X, const Tuint Y) {
  return (X == (X | Y));
}

/*
 * returns true if x in Y, X as idx, Y as bv
 */

template<class Tuint>
bool
isContainedIn(const uint32_t x, const Tuint Y) {
  return (0 != ((((Tuint) 1) << x) & Y));
}

/*
 * returns true if X \cap Y = \emptyset
 */

template<class Tuint>
bool
hasEmptyIntersection(const Tuint X, const Tuint Y) {
  return (0 == (X & Y));
}

/*
 * returns true if X \cap Y \neq \emptyset
 */

template<class Tuint>
bool
hasNonEmptyIntersection(const Tuint X, const Tuint Y) {
  return (0 != (X & Y));
}

/*
 *  calculates the set differentce X \setminus Y
 */

template<class Tuint>
Tuint
setDifference(const Tuint X, const Tuint Y) {
  return (X & (~Y));
}

/*
 *  shifting bits to the left/right according to some given mask
 */


#ifdef __BMI2__
template<class Tuint>
inline Tuint
bit_distribute(const Tuint x, const Tuint aMask);

template<>
inline uint32_t
bit_distribute(const uint32_t x, const uint32_t aMask) {
  return _pdep_u32(x, aMask);
}

template<>
inline uint64_t
bit_distribute(const uint64_t x, const uint64_t aMask) {
  return _pdep_u64(x, aMask);
}

#else
template<typename Tuint>
inline Tuint
bit_distribute(const Tuint x, const Tuint aMask) {
  Tuint m = aMask;
  Tuint r = 0;    // result
  Tuint i = 0;    // index
  uint32_t c = 0; // count
  while(0 != m) {
    i = idx_lowest_bit_set(m);
    m ^= (1 << i);
    r |= (((x >> c) & 0x1) << i);
    ++c;
  }
  return r;
}
#endif


#ifdef __BMI2__
template<class Tuint>
inline Tuint
bit_gather(const Tuint x, const Tuint aMask);

template<class Tuint>
inline uint32_t
bit_gather(const uint32_t x, const uint32_t aMask) {
  return _pext_u32(x, aMask);
}

template<class Tuint>
inline uint64_t
bit_gather(const uint64_t x, const uint64_t aMask) {
  return _pext_u64(x, aMask);
}
#else
template<typename Tuint>
inline Tuint
bit_gather(const Tuint x, const Tuint aMask) {
  Tuint m = aMask;
  Tuint r = 0;    // result
  Tuint i = 0;    // index
  uint32_t c = 0; // count
  while(0 != m) {
    i = idx_lowest_bit_set(m);
    m ^= (1 << i);
    r |= (((x >> i) & 0x1) << c);
    ++c;
  }
  return r;
}
#endif

/*
 *  cyclic shift left
 *  x: element to shift 
 *  s: shift
 *  c: capacity, i.e., number of bits to consider
 *  constraints:
 *  a) 0 < s < c < #bits(Tuint)
 *  b) x must contain zero outside b_0,...b_{c-1}
 */

template<typename Tuint>
inline Tuint
cyclic_shift_left(const Tuint x, const uint32_t s, const uint32_t c) {
  const Tuint m_c = (((Tuint) 1 << c) - 1); // mask for capacity
  return (((x >> (c - s)) | (x << s)) & m_c);
}

template<typename Tuint>
inline Tuint
cyclic_shift_right(const Tuint x, const uint32_t s, const uint32_t c) {
  const Tuint m_c = (((Tuint) 1 << c) - 1); // mask for capacity
  return (((x << (c - s)) | (x >> s))  & m_c);
}


/*
 * more cyclic shifts without capacity
 * in C++ 20: std::rotl and std::rotr
 */


/*
 * check whether a given unsigned integer is a power of 2
*/ 

template<typename Tuint>
inline bool
isPow2(const Tuint x) {
  return (0 == (x & (x - 1)));
}

/*
 * round to the nearest power of 2, nearest defined by q
 * works only for unsigned int
 */

template<typename Tuint>
inline Tuint
roundQPow2(const Tuint x) {
  int lHiIdx = idx_highest_bit_set<Tuint>(x);
  Tuint lRes = (((Tuint) 1) << (lHiIdx + (0x3 == (x >> (lHiIdx - 1)))));
  return lRes;
}

/*
 * round to nearest power of 2, nearest defined by l_1
 * works only for unsigned int
 * on equal terms, it prefers the smaller power of 2
 */

template<typename Tuint>
inline Tuint
roundAbsPow2(const Tuint x) {
  const Tuint z1 = roundQPow2(x);
  Tuint lRes = z1;
  if(z1 > x) {
    const Tuint z2 = (z1 >> 1);
    const Tuint d1 = (z1 - x);
    const Tuint d2 = (x - z2);
    lRes = ((d1 < d2) ? z1 : z2);
  }
  return lRes;
}

/*
 *  is_subset(i1, i2)
 *  returns i1 \subseteq i2
 */

template<typename Tuint>
bool
is_subset(const Tuint i1, const Tuint i2) {
  return (i2 == (i1 | i2));
}

// and the opposite
template<typename Tuint>
bool
not_is_subset(const Tuint i1, const Tuint i2) {
  return (i2 != (i1 | i2));
}

/*
 *  all binary boolean functions, not constant, depending on both parameters
 *  some need mask, others not (if only part of the word in used as a bitvector)
 *  to have a unified interface all ops carry a mask argument
 *  x,y: arguments, m: mask
 */


template<typename Tuint>
inline Tuint
t_common_0(const Tuint x, const Tuint y, const Tuint m) {
  return (((~x) & (~y)) & m);
}

template<typename Tuint>
inline Tuint
t_common_1(const Tuint x, const Tuint y, const Tuint m) {
  return ((x & y) & m);
}

template<typename Tuint>
inline Tuint
t_x_setminus_y(const Tuint x, const Tuint y, const Tuint m) {
  return ((x & (~y)) & m);
}

template<typename Tuint>
inline Tuint
t_y_setminus_x(const Tuint x, const Tuint y, const Tuint m) {
  return (((~x) & y) & m);
}

template<typename Tuint>
inline Tuint
t_union(const Tuint x, const Tuint y, const Tuint m) {
  return ((x | y) & m);
}

// complement of intersection
template<typename Tuint>
inline Tuint
t_cmpl_common_1(const Tuint x, const Tuint y, const Tuint m) {
  return (((~x) | (~y)) & m);
}

template<typename Tuint>
inline Tuint
t_y_union_cmpl_x(const Tuint x, const Tuint y, const Tuint m) {
  return ((y | (~x)) & m);
}

template<typename Tuint>
inline Tuint
t_x_union_cmpl_y(const Tuint x, const Tuint y, const Tuint m) {
  return ((x | (~y)) & m);
}

template<typename Tuint>
inline Tuint
t_unequal_bits(const Tuint x, const Tuint y, const Tuint m) {
  return ((x ^ y) & m);
}

template<typename Tuint>
inline Tuint
t_equal_bits(const Tuint x, const Tuint y, const Tuint m) {
  return ((~(x ^ y)) & m);
}

template<typename Tuint>
inline Tuint
t_set_lowest_n_bits(const Tuint n) {
  if(sizeof(Tuint) * 8 <= n) {
    return ~((Tuint) 0);
  }
  return ((((Tuint) 1) << n) - 1);
}

// for a given bitvector i of length z
// count the number of trailing (least significant) bits which are set to one
template<typename Tuint>
inline uint32_t
no_trailing_one(const Tuint i, const uint32_t z) {
  return idx_lowest_bit_set<Tuint>(~i);
}

// for a given bitvector i of length z
// count the number of leading (most significant) bits which are set to one
template<typename Tuint>
inline uint32_t
no_leading_one(const Tuint i, const uint32_t z) {
  return z - idx_highest_bit_set<Tuint>(~i & ((1 << z) - 1)) - 1;
}

// check whether x is of the form x = 0^a1^b0^c
template<typename Tuint>
inline bool
is_block_010(const Tuint x) {
  if(0 == x) {
    return false;
  }
  return (idx_highest_bit_set<Tuint>(x) - idx_lowest_bit_set<Tuint>(x) + 1 == number_of_bits_set<Tuint>(x));
}



#endif
