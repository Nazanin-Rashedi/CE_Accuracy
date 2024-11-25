#ifndef REGP_ESTIMATOR_HH
#define REGP_ESTIMATOR_HH

#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"

namespace H2D {


class RegPEstimator : public EstimatorBase2dim {
  public:
    RegPEstimator(const Data2dim& aData,
                  const uint      aNx,
                  const uint      aNy,
                  const double    aEpsilon,
                  const uint      aPhi,
                  const double    aQ,
                  const double    aTheta);
    virtual ~RegPEstimator();
  public:
    inline const RegularPartitioning2dim& regp() const { return _regp; }
    inline const Data2dim& outlier() const { return _outlier; }
    inline uint noOutlier() const { return _outlier.size(); }
    inline uint nx()  const { return _nx; }
    inline uint ny()  const { return _ny; }
    inline double epsilon() const { return _epsilon; }
    inline uint phi() const { return _phi; }
    inline double maxEntry() const { return _regp.maxEntry(); }
    inline uint noZero() const { return _regp.noZero(); }
    inline uint noNonZero() const { return (nx() * ny() - _regp.noZero()); }
  public:
    virtual double estimate(const query_t& lQuery) const;
    virtual double estimate(const rectangle_t& r) const;
  public:
    uint size() const;
  public:
    uint outlierCount(const rectangle_t& r) const;
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os, const int aFieldWidth = 4) const;
            std::ostream& printWithRowColSums(std::ostream& os, const int aFieldWidth = 4) const;
  private:
    RegularPartitioning2dim _regp;
    Data2dim                _outlier;
    const uint              _nx;
    const uint              _ny;
    const double            _epsilon; // ignore
    const uint              _phi;
};


} // end namespace


#endif

