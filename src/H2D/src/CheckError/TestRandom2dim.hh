#ifndef TEST_RANDOM_2DIM_HH
#define TEST_RANDOM_2DIM_HH

#include "stdlib.h"

#include "infra/matrix.hh"
#include "infra/q.hh"

#include "infra/types.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "EstimateEvaluator.hh"

namespace H2D {


/*
 * TestRandom2dim
 * partitions a dataset regularly into a aNoX times aNoY  subgrid
 * and generates aN subqueries.
 * For each subquery, aEstimator is called and the result produced
 * is compared with the correct result
 */


class TestRandom2dim {
  public:
   typedef unsigned int uint;
  public:
    TestRandom2dim(const Data2dim& aData2dim,
                   const EstimatorBase2dim& aEstimator, 
                   const uint aNoX, 
                   const uint aNoY,
                   EstimateEvaluator& aEstimateEvaluator);
    ~TestRandom2dim();
  public:
    void run(const uint aN, const badquery_t& aPrintBadQueries);
    const EstimateEvaluator& evaluator() const { return _evaluator; }
          EstimateEvaluator& evaluator()       { return _evaluator; }
  public:
    void getRandomCorners(uint& xlo, uint& ylo, uint& xhi, uint& yhi) const;
    void mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                     rectangle_t& r) const;
    uint getTrueFrequency(const uint xlo, const uint ylo,
                          const uint xhi, const uint yhi) const;
  public:
    void getRandomRectangle(rectangle_t& r) const;
  public:
    inline uint noQueriesGenerated() const { return _noQueriesGenerated; }

  private:
    void init();
  private:
    RegularPartitioning2dim  _rp2d;
    const EstimatorBase2dim& _estimator;
    EstimateEvaluator&       _evaluator;
    Matrix                   _cumfreq; // cumulated frequencies (summed from 0,0 .. i,j [all inclusive])
    const Data2dim&          _data;
    int                      _noQueriesGenerated;
};


} // end namespace


#endif
