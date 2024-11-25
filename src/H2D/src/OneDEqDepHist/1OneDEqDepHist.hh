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
	      const uint   aDim,
              const bool   aTrace);
    virtual ~OneDEqDepHist();
  public:
    inline const Data2dim& outlier() const { return _outlier; }
    inline uint noOutlier() const { return _outlier.size(); }
    inline uint n()  const { return _n; }
    inline uint phi() const { return _phi; }
    inline const entry_vt&  v() const { return _v; }
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimate(const query_t& lQuery) const;
  public:
    uint   outlierCount(const rectangle_t& r) const;
    double estimateIntervals(const rectangle_t& r) const;

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
    uint       _n;
    const uint _phi;
    entry_vt   _v;
    //regp_at    _regp; // array
    bool       _trace;
    uint       _totalCard;
    uint       _dim;
};


std::ostream& operator<<(std::ostream&, const OneDEqDepHist::entry_t&);




} // end namespace


#endif

