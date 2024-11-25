#include "SamplingAlphaOmegaBisection.hh"


double SamplingAlphaOmegaBisection::_eps = 1e-5L;

SamplingAlphaOmegaBisection::SamplingAlphaOmegaBisection(const uint64_t aN,
                                                         const uint64_t aM,
                                                         const bool     aTrace)
                            : _n(aN), _m(aM), _k(0), _trace(aTrace) {
}

SamplingAlphaOmegaBisection::SamplingAlphaOmegaBisection()
                            : _n(0), _m(0), _k(0), _trace(false) {
}

void
SamplingAlphaOmegaBisection::init(const uint64_t aN,
                                  const uint64_t aM,
                                  const bool     aTrace) {
  _n = aN;
  _m = aM;
  _k = 0;
  _trace = aTrace;
}


uint64_t
SamplingAlphaOmegaBisection::alpha(const uint64_t aK) const {
  _k = aK;

  if(0 == _k) {
    return 0;
  }
  if(samprob(_k) >= eps()) {
    return _k;
  }

  int64_t lLo = _k;
  int64_t lHi = (uint64_t) expected_value();
  if(samprob(lHi) < eps()) {
    std::cerr << "problem prob(expectec_value) < eps()" << std::endl;
    return 0;
  }
  int64_t lMi = 0;
  double  lProb = 0;
  while((lHi - lLo) > 2) {
    lMi = (lLo + lHi) / 2;
    lProb = samprob(lMi);
    if(lProb < eps()) {
      lLo = lMi;
    } else {
      lHi = lMi;
    }
    // if(trace()) {
    //   std::cout << "    alpha " << lLo << ' ' << lMi << ' ' << lHi << ' ' 
    //             << lProb << std::endl;
    // }
  }
  if(trace()) {
    std::cout << "  alpha_fin: " 
              << lLo << ' ' << lHi << ' ' 
              << samprob(lLo) << ' ' << samprob(lHi) 
              << std::endl;
  }
  assert(samprob(lLo) <  eps());
  assert(samprob(lHi) >= eps());
  if(samprob(lLo) >= eps()) {
    return lLo;
  }
  if(samprob(lLo + 1) >= eps()) {
    return lLo + 1;
  }
  return lHi;
}

uint64_t
SamplingAlphaOmegaBisection::omega(const uint64_t aK) const {
  _k = aK;

  if(samprob(_n) >= eps()) {
    return _n;
  }

  int64_t lLo = (uint64_t) expected_value();
  int64_t lHi = _n;
  if(samprob(lLo) < eps()) {
    std::cerr << "problem prob(expectec_value) < eps()" << std::endl;
    return 0;
  }
  int64_t lMi = 0; 
  double   lProb = 0;
  while((lHi - lLo) > 1) {
    lMi = (lLo + lHi) / 2;
    lProb = samprob(lMi);
    if(lProb >= eps()) {
      lLo = lMi;
    } else {
      lHi = lMi;
    }
    // if(trace()) {
    //   std::cout << "omega " << lLo << ' ' << lMi << ' ' << lHi << ' ' << lProb << std::endl;
    // }
  }
 
  if(trace()) {
    std::cout << "  omega_fin: " 
              << lLo << ' ' << lHi << ' ' 
              << samprob(lLo) << ' ' << samprob(lHi) 
              << std::endl;
  }
  assert(samprob(lLo) >= eps());
  assert(samprob(lHi) < eps());
  if(samprob(lHi) >= eps()) {
    return lHi;
  }
  return lLo;
}


double
SamplingAlphaOmegaBisection::mue(const double aAlpha, const double aOmega) const {
  return std::sqrt(std::max<double>(1, aAlpha) * aOmega);
}

double
SamplingAlphaOmegaBisection::rho(const double aAlpha, const double aOmega) const {
  return std::sqrt((1.0 / aAlpha) * aOmega);
}

uint64_t
SamplingAlphaOmegaBisection::zeta(const double aMaxQError, zeta_info_t* aZetaOut) const {
  uint64_t k_lo = 10; // lower bound of zeta
  double   lAlphaLo = alpha(k_lo);
  double   lOmegaLo = omega(k_lo);
  double   lRhoLo   = rho(lAlphaLo, lOmegaLo);

  if(lRhoLo <= aMaxQError) {
    for(; 0 < k_lo; k_lo -= 1) {
      lAlphaLo = alpha(k_lo);
      lOmegaLo = omega(k_lo);
      lRhoLo   = rho(lAlphaLo, lOmegaLo);
      if(lRhoLo > aMaxQError) {
        return (k_lo + 1);
      }
    }
    return k_lo;
  }

  // lRhoLo > aMaxQError
  // thus k_lo is too small

  uint32_t k_hi = 20;
  double   lAlphaHi = alpha(k_hi);
  double   lOmegaHi = omega(k_hi);
  double   lRhoHi   = rho(lAlphaHi, lOmegaHi);

  while(aMaxQError < lRhoHi) {
    k_hi *= 2;
    lAlphaHi = alpha(k_hi);
    lOmegaHi = omega(k_hi);
    lRhoHi   = rho(lAlphaHi, lOmegaHi);
  }

  // lRhoLo >  aMaxQError (still)
  // lRhoHi <= aMaxQError

  // perform binary search
  while((k_lo + 1) < k_hi) {
    const uint32_t k_mi = ((k_lo + k_hi) / 2); // the k in the middle 
    const double   lAlphaMi = alpha(k_mi);
    const double   lOmegaMi = omega(k_mi);
    const double   lRhoMi   = rho(lAlphaMi, lOmegaMi);
    if(lRhoMi > aMaxQError) {
      k_lo     = k_mi;
      lAlphaLo = lAlphaMi;
      lOmegaLo = lOmegaMi;
      lRhoLo   = lRhoMi;
    } else {
      k_hi = k_mi;
      lAlphaHi = lAlphaMi;
      lOmegaHi = lOmegaMi;
      lRhoHi   = lRhoMi;
    }
  }

  if(0 != aZetaOut) {
    aZetaOut->_alpha_at_zeta = lAlphaHi;
    aZetaOut->_omega_at_zeta = lOmegaHi;
    aZetaOut->_rho_at_zeta   = lRhoHi;
    aZetaOut->_zeta  = k_hi;
  }
  return k_hi;
}

double
SamplingAlphaOmegaBisection::lambda(const zeta_info_t& aZi) const {
  return (aZi.mue() / _n); 
}

uint64_t
SamplingAlphaOmegaBisection::psi(const double n, const double aQError, const double aTheta) {
  assert(n > aTheta);
  assert(n > 100);

  const bool lTrace = false;

  double m_lo = std::round(n / 10);
  double m_hi = 0;

  if(lTrace) {
    std::cout << std::endl;
    std::cout << "psi(" << n << ", " << aQError << ", " << aTheta << "):" << std::endl;
  }

  if(is_theta_q_acceptable(n, m_lo, aQError, aTheta)) {
    if(lTrace) {
      std::cout << "   1.  acc(" << m_lo << ')' << std::endl;
    }
    m_hi = m_lo;
    do {
      if(lTrace) {
        std::cout << "   1.1 acc(" << m_lo << ')' << std::endl;
      }
      m_hi = m_lo;
      m_lo = ceil(m_lo / 2);
    } while(is_theta_q_acceptable(n, m_lo, aQError, aTheta));
  } else {
    if(lTrace) {
      std::cout << "   2.  !acc(" << m_lo << ')' << std::endl;
    }
    m_hi = m_lo;
    do {
      if(lTrace) {
        std::cout << "   2.2 !acc(" << m_hi << ')' << std::endl;
      }
      m_lo = m_hi;
      m_hi *= 2;
    } while(!is_theta_q_acceptable(n, m_hi, aQError, aTheta));
  }

  // m_lo < m_hi
  // is_theta_q_acceptable(m_lo) && !is_theta_q_acceptable(m_hi)
  // binary search
  if(lTrace) {
    std::cout << "  ITV lo/hi: " << m_lo << '/' << m_hi << " : "
              << is_theta_q_acceptable(n, m_lo, aQError, aTheta)
              << '/'
              << is_theta_q_acceptable(n, m_hi, aQError, aTheta)
              << std::endl;
  }

  while((m_lo + 1) < m_hi) {
    double m_mi = std::floor((m_lo + m_hi) / 2);
    if(is_theta_q_acceptable(n, m_mi, aQError, aTheta)) {
      m_hi = m_mi;
    } else {
      m_lo = m_mi;
    }
  }

  if(lTrace) {
    std::cout << "  RES lo/hi: " << m_lo << '/' << m_hi << " : "
              << is_theta_q_acceptable(n, m_lo, aQError, aTheta)
              << '/'
              << is_theta_q_acceptable(n, m_hi, aQError, aTheta)
              << std::endl;
  }

  return ((uint64_t) std::round(m_hi));
}

/*
 *  determine min k such that standard estimator produces a max q-error of aMaxQError
 */

double   
SamplingAlphaOmegaBisection::eta(const double aMaxQError) const {
  const double q = aMaxQError;
  const double n = _n;
  const double m = _m;
  uint k = 1;
  for(k = 1; k < _m; ++k) {
    const double lAlpha = alpha(k);
    const double lOmega = omega(k);
    const double lEst = (((double) k) * (n / m)); // estimate by standard estimator
    // std::cout << "eta: " << _n << ' ' << _m << ' ' << k << ": " 
    //           << lAlpha << ' ' << lOmega << ' ' << lEst 
    //           << std::endl;
    if((lEst <= (q * lAlpha)) && (lOmega <= (q * lEst))) {
      break;
    }
  }
  return k;
}

bool
SamplingAlphaOmegaBisection::is_theta_q_acceptable(const double n, 
                                                      const double m,
                                                      const double aMaxQError, 
                                                      const double aTheta) {
  SamplingAlphaOmegaBisection lGreek(n, m);
  const double lZeta = lGreek.zeta(aMaxQError);
  if(0 == lZeta) {
    return true;
  }
  return (aTheta >= lGreek.omega(lZeta - 1));
}



double
SamplingAlphaOmegaBisection::expected_value() const {
  return ((double) _n) * (((double) _k) / ((double) _m));
}

double
SamplingAlphaOmegaBisection::samprob_a(const double n, const double m, const double k, const double l) {
  if(0.01 > k) {
    return std::exp(mt::fLogBinomGamma(n-l, m-k) - mt::fLogBinomGamma(n,m));
  }
  return std::exp(mt::fLogBinomGamma(n-l, m-k) + mt::fLogBinomGamma(l,k) - mt::fLogBinomGamma(n,m));
}

double
SamplingAlphaOmegaBisection::samprob_b(const double n, const double m, const double k, const double l) {
  double lRes = 0;
  if(1e-1 > k) {
    lRes =  -mt::fLogFactorial(n-m-l)
            +mt::fLogFactorial(n-l)
            +mt::fLogFactorial(n-m)
            -mt::fLogFactorial(n);
  } else
  if(l == k) {
    lRes =  -mt::fLogFactorial(m-k)
            +mt::fLogFactorial(m)
            +mt::fLogFactorial(n-k)
            -mt::fLogFactorial(n);
  } else {
  // general case
    lRes =  -mt::fLogFactorial(k)
            -mt::fLogFactorial(m-k)
            +mt::fLogFactorial(m)
            -mt::fLogFactorial(l-k)
            +mt::fLogFactorial(l)
            -mt::fLogFactorial(n-m+k-l)
            +mt::fLogFactorial(n-l)
            +mt::fLogFactorial(n-m)
            -mt::fLogFactorial(n);
  }
  return std::exp(lRes);
}


