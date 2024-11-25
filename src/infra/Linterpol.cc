#include "Linterpol.hh"
#include "infra/linfun.hh"

Linterpol::Linterpol() : _x(), _y() {}

double
Linterpol::operator()(const double x) const {
  if(get_x(0) > x) {
    return get_y(0);
  }
  if(get_x(last_idx()) <= x) {
    return get_y(last_idx());
  }
  const uint i = get_idx(x);
  // interpolate using linfun
  LinFun lLinFun(get_x(i), get_y(i), get_x(i+1), get_y(i+1));
  return lLinFun(x);
}

/* 
 * given x
 * return i such that _x[i] <= x <= _x[i+1]
 * prerequisite: _x[0] <= x <= _x[last_idx()]
 */

uint
Linterpol::get_idx(const double x) const {
  uint lb = 0;
  uint ub = no_points();
  uint  m = 0;
  while(1 < (ub - lb)) {
    m = (lb + ub) / 2;
    // std::cout << "lb/m/ub " << lb << '/' << m << '/' << ub << std::endl;
    // std::cout << "  x[lb/m/ub] " << x << " " 
    //           << get_x(lb) << '/' << get_x(m) << '/' << get_x(ub) 
    //           << "  x[m-1,m,m+1] " << get_x(m - 1) << '/' << get_x(m) << '/' << get_x(m + 1) 
    //           << std::endl;
    if(x < get_x(m)) {
      ub = m;
    } else {
      // (get_x(m) <= x)
      if(x <= get_x(m + 1)) {
        break;
      }
      lb = m + 1;
    }
  }
  // std::cout << "fin: lb/m/ub: " << lb << '/' << m << '/' << ub << std::endl;
  if(1 >= (ub - lb)) {
    if((get_x(lb) <= x) && (x <= get_x(lb+1))) {
      m = lb;
    } else 
    if((get_x(ub) <= x) && (x <= get_x(ub+1))) {
      m = ub;
    }
  } 
  assert((get_x(m) <= x) && (x <= get_x(m+1)));
  return m;
}

int
Linterpol::read(std::istream& aIs, const bool aSkipNonMonInc) {
  _x.clear();
  _y.clear();
  double x;
  double y;
  int lCount = 0;
  int lNoSkip = 0;
  while(!aIs.eof()) {
     aIs >> x >> y;
     if(aIs.eof()) { 
       break;
     }
     ++lCount;
     if(aSkipNonMonInc && (0 < _x.size()) && 
        ((x < get_x(last_idx())) || (y < get_y(last_idx())))) {
       ++lNoSkip;
       std::cout << "Warning: " << lCount 
                 << ": skipped: x = " << x << ", y = " << y
                 << "; due to x = " << get_x(last_idx()) << ", y = " << get_y(last_idx())
                 << std::endl;
       continue;
     }
     _x.push_back(x);
     _y.push_back(y);
  }
  return lNoSkip;
}


