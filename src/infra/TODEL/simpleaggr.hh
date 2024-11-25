#ifndef AGGREGATE_SIMPLE_HH
#define AGGREGATE_SIMPLE_HH

#include <iostream>
#include <iomanip>
#include <limits>
#include <math.h>

template<class Num>
class SimpleAggr {
  public:
    typedef unsigned int uint;
    enum kind_t {
      count_e = 1,
      sum_e   = 2,
      avg_e   = 4,
    };
  public:
    SimpleAggr() : _sum(0), _count(0) {}
    ~SimpleAggr() {}
  public:
    inline void init() { _sum = (Num) 0; _count = 0; };
    inline void init(Num x) { _sum = x; _count = 1; };
    inline void step(Num x) { _sum += x; ++_count; }
    inline void negstep(Num x) { _sum -= x; --_count; }
    inline void fin() {}
  public:
     inline uint   count() const { return _count; }
     inline Num    sum() const { return _sum; }
     inline double avg() const { return ((double) sum() / (double) count()); }
  public:
    std::ostream& print(std::ostream& os, int aKindSet, int aFieldWidth = 8) const {
      if(0 != (aKindSet & count_e)) os << std::setw(4) << count() << ' ';
      if(0 != (aKindSet & sum_e))   os << std::setw(aFieldWidth) << sum() << ' ';
      if(0 != (aKindSet & avg_e))   os << std::setw(aFieldWidth) << avg() << ' ';
      return os;
    }
  private:
    Num  _sum;
    uint _count;
};

template<class Num>
std::ostream& 
operator<<(std::ostream& os, const SimpleAggr<Num>& aAggr) {
  os << "min: " << aAggr.min() << ", ";
  os << "max: " << aAggr.max() << ", ";
  os << "avg: " << aAggr.avg();
  return os;
}


#endif

