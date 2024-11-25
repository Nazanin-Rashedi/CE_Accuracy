#ifndef INFRA_GM_STRSTR_16_HH
#define INFRA_GM_STRSTR_16_HH

#include <string.h>
#include <emmintrin.h>
#include <xmmintrin.h>
// #include <nmmintrin.h>


// finds a pattern p in a string x
// returns pointer to first occurrence of the first 16 bytes of p occurring in x
// returns 0 otherwise
// the pattern must be at most 16 bytes long
// the pattern p must provide at least 16 readable bytes but may contain '\0' before byte 15
// the string x must provide at least 16 readable bytes after its end
//
// compile with
/*
icpc -c -O3 gmstrstr16.cc
*/

extern "C" {
  const char*  gmstrstr16(const char* x, const char* p, const int aPatternLength);
}

/*
extern "C" {
  inline const char* 
  gmstrstr16inl(const char* aString, const char* aPattern, const int aPatternLength) {

    const char* x = aString;
    const char* p = aPattern;

    const int lRelevant = (1 << aPatternLength) - 1;
    const __m128i lPattern = _mm_loadu_si128((__m128i*) p);
    const __m128i lFirst   = _mm_set1_epi8(p[0]);
    const __m128i lNull    = _mm_set1_epi8('\0');

    while(true) {
      const __m128i lString = _mm_loadu_si128((__m128i*) x);
      int msk  = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lPattern));
      if(lRelevant == (msk & lRelevant)) {
        return x;
      }

      const int mskF = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lFirst)) & 0xFFFE;  // &xxx ALT1
      const int mskN = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lNull));
      const int idxF = 0; // _bit_scan_forward(mskF);
      const int idxN = 0; // _bit_scan_forward(mskN);

      if(0 == mskN) {
        if(0 == mskF) {
          x += 16;
        } else {
          x += idxF;
        }
      } else {
        if(0 == mskF) {
          return 0;
        } else {
          if((idxF + aPatternLength) > idxN) {
            return 0;
          } else {
            x += idxF;
          }
        }
      }
    }
    return x;
  }

}
*/

#endif
