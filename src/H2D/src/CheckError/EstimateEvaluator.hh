#ifndef DATA_STATISTICS_HIST4_ESTIMATE_EVALUATOR2_HH
#define DATA_STATISTICS_HIST4_ESTIMATE_EVALUATOR2_HH

#include "infra/glob_infra_standard_includes.hh"
#include <string.h>

#include "infra/aggregate.hh"
#include "infra/tmath.hh"
#include "infra/q.hh"
#include <cmath>
namespace H2D {


/*
 * EstimateEvaluator2
 * like EstimateEvaluator
 * but does not use any parameter theta.
 * instead, it checks theta,q-acceptability
 * todo: add 96
 */

class EstimateEvaluator {
  public:
    typedef Aggregate<double>   aggr_t;
    typedef std::vector<aggr_t> aggr_vt;
  public:
    EstimateEvaluator();
    ~EstimateEvaluator();
  public:
    void push_back(const uint aTheta);
  public:
    void init();
    void step(const uint aTrueValue, const uint aEstimate); 
    void step(const uint aTrueValue, const uint aEstimate, std::ostream& fos);
    void nstep(const uint aTrueValue, const uint aEstimate);
    void fin();
  public:
    inline uint no_theta() const { return _theta.size(); }
    inline uint theta(const uint i) const { return _theta[i]; }
    inline const aggr_t& aggregate(const uint i) const { return _aggregates[i]; }
    inline const aggr_t& aggregateNZ(const uint i) const { return _aggregatesNZ[i]; }
  public:
    inline       uint      noThetaFixed() const { return 4; }
    inline       uint      thetaFixed(const uint i) const { return _thetaFixed[i]; }
    inline const aggr_t&   aggrFixed(const uint i) const { return _aggrFixed[i]; }
    inline const aggr_t&   aggrFixedNZ(const uint i) const { return _aggrFixedNZ[i]; }
    inline       uint      countBadFixedNZ(const uint i) const { return _countBadFixedNZ[i]; }
    inline const uint_vvvt& queryClass() const { return _queryClass; }
    inline const uint_vvt& thetaQueryClass(const uint i) const {return _queryClass[i];}
    inline const uint_vvt& qerrProfile() const { return _qerrProfile; }
    inline const uint_vt&  qerrProfile(const uint i) const { return _qerrProfile[i]; }
    inline void setTotalCard(uint total) {_total = total; }
    inline uint getTotal() const { return _total; }

  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& nprint(std::ostream& os) const;
    std::ostream& printShort(std::ostream& os) const;
    std::ostream& printAllforOneEst(std::ostream& os) const;

    std::ostream& printOneLine(std::ostream& os, const std::string& aLineMarker) const;
    std::ostream& printQErrProfile(std::ostream& os, const std::string& aLineMarker) const;
    std::ostream& printHighQerrorSelPercentile(std::ostream& os) const;
    std::ostream& printQuerySelDistr(std::ostream& os) const;
//NR
    void dstep(const uint aQNum, const uint aTrueValue, const uint aEstimate);
    std::ostream& dprint(std::ostream& os) const;


  private:
    uint_vt  _theta;
    aggr_vt  _aggregates; // simple aggregates of q-error for card/est exceeding theta
    aggr_vt  _aggregatesNZ; // simple aggregates of q-error for card/est exceeding theta, 0 < true-val 
    uint_vvt _qerrProfile; // <= 2, <= 3, <= 4, <= 5, ... <= 10, > 10, #underestimates, #overestimates
    uint_vt  _thetaFixed; // fixed absolute theta: 1000, 5000, 10000, 20000
    aggr_vt  _aggrFixed;
    aggr_vt  _aggrFixedNZ; // 0 < true val
    uint_vt  _countBadFixedNZ;
    uint     _total;
    uint _qselPercentile[6][4] = {0};
    uint _allQuerySelPercentiles[4] = {0};
    uint_vvvt _queryClass;


    struct EstDetail {
    uint qNum;
    uint trueCard;
    uint estCard;
    double qSel;
   
    EstDetail(uint q, double qs,uint t,uint e ): qNum(q), trueCard(t),estCard(e),qSel(qs){}
    
    };

    std::vector<EstDetail> det_v;
   public:
    inline uint selectivity_class(const double aSelectivity) {
    return ((uint) std::floor(std::log2(double{1} / aSelectivity)));}
    inline uint selectivity_class(const double aCardSel, const double aCardTot) {
    return selectivity_class(aCardSel / aCardTot);}
    inline uint cardinality_class(const double aCardinality) {
    return ((uint) std::floor(std::log2(aCardinality)));}
    inline uint qerror_class(const double aQError) {
    return std::min<uint>(32, ((uint) std::floor(std::log(aQError) / std::log(std::sqrt(2)))));}


    
};


} // end namespace

#endif
