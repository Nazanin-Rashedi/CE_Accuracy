#include "CardEstEval.hh"

CardEstEval::CardEstEval() : _perr_min(0), _perr_max(0), _count_lt(0), _count_gt(0) {
}

void
CardEstEval::init() {
  _perr_min = 0;
  _perr_max = 0;
  _count_lt = 0;
  _count_gt = 0;
}

void
CardEstEval::step(const double aEst, const double aTru) {
  if(aEst < aTru) {
    _perr_min = std::min<double>(_perr_min, -(aTru / aEst));
    ++_count_lt;
  } else {
    _perr_max = std::max<double>(_perr_max, (aEst / aTru));
    ++_count_gt;
  } 
}

void
CardEstEval::fin() {
}


void
CardEstEval::step(const CardEstEval& x) {
  _perr_min = std::min<double>(_perr_min, x.perr_min());
  _perr_max = std::max<double>(_perr_max, x.perr_max());
  _count_lt += x.count_lt();
  _count_gt += x.count_gt();
}

std::ostream&
CardEstEval::print(std::ostream& os) const {
  os << perr_min() << ' ' << perr_max() << ' ' << count_lt() << ' ' << count_gt();
  return os;
}


std::ostream& 
operator<<(std::ostream& os, const CardEstEval& x) {
  return x.print(os);
}

