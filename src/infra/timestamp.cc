#include "timestamp.hh"

const double Time::_secondsPerDay = (double) (3600*24);


Time::Time() : _time(0) {} Time::Time(double x) : _time(x) {}

Time::Time(double x, bool) : _time(0) {
  double lTrunc = trunc(x);
  int    lInt = lrint(lTrunc);
  double lFrac = x - lTrunc;
  set(lInt);
  _time += lFrac;
}

Time::Time(int h, int m, int s, double r) : _time(0) {
  set(h,m,s,r);
}

void
Time::set(int h, int m, int s, double r) {
  _time  = h * 3600.0;
  _time += m * 60.0;
  _time += s;
  _time += r;
}

Time::Time(const std::string& aString, char sep) : _time(0) {
  const char* x = aString.c_str();
  set(x, sep);
}

bool
Time::set(const char*& aStringPtr, char sep) {
  const char* x = aStringPtr;
  char* end;
  int h = strtol(x, &end, 10);
  if(end == x) { return false; }
  ++end;
  x = end;
  int m = strtol(x, &end, 10);
  if(end == x) { return false; }
  ++end;
  x = end;
  int s = strtol(x, &end, 10);
  if(end == x) { return false; }
  x = end;
  double r = 0.0;
  if('.' == *x) {
    ++x;
    if(!isspace(*x)) {
       r = strtod(x, &end);
       if(end == x) { return false; }
       x = end;
       r /= 100.0;
    }
  }
  set(h, m, s, r);
  aStringPtr = x;
  return true;
}

Time::Time(int x) : _time(0) {
  set(x);
}

void
Time::set(int x) {
  int s = x % 100;
  int m = ((x - s) / 100) % 100;
  int h = x / 10000;
  _time = (double) (h * 3600 + m * 60 + s);
}

int
Time::setTimeWithinOneDayInterval() {
  if(0 > time()) {
    addOneDay();
    return -1;
  } else
  if(secondsPerDay() < time()) {
    subtractOneDay();
    return +1;
  }
  return 0;
}


void
Time::addOneDay() {
  _time += secondsPerDay();
}

void
Time::subtractOneDay() {
  _time -= secondsPerDay();
}

void
Time::addSeconds(int x) {
  _time += (double) x;
}

void
Time::addHours(int x) {
  _time += (double) (x * 3600);
}

void
Time::subtractSeconds(int x) {
  _time -= (double) x;
}

void
Time::subtractHours(int x) {
  _time -= (double) (x * 3600);
}

/*
bool
Time::operator==(const Time& x) const {
  return (_time == x._time);
}

bool
Time::operator<(const Time& x) const {
  return (_time < x._time);
}

bool
Time::operator<=(const Time& x) const {
  return (_time <= x._time);
}

bool
Time::operator>(const Time& x) const {
  return (_time > x._time);
}

bool
Time::operator>=(const Time& x) const {
  return (_time >= x._time);
}
*/

double
Time::operator-(const Time& x) const {
  return (_time - x._time);
}


std::ostream&
Time::print(std::ostream& os, char sep) const {
  double lTrunc = trunc(_time);
  int    lInt   = lrint(lTrunc);
  double lFrac  = _time - lTrunc;

  int h = lInt / 3600;
  os << (h < 10 ? "0" : "") << h << sep;
  lInt = lInt % 3600;
  int m = lInt / 60;
  os << (m < 10 ? "0" : "") << m << sep;
  lInt = lInt % 60;
  os << (lInt < 10 ? "0" : "") << lInt << '.';
  lInt =  (int) round(lFrac * 100.0);
  os << (lInt < 10 ? "0" : "") << lInt;
  return os;
}

std::ostream&
Time::printXml(std::ostream& os) const {
  // os << lrint(trunc(time())) / 3600 << ':' << minute() << ':' << secAndFrac();
  if(10.0 > hour()) {
    os << '0';
  }
  os << hour() << ':';
  if(10.0 > minute()) {
    os << '0';
  }
  os << minute() << ':';
  if(10.0 > secAndFrac()) {
    os << '0';
  } 
  os << secAndFrac();
  return os;
}

Date::Date() : _y(0), _m(0), _d(0) {}

Date::Date(int x) : _y(0), _m(0), _d(0) {
  _y = x % 100;
  _m = ((x - _y) / 100) % 100;
  _d = x / 10000;
  _y += 2000;
}

Date::Date(int y, int m, int d) : _y(y), _m(m), _d(d) {}

Date::Date(const std::string& s, bool aYearHigh, char sep) : _y(0), _m(0), _d(0) {
  const char* x = s.c_str();
  set(x, aYearHigh, sep);
}

bool
Date::set(const char*& aStringPtr, bool aYearHigh, char /*sep*/) {
  const char* x = aStringPtr;
  char* end;

  if(aYearHigh) {
    _y = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    _m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    _d = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  } else {
    _d = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    _m = strtol(x, &end, 10);
    if(end == x) { return false; }
    ++end;
    x = end;
    _y = strtol(x, &end, 10);
    if(end == x) { return false; }
    x = end;
  }
  aStringPtr = x;
  return true;
}

Date&
Date::set(int y, int m, int d) {
  _y = y;
  _m = m;
  _d = d;
  return (*this);
}

int
Date::julianDay() const {
    double Y = (double) year();
    double M = (double) month();
    double D = (double) day();
    if(M < 3.0) {
      Y -= 1.0;
      M += 12.0;
    }
    double A = floor(Y / 100.0);
    // cout.precision(10);
    // cout << "A " << A << endl;
    double B = floor(A / 4);
    // cout << "B " << B << endl;
    double C = 2 - A + B;
    // cout << "C " << C << endl;
    double E = floor(365.25 * (Y + 4716));
    // cout << "E " << E << endl;
    double F = floor(30.6001 * (M + 1.0));
    // cout << "F " << F << endl;
    return (int) round(C + D + E + F - 1524.5);
}

bool
Date::operator==(const Date& x) const {
  return (_y == x._y && _m == x._m && _d == x._d);
}


bool
Date::operator<(const Date& x) const {
  return (_y < x._y || (_y == x._y &&  (_m < x._m ||  (_m == x._m && _d < x._d))));
}

bool
Date::operator<=(const Date& x) const {
  return (_y < x._y || (_y == x._y &&  (_m < x._m ||  (_m == x._m && _d <= x._d))));
}

bool
Date::operator>(const Date& x) const {
  return (_y > x._y || (_y == x._y &&  (_m > x._m ||  (_m == x._m && _d > x._d))));
}

bool
Date::operator>=(const Date& x) const {
  return (_y > x._y || (_y == x._y &&  (_m > x._m ||  (_m == x._m && _d >= x._d))));
}

int
Date::operator-(const Date& x) const {
  return (int) (julianDay() - x.julianDay());
}

Date&
Date::operator++() {
  ++_d;
  if(30 < _d) {
    _d = 1;
    ++_m;
  }
  return (*this);
}

Date&
Date::operator--() {
  --_d;
  if(0 > _d) {
    _d = 30;
    --_m;
  }
  return (*this);
}


std::ostream&
Date::print(std::ostream& os, char sep) const {
  os << year() << sep
     << (month() < 10 ? "0" : "") << month() << sep
     << (day() < 10 ? "0" : "") << day();
  return os;
}

std::ostream&
Date::printXml(std::ostream& os) const {
  if(10 > year()) {
    os << '0';
  }
  os << year() << '-';
  if(10 > month()) {
    os << '0';
  }
  os << month() << '-';
  if(10 > day()) {
    os << '0';
  }
  os << day();
  return os;
}


Timestamp::Timestamp() : _date(), _time() {}

Timestamp::Timestamp(const Date& d, const Time& t) : _date(d), _time(t) {}

bool
Timestamp::operator==(const Timestamp& x) const {
  return (_date == x._date && _time == x._time);
}

bool
Timestamp::operator<(const Timestamp& x) const {
  return (_date < x._date || (_date == x._date && _time < x._time));
}

bool
Timestamp::operator<=(const Timestamp& x) const {
  return (_date < x._date || (_date == x._date && _time <= x._time));
}

bool
Timestamp::operator>(const Timestamp& x) const {
  return (_date > x._date || (_date == x._date && _time > x._time));
}

bool
Timestamp::operator>=(const Timestamp& x) const {
  return (_date > x._date || (_date == x._date && _time >= x._time));
}

double
Timestamp::operator-(const Timestamp& x) const {
  return  (  ((double) (_date - x._date) * Time::secondsPerDay()) + (_time - x._time) );
}

double
Timestamp::relativeDifference(const Timestamp& aSmaller, const Timestamp& aBigger) const {
return ( ((*this) - aSmaller) / (aBigger - aSmaller) );
}

void
Timestamp::subtractHours(int x) {
  _time.subtractHours(x);
  int d = _time.setTimeWithinOneDayInterval();
  if(0 < d) {
    ++_date;
  } else
  if(0 > d) {
    --_date;
  }
}

void
Timestamp::subtractSeconds(int x) {
  _time.subtractSeconds(x);
  int d = _time.setTimeWithinOneDayInterval();
  if(0 < d) {
    ++_date;
  } else
  if(0 > d) {
    --_date;
  }
}

std::ostream&
Timestamp::print(std::ostream& os) const {
  os << date() << "   " << time();
  return os;
}


