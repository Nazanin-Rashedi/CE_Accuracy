#ifndef SAMPLING_ALPHA_OMEGA_BINARY_SEARCH_HH
#define SAMPLING_ALPHA_OMEGA_BINARY_SEARCH_HH

#include "infra/glob_infra_standard_includes.hh"
#include "infra/tmath.hh"

//   R: relation
//   S: sample
//   n: |R| relation cardinality
//   m: |S| sample cardinality
//   k: |\select_p(S)| number of qualifying sample tuples
//   l: |\select_p(R)| number of qualifying relation tuples (output)
// most important stuff:
//   alpha is lower bound for l with SamProb(n,m,k,l) >= eps
//   omega is upper bound for l with SamProb(n,m,k,l) >= eps
// also important: epsilon is fixed and hardcoded:
//     eps = 10^-5
// alpha(n,m,k): min{l|\Prob(n,m,l,k) \geq eps} 
//                (lower bound on number of qualifying tuples in relation)
// omega(n,m,k): max{l|\Prob(n,m,l,k) \geq eps}
//                (upper bound of number of qualifying tuples in relation)
//          mue: q-middle(alpha, omega) is the best estimate possible
//          rho: q-error(q-middle, alpha/omega)
//                (worst case error (with probability eps))
//  zeta_q(n,m): min{k| \rho(n, m, k) \leq q}
// lambda_q(n,m): \mu(n,m,\zeta_q(n,m))/n
// psi(\theta,q): min{m|\forall k \rho(n,m,k) \leq q \vee \omega(n,m,k) \leq \theta}
// all these numbers are calculated with a given probability eps = 10^{-5}.
// intuitive meaning:
//  alpha: minimum number of qualifying relation tuples (with probability eps)
//  omega: maximum number of qualifying relation tuples (with probability eps)
//    mue: q-middle of min/max number of qualifying relation tuple
//    rho: worst case q-error of q-middle 
//   zeta: minimum number of qualifying sample tuples such that 
//         a q-error of at most q is guaranteed with probability eps
//         it additionally fills an optional zeta_info_t struct with information
//         useful to calculate lambda
// lambda: minimum selectivity such that a q-error of at most q is guaranteed
//    psi: minimum sample size such that the sample becomes theta,q-acceptable


class SamplingAlphaOmegaBisection {
  public:
    // struct collecting all greek letters
    struct zeta_info_t {
      double _zeta;
      double _alpha_at_zeta;
      double _omega_at_zeta;
      double _rho_at_zeta;
      inline double mue() const { return std::sqrt(_alpha_at_zeta * _omega_at_zeta); }
    };
  public:
    SamplingAlphaOmegaBisection(const uint64_t n, const uint64_t m, const bool aTrace = false);
  public:
    SamplingAlphaOmegaBisection();
    void init(const uint64_t n, const uint64_t m, const bool aTrace = false);
  public:
    uint64_t alpha(const uint64_t aK) const;
    uint64_t omega(const uint64_t aK) const;
    double   mue(const double aAlpha, const double aOmega) const;
    double   rho(const double aAlpha, const double aOmega) const;
    uint64_t zeta(const double aMaxQError, zeta_info_t* aZetaInfoOut = 0) const; // expensive!
    double   lambda(const zeta_info_t& aZetaInfo) const;
    static uint64_t psi(const double n, const double aQError, const double aTheta);
    double   eta(const double aMaxQError) const;
  public:
    inline double samprob(const double l) const {
                    return samprob_b(_n, _m, _k, l);
                  }
           double expected_value() const;
    inline bool trace() const { return _trace; }
  public:
    static inline double eps() { return _eps; }
    static inline void   eps(const double aEps) { _eps = aEps; } // do not use this one
    static double samprob_a(const double n, const double m, const double k, const double l);
    static double samprob_b(const double n, const double m, const double k, const double l);
    // true if for all k the estimate mue(alpha(k), omega(k)) is theta-q-acceptable
    static bool is_theta_q_acceptable(const double n, const double m,
                                      const double aMaxQError, const double aTheta);
  private:
    uint64_t _n;
    uint64_t _m;
    mutable uint64_t _k;
    bool     _trace;
  private:
    static double _eps;
};


#endif
