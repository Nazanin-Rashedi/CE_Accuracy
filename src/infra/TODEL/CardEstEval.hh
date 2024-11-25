#ifndef INFRA_CARD_EST_EVAL_HH
#define INFRA_CARD_EST_EVAL_HH

#include "infra/glob_infra_standard_includes.hh"

class CardEstEval {
  public:
    CardEstEval();
  public:
    void init();
    void step(const double aEst, const double aTru);
    void fin();
  public:
    inline double   perr_min() const { return _perr_min; }
    inline double   perr_max() const { return _perr_max; }
    inline uint64_t count_lt() const { return _count_lt; }
    inline uint64_t count_gt() const { return _count_gt; }
  public:
    // for aggregating multiple CardEstEval
    void step(const CardEstEval&);
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    double   _perr_min;
    double   _perr_max;
    uint64_t _count_lt;
    uint64_t _count_gt;
};

std::ostream& operator<<(std::ostream& os, const CardEstEval& x);
#endif
