#ifndef H2D_OneDEqDepHist_HH
#define H2D_OneDEqDepHist_HH

#include <vector>
#include <algorithm>

#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"

namespace H2D {


class OneDEqDepHist : public EstimatorBase2dim {
public:
    struct entry_t {
      double _lo; // lower bound of interval
      double _hi; // higher bound of interval
      uint   _cf; // cumulated frequency

      entry_t() : _lo(0), _hi(0), _cf(0) {}
      entry_t(const entry_t& e) : _lo(e.lo()), _hi(e.hi()), _cf(e.cf()) {}

      inline double lo() const { return _lo; }
      inline double hi() const { return _hi; }
      inline uint   cf() const { return _cf; }
    };
    typedef std::vector<entry_t>  entry_vt;
    typedef std::vector<entry_vt> entry_vvt;
private:
    OneDEqDepHist(const OneDEqDepHist&);
    OneDEqDepHist& operator=(const OneDEqDepHist&);
  public:
    OneDEqDepHist(const Data2dim& aData,
              const uint   aPhi,
              const double aQ,
              const double aTheta,
	      const uint aN,
              const bool   aTrace);
    virtual ~OneDEqDepHist();
  public:
    inline const Data2dim& outlier() const { return _outlier; }
    inline uint noOutlier() const { return _outlier.size(); }
    inline uint nx()  const { return _nx; }
    inline uint phi() const { return _phi; }
    inline const entry_vt&  vx() const { return _vx; }
    inline const entry_vt&  vy() const {return _vy; }
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimateX(const query_t& lQuery) const;
    virtual double estimateY(const query_t& lQuery) const;

    virtual double estimate(const query_t& lQuery) const;
  public:
    uint   outlierCountX(const rectangle_t& r) const;
    uint   outlierCountY(const rectangle_t& r) const;
    double estimateIntervalsX(const rectangle_t& r) const;
    double estimateIntervalsY(const rectangle_t& r) const;

uint   size() const;
    // find the best end according to aCumBucketFreq
    uint   findEnd(const Data2dim& aData, 
                   const uint aBegin, // beginning of new bucket
                   const uint aEnd,   // maximal end of new bucket (will not belong to bucket)
                   uint       aCumFreqWish,  // approximate cumulated frequency of bucket to be build
                   uint&      aCumFreqOut) const; // actual cumulated frequency of new bucket
    static bool lowerIsCloser(const uint aLo, const uint aHi, const uint aGoal); public:
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os) const;
  public:
    inline bool trace() const { return _trace; }
    inline void trace(const bool x) { _trace = x; }
  private:
    void initV(Data2dim& aData);
  private:
    Data2dim   _outlier;
    uint       _nx;
    const uint _phi;
    entry_vt   _vx;
    entry_vt   _vy;
    //regp_at    _regp; // array
    bool       _trace;
    uint       _totalCard;
};


std::ostream& operator<<(std::ostream&, const OneDEqDepHist::entry_t&);




} // end namespace


#endif

