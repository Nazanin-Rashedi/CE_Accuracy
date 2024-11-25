#include "gmstrstr16.hh"

#include <iostream>

// #include <ia32intrin.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <nmmintrin.h>

// SSE 4.2
// #define MODE_FIRST_OCC (_SIDD_CMP_EQUAL_ANY | _SIDD_SBYTE_OPS | _SIDD_LEAST_SIGNIFICANT)
// like '%s%'
// wie strstr

const char*  
gmstrstr16(const char* aString, const char* aPattern, const int aPatternLength) {
  const char* x = aString;
  const char* p = aPattern;

  const int lRelevant = (1 << aPatternLength) - 1;
  // __m128i lString; 
  const __m128i lPattern = _mm_loadu_si128((__m128i*) p);
  const __m128i lFirst   = _mm_set1_epi8(p[0]);
  const __m128i lNull    = _mm_set1_epi8('\0');

  while(true) {
    const __m128i lString = _mm_loadu_si128((__m128i*) x);

    // std::cout << "compare." << std::endl;
    // idx = _mm_cmpistri(_mm_loadu_si128((__m128i*) x), lFirst, MODE_FIRST_OCC);  // SSE 4.2

    int msk  = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lPattern));
    if(lRelevant == (msk & lRelevant)) {
      // std::cout << "match found." << std::endl;
      return x;
    }
    // if(*x == *p) { ++x; } // ALT0

    const int mskF = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lFirst)) & 0xFFFE;  // &xxx ALT1
    const int mskN = _mm_movemask_epi8(_mm_cmpeq_epi8(lString, lNull));
    const int idxF = 0; // XXX _bit_scan_forward(mskF);
    const int idxN = 0; // XXX _bit_scan_forward(mskN);

    // std::cout << " mask 1: 0x" << std::hex << mskF << std::dec << std::endl;
    // std::cout << " mask 2: 0x" << std::hex << mskN << std::dec << std::endl;
    // std::cout << "index 1: " << idxF << std::endl;
    // std::cout << "index 2: " << idxN << std::endl;

    // if((0 != mskN) & (0 == mskF)) { return 0; } // dies neu. schlechter als vorheriges 0.172

    if(0 == mskN) {
      if(0 == mskF) {
        x += 16;
      } else {
       // const  unsigned int idxF = _bit_scan_forward(mskF);
        x += idxF;
      }
    } else {
      if(0 == mskF) {
        return 0;
      } else {
        // const unsigned int idxN = _bit_scan_forward(mskN);
        // const unsigned int idxF = _bit_scan_forward(mskF);
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

