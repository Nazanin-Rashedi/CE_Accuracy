#ifndef LAGEPARAMETER_HH
#define LAGEPARAMETER_HH

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

class Lageparameter {
  public:
   typedef double (Lageparameter::*mfun_t)(double) const; 
   typedef std::vector<double> double_vt;
  public:
    Lageparameter();
    Lageparameter(const double_vt& x);
    ~Lageparameter();
  public:
    void init();
    void step(double);
    void fin();
  public:
    inline int    size() const { return _v.size(); }
    inline uint   count() const { return _v.size(); }
    inline double n() const { return (double) _v.size(); }
    inline double min() const { return v()[0]; } // sorted!
    inline double max() const { return v()[size() - 1]; } // sorted!
  public:
    double mean() const;
    double median() const;
    double middle() const;
    double geomiddle() const; // geometric middle
  public:
    double Error1(double aEstimate) const; // sum |x_i - x_estimate|
    double Error2(double aEstimate) const; // sum (x_i - x_estimate)^2
    double ErrorI(double aEstimate) const; // max |x_i - x_estimate|
    double ErrorQ(double aEstimate) const; // q-error
  public:
    double spread() const;    // for ErrorI
    double geospread() const; // for ErrorQ
  public:
    inline Lageparameter& operator<<(double x) { step(x); return (*this); }
    void plot(std::ostream&, int aNoFun, mfun_t aFun[], double aStep) const;
  private:
   static double q(double, double);
   inline const double_vt& v() const { return _v; }
  private:
    bool      _isSorted;
    double_vt _v;
};
/*
Lageparameter&
operator<<(Lageparameter& aLp, double x) {
  aLp.step(x);
  return aLp;
*/


#endif
