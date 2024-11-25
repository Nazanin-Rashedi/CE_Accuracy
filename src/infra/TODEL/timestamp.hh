#ifndef TIMESTAMP_HH
#define TIMESTAMP_HH

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <cstdint> //Nazanin

class Time {
  public:
    Time();
    Time(double);
    Time(double, bool aFormated); // hhmmss.xx
    Time(int); // hhmmss
    Time(int,int,int,double=0);
    Time(const std::string&, char sep = ':');
  public:
    inline double time() const { return _time; }
    inline int asInt() const { return (lrint(trunc(_time))); }
    inline int hour() const { return (asInt() / 3600); }
    inline int minute() const { return ((asInt() % 3600) / 60); }
    inline int second() const { return (asInt() % 60); } 
    inline double secfrac() const { return (_time - floor(_time)); }
    inline double secAndFrac() const { return (double) second() + secfrac(); }
  public:
    bool set(const char*& s /* in/out */, char sep = ':');
    int  setTimeWithinOneDayInterval(); // returns -1,0,1 for days cut out
    void addOneDay();
    void addSeconds(int);
    void addHours(int);
    void subtractOneDay();
    void subtractSeconds(int);
    void subtractHours(int);
  public:
    inline bool operator==(const Time& x) const { return (_time == x._time); }
    inline bool operator<(const Time& x) const { return (_time < x._time); }
    inline bool operator<=(const Time& x) const { return (_time <= x._time); }
    inline bool operator>(const Time& x) const { return (_time > x._time); }
    inline bool operator>=(const Time& x) const { return (_time >= x._time); }
    double operator-(const Time&) const; // returns diff in no days 
  public:
    // inline unsigned int hash2() const { 
    //          return ((*((const unsigned int*) (void*) &_time)) ^ (*(((const unsigned int*) (void*) &_time) + 1))); 
    //        }
    inline unsigned int hash() const {
             const uint64_t* x = (uint64_t*) &_time;
             return (unsigned int) ((*x) ^ (*x >> 32));
           }
  public:
    std::ostream& print(std::ostream&, char sep = ':') const;
    std::ostream& printXml(std::ostream&) const;
  private:
    void set(int);
    void set(int,int,int,double=0);
  private:
    double _time;  // in seconds (since midnight)
  private:
    static const double _secondsPerDay;
  public:
    static inline double secondsPerDay() { return _secondsPerDay; }
};




class Date {
  public:
    Date();
    Date(int); // ddmmyy
    Date(int,int,int);
    Date(const std::string&, bool aYearHigh, char sep = ':');
  public:
    inline int year() const { return _y; }
    inline int month() const { return _m; }
    inline int day() const { return _d; }
    Date& set(int y, int m, int d);
    bool  set(const char*& s /* in/out */, bool aYearHigh, char sep = ':');
  public:
    int julianDay() const;
  public:
    bool operator==(const Date&) const;
    bool operator<(const Date&) const;
    bool operator<=(const Date&) const;
    bool operator>(const Date&) const;
    bool operator>=(const Date&) const;
    int operator-(const Date&) const; // returns diff in no days 
    Date& operator++(); // add one day
    Date& operator--(); // subtract one day
  public:
    std::ostream& print(std::ostream&, char sep = ':') const;
    std::ostream& printXml(std::ostream&) const;
  private:
    int _y;
    int _m;
    int _d;
};

class Timestamp {
  public:
    Timestamp();
    Timestamp(const Date&, const Time&);
  public:
    inline const Date& date() const { return _date; }
    inline void  date(const Date& x) { _date = x; }
    inline const Time& time() const { return _time; }
    inline void  time(const Time& x) { _time = x; }
  public:
    inline Date& date() { return _date; }
    inline Time& time() { return _time; }
  public:
    bool operator==(const Timestamp&) const;
    bool operator<(const Timestamp&) const;
    bool operator<=(const Timestamp&) const;
    bool operator>(const Timestamp&) const;
    bool operator>=(const Timestamp&) const;
    double operator-(const Timestamp&) const; // returns diff in seconds.fracsec
    double relativeDifference(const Timestamp& aSmaller, const Timestamp& aBigger) const; // return in [0,1]
  public:
    void subtractHours(int);
    void subtractSeconds(int);
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    Date _date;
    Time _time;
};


inline std::ostream&
operator<<(std::ostream& os, const Time& x) {
  return x.print(os);
}

inline std::ostream&
operator<<(std::ostream& os, const Date& x) {
  return x.print(os);
}

inline std::ostream&
operator<<(std::ostream& os, const Timestamp& x) {
  return x.print(os);
}

#endif // TIME_STAMP_HH

