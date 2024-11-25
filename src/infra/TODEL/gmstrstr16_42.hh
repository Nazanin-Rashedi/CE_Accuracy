#ifndef INFRA_GM_STRSTR_16_42_HH
#define INFRA_GM_STRSTR_16_42_HH

#include <string.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <nmmintrin.h>


#define MODE_FIRST_OCC (_SIDD_CMP_EQUAL_ANY | _SIDD_LEAST_SIGNIFICANT | _SIDD_SBYTE_OPS )
#define MODE_ALL_EQUAL (_SIDD_CMP_EQUAL_EACH | _SIDD_SBYTE_OPS)
#define MODE_SUBSTR    (_SIDD_CMP_EQUAL_ORDERED | _SIDD_SBYTE_OPS)


// finds a pattern p in a string x
// returns pointer to first occurrence of the first 16 bytes of p occurring in x
// returns 0 otherwise
// the pattern must be at most 16 bytes long
// the pattern p must provide at least 16 readable bytes but may contain '\0' before byte 15
// the string x must provide at least 16 readable bytes after its end
// it requires SSE 4.2
// compile with
/*
icpc -c -O3 gmstrstr16_42.cc
*/

extern "C" {
  const char*  gmstrstr16_42(const char* x, const char* p, const int aPatternLength);
}


inline
const char* gmstrstr16_42_inl(const char* aString, const char* aPattern, const int aPatternLength) {
  const char* x = aString;
  const char* p = aPattern;
  const __m128i lPattern = _mm_loadu_si128((__m128i*) p);
  const int lMinShift = (16 - aPatternLength);

  while(true) {
    __m128i lString = _mm_loadu_si128((__m128i*) x);
    int idxS   = _mm_cmpistri(lPattern, lString, MODE_SUBSTR);
    int lBreak = _mm_cmpistrz(lPattern, lString, MODE_SUBSTR);
    if(0 == idxS) {
       return x;
    }
    if(lBreak) {
      if(lMinShift < idxS) {
        return 0;
      }
    }
    x += idxS;
  }
}

// is pattern p prefix of string x?
inline
bool
gmprefix16_42_inl(const char* x, const char* p, const int aPatternLength) {
  const __m128i lPattern = _mm_loadu_si128((__m128i*) p);
  const __m128i lString = _mm_loadu_si128((__m128i*) x);
  int lIdx = _mm_cmpestri(lPattern, aPatternLength, lString, aPatternLength, MODE_SUBSTR);
  return (0 == lIdx);
}


#endif
