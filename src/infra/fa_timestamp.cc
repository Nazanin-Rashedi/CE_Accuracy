#include "fa_timestamp.hh"

namespace fa {

char fa_timestamp_t::_default_time_separator = '.';
char fa_timestamp_t::_default_date_separator = '-';
char fa_timestamp_t::_default_date_time_separator = ':';

fa_timestamp_t::fa_timestamp_t() : _time(), _date() {}
fa_timestamp_t::fa_timestamp_t(const fa_date_t& aDate, const fa_time_t& aTime)
               : _time(aTime), _date(aDate) {
}


bool
fa_timestamp_t::operator<(const fa_timestamp_t& x) const {
  return (date() < x.date() ||
          (date() == x.date() && time() < x.time()));
}

bool
fa_timestamp_t::operator<=(const fa_timestamp_t& x) const {
  return (date() < x.date() ||
          (date() == x.date() && time() <= x.time()));
}

bool
fa_timestamp_t::operator>(const fa_timestamp_t& x) const {
  return (date() > x.date() ||
          (date() == x.date() && time() > x.time()));
}

bool
fa_timestamp_t::operator>=(const fa_timestamp_t& x) const {
  return (date() > x.date() ||
          (date() == x.date() && time() >= x.time()));
}

bool
fa_timestamp_t::set(const char*& x) {
  return set(x, _default_date_separator, _default_date_time_separator, _default_time_separator);
}

bool
fa_timestamp_t::set(const char*& x,
                    const char aSepDate,
                    const char aSepDateTime,
                    const char aSepTime) {
 if(!(_date.set(x, true, aSepDate))) {
    std::cerr << "ERR I001 fa_timestamp_t::set" << std::endl;
    return false;
  }
  if(aSepDateTime != *x) {
    std::cerr << "ERR I002 fa_timestamp_t::set" << std::endl;
    return false;
  }
  ++x;
  // std::cerr << "I003 STimestamp::set" << std::endl;
  return _time.set(x, aSepTime);
}

void 
fa_timestamp_t::set_to_current_time_local() {
  _time = fa_time_t::time_of_day_local();
  _date = fa_date_t::today_local();

}

void
fa_timestamp_t::set_to_current_time_utc() {
  _time = fa_time_t::time_of_day_utc();
  _date = fa_date_t::today_utc();
}


std::ostream&
fa_timestamp_t::print(std::ostream& os) const {
  return print(os, _default_date_separator, 
                   _default_date_time_separator, 
                   _default_time_separator);

}

std::ostream&
fa_timestamp_t::print(std::ostream& os,
                      const char aSepDate,
                      const char aSepDateTime,
                      const char aSepTime) const {
  date().print(os, aSepDate);
  os << aSepDateTime;
  time().print(os, aSepTime);
  return os;
}




std::ostream& 
operator<<(std::ostream& os, const fa_timestamp_t& x) {
  return x.print(os);
}



} // end namespace
