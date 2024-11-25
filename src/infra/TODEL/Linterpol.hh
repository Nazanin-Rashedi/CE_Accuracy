#ifndef GLOB_INFRA_LINTERPOL_HH
#define GLOB_INFRA_LINTERPOL_HH

#include "infra/glob_infra_standard_includes.hh"


/*
 * class Linterpol
 * interpolates between a given set of points (x,y) linearily
 * while reading, it can skip points if non-increasing
 */

class Linterpol {
  public:
    Linterpol();
  public:
    inline uint   no_points() const { return _x.size(); }
    inline uint   last_idx() const { return (_x.size() - 1); }
           uint   get_idx(const double x) const;
    inline double get_x(const uint i) const { return _x[i]; }
    inline double get_y(const uint i) const { return _y[i]; }
  public:
    double operator()(const double x) const;
  public:
    int read(std::istream& aIs, const bool aSkipNonMonInc); // returns number of skipped elements
  private:
    double_vt _x;
    double_vt _y;
};

#endif
