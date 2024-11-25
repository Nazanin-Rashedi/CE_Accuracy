#include "fa_time.hh"
#include <sys/time.h>
#include <string.h>

namespace fa {

char fa_time_t::_default_separator    = ':';
bool fa_time_t::_print_with_separator = true;

fa_time_t::fa_time_t(const std::string& x, char sep) : _val(0) {
  const char* s = x.c_str();
  set(s, sep);
}

fa_time_t::fa_time_t(const char* x, const char sep) : _val(0) {
  const char* s = x;
  set(s, sep);
}

#define TRY_NOW

fa_time_t
fa_time_t::time_of_day_local() {
  time_t  lTimeTics = time(0);
  tm      lTimeRes;
  timeval lTime;
  localtime_r(&lTimeTics, &lTimeRes);
  gettimeofday(&lTime, nullptr); 

  fa_time_t lRes;
  lRes.set(lTimeRes.tm_hour, lTimeRes.tm_min, (double) lTimeRes.tm_sec);
  lRes._val += lTime.tv_usec * ns_per_us();

  // std::cerr << "LOC: " << asctime(&lTimeRes) << std::endl;
  // std::cerr << "LOC date: "
  //           << lTimeRes.tm_year << ':'
  //           << lTimeRes.tm_mon  << ':'
  //           << lTimeRes.tm_mday << std::endl;


  return lRes;
}

fa_time_t
fa_time_t::time_of_day_utc() {
/*
  time_t lTimeTics = time(0);
  tm     lTimeRes;
  gmtime_r(&lTimeTics, &lTimeRes);

  // std::cerr << "UTC: " << asctime(&lTimeRes) << std::endl;
  // std::cerr << "UTC date: "
  //           << lTimeRes.tm_year << ':'
  //           << lTimeRes.tm_mon  << ':'
  //           << lTimeRes.tm_mday << std::endl;
  set(lTimeRes.tm_hour, lTimeRes.tm_min, (double) lTimeRes.tm_sec);
*/

  timeval  lTime;
  gettimeofday(&lTime, nullptr); 
  fa_time_t lRes;
  lRes._val = (lTime.tv_sec % s_per_d()) * ns_per_s() + lTime.tv_usec * ns_per_us();

  return lRes;
}

fa_time_t&
fa_time_t::set_to_time_of_day_local() {
  (*this) = time_of_day_local();
  return (*this);
}

fa_time_t&
fa_time_t::set_to_time_of_day_utc() {
  (*this) = time_of_day_utc();
  return (*this);
}



bool
fa_time_t::set(const rep_t x) {
  if(x > ns_per_d()) {
    return false;
  }
  _val = x;
  return true;
}

bool
fa_time_t::set(const unsigned int h, const unsigned int m, const double s) {
  if(h > 24) {
    return false;
  }
  if(m > 60) {
    return false;
  }
  if((60 < s) || (0 > s)) {
    return false;
  }
  _val = 0;
  _val += h * ns_per_h();
  _val += m * ns_per_m();
  _val += (uint64_t) std::round(s * ns_per_s());
  if(_val > ns_per_d()) {
    return false;
  }
  return true;
}

bool
fa_time_t::set(const char*& x, const char sep) {
  if(nullptr == memchr(x, sep, strlen(x))) {
    return set_no_sep(x);
  }
  char* lEnd = 0;
  int h = 0;
  h = strtol(x, &lEnd, 10);
  if((x == lEnd) || (0 > h)) {
    std::cerr << "I011 fa_time_t::set " << *x << std::endl;
    return false;
  }
  x = lEnd;
  if(sep != *x) {
    std::cerr << "I012 fa_time_t::set " << *x << std::endl;
    return false;
  }
  ++x;
  int m = strtol(x, &lEnd, 10);
  if((x == lEnd) || (0 > m) || (60 < m)) {
    std::cerr << "I021 fa_time_t::set " << *x << std::endl;
    return false;
  }
  x = lEnd;
  double s = 0;
  if(sep == *x) {
    ++x;
    s = strtod(x, &lEnd);
    if((x == lEnd) || (0 > s)){
      std::cerr << "I031 fa_time_t::set " << *x << std::endl;
      return false;
    }
    x = lEnd;
  }
  return set(h, m, s);
}

bool
fa_time_t::set_from_ptr(const char* s, const char sep) {
  const char* x = s;
  return set(x, sep);
}

// hhmm[0-9]*(.[0-9*])?
bool
fa_time_t::set_no_sep(const char*& x) {
  for(uint i = 0; i < 4; ++i) {
    if(!isdigit(x[i])) { return false; }
  }
  int h = (x[0] - '0') * 10 + (x[1] - '0');
  int m = (x[2] - '0') * 10 + (x[3] - '0');
  char* lEnd = nullptr;
  x += 4;
  double s = strtod(x, &lEnd);
  if(lEnd == x) {
    return false;
  }
  x = lEnd;
  return set(h, m, s);
}

bool
fa_time_t::set(const std::string& s, char sep) {
  return set_from_ptr(s.c_str(), sep);
}

std::ostream&
fa_time_t::print(std::ostream& os, char sep) const {
  const unsigned int h = hour();
  if(10 > h) {
    os << '0';
  }
  os << h << sep;
  const unsigned int m = minute();
  if(10 > m) {
    os << '0';
  }
  os << m << sep;
  const double s = sec_frac();
  if(10 > s) {
    os << '0';
  }
  os << s;
  return os;
}

std::ostream&
operator<<(std::ostream& os, const fa_time_t& x) {
  return x.print(os);
}

fa_time_t 
operator""_ns(unsigned long long x) {
  return fa_time_t(x);
}

fa_time_t 
operator""_us(unsigned long long x) {
  return fa_time_t(x * fa_time_t::ns_per_us());
}

fa_time_t 
operator""_ms(unsigned long long x) {
  return fa_time_t(x * fa_time_t::ns_per_ms());
}

fa_time_t
operator""_ft(const char* s, const size_t aSize) {
  fa_time_t lRes;
  if(!lRes.set_from_ptr(s)) {
    std::cout << "Bad: " << s << " is not a valid date string." << std::endl;
  }
  return lRes;
}


} // end namspace

