#ifndef INFRA_LOG_COUNTER_HH
#define INFRA_LOG_COUNTER_HH

#include <random>

/*
 *  log counter incrementer
 *  a counter value of n is represented by the value v := roundedinsomeway(log(1 + n))
 *  n = 0 is represented by 0.
 *  thus, the estimate of the counter is \hat{n} := e^v - 1
 *  use to count many things (e.g. more than 10) as for small n the error can be large
 *  see papers by Morris 1978 and Flajolet 1985
 *  possible improvements:
 *  if number of bits for counter is known, one can produce a table materilizing the lDelta's
 *  this should speed up increment 
 */

class LogCounter {
  public:
    typedef std::mt19937 rng32_t;
  public:
    LogCounter(const double aBase) : _base(aBase), _dist(0.0, 1.0) {}
  public:
    inline double base() const { return _base; }
  public:
    // c >= 0
    // get increment
    inline int get_inc(const int c, rng32_t& aRng);
    // do increment
    inline void do_inc(int& c, rng32_t& aRng) { c += get_inc(c, aRng); }
    // retrieve value from encoding
    inline double val(const int c) const { return (std::pow(base(), c) - 1.0); }
    inline double get_rand(rng32_t& aRng) { return _dist(aRng); }
  private:
    const double _base;
    std::uniform_real_distribution<double>  _dist;
};


int
LogCounter::get_inc(const int c, rng32_t& aRng) {
  if(0 == c) {
    return 1;
  }

  // std::cout << c << ": " << val(c) << " /+1 " << val(c + 1) << std::endl;

  // const double lDelta = (((double) 1.0) / (val(c + 1) - val(c)));                // 117 ns pro increment
  const double lDelta = (((double) 1.0) / (std::pow(base(), c)  * (base() - 1.0))); //  68 ns pro increment

  const double r = get_rand(aRng);
  // std::cout << c << ": " << lDelta << " <> " << r << std::endl;
  if(r < lDelta) {
    return 1;
  }
  return 0;
}


#endif
