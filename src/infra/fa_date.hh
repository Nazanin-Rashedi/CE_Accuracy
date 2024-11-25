#ifndef INFRA_FA_DATE_JULIAN_DAY_HH
#define INFRA_FA_DATE_JULIAN_DAY_HH
#pragma once

#include <iostream>
#include <string>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

/*
 * class fa_date
 * represents a date
 * using julian day
 * valid between   -4713-11-24 and 5874898-6-3
 */

namespace fa {

class fa_date_t {
  public:
  enum weekday_et { Sunday     = 0,
                    Monday     = 1,
                    Tuesday    = 2,
                    Wednesday  = 3,
                    Thursday   = 4,
                    Friday     = 5,
                    Saturday   = 6,
                    kNoWeekday = 7 };

  ///
  enum month_et { January   = 0,
                  February  = 1,
                  March     = 2,
                  April     = 3,
                  May       = 4,
                  June      = 5,
                  July      = 6,
                  August    = 7,
                  September = 8,
                  October   = 9,
                  November  = 10,
                  December  = 11,
                  kNoMonth  = 12 };


  public:
    typedef int32_t rep_t; // representation type of a date
  public:
    fa_date_t();
    fa_date_t(rep_t aDayNo);
    fa_date_t(uint32_t x, bool aYearHigh); // yyyymmdd oder ddmmyyyy
    fa_date_t(int aYear, int aMonth, int aDay);
    fa_date_t(const std::string& aDate, const bool aYearHigh = true, const char aSep = '-');
    fa_date_t(const char*        aDate, const bool aYearHigh, const char aSep = '-');
  public:
    inline rep_t      julianDay() const { return _jd; }
    inline rep_t      jd() const { return _jd; }
    inline void       get(int& y, int& m, int& d) const { jd2ymd(_jd, y, m, d); }
    inline fa_date_t& set_to_current_date_utc()    { _jd = jd_today_utc(); return (*this); }
    inline fa_date_t& set_to_current_date_local()  { _jd = jd_today_local(); return (*this); }
  public:
    static inline fa_date_t current_date_utc()   { return fa_date_t(jd_today_utc()); }
    static inline fa_date_t today_utc()          { return fa_date_t(jd_today_utc()); }
    static inline fa_date_t current_date_local() { return fa_date_t(jd_today_local()); }
    static inline fa_date_t today_local()        { return fa_date_t(jd_today_local()); }
  public:
    inline rep_t get_jd() const { return _jd; }
    inline void  set_jd(const rep_t aJd) { _jd = aJd; }
  public:
    uint year() const;
    uint month() const;
    uint day() const;
  public:
    fa_date_t& set(const uint y, const uint m, const uint d);
    bool     simple_set(const char* s, const bool aYearHigh, const char sep = '-');
    bool     set(const char*& s /* in/out */, bool aYearHigh, char sep = '-');
    bool     set(const char*& s); // format: YYYYMMDD
    void     set(const uint aYYYYMMDD);
    void     set(std::istream& is, const char aSep);
  public:
    uint32_t   day_of_year() const;  // starts with first of january = 1
    uint32_t   dayOfTheYear() const; // starts with first of january = 0
    month_et   month_of_year() const;
    weekday_et day_of_week() const;
    bool       is_leap_year() const;
  public:
    inline fa_date_t& add_days(int x) { _jd += x; return (*this); }
    inline fa_date_t& subtract_days(int x) { assert(_jd > x); _jd -= x; return (*this); }
    inline int64_t subtract_date(const fa_date_t& x) const { return (((int64_t) _jd) - ((int64_t) x._jd)); }
  public:
    static rep_t ymd2jd(const int y, const int m, const int d);
    static void  jd2ymd(const rep_t aJulianDay, int& y, int& m, int& d);
    static rep_t jd_today_utc();
    static rep_t jd_today_local();
  public:
    inline fa_date_t& operator=(const fa_date_t& x) { _jd = x._jd; return (*this); }
    inline fa_date_t& operator++() { ++_jd; return (*this); } // add one day
    inline fa_date_t& operator--() { --_jd; return (*this); } // subtract one day
    inline fa_date_t& operator+=(int x) { _jd += x; return (*this); }
    inline fa_date_t& operator-=(int x) { _jd -= x; return (*this); }
    inline fa_date_t  operator+(int x) const { return fa_date_t(jd() + x); }
    inline fa_date_t  operator-(int x) const { return fa_date_t(jd() - x); }
  public:
    inline bool operator==(const fa_date_t& x) const { return (_jd == x._jd); }
    inline bool operator!=(const fa_date_t& x) const { return (_jd != x._jd); }
    inline bool operator< (const fa_date_t& x) const { return (_jd <  x._jd); }
    inline bool operator<=(const fa_date_t& x) const { return (_jd <= x._jd); }
    inline bool operator> (const fa_date_t& x) const { return (_jd >  x._jd); }
    inline bool operator>=(const fa_date_t& x) const { return (_jd >= x._jd); }
    inline int  operator- (const fa_date_t& x) const { return (_jd -  x._jd); }
  public:
    static month_et   str2month(  const std::string& aMonthName);
    static month_et   str2month(  const char*        aMonthName);
    static weekday_et str2weekday(const std::string& aWeekdayName);
    static weekday_et str2weekday(const char*        aWeekdayName);
  public:
    std::string to_string(const char aSep = '-') const;
    void printToString(char* s) const;
  public:
    std::ostream& print(std::ostream&, const char aSep = '-') const;
    std::ostream& printXml(std::ostream&) const;
  private:
    rep_t _jd;
  public:
    static bool _print_with_separator;
    static char _default_separator;
};

std::ostream& operator<<(std::ostream& os, const fa_date_t& x);
std::istream& operator>>(std::istream& is, fa_date_t& x);

} // end namespace

#endif
