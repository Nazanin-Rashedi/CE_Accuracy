#include "stime.hh"

STime::STime(const std::string& x, char sep) : _val(0) {
  const char* s = x.c_str();
  set(s, sep);
}

STime::STime(const char* x, char sep) : _val(0) {
  const char* s = x;
  set(s, sep);
}

bool
STime::set(const char*& x, char sep) {
  char* lEnd = 0;
  int v = 0;
  v = strtol(x, &lEnd, 10);
  if(x == lEnd) {
    std::cerr << "I011 STime::set " << *x << std::endl;
    return false;
  }
  _val = v * 3600;
  x = lEnd;
  if(sep != *x) {
    std::cerr << "I012 STime::set " << *x << std::endl;
    return false;
  }
  ++x;
  v = strtol(x, &lEnd, 10);
  if(x == lEnd) {
    std::cerr << "I021 STime::set " << *x << std::endl;
    return false;
  }
  _val += v * 60;
  x = lEnd;
  if(sep == *x) {
    ++x;
    v = strtol(x, &lEnd, 10);
    if(x == lEnd) {
      std::cerr << "I031 STime::set " << *x << std::endl;
      return false;
    }
    _val += v;
    x = lEnd;
  }
  return true;
}

bool
STime::simple_set(const char* s, const char sep) {
  const char* x = s;
  char* lEnd = 0;
  int v = 0;
  v = strtol(x, &lEnd, 10);
  if(x == lEnd) {
    std::cerr << "I011 STime::set " << *x << std::endl;
    return false;
  }
  _val = v * 3600;
  x = lEnd;
  if(sep != *x) {
    std::cerr << "I012 STime::set " << *x << std::endl;
    return false;
  }
  ++x;
  v = strtol(x, &lEnd, 10);
  if(x == lEnd) {
    std::cerr << "I021 STime::set " << *x << std::endl;
    return false;
  }
  _val += v * 60;
  x = lEnd;
  if(sep == *x) {
    ++x;
    v = strtol(x, &lEnd, 10);
    if(x == lEnd) {
      std::cerr << "I031 STime::set " << *x << std::endl;
      return false;
    }
    _val += v;
    x = lEnd;
  }
  return true;
}

std::ostream&
STime::print(std::ostream& os, char sep) const {
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
  const unsigned int s = second();
  if(10 > s) {
    os << '0';
  }
  os << s;

  return os;
}

