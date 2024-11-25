#ifndef TEMPLATE_SPREAD_HH
#define  TEMPLATE_SPREAD_HH

#include <iostream>
#include <iomanip>
#include "aggregate.hh"


/*
 * spread calculations,
 * input: sorted by (increasing) values
 */

template <class T>
class Spread {
  public:
    Spread() : _last(), _aggr() {}
  public:
    void init(T* x) { _last = (*x); _aggr.init(); }
    void step(T* x) { _aggr.step((*x) - _last); _last = (*x); }
    void fin() { _aggr.fin(); }
  public:
    inline T min() const { return _aggr.min(); }
    inline T max() const { return _aggr.max(); }
    inline double avg() const { return _aggr.avg(); }
    inline double middle() const { return _aggr.middle(); }
    inline double qmiddle() const { return _aggr.qmiddle(); }
  public:
    std::ostream& print(std::ostream& os, int w) const {
                    os << std::setw(w) << _aggr.min() << ' '
                       << std::setw(w) << _aggr.max() << ' '
                       << std::setw(8) << _aggr.avg() << ' '
                       << std::setw(8) << _aggr.qmiddle() << ' '
                       << std::setw(8) << _aggr.middle() << ' ';
                    return os;
                  }
  private:
    T            _last;
    Aggregate<T> _aggr;
};

#endif
