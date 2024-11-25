#ifndef INFRA_UVAL_HH
#define INFRA_UVAL_HH

#include <inttypes.h>

typedef uint8_t byte_t;

union uval_t {
  char      _c[8];
  int8_t    _i8[8];
  int16_t   _i16[4];
  int32_t   _i32[2];
  int64_t   _i64;
  uint8_t   _u8[8];
  uint16_t  _u16[4];
  uint32_t  _u32[2];
  uint64_t  _u64;
  float     _f[2];
  double    _d;
  byte_t*   _byte_ptr;
  char*     _char_ptr;
  uint32_t* _i32_ptr;
  uval_t*   _next;
  uval_t*   _tuple;
}; 
  



#endif
