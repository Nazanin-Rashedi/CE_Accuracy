#ifndef INFRA_TIME_T_HH
#define INFRA_TIME_T_HH
#pragma once

#include <inttypes.h>
#include <math.h>
#include <ctype.h>
#include <iostream>
#include <string>

#include "infra/hasht.hh"

/* 
 * class time 
 * time in nanoseconds from start of the day
 */

namespace fa {

class fa_time_t {
  public:
    typedef int64_t rep_t;
  public:
    fa_time_t() : _val(0) {}
    fa_time_t(const rep_t x) : _val(x) {}
    fa_time_t(unsigned int h, unsigned int m, unsigned int s) 
          : _val((((h * m_per_h()) + m) * s_per_m() + s) * ns_per_s()) {}
    fa_time_t(const std::string&, char sep = _default_separator);
    fa_time_t(const char*, char sep = _default_separator);
  public:
    inline rep_t val() const { return _val; }
  public:
    // get h/m/s/ms/us/ns of the time, ignoring those of higher and lower granularity
    inline unsigned int hour()         const { return  (val() / ns_per_h()); }
    inline unsigned int minute()       const { return ((val() % ns_per_h())  / ns_per_m()); }
    inline unsigned int second()       const { return ((val() % ns_per_m())  / ns_per_s()); }
    inline unsigned int milliseconds() const { return ((val() % ns_per_s())  / ns_per_ms()); }
    inline unsigned int microseconds() const { return ((val() % ns_per_ms()) / ns_per_us()); }
    inline unsigned int nanoseconds()  const { return  (val() % ns_per_us()); }
  public:
    // get seconds including fractions thereof
    inline double sec_frac() const { return  (  ((double) (val() % ns_per_m())) 
                                              / ((double) ns_per_s())); }
  public:
    static fa_time_t time_of_day_local();
    static fa_time_t time_of_day_utc();
    fa_time_t& set_to_time_of_day_local();
    fa_time_t& set_to_time_of_day_utc();
  public:
    inline bool operator==(const fa_time_t& x) const { return (val() == x.val()); }
    inline bool operator!=(const fa_time_t& x) const { return (val() != x.val()); }
    inline bool operator<(const fa_time_t& x)  const { return (val() <  x.val()); }
    inline bool operator<=(const fa_time_t& x) const { return (val() <= x.val()); }
    inline bool operator>(const fa_time_t& x)  const { return (val() >  x.val()); }
    inline bool operator>=(const fa_time_t& x) const { return (val() >= x.val()); }
  public:
    inline fa_time_t& operator+=(const fa_time_t& x) { _val += x.val(); return (*this); }
  public:
    inline uint64_t hash() const { return ht::murmur_hash<uint64_t>(val()); }
  public:
    std::ostream& print(std::ostream&, char aSep = _default_separator) const;
  public:
    bool set(const rep_t x);
    bool set(const unsigned int h, const unsigned int m, const double s);
    bool set(const char*& s, char sep = _default_separator);
    bool set_from_ptr(const char* s, const char sep = _default_separator);
    bool set_no_sep(const char*& s);
    bool set(const std::string& s, char sep = _default_separator);
  private:
    rep_t _val; // nanoseconds since midnight
  public:
    static char _default_separator;
    static bool _print_with_separator;
  public:
    static inline rep_t nanosecondsPerSecond() { return _ns_per_s; }
    static inline rep_t secondsPerMinute()     { return _s_per_m; }
    static inline rep_t secondsPerDay()        { return _s_per_d;   }
    static inline rep_t nanosecondsPerDay()    { return _ns_per_d;  }
  public:
    static inline rep_t h_per_d()   { return _h_per_d; }
    static inline rep_t m_per_h()   { return _m_per_h; }
    static inline rep_t m_per_d()   { return _m_per_d; }
    static inline rep_t s_per_m()   { return _s_per_m; }
    static inline rep_t s_per_h()   { return _s_per_h; }
    static inline rep_t s_per_d()   { return _s_per_d; }
    static inline rep_t ms_per_s()  { return _ms_per_s; }
    static inline rep_t ms_per_m()  { return _ms_per_m; }
    static inline rep_t ms_per_h()  { return _ms_per_h; }
    static inline rep_t ms_per_d()  { return _ms_per_d; }
    static inline rep_t us_per_ms() { return _us_per_ms; }
    static inline rep_t us_per_s()  { return _us_per_s; }
    static inline rep_t us_per_m()  { return _us_per_m; }
    static inline rep_t us_per_h()  { return _us_per_h; }
    static inline rep_t us_per_d()  { return _us_per_d; }
    static inline rep_t ns_per_us() { return _ns_per_us; }
    static inline rep_t ns_per_ms() { return _ns_per_ms; }
    static inline rep_t ns_per_s()  { return _ns_per_s; }
    static inline rep_t ns_per_m()  { return _ns_per_m; }
    static inline rep_t ns_per_h()  { return _ns_per_h; }
    static inline rep_t ns_per_d()  { return _ns_per_d; }
  private:
    static constexpr rep_t _h_per_d   = 24;
    static constexpr rep_t _m_per_h   = 60;
    static constexpr rep_t _m_per_d   = _m_per_h * _h_per_d;
    static constexpr rep_t _s_per_m   = 60;
    static constexpr rep_t _s_per_h   = _s_per_m * _m_per_h;
    static constexpr rep_t _s_per_d   = _s_per_h * _h_per_d;
    static constexpr rep_t _ms_per_s  = 1000;
    static constexpr rep_t _ms_per_m  = _ms_per_s * _s_per_m;
    static constexpr rep_t _ms_per_h  = _ms_per_m * _m_per_h;
    static constexpr rep_t _ms_per_d  = _ms_per_h * _h_per_d;
    static constexpr rep_t _us_per_ms = 1000;
    static constexpr rep_t _us_per_s  = _us_per_ms * _ms_per_s;
    static constexpr rep_t _us_per_m  = _us_per_ms * _ms_per_m;
    static constexpr rep_t _us_per_h  = _us_per_ms * _ms_per_h;
    static constexpr rep_t _us_per_d  = _us_per_ms * _ms_per_d;
    static constexpr rep_t _ns_per_us = 1000;
    static constexpr rep_t _ns_per_ms = _ns_per_us * _us_per_ms;
    static constexpr rep_t _ns_per_s  = _ns_per_us * _us_per_s;
    static constexpr rep_t _ns_per_m  = _ns_per_us * _us_per_m;
    static constexpr rep_t _ns_per_h  = _ns_per_us * _us_per_h;
    static constexpr rep_t _ns_per_d  = _ns_per_us * _us_per_d;
};

std::ostream& operator<<(std::ostream&, const fa_time_t&);
fa_time_t operator""_ns(unsigned long long int x);
fa_time_t operator""_ms(unsigned long long int x);
fa_time_t operator""_us(unsigned long long int x);
fa_time_t operator""_ft(const char* s, const size_t aSize);

} // end namespace


#endif

