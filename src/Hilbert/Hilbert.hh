#ifndef INFRA_HILBERT_HH
#define INFRA_HILBERT_HH

#include <algorithm>

/*
 * Hilbert Curve
 *   maps coordinates (x,y) to distance (on curve) d
 *   and d -> (xy)
 */

class Hilbert {
  public:
    Hilbert(const int n);
  public:
    inline int get_n() const { return _n; }
  public:
    inline int  xy2d(const int x, const int y) const;
    inline void d2xy(const int d, int& x, int& y) const;
  private:
    static        int  xy2d(const int n, int x, int y);
    static        void d2xy(const int n, const int d, int& x, int& y);
    static inline void rot(const int n, const int rx, const int ry, int& x, int& y);
  private:
    int _n;
};




int
Hilbert::xy2d(const int x, const int y) const {
  return xy2d(get_n(), x, y);
}


void
Hilbert::d2xy(const int d, int& x, int &y) const {
  return d2xy(get_n(), d, x, y);
}

void
Hilbert::rot(const int n,  const int rx, const int ry, int& x, int& y) {
  if(0 == ry) {
    if(1 == rx) {
      x = n - 1 - x;
      y = n - 1 - y;
    }
    std::swap<int>(x,y);
  }
}

#endif

