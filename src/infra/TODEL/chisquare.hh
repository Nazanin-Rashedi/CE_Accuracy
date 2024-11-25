#ifndef INFRA_CHI_SQUARE_HH
#define INFRA_CHI_SQUARE_HH

#include "infra/glob_infra_standard_includes.hh"
#include <cmath>
#include "infra/matrix.hh"
#include "infra/vector_ops.hh"

/*
 * class ChiSquere
 * divide input one into n equidistant parts
 * divide input two into m equidistant parts
 * builds nxm matrix counting occurrences or 2-dim points in class
 * return chisquare = sum_i sum_J (n_{i,j} - n^{*}_{i,j})^2 / n^{*}_{i,j}
 * where n_{i,j} = |{(x,y)| x in class i and y in class j}|
 * and   n^{*}_{i,j} = n_i * n_j / n
 * and   n_i/j = |{(x,y) | x/y in class i/j}|
 * all calculations performed via doubles
 */


class ChiSquare {
  public:
    ChiSquare(const uint aN, const uint aM);
  public:
    void init(const double aXmin, const double aXmax, const double aYmin, const double aYmax);
    void step(const double x, const double y);
    void fin();
    double get_chi_square() const;
    double get_max_qerror() const;
  public:
    inline uint get_n() const { return _matrix.noRows(); }
    inline uint get_m() const { return _matrix.noCols(); }
    inline const Matrix& matrix() const { return _matrix; }
    inline const double_vt&  margin_x() const { return _margin_x; }
    inline const double_vt&  margin_y() const { return _margin_y; }
  public:
    inline uint idx_x(const double x) const { return idx(x, _min_x, _delta_x, get_n()); }
    inline uint idx_y(const double y) const { return idx(y, _min_y, _delta_y, get_m()); }
  public:
    static inline uint idx(const double v, const double aMin, const double aDelta, const uint aMaxIdx);
  public:
    Matrix _matrix;
    double_vt _margin_x;
    double_vt _margin_y;
    double _min_x;
    double _max_x;
    double _min_y;
    double _max_y;
    double _delta_x;
    double _delta_y;
    uint   _count;
};

uint
ChiSquare::idx(const double v, const double aMin, const double aDelta, const uint aMaxIdx) {
  assert(v >= aMin);
  uint lRes = (uint) std::floor((v - aMin) / aDelta);
  if(aMaxIdx <= lRes) {
     // std::cout << "ChiSquare::idx: maxidx = " << aMaxIdx << " <= " << lRes << " = lRes " << std::endl;
     lRes = aMaxIdx - 1;
  }
  return lRes;
}




#endif
