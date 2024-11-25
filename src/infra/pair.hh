#ifndef PAIR_HH
#define PAIR_HH

#include <map>

// fpair: sorting on first component only

typedef unsigned int id_t;

struct fpair_t {
  fpair_t() : a(0), b(0) {}
  fpair_t(id_t aa, id_t bb) : a(aa), b(bb) {}
  id_t a;
  id_t b;
};

inline bool
operator<(const fpair_t& t1, const fpair_t& t2) {
  if(t1.a < t2.a) {
    return true;
  }
  return false;
}

inline std::ostream&
operator<<(std::ostream& os, const fpair_t& x) {
  os << x.a << ' ' << x.b;
  return os;
}

// lexpair: sorting lexicographically on both components

struct lexpair_t {
  lexpair_t() : a(0), b(0) {}
  lexpair_t(id_t aa, id_t bb) : a(aa), b(bb) {}
  id_t a;
  id_t b;
};

inline bool
operator<(const lexpair_t& t1, const lexpair_t& t2) {
  if(t1.a < t2.a) {
    return true;
  } else
  if(t1.a == t2.a && t1.b < t2.b) {
    return true;
  }
  return false;
}

inline std::ostream&
operator<<(std::ostream& os, const lexpair_t& x) {
  os << x.a << ' ' << x.b;
  return os;
}

#endif
