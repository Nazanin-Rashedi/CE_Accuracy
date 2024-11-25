#ifndef INFRA_LINFUN_HH
#define INFRA_LINFUN_HH

#include <stdlib.h>
#include <cmath>

/*
 *  Given two points (x1, y1) and (x2, y2)
 *  they are approximated by a function
 *    f(x) := a x + b         [class LinFun]
 *    f(x) := a * log(x) + b  [class LogLinFun]
 *  further these functions provide
 *  -  operator()(x) to calculate f(x)
 *  -  root(e) returns a value x0 such that e = f(x0), i.e., it calculates the inverse $f^{-1}(e)$
 *  for example see main_linfun.cc
 */


class LinFun {
  public:
    LinFun() : _a(0), _b(0) {}
    LinFun(const double aA, const double aB) : _a(aA), _b(aB) {}
    LinFun(const double x1, const double y1, const double x2, const double y2) : _a(0), _b(0) {
      init(x1, y1, x2, y2);
    }
  public:
    inline void init(const double x1, const double y1, const double x2, const double y2) {
                  _a = (y1 - y2) / (x1 - x2);
                  _b = y2 - _a * x2;
                }
    inline double operator()(const double x) const {
                    return (_a * x + _b);
                  }
    inline double root(const double e) const {
                     return (e - _b) / _a;
                  }
    inline double a() const { return _a; }
    inline double b() const { return _b; }
  private:
    double _a;
    double _b;
};

class LogLinFun {
  public:
    LogLinFun() : _a(0), _b(0) {}
    LogLinFun(const double aA, const double aB) : _a(aA), _b(aB) {}
    LogLinFun(const double x1, const double y1, const double x2, const double y2) : _a(0), _b(0) {
      init(x1, y1, x2, y2);
    }
  public:
    inline void init(const double x1, const double y1, const double x2, const double y2) {
                  // _a = (std::log(y1) - std::log(y2)) / (std::log(x1) - std::log(x2));
                  // _b = std::log(y2) - _a * std::log(x2);
                  _a = (y1 - y2) / (std::log(x1) - std::log(x2));
                  _b = y2 - _a * std::log(x2);
                }
    inline double operator()(const double x) const {
                    // return std::exp(_a * std::log(x) + _b);
                    return (_a * std::log(x) + _b);
                  }
    inline double root(const double e) const {
                     return std::exp((e - _b) / _a);
                     // return ((e - _b) / _a);
                  }
    inline double a() const { return _a; }
    inline double b() const { return _b; }
  private:
    double _a;
    double _b;
};

#endif
