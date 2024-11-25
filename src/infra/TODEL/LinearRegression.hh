#ifndef INFRA_LINEAR_REGRESSION_HH
#define INFRA_LINEAR_REGRESSION_HH

#include <limits>

/*
 *  Lineare Regression 
 *  mittels alpha x + beta
 *  durch Menge von Punkte x_i, y_i
 *  einzeln mit step eingegeben
 */

template<typename Tfloat>
class LinearRegression {
  public:
    typedef unsigned int uint;
    typedef Tfloat       float_t;
  public:
    LinearRegression();
  public:
    void init();
    void step(const float_t x, const float_t y);
    void fin();
  public:
    inline float_t alpha() const { return _alpha; }
    inline float_t beta()  const { return _beta;  }
    inline bool senkrecht() const { return _senkrecht; } // if all x_i are equal
    inline uint count() const { return _count; }
  private:
    uint   _count;
    float_t _sum_x;
    float_t _sum_y;
    float_t _sum_xx;
    float_t _sum_xy;
    float_t _alpha;
    float_t _beta;
    bool    _senkrecht;
};

template<typename Tfloat>
LinearRegression<Tfloat>::LinearRegression() : _count(0), 
                                               _sum_x(0), _sum_y(0), _sum_xx(0), _sum_xy(0),
                                               _alpha(0), _beta(0), _senkrecht(false) {
}

template<typename Tfloat>
void
LinearRegression<Tfloat>::init() {
  _count  = 0;
  _sum_x  = 0;
  _sum_y  = 0;
  _sum_xx = 0;
  _sum_xy = 0;
  _alpha  = 0;
  _beta   = 0;
}


template<typename Tfloat>
void
LinearRegression<Tfloat>::step(const float_t x, const float_t y) {
  ++_count;
  _sum_x += x;
  _sum_y += y;
  _sum_xx += x * x;
  _sum_xy += x * y;
}

template<typename Tfloat>
void
LinearRegression<Tfloat>::fin() {
  const float_t n = (float_t) _count;
  const float_t lAvgX = (_sum_x / n);
  const float_t lAvgY = (_sum_y / n);

  const float_t lNenner = _sum_xx - (n * lAvgX * lAvgX);
  if(0 == lNenner) {
    _alpha = std::numeric_limits<double>::max();
    _senkrecht = true;
  } else {
    _alpha = (_sum_xy - n * lAvgX * lAvgY) / lNenner;
    _beta = lAvgY - alpha() * lAvgX;
    _senkrecht = false;
  }
}



#endif
