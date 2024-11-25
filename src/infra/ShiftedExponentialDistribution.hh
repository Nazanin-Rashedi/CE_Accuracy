#ifndef SHIFTED_EXPONENTIAL_DISTRIBUTION_HH
#define SHIFTED_EXPONENTIAL_DISTRIBUTION_HH

#include <inttypes.h>
#include <random>
#include <cmath>

/*
 *  class ShiftedExponentialDistribution
 *  produces random unsigned integers
 *  in the range [0,aMod[
 *  using std::exponential_distribution
 *  converting the result to an unsigned integer
 *  and shifting by some amount aShift
 */

template<typename Trng>
class ShiftedExponentialDistribution {
  public:
    typedef Trng rng_t;
    typedef std::exponential_distribution<double> dist_t;
  public:
    ShiftedExponentialDistribution() : _shift(0), _mod(1), _dist(1) {}
    ShiftedExponentialDistribution(const uint aShift, const uint aMod, const double aLambda)
      : _shift(aShift), _mod(aMod), _dist(aLambda) {}
  public:
    void init(const uint aShift, const uint aMod, const double aLambda) {
            _shift = aShift;
            _mod   = aMod;
            _dist.param(dist_t::param_type(aLambda));
         }
  public:
    inline uint32_t shift() const { return _shift; }
    inline uint32_t mod() const { return _mod; }
    inline double   lambda() const { return _dist.lambda(); }
  public:
    uint32_t operator()(rng_t& aRng) { return (((uint32_t) std::floor((_dist(aRng) * mod()) + shift())) % mod()); }
  private:
    uint32_t _shift;
    uint32_t _mod;
    dist_t   _dist;
};

#endif
