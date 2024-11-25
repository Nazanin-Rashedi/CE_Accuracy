#include <iostream>
#include "glob_infra_standard_includes.hh"
#include "immintrin.h"
#include "bitvectorsmall.hh"
#include "bit_intrinsics.hh"

/* 
 try compiling with

g++ -mbmi -mlzcnt main_blsi.cc 

*/

#ifdef __GNUC__
#define clz(x) __builtin_clz(x)
#define ctz(x) __builtin_ctz(x)
#else
static uint32_t ALWAYS_INLINE popcnt( uint32_t x )
{
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return x & 0x0000003f;
}
static uint32_t ALWAYS_INLINE clz( uint32_t x )
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return 32 - popcnt(x);
}
static uint32_t ALWAYS_INLINE ctz( uint32_t x )
{
    return popcnt((x & -x) - 1);
}
#endif

/*
unsigned int _blsi_u32 (unsigned int a)
#include "immintrin.h"
Instruction: blsi r32, r32
CPUID Flags: BMI1
Description
Extract the lowest set bit from unsigned 32-bit integer a and set the corresponding bit in dst. All other bits in dst are zeroed, and all bits are zeroed if no bits are set in a.
Operation
dst := (-a) BITWISE AND a
*/

void
test0() {
  std::cout << "blsi" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    const uint x = (-i) & i;
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    std::cout << std::setw(2) << i << ' '
              << lBv_i << ' ' << lBv_x
              << std::endl;
  }
}

/*
unsigned int _blsmsk_u32 (unsigned int a)
#include "immintrin.h"
Instruction: blsmsk r32, r32
CPUID Flags: BMI1
Description
Set all the lower bits of dst up to and including the lowest set bit in unsigned 32-bit integer a.
Operation
dst := (a - 1) XOR a
*/

void
test1() {
  std::cout << "blsmsk" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    const uint x = (i - 1) ^ i;
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    std::cout << std::setw(2) << i << ' '
              << lBv_i << ' ' << lBv_x
              << std::endl;
  }
}

/*
unsigned int _blsr_u32 (unsigned int a)
#include "immintrin.h"
Instruction: blsr r32, r32
CPUID Flags: BMI1
Description
Copy all bits from unsigned 32-bit integer a to dst, and reset (set to 0) the bit in dst that corresponds to the lowest set bit in a.
Operation
dst := (a - 1) BITWISE AND a
*/

void
test2() {
  std::cout << "blsr" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    const uint x = (i - 1) & i;
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    std::cout << std::setw(2) << i << ' '
              << lBv_i << ' ' << lBv_x
              << std::endl;
  }
}

/*
unsigned int _bzhi_u32 (unsigned int a, unsigned int index)
#include "immintrin.h"
Instruction: bzhi r32, r32, r32
CPUID Flags: BMI2
Description
Copy all bits from unsigned 32-bit integer a to dst, and reset (set to 0) the high bits in dst starting at index.
Operation
n := index[7:0]
dst := a
IF (n < 32)
	dst[31:n] := 0
FI
*/

void
test3() {
  uint z = 7;
  std::cout << "bzhi" << std::endl;
  uint y = 0;
  for(uint i = 128; i < 155; ++i) {
    const uint x = (i & ((1 << z) - 1));
    // asm volatile("bzhi  %0, %1, %2" : "=r" (y) : "r" (i), "r" (z)); // illegal instruction
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' ' << lBv_x << ' '
              << Bitvector32(y) << ' ' 
              << std::endl;
  }
}

/*
unsigned int _andn_u32 (unsigned int a, unsigned int b)
#include "immintrin.h"
Instruction: andn r32, r32
CPUID Flags: BMI1
Description
Compute the bitwise NOT of 32-bit integer a and then AND with b, and store the results in dst.
Operation
dst[31:0] := ((NOT a[31:0]) AND b[31:0])
*/

void
test4() {
  const uint b = 13;
  std::cout << "andn" << std::endl;
  for(uint i = 128; i < 155; ++i) {
    const uint x = (~i) & b;
    // const uint y = _andn_u32(i, b); //  error: ‘_andn_u32’ was not declared in this scope
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    // Bitvector32 lBv_y(y);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' ' 
              << lBv_x << ' '
              // << lBv_y << ' '
              << std::endl;
  }
}


/*
unsigned int _bextr_u32 (unsigned int a, unsigned int start, unsigned int len)
#include "immintrin.h"
Instruction: bextr r32, r32, r32
CPUID Flags: BMI1
Description
Extract contiguous bits from unsigned 32-bit integer a, and store the result in dst. Extract the number of bits specified by len, starting at the bit specified by start.
Operation
tmp := ZERO_EXTEND_TO_512(a)
dst := ZERO_EXTEND(tmp[start+len-1:start])
*/

/*
void
test5() {
  const uint z = 4;
  const uint l = 3;
  std::cout << "bextr" << std::endl;
  for(uint i = 128; i < 155; ++i) {
    const uint x = (i >> z) & ((1 << l) - 1);
    // const uint y = _bextr_u32(i, z, l); // illegal instruction
    const uint y = 0;
    Bitvector32 lBv_i(i);
    Bitvector32 lBv_x(x);
    Bitvector32 lBv_y(y);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' ' 
              << lBv_x << ' '
              << lBv_y << ' '
              << std::endl;
    assert(x == y);
  }
}
*/


/*
unsigned char _bittest (__int32* a, __int32 b)
#include "immintrin.h"
Instruction: bt r32, r32
Description
Return the bit at index b of 32-bit integer a.
Operation
dst := a[b]
*/

void
test6() {
  const uint i = 3;
  std::cout << "_bittest" << std::endl;
  for(uint x = 0; x < 32; ++x) {
    const uint r1 = (x >> i) & 0x1;
    const uint r2 = test_bit(x, i); //  error: ‘_bittest’ was not declared in this scope
    // asm("movl %1, %%eax \n bts %1, %%eax \n movl %%eax, %0" : "=r" (y) : "r" (i), "r" (z));
    Bitvector32 lBv_x(x);
    std::cout << std::setw(3) << i << ' '
              << lBv_x << ' '
              << r1 << ' '
              << r2 << ' '
              << std::endl;
    // assert(x == y);
  }
}

/*
int _mm_tzcnt_32 (unsigned int a)
#include "immintrin.h"
Instruction: tzcnt r32, r32
CPUID Flags: BMI1
Description
Count the number of trailing zero bits in unsigned 32-bit integer a, and return that count in dst.
Operation
tmp := 0
dst := 0
DO WHILE ((tmp < 32) AND a[tmp] = 0)
	tmp := tmp + 1
	dst := dst + 1
OD
*/

/*
void
testA() {
  std::cout << "tzcnt" << std::endl;
  for(uint i = 128; i < 155; ++i) {
    const uint x = _tzcnt_u32(i);
    Bitvector32 lBv_i(i);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' ' 
              << std::setw(3) << x
              << std::endl;
  }
}
*/


/*
unsigned int _lzcnt_u32 (unsigned int a)
#include "immintrin.h"
Instruction: lzcnt r32, r32
CPUID Flags: LZCNT
Description
Count the number of leading zero bits in unsigned 32-bit integer a, and return that count in dst.
Operation
tmp := 31
dst := 0
DO WHILE (tmp >= 0 AND a[tmp] = 0)
	tmp := tmp - 1
	dst := dst + 1
OD
*/

/*
void
testB() {
  std::cout << "lzcnt" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    const uint x = _lzcnt_u32(i); // error: ‘_lzcnt_u32’ was not declared in this scope
    Bitvector32 lBv_i(i);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' '
              << std::setw(3) << x
              << std::endl;
  }
}
*/



/*
int _bit_scan_forward (int a)
#include "immintrin.h"
Instruction: bsf r32, r32
Description
Set dst to the index of the lowest set bit in 32-bit integer a. If no bits are set in a then dst is undefined.
Operation
tmp := 0
IF a = 0
	dst := undefined
ELSE
	DO WHILE ((tmp < 32) AND a[tmp] = 0)
		tmp := tmp + 1
		dst := tmp
	OD
FI
*/

void
testC() {
  std::cout << "_bit_scan_forward" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    // const uint  x = _bit_scan_forward(i);
    const uint  x = ctz(i);
    Bitvector32 lBv_i(i);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' '
              << std::setw(3) << x
              << std::endl;
  }
}

/*
int _bit_scan_reverse (int a)
#include "immintrin.h"
Instruction: bsr r32, r32
Description
Set dst to the index of the highest set bit in 32-bit integer a. If no bits are set in a then dst is undefined.
Operation
tmp := 31
IF a = 0
	dst := undefined
ELSE
	DO WHILE ((tmp > 0) AND a[tmp] = 0)
		tmp := tmp - 1
		dst := tmp
	OD
FI
*/

void
testD() {
  std::cout << "_bit_scan_reverse" << std::endl;
  for(uint i = 0; i < 32; ++i) {
    // const uint  x = _bit_scan_reverse(i);
    const uint  x = clz(i);
    Bitvector32 lBv_i(i);
    std::cout << std::setw(3) << i << ' '
              << lBv_i << ' '
              << std::setw(3) << x
              << std::endl;
  }
}


// BLSR — Reset Lowest Set Bit
// BZHI — Zero High Bits Starting with Specified Bit Position

/*
void
fCounts() {
  std::cout << std::setw(3)  << "i" << ' '
            << std::setw(32) << "bv(i)" << ' '
            << std::setw(6) << "bsfwd" << ' '
            << std::setw(6) << "tzcnt" << ' '
            << std::setw(6) << "bsbwd" << ' '
            << std::setw(6) << "lzcnt" << ' '
            << std::setw(6) << "popcnt" << ' '
            << std::setw(6) << "idxlo" << ' '
            << std::setw(6) << "idxhi" << ' '
            << std::endl;

  uint lStart = 2;
  uint lLen   = 2;
  uint lCode  = (lLen << 8) | lStart;
  uint lLzCnt = 0;
  uint lTzCnt = 0;
  uint lBitScanFwd = 0;
  uint lBitScanBwd = 0;
  uint lBExtr = 0;
  uint lBlsr  = 0;
  uint lInc   = 0;
  uint lDec   = 0;
  for(uint i = 0; i < 32; ++i) {
  asm("lzcnt %1, %0" : "=r" (lLzCnt)      : "r" (i));
  asm("tzcnt %1, %0" : "=r" (lTzCnt)      : "r" (i));
  asm("bsf   %1, %0" : "=r" (lBitScanFwd) : "r" (i));
  asm("bsr   %1, %0" : "=r" (lBitScanBwd) : "r" (i));

  // asm("blsr  %1, %0" : "=r" (lBlsr)       : "r" (i)); // illegal instruction
 
  // lInc = i;
  // lDec = i; 
  // asm("dec %0" : "=r" (lDec) : ); // illegal instruction
  // asm("inc %0" : "=r" (lInc) : ); // illegal instruction


  // asm("bextr %1, %2, %0" : "=r" (lBExtr) : "r" (i), "r" (lCode)); // illegal instruction

  std::cout << std::setw(3) << i << ' '
                            << Bitvector32(i) << ' '
            << std::setw(6) << __builtin_ctz(i) << ' '
            << std::setw(6) << _tzcnt_u32(i) << ' '
            << std::setw(6) << __builtin_clz(i) << ' '
            << std::setw(6) << _lzcnt_u32(i) << ' '
            << std::setw(6) << number_of_bits_set<uint>(i) << ' '
            << std::setw(6) << idx_lowest_bit_set<uint>(i) << ' '
            << std::setw(6) << idx_highest_bit_set<uint>(i) << "    "
                            << lLzCnt << ' '
                            << lTzCnt << ' '
                            << lBitScanFwd << ' '
                            << lBitScanBwd << ' '
            << std::endl;
  }
}
*/

int
main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  // test5();
  test6();

  // testA();
  // testB();
  // testC();
  // testD();

  // fCounts();
  return 0;
}


