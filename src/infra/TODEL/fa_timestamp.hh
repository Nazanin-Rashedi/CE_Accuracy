#ifndef INFRA_FA_TIMSTAMP_HH
#define INFRA_FA_TIMSTAMP_HH
#pragma once

#include "fa_date.hh"
#include "fa_time.hh"


namespace fa {

class fa_timestamp_t {
  public:
    fa_timestamp_t();
    fa_timestamp_t(const fa_date_t&, const fa_time_t&);
  public:
    inline const fa_date_t& date() const { return _date; }
    inline const fa_time_t& time() const { return _time; }
  public:
    inline bool operator==(const fa_timestamp_t& x) const {
                   return ((date() == x.date()) && (time() == x.time()));
                 }
    bool operator<(const fa_timestamp_t&) const;
    bool operator<=(const fa_timestamp_t&) const;
    bool operator>(const fa_timestamp_t&) const;
    bool operator>=(const fa_timestamp_t&) const;
  public:
    bool set(const char*& x);
    bool set(const char*& x,
             const char aSepDate,
             const char aSepDateTime,
             const char aSepTime);
    bool simple_set(const char* x,
                    const char aSepDate,
                    const char aSepDateTime,
                    const char aSepTime);
  public:
    void set_to_current_time_utc();   // sets to current time in UTC
    void set_to_current_time_local(); // sets to current in local time
  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& print(std::ostream& os, 
                        const char aSepDate,
                        const char aSepDateTime,
                        const char aSepTime) const;
  private:
    fa_time_t _time;
    fa_date_t _date;
  public:
    static char _default_time_separator;
    static char _default_date_separator;
    static char _default_date_time_separator;

};
std::ostream& operator<<(std::ostream& os, const fa_timestamp_t&);

} // end namespace

#endif
