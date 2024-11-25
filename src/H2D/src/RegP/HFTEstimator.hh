#ifndef H2D_REGP_HFT_ESTIMATOR_HH
#define H2D_REGP_HFT_ESTIMATOR_HH

#include <iostream>
#include <iomanip>
#include <vector>
#include "infra/types.hh"
#include "infra/cb.hh"

#include "infra/RegularPartitioning2dim.hh"
#include "infra/HighlyFrequentTile.hh"
#include "infra/EstimatorBase2dim.hh"

namespace H2D {

class HFTEstimator : public EstimatorBase2dim {
  public:
    typedef std::vector<HighlyFrequentTile*> hft_pvt;
  private:
    HFTEstimator(const HFTEstimator&);
    HFTEstimator& operator=(const HFTEstimator&);
  public:
    HFTEstimator(const int       aLogHi, 
                 const int       aLogLo, 
                 const uint      aKappa, 
                 const uint      aPhi, 
                 const double    aQ,
                 const double    aTheta,
                 const bool      aTrace);
    HFTEstimator(const Data2dim& aData,
                 const int       aLogHi, 
                 const int       aLogLo, 
                 const uint      aKappa, 
                 const uint      aPhi, 
                 const double    aQ, 
                 const double    aTheta,
                 const bool      aTrace);
    virtual ~HFTEstimator();
  public:
    void init(const Data2dim& aData);
  public:
    virtual double estimate(const query_t& lQuery) const;
    virtual double estimate(const rectangle_t& r) const;
    uint outlierCount(const rectangle_t& aQueryRectangle) const;
  public:
    uint size() const;
  public:
    inline const rectangle_t& br() const { return _br; }
    inline       uint         noOutlier() const { return _outlier.size(); }
    inline const Data2dim&    outlier() const { return _outlier; }
    inline       int          logHi() const { return _logHi; }
    inline       int          logLo() const { return _logLo; }
    inline       int          kappa() const { return _kappa; }
    inline       int          phi()   const { return _phi; }
    inline const hft_pvt&     hf() const { return _hftv; }
    inline       bool         trace() const { return _trace; }
    inline       double       restFreq() const { return _restFrequency; }
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream&) const;
  public:
    rectangle_t _br;
    Data2dim    _outlier;
    int         _logHi;
    int         _logLo;
    uint        _kappa;
    uint        _phi;
    hft_pvt     _hftv;
    double      _restFrequency; // after highly frequent tiles have been eliminated
    bool        _trace;
};


} // end namespace

#endif
