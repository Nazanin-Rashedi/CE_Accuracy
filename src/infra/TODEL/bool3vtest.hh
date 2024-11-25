#ifndef BOOL3_HH
#define BOOL3_HH

#include <iostream>

// three values logic

typedef int bool3v_t;

typedef bool3v_t (*bool3v_tunaop_t)(bool3v_t);
typedef bool3v_t (*bool3v_tbinop_t)(bool3v_t,bool3v_t);

inline char 
name3(bool3v_t x) {
  char lRes = 'x';
  switch(x) {
    case 0x0: lRes = 'f'; break;
    case 0x1: lRes = '-'; break;
    case 0x3: lRes = 't'; break;
    default: lRes = 'x'; break;
  }
  return lRes;
}


inline bool3v_t true3() { return 0x3; }
inline bool3v_t false3() { return 0x0; }
inline bool3v_t unknown3() { return 0x1; }

inline bool  is_true3(bool3v_t x) { return (0x3 == x); }
inline bool  is_false3(bool3v_t x) { return (0x0 == x); }
inline bool  is_unknown3(bool3v_t x) { return (0x1 == x); }

inline bool3v_t and3(bool3v_t x, bool3v_t y) { return (x & y); }
inline bool3v_t or3(bool3v_t x, bool3v_t y) { return (x | y); }
inline bool3v_t not3(bool3v_t x) { return (0x3 >> x); } // 1 careful: 0x2 --> 0x0

inline bool3v_t not3_a(bool3v_t x) { return ((((x ^ 0x3) << 2 | (x ^ 0x3)) >> 1) & 0x3) ; } // 6
inline bool3v_t not3_b(bool3v_t x) { return ((~((x >> 1) | (x << 1))) & 0x3); } // 5
inline bool3v_t not3_c(bool3v_t x) { return (((~(x+2) ^ 0x3) - 2) & 0x3); } // 5
inline bool3v_t not3_d(bool3v_t x) { return ((((x ^ 0x3) *5)) >> 1) & 0x3; } // 4M

inline bool3v_t xor3(bool3v_t x, bool3v_t y) { return ((x | y) & not3(x & y)); }  // 8
inline bool3v_t xor3_a(bool3v_t x, bool3v_t y) { return (((((x*3) | (y*3)) >> 1) & 0x3)); }
inline bool3v_t xor3_b(bool3v_t x, bool3v_t y) { return (((((x*3) >> 1) | y )) ^ (x & y)); }
inline bool3v_t xor3_c(bool3v_t x, bool3v_t y) { return (((x-2)^(y-2))+2) & 0x3; }

inline bool3v_t impl3(bool3v_t x, bool3v_t y) { return (not3(x) | y); }  // 6


inline bool3v_t pat1_1a(bool3v_t x) { return ((x + 1) & 0x3); }
inline bool3v_t pat1_1b(bool3v_t x) { return (((x + 1) >> 1) & 0x3); }
inline bool3v_t pat1_2a(bool3v_t x) { return ((x + 2) & 0x3); }
inline bool3v_t pat1_2b(bool3v_t x) { return (((x + 2) >> 1) & 0x3); }
inline bool3v_t pat1_2c(bool3v_t x) { return (((x + 2) >> 2) & 0x3); }
inline bool3v_t pat1_3a(bool3v_t x) { return ((x + 3) & 0x3); }
inline bool3v_t pat1_3b(bool3v_t x) { return (((x + 3) >> 1) & 0x3); }
inline bool3v_t pat1_3c(bool3v_t x) { return (((x + 3) >> 2) & 0x3); }
inline bool3v_t pat1_4a(bool3v_t x) { return ((x * 3) & 0x3); }
inline bool3v_t pat1_4b(bool3v_t x) { return (((x * 3) >> 1) & 0x3); }
inline bool3v_t pat1_4c(bool3v_t x) { return (((x * 3) >> 2) & 0x3); }
inline bool3v_t pat1_4d(bool3v_t x) { return (((x * 3) >> 3) & 0x3); }
inline bool3v_t pat1_5a(bool3v_t x) { return ((x * 5) & 0x3); }
inline bool3v_t pat1_5b(bool3v_t x) { return (((x * 5) >> 1) & 0x3); }
inline bool3v_t pat1_5c(bool3v_t x) { return (((x * 5) >> 2) & 0x3); }
inline bool3v_t pat1_5d(bool3v_t x) { return (((x * 5) >> 3) & 0x3); }
inline bool3v_t pat1_6a(bool3v_t x) { return ((x * 7) & 0x3); }
inline bool3v_t pat1_6b(bool3v_t x) { return (((x * 7) >> 1) & 0x3); }
inline bool3v_t pat1_6c(bool3v_t x) { return (((x * 7) >> 2) & 0x3); }
inline bool3v_t pat1_6d(bool3v_t x) { return (((x * 7) >> 3) & 0x3); }
inline bool3v_t pat1_6e(bool3v_t x) { return (((x * 7) >> 4) & 0x3); }

inline bool3v_t pat1_7a(bool3v_t x) { return (x ^ 0x1); }
inline bool3v_t pat1_7b(bool3v_t x) { return (x ^ 0x2); }
inline bool3v_t pat1_7c(bool3v_t x) { return (x ^ 0x3); }
inline bool3v_t pat1_8a(bool3v_t x) { return (((x * 5) >> 1) & 0x3); }
inline bool3v_t pat1_8b(bool3v_t x) { return ((((x << 2) | x) >> 1) & 0x3); }
inline bool3v_t pat1_9a(bool3v_t x) { return (((x >> 1) & 0x1) + (x & 0x1)); }



inline bool3v_t pat2_1(bool3v_t x, bool3v_t y) { return (x & y); }
inline bool3v_t pat2_2(bool3v_t x, bool3v_t y) { return (x | y); }
inline bool3v_t pat2_3(bool3v_t x, bool3v_t y) { return (x ^ y); }
inline bool3v_t pat2_4(bool3v_t x, bool3v_t y) { return (x + y); }
inline bool3v_t pat2_5(bool3v_t x, bool3v_t y) { return (x - y); }
inline bool3v_t pat2_6(bool3v_t x, bool3v_t y) { return (((x*3) | (y*3)) >> 1); }

inline bool3v_t pat2_a(bool3v_t x, bool3v_t y) { return (x * y); }

/*
std::ostream&
operator<<(std::ostream& os, bool3v_t x) {
  static char name[] = {'f', '-', 'x', 't'};
  os << name[x];
  return os;
}
*/

#endif

