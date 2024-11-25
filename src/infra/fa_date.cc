#include "fa_date.hh"
#include <vector>

namespace fa {

fa_date_t::fa_date_t() : _jd(0) {}

fa_date_t::fa_date_t(rep_t x) : _jd(x) {}


fa_date_t::fa_date_t(uint32_t x, bool aYearHigh) : _jd(0) {
  if(aYearHigh) {
    int d = x % 100;
    int r = x / 100;
    int m = r % 100;
    int y = r / 100;
    _jd = ymd2jd(y,m,d);
  } else {
    int y = x % 10000;
    int r = x / 10000;
    int m = r / 100;
    int d = r % 100;
    _jd = ymd2jd(y,m,d);
  }
}

fa_date_t::fa_date_t(int y, int m, int d) : _jd(ymd2jd(y,m,d)) {}

fa_date_t::fa_date_t(const std::string& aDate, const bool aYearHigh, const char aSep) : _jd(0) {
  const char* x = aDate.c_str();
  set(x, aYearHigh, aSep);
}

fa_date_t::fa_date_t(const char* aDate, const bool aYearHigh, const char aSep) : _jd(0) {
  set(aDate, aYearHigh, aSep);
}


fa_date_t::rep_t
fa_date_t::jd_today_utc() {
  time_t lTimeTics = time(0);
  tm     lTimeRes;
  gmtime_r(&lTimeTics, &lTimeRes);
  // std::cerr << "UTC: " << asctime(&lTimeRes) << std::endl;
  // std::cerr << "UTC date: "
  //           << lTimeRes.tm_year << ':'
  //           << lTimeRes.tm_mon  << ':'
  //           << lTimeRes.tm_mday << std::endl;
  return ymd2jd(lTimeRes.tm_year + 1900, lTimeRes.tm_mon + 1, lTimeRes.tm_mday);
}

fa_date_t::rep_t
fa_date_t::jd_today_local() {
  time_t c;
  time(&c);
  struct tm* today = localtime(&c);
  return ymd2jd(today->tm_year + 1900, today->tm_mon + 1, today->tm_mday);
}

bool
fa_date_t::simple_set(const char* aStringPtr, const bool aYearHigh, const char sep) {
  const char* x = aStringPtr;
  char* end;

  int y = 0;
  int m = 0;
  int d = 0;
  if(aYearHigh) {
    y = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    d = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  } else {
    d = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    y = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  }
  _jd = ymd2jd(y,m,d);
  return true;
}

bool
fa_date_t::set(const char*& aStringPtr, bool aYearHigh, char sep) {
  const char* x = aStringPtr;
  char* end;
 
  uint y = 0;
  uint m = 0;
  uint d = 0;
  if(aYearHigh) {
    y = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    d = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  } else {
    d = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    y = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  }
  aStringPtr = x;
  _jd = ymd2jd(y,m,d);
  return true;
}

bool
fa_date_t::set(const char*& aStringPtr) { // format: YYYYMMDD
  const char* x = aStringPtr;
  char* lEnd;
  uint lDate = 0;
  lDate = strtol(x, &lEnd, 10);
  if(lEnd == x) { 
    return false; 
  }
  aStringPtr = lEnd;
  set(lDate);
  return true;
}

fa_date_t&
fa_date_t::set(const uint y, const uint m, const uint d) {
  _jd = ymd2jd(y,m,d);
  return (*this);
}

void
fa_date_t::set(const uint aYYYYMMDD) {
  uint x = aYYYYMMDD;
  const uint y = (x / 10000);
  x = (x % 10000);
  const uint m = x / 100;
  const uint d = x % 100;
  set(y,m,d);
}

void
fa_date_t::set(std::istream& is, const char aSep) {
  char lSep;
  uint y,m,d;
  is >> y;
  if(is.eof()) { return; }
  is >> lSep;
  if(is.eof()) { return; }
  if(aSep != lSep) {
    std::cerr << "Warning: wrong field separator in date: `" << lSep << "'." << std::endl;
  }
  is >> m;
  if(is.eof()) { return; }
  is >> lSep;
  if(is.eof()) { return; }
  if(aSep != lSep) {
    std::cerr << "Warning: wrong field separator in date: `" << lSep << "'." << std::endl;
  }
  is >> d;
  set(y,m,d);
}

fa_date_t::rep_t
fa_date_t::ymd2jd(const int ay, const int am, const int ad) {
   const int64_t y = ay;
   const int64_t m = am;
   const int64_t d = ad;
   return ( 1461 * ( y + 4800 + ( m - 14 ) / 12 ) ) / 4 +
          ( 367 * ( m - 2 - 12 * ( ( m - 14 ) / 12 ) ) ) / 12 -
          ( 3 * ( ( y + 4900 + ( m - 14 ) / 12 ) / 100 ) ) / 4 +
          d - 32075;
}

void
fa_date_t::jd2ymd(const rep_t jd, int& y, int& m, int& d) {
          int64_t l = (int64_t) jd + (int64_t) 68569;
    const int64_t n = ( 4 * l ) / (int64_t) 146097;
                  l = l - ( 146097 * n + 3 ) / (int64_t) 4;
    const int64_t i = ( 4000 * ( l + 1 ) ) / (int64_t) 1461001;
                  l = l - ( 1461 * i ) / 4 + 31;
    const int64_t j = ( 80 * l ) / 2447;
                  d = l - ( 2447 * j ) / 80;
                  l = j / 11;
                  m = j + 2 - ( 12 * l );
                  y = 100 * ( n - 49 ) + i + l;
}



std::ostream&
fa_date_t::print(std::ostream& os, const char aSep) const {
  int y = 0;
  int m = 0;
  int d = 0;
  get(y,m,d);
  os << y << aSep
     << (m < 10 ? "0" : "") << m << aSep
     << (d < 10 ? "0" : "") << d;
  return os;
}

// 20170222
void
fa_date_t::printToString(char* s) const {
  int y = 0;
  int m = 0;
  int d = 0;
  get(y,m,d);
  snprintf(s, 5, "%d", y);
  s += 4;

  if(10 > m) {
    *s++ = '0';
  }
  snprintf(s, 3, "%d", m);
  if(10 > m) {
    ++s;
  } else {
    s += 2;
  }
  if(10 > d) {
    *s++ = '0';
  }
  snprintf(s, 3, "%d", d);
}

std::ostream&
fa_date_t::printXml(std::ostream& os) const {
  int y = 0;
  int m = 0;
  int d = 0;
  get(y,m,d);
  if(10 > y) {
    os << '0';
  }
  os << y << '-';
  if(10 > m) {
    os << '0';
  }
  os << m << '-';
  if(10 > d) {
    os << '0';
  }
  os << d;
  return os;
}


uint32_t
fa_date_t::year() const {
        rep_t l = _jd + 68569;
  const rep_t n = ( 4 * l ) / 146097;
              l = l - ( 146097 * n + 3 ) / 4;
  const rep_t i = ( 4000 * ( l + 1 ) ) / 1461001;
              l = l - ( 1461 * i ) / 4 + 31;
  const rep_t j = ( 80 * l ) / 2447;
              l = j / 11;
        rep_t y = 100 * ( n - 49 ) + i + l;
  return y;
}



uint32_t
fa_date_t::month() const {
  int y,m,d;
  jd2ymd(_jd,y,m,d);
  return m;
}

uint32_t
fa_date_t::day() const {
  int y,m,d;
  jd2ymd(_jd,y,m,d);
  return d;
}

uint32_t
fa_date_t::day_of_year() const {
  return (_jd - fa_date_t(year(),1,1).jd() + 1);
}

uint32_t
fa_date_t::dayOfTheYear() const {
  return (_jd - fa_date_t(year(),1,1).jd());
}

fa_date_t::month_et
fa_date_t::month_of_year() const {
  return (month_et) (month() - 1);
}

fa_date_t::weekday_et
fa_date_t::day_of_week() const {
  fa_date_t someSunday(1997,10,12);
  return (weekday_et) ((someSunday <= (*this)) ? 
                      (subtract_date(someSunday) % 7) 
                    : (someSunday.subtract_date(*this) % 7));
}

/** Leap Year rules:
    1. Years evenly divisible by four are to be considered Leap Years.
    2. Century years (double 00) must be evenly divisible by 400
    (2000 is a leap year).
    3. The years 4000, 8000, and 12000
    are considered to be "common years" (not leap years).
    4. Century years divisible by 900 will be leap years
    only if the remainder is 200 or 600 (2000 is a leap year).
*/

bool 
fa_date_t::is_leap_year() const {
  uint y = year();
  uint c = y % 100;
  uint r = y % 900;
  if(y == 4000 || y == 8000 || y == 12000) { return false; }      // 3
  if((r == 0) && ((y % 200) == 0 || ((y % 600) == 0))) { return true; } // 4
  if(c == 0 && (y % 400) == 0) { return true; }
  return (c != 0) && ((y % 4) == 0);
}

typedef std::vector<std::string> string_vt;

const string_vt gMonthName = {
  "January",
  "February",
  "March",
  "April",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December",
  "<NoValidMonth>"
};

fa_date_t::month_et
fa_date_t::str2month(const char* aMonth) {
  for(int i = 0; i < kNoMonth; ++i) {
    if(0 == strcmp(aMonth, gMonthName[i].c_str())) {
      return ((month_et) i);
    }
  }
  return kNoMonth;
}

fa_date_t::month_et
fa_date_t::str2month(const std::string& aMonth) {
  for(int i = 0; i < kNoMonth; ++i) {
    if(aMonth == gMonthName[i]) {
      return ((month_et) i);
    }
  }
  return kNoMonth;
}

const string_vt gWeekdayName = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "<NoValidWeekday>"
};

fa_date_t::weekday_et
fa_date_t::str2weekday(const char* aWd) {
  for(int i = 0; i < kNoWeekday; ++i) {
    if(0 == strcmp(aWd, gWeekdayName[i].c_str())) {
      return ((weekday_et) i);
    }
  }
  return kNoWeekday;
}

fa_date_t::weekday_et
fa_date_t::str2weekday(const std::string& aWd) {
  for(int i = 0; i < kNoWeekday; ++i) {
    if(aWd == gWeekdayName[i]) {
      return ((weekday_et) i);
    }
  }
  return kNoWeekday;
}

std::string
fa_date_t::to_string(const char aSep) const {
  int y, m, d;
  jd2ymd(jd(), y, m, d);
  std::string lRes = std::to_string(y) + aSep +
                     std::to_string(m) + aSep +
                     std::to_string(d);
  return lRes;
}

std::ostream&
operator<<(std::ostream& os, const fa_date_t& x) {
  return x.print(os);
}

std::istream&
operator>>(std::istream& aIs, fa_date_t& x) {
  return aIs;
}


} // end namespace

