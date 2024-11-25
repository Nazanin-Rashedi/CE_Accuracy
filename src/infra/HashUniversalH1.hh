#ifndef INFRA_HASH_UNIVERSAL_H1_HH
#define INFRA_HASH_UNIVERSAL_H1_HH

#include "infra/glob_infra_standard_includes.hh"
#include <random>

typedef std::mt19937_64 rng64_t;

class HashUniversalH1 {
  public:
    HashUniversalH1(const uint64_t aM, 
                    const uint64_t aN, 
                    const uint64_t aPrime,
                    const uint64_t aB)
                   : _m(aM), _n(aN), _p(aPrime) {
       assert( 0 < _m);
       assert(_m < _p);
       assert(_n < _p);
    }
    HashUniversalH1(const uint64_t aB, rng64_t& aRng)
                   : _m((aRng() % (BigPrime - 1)) + 1),
                     _n(aRng() % BigPrime),
                     _p(BigPrime),
                     _b(aB) {
    }
    HashUniversalH1() : _m(0), _n(0), _p(0), _b(0) {}
  public:
    void init(const uint64_t aB, rng64_t& aRng) {
           _m = (aRng() % (BigPrime - 1)) + 1;
           _n = aRng() % BigPrime;
           _p = BigPrime;
           _b = aB;
         }
  public:
    // A = {0,\ldots,a-1}
    // B = {0,\ldots,b-1}
    // p \geq a
    // g(z) = z mod b
    // h_{m,n}(x) = (mx + n) mod p
    // f_{m,n}(x) = g(h_{m,n}(x)) // the hash function
    inline uint64_t hash(const uint64_t x) const {
                      return ((_m * x + _n) % _p) % _b;
                    }
  public:
    inline uint64_t m() const { return _m; }
    inline uint64_t n() const { return _n; }
    inline uint64_t p() const { return _p; }
    inline uint64_t b() const { return _b; }
  private:
    uint64_t _m;
    uint64_t _n;
    uint64_t _p;
    uint64_t _b;
  public:
    static constexpr uint64_t BigPrime = 4294967311ULL; // > max uint32
};



#endif
