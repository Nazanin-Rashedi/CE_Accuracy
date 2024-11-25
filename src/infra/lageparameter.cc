#include "lageparameter.hh"


Lageparameter::Lageparameter() : _isSorted(false), _v() {
  init();
}
Lageparameter::Lageparameter(const std::vector<double>& x) : _isSorted(false), _v(x) {
  fin();
}

Lageparameter::~Lageparameter() {}

void
Lageparameter::init() {
}

void
Lageparameter::step(double x) {
  _v.push_back(x);
}

void
Lageparameter::fin() {
  std::sort<double_vt::iterator>(_v.begin(), _v.end());
  _isSorted = true;
}

double
Lageparameter::mean() const {
  double lSum = 0.0;
  for(int i = 0; i < size(); ++i) {
    lSum += v()[i];
  }
  return (lSum/n());
}

double
Lageparameter::median() const {
  if(0 == size()) {
    return (double) 1.0 / 0.0;
  }
  if(1 == (size() % 2)) {
    return v()[(size() - 1) / 2];
  } else {
    return (v()[size() / 2] + v()[(size() / 2) - 1]) / 2.0;
  }
}

double
Lageparameter::middle() const {
  return ((min() + max()) / 2.0);
}

double
Lageparameter::geomiddle() const {
  return sqrt(max() * min());
}

double
Lageparameter::Error1(double x) const {
  double lSum = 0.0;
  for(int i = 0; i < size(); ++i) {
    lSum += fabs(v()[i] - x);
  }
  return lSum;
}

double
Lageparameter::Error2(double x) const {
  double lSum = 0.0;
  for(int i = 0; i < size(); ++i) {
    lSum += ((v()[i] - x) * (v()[i] - x));
  }
  return sqrt(lSum);
}

double
Lageparameter::ErrorI(double x) const {
  double lRes = std::numeric_limits<double>::min();
  for(int i = 0; i < size(); ++i) {
    lRes = std::max<double>(lRes, fabs(v()[i] - x));
  }
  return lRes;
}

double
Lageparameter::ErrorQ(double x) const {
  double lRes = std::numeric_limits<double>::min();
  for(int i = 0; i < size(); ++i) {
    lRes = std::max<double>(lRes, q(v()[i], x));
  }
  return lRes;
}

double
Lageparameter::spread() const {
  return ((max() - min()) / 2.0);
}

double
Lageparameter::geospread() const {
  return sqrt(max()/min());
}

void
Lageparameter::plot(std::ostream& os, int aNoFun, mfun_t aFun[], double aStep) const {
  for(double x = min(); x <= max(); x += aStep) {
    os << std::setw(5) << x << ' ';
    for(int i = 0; i < aNoFun; ++i) {
      os << std::setw(5) << (this->*(aFun[i]))(x) << ' ';
    }
    os << std::endl;
  }
}

double
Lageparameter::q(double x, double y) {
  return std::max<double>(x/y, y/x);
}



