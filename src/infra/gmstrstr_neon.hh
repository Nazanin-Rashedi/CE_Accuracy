#ifndef INFRA_GMSTRSTR_NEON_HH
#define INFRA_GMSTRSTR_NEON_HH

#include <assert.h>
#include <string.h>
#include <arm_neon.h>

// http://0x80.pl/articles/simd-strfind.html
// https://github.com/WojciechMula/sse4-strstr/blob/master/README.rst

inline const char*
strstr_mula(const char* s, size_t n, const char* needle, size_t k) {

    assert(k > 0);
    if(n <= 0) { return 0; }
    // assert(n > 0);

    // vdupXXX: set all lanes to the same value
    const uint8x16_t first = vdupq_n_u8(needle[0]);
    const uint8x16_t last  = vdupq_n_u8(needle[k - 1]);
    const uint8x8_t  half  = vdup_n_u8(0x0f);

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(s);

    union {
        uint8_t  tmp[8];
        uint32_t word[2];
    };

    for (size_t i = 0; i < n; i += 16) {

        const uint8x16_t block_first = vld1q_u8(ptr + i); // load first 16 chars into register
        const uint8x16_t block_last  = vld1q_u8(ptr + i + k - 1); // load into register

        const uint8x16_t eq_first = vceqq_u8(first, block_first); // bytwise comparision -> 0xFF or 0x00
        const uint8x16_t eq_last  = vceqq_u8(last, block_last);
        const uint8x16_t pred_16  = vandq_u8(eq_first, eq_last);
        const uint8x8_t pred_8    = vbsl_u8(half, vget_low_u8(pred_16), 
                                                  vget_high_u8(pred_16));
        // vget_los/high_u8: split a 128 bit vector into 2 component 64 bit vectors
        // VBSL (Bitwise Select) selects each bit for the destination 
        // from 
        // the first operand if the corresponding bit of the destination is 1,
        // or 
        // the second operand if the corresponding bit of the destination is 0.

        vst1_u8(tmp, pred_8);
        

        if ((word[0] | word[1]) == 0) {
            continue;
        }

        for (int j=0; j < 8; j++) {
            if (tmp[j] & 0x0f) {
                if (memcmp(s + i + j + 1, needle + 1, k - 2) == 0) {
                    // printf("exit A\n");
                    return s + i + j;
                }
            }
        }

        for (int j=0; j < 8; j++) {
            if (tmp[j] & 0xf0) {
                if (memcmp(s + i + j + 1 + 8, needle + 1, k - 2) == 0) {
                    const size_t x = i + j + 8;
                    if(x < n) {
                      // printf("exit B1\n");
                      return s + x;
                    } else {
                      // printf("exit B2\n");
                      return 0;
                    }
                }
            }
        }
    }
    // printf("exit C\n");
    return 0;
}

#endif
