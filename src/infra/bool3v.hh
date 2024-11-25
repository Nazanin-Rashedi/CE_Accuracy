#ifndef INFRA_BOOL3_HH
#define INFRA_BOOL3_HH

#include <iostream>
#include <string>


// three values logic


class bool3v {
  private:
    typedef unsigned int bool3v_t;
    bool3v(const bool3v_t& x) : _val(x) {}
  public:
    bool3v() : _val(0x1) {}
    bool3v(const bool3v& x) : _val(x._val) {}
  public:
    static inline bool3v True()    { return bool3v(0x3); }
    static inline bool3v False()   { return bool3v(0x0); }
    static inline bool3v Unknown() { return bool3v(0x1); }
  public:
    inline bool isTrue()    const { return (0x3 == _val); }
    inline bool isFalse()   const { return (0x0 == _val); }
    inline bool isUnknown() const { return (0x1 == _val); }
    inline bool operator==(const bool3v& x) const { return (_val == x._val); }
  public:
    inline bool3v Not() const { const bool3v_t x = _val ^ 0x3; return bool3v(x >> (3 - x)); }
    inline bool3v And(const bool3v x) const { return bool3v(_val & x._val); }
    inline bool3v Or(const bool3v x) const { return bool3v(_val | x._val); }
    // inline bool3v Xor(const bool3v x) const {
    //                     const bool3v_t y = _val;
    //                     return ((x | y) & not3(x & y));
    //               }
    inline bool3v Impl(const bool3v x) const { return bool3v((*this).Not()._val | x._val); }
    inline bool3v Equiv(const bool3v x, const bool3v y) { return ((x == y) + ((x == y) << 1)); }
  public:
    inline bool check() const;
  public:
    inline char name1() const;
    inline const std::string& name() const;
  private:
    bool3v_t _val;
};


// returns true if o.k.
inline bool
bool3v::check() const {
  return (_val == 0x0 || _val == 0x1 || _val == 0x3);
}

inline char 
bool3v::name1() const {
  char lRes = 'x';
  switch(_val) {
    case 0x0: lRes = 'F'; break;
    case 0x1: lRes = '-'; break;
    case 0x3: lRes = 'T'; break;
    default: lRes = 'x'; break;
  }
  return lRes;
}


inline const std::string&
bool3v::name() const {
  static const std::string lName[4] = { "False", "Unknown", "Unknown", "True" };
  return lName[_val & 0x3];
}


inline std::ostream&
operator<<(std::ostream& os, const bool3v& x) {
  os << x.name1();
  return os;
}



#endif

