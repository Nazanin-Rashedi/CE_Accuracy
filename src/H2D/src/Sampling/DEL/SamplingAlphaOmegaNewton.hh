#ifndef OPT_CARD_SAMPLING_ALPHA_OMEGA_NEWTON_HH
#define OPT_CARD_SAMPLING_ALPHA_OMEGA_NEWTON_HH

#include "../infra/glob_infra_standard_includes.hh"

class SamplingAlphaOmegaNewton {
  public:
    SamplingAlphaOmegaNewton(const uint64_t aN, const uint64_t aM, const double aEps = 0.00001);
  public:
    inline double n() const { return _n; }
    inline double m() const { return _m; }
    inline double k() const { return _k; }
    inline double eps() const { return _eps; }
    inline double log_eps() const { return _log_eps; }
  public:
    inline double log_fac_n() const { return _log_fac_n; }
    inline double log_fac_m() const { return _log_fac_m; }
    inline double log_fac_k() const { return _log_fac_k; }
    inline double log_fac_n_m() const { return _log_fac_n_m; }
    inline double log_fac_n_k() const { return _log_fac_n_k; }
    inline double log_fac_m_k() const { return _log_fac_m_k; }
    inline double log_d_1() const { return _log_d_1; }
    inline double log_d_2() const { return _log_d_2; }
    inline bool   use_a() const { return _use_a; }
  public:
    inline double log_samprob(const double l) const { return log_samprob_a(l); }
           double log_samprob_a(const double l) const;
           double log_samprob_b(const double l) const;
  public:
    // f(l) = log(samprog(n,m,k,l)) - log(eps)
    inline double f(const double l) const {
                      return log_samprob(l) - _log_eps;
                  }
    // ableitung von f
           double f_der(const double l) const;
  public:
    double alpha(const uint64_t aK);
    double omega(const uint64_t aK);
  private:
    void   init_k(const uint64_t aK);
    double get_initial_value_alpha(const double k) const;
    double get_initial_value_omega(const double k) const;
  private:
    double _n;
    double _m;
    double _k;
    double _log_fac_n;
    double _log_fac_m;
    double _log_fac_k;
    double _log_fac_n_m;
    double _log_fac_n_k;
    double _log_fac_m_k;
    double _log_d_1;
    double _log_d_2;
    double _eps;
    double _log_eps;
    bool   _use_a;
};

#endif

