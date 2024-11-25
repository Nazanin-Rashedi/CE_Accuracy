#ifndef TEST_FIXED_2DIM_HH
#define TEST_FIXED_2DIM_HH

#include "stdlib.h"

#include "infra/matrix.hh"
#include "infra/q.hh"

#include "infra/types.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"

#include "EstimateEvaluator.hh"

namespace H2D {


/*
 * TestFixed2dim
 * partitions a dataset regularly into a aNoX times aNoY  main grid
 * every grid point (except the high ones) is used as a starting point 
 * to generate the set of queries.
 */


class TestFixed2dim {
  public:
   typedef unsigned int uint;
  private:
    TestFixed2dim(const TestFixed2dim&);
    TestFixed2dim& operator=(const TestFixed2dim&);
  public:
    TestFixed2dim(const Data2dim& aData2dim,
                  const EstimatorBase2dim& aEstimator, 
                        EstimateEvaluator& aEstimateEvaluator,
                  const uint aNoX, 
                  const uint aNoY, 
                  const RegularPartitioning2dim* aRpQuery, // fine grained grid for query generation, if not given it is allocated,
                  const RegularPartitioning2dim* aRpToTest,  // equals estimator if it is an instance of RegularPartitioning2dim. this allows for ignoring class L tiles during query generation
                  const bool aIgnoreClassL, // skip Class L tiles for error calculation
                  const uint aLimitClassL   // limit for determining Class L tiles
                 );
    ~TestFixed2dim();
  public:
    void run(const badquery_t& aPrintBadQueries);
    void runComplete(const badquery_t& aPrintBadQueries);
    const EstimateEvaluator& evaluator() const { return _evaluator; }
          EstimateEvaluator& evaluator()       { return _evaluator; }
  public:
    void generateQueriesForOneGridPoint(const uint aXmain, const uint aYmain, 
                                        const badquery_t& aPrintBadQueries);
    void mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                     rectangle_t& r) const;
    uint getTrueFrequency(const uint xlo, const uint ylo,
                          const uint xhi, const uint yhi) const;
    void handleQuery(const uint xlo, const uint ylo,
                     const uint xhi, const uint yhi, 
                     const badquery_t&  aPrintBadQueries);
    bool containedInClassLTile(const uint aQueryIdxX, const uint aQueryIdxY) const;
  public:
    inline uint   noX() const { return _noX; }
    inline uint   noY() const { return _noY; }
    inline double mainWidthX() const { return _mainWidthX; }
    inline double mainWidthY() const { return _mainWidthY; }
    inline uint   noQueriesGenerated() const { return _noQueriesGenerated; }
    inline const RegularPartitioning2dim& rpQuery() const { return (*_rpQuery); }
    inline const RegularPartitioning2dim& rpToTest() const { return (*_rpToTest); }
    inline bool ignoreClassL() const { return _ignoreClassL; }
    inline uint limitClassL() const { return _limitClassL; }
  private:
    void init();
  private:
    uint   _noX;
    uint   _noY;
    uint   _noQueriesGenerated;
    double _mainWidthX; // width in X-direction of main grid
    double _mainWidthY; // width in Y-direction of main grid
    const EstimatorBase2dim&       _estimator;
    const RegularPartitioning2dim* _rpQuery;  // fine grained aNoX * aNoY grid for query generation
    const RegularPartitioning2dim* _rpToTest; // maybe null
    EstimateEvaluator&             _evaluator;
    Matrix                         _cumfreq; // cumulated frequencies (summed from 0,0 .. i,j [all inclusive]) for fine grained grid
    const bool                     _ignoreClassL;
    const uint                     _limitClassL;
    bool                           _rpQueryAllocatedQ;
    const Data2dim&                _data;
};


} // end namespace


#endif
