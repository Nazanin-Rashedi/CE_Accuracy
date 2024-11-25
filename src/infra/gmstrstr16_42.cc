#include "gmstrstr16_42.hh"

#include <iostream>

// #include <ia32intrin.h>
// #include <emmintrin.h>
// #include <xmmintrin.h>
// #include <nmmintrin.h>

// SSE 4.2
// #define MODE_FIRST_OCC (_SIDD_CMP_EQUAL_ANY | _SIDD_LEAST_SIGNIFICANT | _SIDD_SBYTE_OPS )
// #define MODE_ALL_EQUAL (_SIDD_CMP_EQUAL_EACH | _SIDD_SBYTE_OPS)
// #define MODE_SUBSTR    (_SIDD_CMP_EQUAL_ORDERED | _SIDD_SBYTE_OPS)

const char*  
gmstrstr16_42(const char* aString, const char* aPattern, const int aPatternLength) {
  const char* x = aString;
  const char* p = aPattern;

  const __m128i lPattern = _mm_loadu_si128((__m128i*) p);
  // const __m128i lFirst   = _mm_set1_epi8(p[0]);
  // const __m128i lNull    = _mm_set1_epi8('\0');

  const int lMinShift = (16 - aPatternLength);

  while(true) {
    __m128i lString = _mm_loadu_si128((__m128i*) x);

    // does lString contain any '\0'? 
    // calculate index of first '\0', 16 if no null contained.
    // int idxN = _mm_cmpistri(lNull, lString, MODE_FIRST_OCC); // does not work anyway


    // calculate the index where the first char of the pattern occurs
    // it is 16 if the first char of the pattern does not occur in x[0-15].
    // int idxF = _mm_cmpistri(lFirst, lString, MODE_FIRST_OCC);
    // ZFlag is set if any byte is '\0', reset otherwise
    // int lBreak = _mm_cmpistrz(lFirst, lString, MODE_FIRST_OCC);

    // substring check, is lPattern a substring of lString?
    int idxS   = _mm_cmpistri(lPattern, lString, MODE_SUBSTR);
    int lBreak = _mm_cmpistrz(lPattern, lString, MODE_SUBSTR);

    // note the case that idxF = 1, i.e., x[0] == p[0] but no match
    // const int lAddOneNeeded = (idxF == 0);
    // if(0 == idxF) { idxF = lMinShift; }

    // std::cout << "x: " << x << std::endl;
    // std::cout << "p: " << p << std::endl;
    // std::cout << " break: " << lBreak // idxN
    //           << ", idxS: " << idxS << std::endl;

    if(0 == idxS) {
       // std::cout << " ==> true" << std::endl;
       return x;
    }
    if(lBreak) {
      if(lMinShift < idxS) {
        // std::cout << " ==> false" << std::endl;
        return 0;
      }
    }
    x += idxS; 
  }
}

// q13(s):
// gmstrstr16_42: 0.158876
// gmstrstr16_42 Alt0: 0.329951
// gmstrstr16_42 Alt1: 0.347947

