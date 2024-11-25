#include "SamplingAlphaOmegaNewton.hh"

#include "../infra/tmath.hh"
#include "../infra/linfun.hh"
#include "../infra/newton_tt.hh"
#include "FukushimaLambertW.hh"

SamplingAlphaOmegaNewton::SamplingAlphaOmegaNewton(const uint64_t aN, const uint64_t aM, const double aEps)
          : _n(aN), _m(aM), _k(0), 
            _log_fac_n(0),
            _log_fac_m(0),
            _log_fac_k(0),
            _log_fac_n_m(0),
            _log_fac_n_k(0),
            _log_fac_m_k(0),
            _log_d_1(0), _log_d_2(0),
            _eps(aEps), _log_eps(std::log(aEps)),
            _use_a(false) {

}

double
SamplingAlphaOmegaNewton::alpha(const uint64_t aK) {
  if(0 == aK) {
    return 0;
  } else
  if(1 == aK) {
//  if(4 > aK){
    _k = aK;
    const double   t = ((double) _m) / ((double) _n);
    const double   a = 1 - _m;
    const double   b = std::exp(log_eps() - std::log(_n) - std::log(t) + t);
    const double   c = _n * (Fukushima::LambertW0(a*b) / a); // needs max(1,round(r));
          uint64_t r = std::max<double>(1, ((std::floor(c) + 0.1 < c) ?  std::ceil(c) : std::round(c)));
          double   p = log_samprob(r);
    // uint lCount = 0;
    while(log_eps() > p) {
     // std::cout << "# " << lCount << ' '<< n() << ' ' << m() << ' ' << aK << ' ' << r << ' ' << p << std::endl;
     ++r;
     p = log_samprob(r);
     // ++lCount;
    }
    return r;
  }
  init_k(aK);
  const bool lTrace = false;
  if(lTrace) {
    const double x_ini = get_initial_value_alpha(aK);
    const double x_new = newton_tt(*this, x_ini, eps(), 0);
    std::cout << "  SamplingAlphaOmegaNewton::alpha(" 
              << ((uint64_t) n()) << ',' 
              << ((uint64_t) m()) << ',' 
              << aK << "):" 
              << std::endl;
    std::cout << "      x_ini      = " << x_ini << std::endl
              << "      x_new      = " << x_new << std::endl
              << "      P(n,m,k,x) = " << log_samprob(x_new) << std::endl
              << "      log(eps)   = " << log_eps() << std::endl
              ;
  }
//  return std::ceil(newton_tt(*this, get_initial_value_alpha(aK), eps(), 0));
 // return std::ceil(newton_tt(*this, get_initial_value_alpha(aK), eps(), 0));//NR changes 1e-4 to eps() and added the max with k
 //
  const double lRes = newton_tt(*this, get_initial_value_alpha(aK), 1e-4, 0);
  //if(std::isnan(lRes) || (k() > lRes)) {
  //   return k();
  // }
  return std::ceil(lRes);

}

double
SamplingAlphaOmegaNewton::omega(const uint64_t aK) {
  if(0 == aK) {
    return ((std::sqrt(1 - 2*(log_eps() / m())) - 1) * n());
  } else
  if(1 == aK) {
    const double t = m() / n();
    const double a = 1 - m();
    const double b = std::exp(log_eps() - std::log(n()) - std::log(t) - t);
    const double r = n() * (Fukushima::LambertWm1(a*b) / a);
    return r;
  }
  init_k(aK);
 // return std::max<double>(aK,std::floor(newton_tt(*this, get_initial_value_omega(aK), eps(), 0)));//NR changed 1e-4 to eps() and added the max with k
//}
//
return std::floor(newton_tt(*this, get_initial_value_omega(aK), 1e-4, 0));//NR changed 1e-4 to eps() and added the max with k
}


void
SamplingAlphaOmegaNewton::init_k(const uint64_t aK) {
  if(k() != aK) {
    _k = aK;
    _log_fac_n   = mt::fLogFactorial(n());
    _log_fac_m   = mt::fLogFactorial(m());
    _log_fac_k   = mt::fLogFactorial(k());
    _log_fac_n_m = mt::fLogFactorial(n() - m());
    _log_fac_n_k = mt::fLogFactorial(n() - k());
    _log_fac_m_k = mt::fLogFactorial(m() - k());
    if(use_a()) {
    } else {
      _log_d_1 = ((_log_fac_m_k - _log_fac_m) + _log_fac_k);
      _log_d_2 =  (_log_fac_n   - _log_fac_n_m);
    }
  }
}

double 
SamplingAlphaOmegaNewton::log_samprob_a(const double l) const {
  double lRes = 0;
  if(1e-1 > k()) {
    lRes =  -mt::fLogFactorial(n() - m() - l)
            +mt::fLogFactorial(n() - l)
            +_log_fac_n_m // mt::fLogFactorial(n() - m())
            -_log_fac_n;  // mt::fLogFactorial(n());
  } else
  if(l == k()) {
    lRes =  -_log_fac_m_k // mt::fLogFactorial(m() - k())
            +_log_fac_m   // mt::fLogFactorial(m())
            +_log_fac_n_k // mt::fLogFactorial(n() - k())
            -_log_fac_n;  // mt::fLogFactorial(n());
  } else {
  // general case
    lRes =  -_log_fac_k // mt::fLogFactorial(k())
            -_log_fac_m_k // mt::fLogFactorial(m() - k())
            +_log_fac_m   // mt::fLogFactorial(m())
            -mt::fLogFactorial(l - k())
            +mt::fLogFactorial(l)
            -mt::fLogFactorial(n() - m() + k() - l)
            +mt::fLogFactorial(n() - l)
            +_log_fac_n_m // mt::fLogFactorial(n() - m())
            -_log_fac_n;  // mt::fLogFactorial(n());
  }
  return lRes;
}

double
SamplingAlphaOmegaNewton::log_samprob_b(const double l) const {
  double lRes = 0;
  if(1e-1 > k()) {
    lRes =  +log_fac_n_m()
            -mt::fLogFactorial(n() - m() - l)
            +mt::fLogFactorial(n() - l)
            -log_fac_n();
  } else
  if(l == k()) {
    lRes =  -log_fac_m_k()
            +log_fac_m()
            +log_fac_n_k()
            -log_fac_n();
  } else {
    const double log_l       = std::log(l);
    const double log_l_k     = std::log(l-k());
    const double log_n_m_k_l = std::log(n()-m()+k()-l);
    const double log_n_l     = std::log(n()-l);

    const double t1 = + (l            ) * log_l;
    const double t2 = - (l         - k()    ) * log_l_k;
    const double t3 = - (n() - m() + k() - l) * log_n_m_k_l;
    const double t4 = + (n()             - l) * log_n_l;
    const double t5 = 0.5 * (log_l - log_l_k - log_n_m_k_l + log_n_l);

    const double r1 = ((-log_d_2() + (t3 + t4)) - m());
    const double r2 = (((t1 + t2) + t5) - log_d_1());
    lRes = r1 + r2;
  }
  return lRes;
}

double
SamplingAlphaOmegaNewton::f_der(const double x) const {
  return (k() - m() + n() + 0.5)/(k() - m() + n() - x) 
          - x/(k() - m() + (n() - x)) 
          + std::log(k() - m() + (n() - x)) 
          + (k() - 0.5)/(x - k()) 
          - x/(x - k()) 
          - std::log(x - k()) //NR had added abs to avoid nan - std::log(std::abs(x - k()))
          - (n() + 0.5)/(n() - x) 
          + x/(n() - x) 
          - std::log(n() - x) 
          + 0.5/x + std::log(x) + 1;
}

double 
SamplingAlphaOmegaNewton::get_initial_value_alpha(const double k) const {
  static const double lErrFac[13] = {0, 100000, 500, 80, 40, 20, 15, 10, 8, 7, 6, 6, 5};
  const double n_div_m = (_n / _m);
  double lF = 0;

  if(_m < (3 * k)) {
     lF = ((8.0 / 11.0) * n_div_m * k);
  } else
  if(50 < k) {
    lF = 0.4 * n_div_m * k;
  } else
  if(30 < k) {
    lF = 0.3 * n_div_m * k;
  } else
  if(20 < k) {
    lF = 0.2 * n_div_m * k;
  } else
  if(12 < k) {
    lF = 0.1 * n_div_m * k;
  } else
  if(0 < k) {
    lF = (n_div_m * k) / lErrFac[(int)k];
  } else {
    assert(0 == 1);
  }
  if((k + 4) > lF) {
    lF = (k + 4);
  }
  return lF;
}

double 
SamplingAlphaOmegaNewton::get_initial_value_omega(const double k) const {
  double lRes = 0;
  if(0 == k) {
    lRes = (_n/_m)*12.0;
  } else {
      if(k > 0.95 * _m) {
        // std::cout << "X.95" << std::endl;
        lRes = std::min(1.01 * _n * (k / _m), _n - 10);
      } else
      if(k > 0.9 * _m) {
        // std::cout << "X.9" << std::endl;
        lRes = 1.04 * _n * (k / _m);
      } else
      if(k > 0.8 * _m) {
        // std::cout << "X.8" << std::endl;
        lRes = 1.06 * _n * (k / _m);
      } else
      if(k > 0.7 * _m) {
        // std::cout << "X.7" << std::endl;
        lRes = 1.09 * _n * (k / _m);
      } else
      if(k > 0.6 * _m) {
        // std::cout << "X.6" << std::endl;
        lRes = 1.13 * _n * (k / _m);
      } else
      if(k > 0.4 * _m) {
        // std::cout << "X.4" << std::endl;
        lRes = 1.17 * _n * (k / _m);
      } else
      if(k > 0.3 * _m) {
        // std::cout << "X.3" << std::endl;
        lRes = 1.20 * _n * (k / _m);
      } else {
        lRes = _n / (_m * (0.75 / (10.0 + 1.7 * k)));
      }
  }
  if(lRes > _n) {
    lRes = _n;
  }
  return lRes;
}

