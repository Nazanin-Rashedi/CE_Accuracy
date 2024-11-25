#ifndef VARIANCE_HH
#define VARIANCE_HH

#include <iostream>
#include <iomanip>
#include <limits>
#include <math.h>

template<class Tdom>
class Variance {
  public:
    typedef unsigned int uint;
  public:
    Variance() : _sum((Tdom) 0), _sumsq((Tdom) 0), _count(0) {}
    ~Variance() {}
  public:
    inline
    void init() { _sum   = (Tdom) 0;
                  _sumsq = (Tdom) 0;
                  _count = 0;
                };
    inline
    void step(Tdom x) {
      _sum += x;
      _sumsq += (x*x);
      ++_count;
    }
    inline
    void step(Tdom aVal, uint aFreq) {
      _sum   += (aVal * aFreq);
      _sumsq += aFreq * (aVal * aVal);
      _count += aFreq;
    }
    inline
    void reverseStep(Tdom x) {
      _sum   -= x;
      _sumsq -= (x*x);
      --_count;
    }

    inline void fin() {}
  public:
     inline uint   count() const { return _count; }
     inline double avg() const { return ((double) sum() / (double) count()); }
     inline double mean() const { return avg(); }
     inline Tdom   sum() const { return _sum; }
     inline Tdom   sumsq() const { return _sumsq; }
     double variance() const;
     double sse() const;
     double standardDeviation() const;
  public:
    std::ostream& print(std::ostream& os, int aKindSet, int aFieldWidth = 8) const {
      os << std::setw(aFieldWidth) << count() << ' ' 
         << std::setw(aFieldWidth) << sum() << ' ' 
         << std::setw(aFieldWidth) << sumsq() << ' ' 
         << std::setw(aFieldWidth) << variance() << ' '
         << std::endl;
      return os;
    }
  private:
    Tdom _sum;
    Tdom _sumsq; // sum squared
    uint _count;
  private:
    static int _kindset;
  public:
    static int  setToPrint() { return _kindset; }
    static void setToPrint(int aKindset) { _kindset = aKindset; }
};

template<class Tdom>
double
Variance<Tdom>::variance() const {
  const double x = ((double) 1.0 / (double) count());
  return ( ( x * (double) sumsq()) - ((x * sum()) * (x * sum())) );
}

template<class Tdom>
double
Variance<Tdom>::sse() const {
  return ( ((double) sumsq()) - (sum() * sum()) );
}

template<class Tdom>
double
Variance<Tdom>::standardDeviation() const {
  return sqrt(variance());
}

template<class Tdom>
std::ostream& 
operator<<(std::ostream& os, const Variance<Tdom>& aAggr) {
  if(0 == Variance<Tdom>::setToPrint()) {
    os << "min: " << aAggr.min() << ", ";
    os << "max: " << aAggr.max() << ", ";
    os << "avg: " << aAggr.avg() << ", ";
    os << "qmid: " << aAggr.qmiddle();
  } else {
    aAggr.print(os, Variance<Tdom>::setToPrint());
  }
  return os;
}


#endif

