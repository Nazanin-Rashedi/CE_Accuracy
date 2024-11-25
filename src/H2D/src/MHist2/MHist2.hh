#ifndef H2D_MHIST2_HH
#define H2D_MHIST2_HH

#include <assert.h>

#include "../infra/EstimatorBase2dim.hh"
#include "../infra/types.hh"
#include "../infra/data2dim.hh"
#include "../infra/variance.hh"
#include "MHist2/MHist2Bucket.hh"

namespace H2D {


class MHist2 : public EstimatorBase2dim {
public:
  enum kind_t {
    k_invalid = 0,
    k_maxDiffSpread = 1,
    k_maxSpread = 2,
    k_maxDiffCount = 3,
    k_maxDiffArea = 4,
    k_minVariance = 5,
  };
private:
  struct partialDistribution_t {
    double extremeDiff; // min or max diff depending on kind
    double value;
    char dimension;

    Data2dim data;


    partialDistribution_t(const Data2dim& aData) :
      extremeDiff(0.0), value(), dimension(' '), data(aData) {

    }
  };
  typedef std::vector<partialDistribution_t> partialDistribution_vt;

public:
  MHist2();
  MHist2(const uint      aNumBuckets, 
         const kind_t    aKind, 
         const Data2dim& aData);
  virtual ~MHist2();
public:
  inline kind_t kind() const { return _kind; }
public:
  void init(const uint aNumBuckets, const kind_t aKind, const Data2dim& aData);
public:
  double estimate(const rectangle_t& aRect) const;
  double estimate(const query_t& lQuery) const;

  uint size() const; //  { return _buckets.size() * (4 * sizeof(double) + sizeof(uint)); };
  virtual std::ostream& print_name_param(std::ostream& os) const;
  std::ostream& printSvg(std::ostream&, const Data2dim& aData, const bool printDot) const;
private:
  void maxDiffSpread(partialDistribution_t& aPd) const;
  void maxSpread(partialDistribution_t& aPd) const;
  void maxDiffCount(partialDistribution_t& aPd) const;
  void maxDiffArea(partialDistribution_t& aPd) const;
  void minVariance(partialDistribution_t& aPd) const;

  inline xyc_vt::iterator nextX(xyc_vt::iterator aIter, const xyc_vt::iterator& aIterEnd) const { while (++aIter < aIterEnd && (aIter - 1)->x == aIter->x){}; return aIter; };
  inline xyc_vt::iterator nextY(xyc_vt::iterator aIter, const xyc_vt::iterator& aIterEnd) const { while (++aIter < aIterEnd && (aIter - 1)->y == aIter->y){}; return aIter; };
  inline uint cntX(xyc_vt::iterator aIter, const xyc_vt::iterator& aIterEnd) const { uint lRes = aIter->c; while (++aIter < aIterEnd && (aIter - 1)->x == aIter->x){lRes += aIter->c;}; return lRes; };
  inline uint cntY(xyc_vt::iterator aIter, const xyc_vt::iterator& aIterEnd) const { uint lRes = aIter->c; while (++aIter < aIterEnd && (aIter - 1)->y == aIter->y){lRes += aIter->c;}; return lRes; };
private:
  kind_t                    _kind;
  std::vector<MHist2Bucket> _buckets;
};


} // end namespace

#endif /* MHIST2_HH */
