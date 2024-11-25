#ifndef INFRA_LARSON_HASH_HH
#define INFRA_LARSON_HASH_HH

#include "glob_infra_standard_includes.hh"
#include <string.h>
#include <set>

/*
 * Mann braucht den SALT nur wegen hash tabelle attacks, 
 * sonst kann 0 sein.
 */

template<typename Tuint>
struct eqstr {
   inline Tuint operator() (const char* s) const {
       Tuint h = 0x381302; // SALT
       while (*s)
           h = h * 101 + (unsigned char) *s++;
       return h;
   }

   inline bool operator()(const char *s1, const char *s2) const {
       return strcmp(s1, s2) == 0;
   }
};

#endif
