#include "cstring_hash.h"


/*
 * larson
 */

#define SALT32 784728774
#define SALT64 78478302118428774LL

uint32_t
cstring_hash_larson_32(const char* s) {
  uint32_t h = SALT32;
  while(*s)
    h = h * 101 + (unsigned char) *s++;
  return h;
}

uint64_t
cstring_hash_larson_64(const char* s) {
  uint64_t h = SALT64;
  while(*s)
    h = h * 101 + (unsigned char) *s++;
  return h;
}

/*
 *  Ramakrishna, Zobel
 */

uint32_t
cstring_hash_razo_32(const char* s) {
  const uint32_t L = 5;
  const uint32_t R = 2;
  uint32_t h = SALT32;
  while(*s) {
    h ^= ((h << L) + (h >> R) + *s++);
  }
  return h;
}


uint64_t
cstring_hash_razo_64(const char* s) {
  const uint32_t L = 5;
  const uint32_t R = 2;
  uint64_t h = SALT64;
  while(*s) {
    h ^= ((h << L) + (h >> R) + *s++);
  }
  return h;
}


#undef SALT32
#undef SALT64


/*
 *  hashpjw
 */

#define FF32 0xf0000000L
#define FF64 0xf00000000000000L

uint32_t 
cstring_hash_pjw_32(const char* s) {
  int32_t h = 0;
  int32_t g;
  for(const char* p = s; *p != '\0'; p++) {
    h = (h << 4) + (*p); 
    if((g = h & FF32)) {
      h ^= (g >> 24);
      h ^= g;
    }
  }
 return h;
}


uint64_t 
cstring_hash_pjw_64(const char* s) {
  int64_t h = 0;
  int64_t g;
  for(const char* p = s; *p != '\0'; p++) {
    h = (h << 4) + (*p);
    if((g = h & FF64)) {
      h ^= (g >> 56);
      h ^= g;
    }
  }
 return h;
}

/*
 * from Binstock (Dr. Dobb's Journal April 1997. 24-33)
 */

#define BITS_IN_INT32        ( sizeof(uint32_t) * CHAR_BIT )
#define THREE_QUARTERS_INT32 ((int) ((BITS_IN_INT32 * 3) / 4))
#define ONE_EIGHTH_INT32     ((int)  (BITS_IN_INT32 / 8))
#define HIGH_BITS_INT32      (~((uint32_t)(~((uint32_t) (0)) >> ONE_EIGHTH_INT32)))

uint32_t 
cstring_hash_binstock_pjw_32(const char* s) {
  uint32_t h = 0, i = 0;
  for(h = 0; *s; ++s) {
    h = (h << ONE_EIGHTH_INT32) + *s;
    if(0 != (i = (h & HIGH_BITS_INT32))) {
      h = (h ^ (i >> THREE_QUARTERS_INT32)) & ~HIGH_BITS_INT32;
    }
  }
  return h; 
}

#undef BITS_IN_INT32
#undef THREE_QUARTERS_INT32
#undef ONE_EIGHTH_INT32
#undef HIGH_BITS_INT32

#define BITS_IN_INT64        ( sizeof(uint64_t) * CHAR_BIT )
#define THREE_QUARTERS_INT64 ((int) ((BITS_IN_INT64 * 3) / 4))
#define ONE_EIGHTH_INT64     ((int)  (BITS_IN_INT64 / 8))
#define HIGH_BITS_INT64      (~((uint64_t)(~((uint64_t) (0)) >> ONE_EIGHTH_INT64)))

uint64_t 
cstring_hash_binstock_pjw_64(const char* s) {
  uint64_t h = 0, i = 0;
  for(h = 0; *s; ++s) {
    h = (h << ONE_EIGHTH_INT64) + *s;
    if(0 != (i = (h & HIGH_BITS_INT64))) {
      h = (h ^ (i >> THREE_QUARTERS_INT64)) & ~HIGH_BITS_INT64;
    }
  }
  return h;
}

#undef BITS_IN_INT64
#undef THREE_QUARTERS_INT64
#undef ONE_EIGHTH_INT64
#undef HIGH_BITS_INT64

#define MASK_32 (((uint32_t) 0xF) << 28)
uint32_t 
cstring_hash_binstock_elf_32(const char* s) {
  uint32_t h = 0, g = 0;
  while(*s) {
    h = (h << 4) + *s++;
    if((g = (h & MASK_32))) {
      h ^= g >> 24;
    }
    h &= ~g;
  }
  return h;
}

#define MASK_64 (((uint64_t) 0xF) << 60)
uint64_t 
cstring_hash_binstock_elf_64(const char* s) {
  uint64_t h = 0, g = 0;
  while(*s) {
    h = (h << 4) + *s++;
    if((g = (h & MASK_64))) {
      h ^= g >> 54;
    }
    h &= ~g;
  }
  return h;
}
#undef MASK_64

/*
 * Kernighan Ritchie
 */

uint32_t
cstring_hash_keri_32(const char* s) {
  uint32_t h = 0;
  const uint32_t M = 31;
  while(*s) {
    h = M * h + *s++;
  }
  return h;
}

/*
 * hash functions from http://www.cse.yorku.ca/~oz/hash.html
 */

uint32_t
cstring_hash_bern_32(const char* s) {
  uint32_t h = 5381;
  // const uint32_t M = 33; // x * 33 = x * (32 + 1)
  int c = 0;
  while((c = *s++)) {
    h = ((h << 5) + h) + c;
  }
  return h;
}

uint32_t
cstring_hash_sdbm_32(const char* s) {
  uint32_t h = 0;
  int c = 0;
  while((c = *s++)) {
    h = c + (h << 6) + (h << 16) - h;
  }
  return h;
}

uint32_t
cstring_hash_loser_32(const char* s) {
  uint32_t h = 0;
  int c = 0;
  while((c = *s++)) {
    h += c;
  }
  return h;
}
 



