#include "chisquare.hh"


ChiSquare::ChiSquare(const uint aN, const uint aM) : _matrix(aN, aM), 
                                                     _margin_x(aN), _margin_y(aM),
                                                     _min_x(0), _max_x(0),
                                                     _min_y(0), _max_y(0),
                                                     _delta_x(0), _delta_y(0),
                                                     _count(0) {
}


void
ChiSquare::init(const double aXmin, const double aXmax, const double aYmin, const double aYmax) {
  _min_x = aXmin;
  _max_x = aXmax;
  _min_y = aYmin;
  _max_y = aYmax;
  assert(_min_x < _max_x);
  assert(_min_y < _max_y);
  _delta_x = (_max_x - _min_x) / get_n();
  _delta_y = (_max_y - _min_y) / get_m();

  for(uint i = 0; i < get_n(); ++i) {
    for(uint j = 0; j < get_m(); ++j) {
      _matrix(i,j) = 0;
    }
  }

  _count = 0;

  std::cout << "x-min = " << aXmin << std::endl;
  std::cout << "x-max = " << aXmax << std::endl;
  std::cout << "x-dta = " << _delta_x << std::endl;
  std::cout << "y-min = " << aYmin << std::endl;
  std::cout << "y-max = " << aYmax << std::endl;
  std::cout << "y-dta = " << _delta_y << std::endl;
}

void
ChiSquare::step(const double x, const double y) {

  // std::cout << "ChiSquare::step (" << _count << "):" << std::endl;
  // std::cout << "  val x: " << _min_x << ' ' << x << ' ' << _max_x << std::endl;
  // std::cout << "  val y: " << _min_y << ' ' << y << ' ' << _max_y << std::endl;
  // std::cout << "  idx x: " << idx_x(x) << std::endl;
  // std::cout << "  idx y: " << idx_y(y) << std::endl;

  _matrix(idx_x(x), idx_y(y)) += 1;
  ++_count;
}

void
ChiSquare::fin() {
  const uint n = get_n();
  const uint m = get_m();
  double lSum = 0;

  for(uint i = 0; i < n; ++i) {
    lSum = 0;
    for(uint j = 0; j < m; ++j) {
      lSum += _matrix(i,j);
    }
    _margin_x[i] = lSum;
  }

  for(uint j = 0; j < m; ++j) {
    lSum = 0;
    for(uint i = 0; i < n; ++i) {
      lSum += _matrix(i,j);
    }
    _margin_y[j] = lSum;
  }
}


double
ChiSquare::get_chi_square() const {
  const uint n = get_n();
  const uint m = get_m();
  double lRes = 0;
  double nstar = 0;
  double z = 0;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < m; ++j) {
      nstar = (_margin_x[i] * _margin_y[j]) / (double) _count;
      z = (_matrix(i,j) - nstar);
      lRes += ((z*z) / (nstar));
    }
  }
  return lRes;
}

double
ChiSquare::get_max_qerror() const {
  const uint n = get_n();
  const uint m = get_m();
  double lRes = 0;
  double x = 0; // independent estimate
  double y = 0;
  for(uint i = 0; i < n; ++i) {
    for(uint j = 0; j < m; ++j) {
      x = (_margin_x[i] * _margin_y[j]) / (double) _count;
      y = _matrix(i,j);
      lRes = std::max<double>(lRes, q::qerror(std::max<double>(1, x) , std::max<double>(1, y)));
    }
  }
  return lRes;
}
