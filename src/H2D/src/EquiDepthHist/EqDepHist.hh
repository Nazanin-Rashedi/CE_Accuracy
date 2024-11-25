#ifndef H2D_EQDEPTH_HIST_HH
#define H2D_EQDEPTH_HIST_HH

#include <vector>
#include <algorithm>

#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"

namespace H2D {


class EqDepHist : public EstimatorBase2dim {
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
    enum kind_t {
      k_simple = 0,
      k_matrix = 1 
    };
    typedef RegularPartitioning2dim* regp_at;
  private:
    EqDepHist(const EqDepHist&);
    EqDepHist& operator=(const EqDepHist&);
  public:
    EqDepHist(const Data2dim& aData,
              const kind_t aKind,
              const uint   aNx,
              const uint   aNy,
              const uint   aRx,
              const uint   aRy,
              const uint   aPhi,
              const double aQ,
              const double aTheta,
              const bool   aTrace);
    virtual ~EqDepHist();
  public:
    inline const Data2dim& outlier() const { return _outlier; }
    inline uint noOutlier() const { return _outlier.size(); }
    inline uint nx()  const { return _nx; }
    inline uint ny()  const { return _ny; }
    inline uint rx()  const { return _rx; }
    inline uint ry()  const { return _ry; }
    inline uint phi() const { return _phi; }
    inline const entry_vt&  vx() const { return _vx; }
    inline const entry_vvt& vy() const { return _vy; }
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimate(const query_t& lQuery) const;
  public:
    uint   outlierCount(const rectangle_t& r) const;
    // for k_simple
    double estimateIntervals(const rectangle_t& r) const;
    double estimateY(const rectangle_t& r,
                     const uint         aIdxX,
                     const double       aXlo,
                     const double       aXhi) const;
    // for k_matrix
    double estimateIntervalsR(const rectangle_t& r) const;
    double estimateYR(const rectangle_t& r,
                     const uint         aIdxX,
                     const double       aXlo,
                     const double       aXhi) const;

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
    inline kind_t kind() const { return _kind; }
  private:
    void initVx(Data2dim& aData);
    void initVy(Data2dim& aData, const uint aBegin, const uint aEnd, const uint aIdxX);
    void initVyR(Data2dim& aData, const uint aBegin, const uint aEnd, const uint aIdxX);
  private:
    Data2dim   _outlier;
    kind_t     _kind;
    uint       _nx;
    uint       _ny;
    uint       _rx; // refinement in x dimension for k_matrix
    uint       _ry; // refinement in y dimension for k_matrix
    const uint _phi;
    entry_vt   _vx;
    entry_vvt  _vy;
    regp_at    _regp; // array
    bool       _trace;
};


std::ostream& operator<<(std::ostream&, const EqDepHist::entry_t&);


} // end namespace

#endif


