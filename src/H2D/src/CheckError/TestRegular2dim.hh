#ifndef TEST_REGULAR_2DIM_HH
#define TEST_REGULAR_2DIM_HH

#include "stdlib.h"

#include "infra/matrix.hh"
#include "infra/q.hh"

#include "infra/types.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "EstimateEvaluator.hh"

namespace H2D {


/*
 * TestRegular2dim
 * partitions a dataset regularly into a aNoX times aNoY  main grid
 * every grid point (except the high ones) is used as a starting point 
 * to generate the set of queries.
 * This set of queries is determined by aSubNoX and aSubNoY:
 * A tile of the main grid is subdivided into a 
 *   aSubNoX * aSubNoY subgrid.
 * The number of queries per main grid point can be determined 
 * by main_noquery.cc for the case 2*aSubNoX = 2*aSubNoY = p:
 *  p   #queries
 *  1          0
 *  2          1
 *  3          9
 *  4         36
 *  5        100
 *  6        225
 *  7        441
 *  8        784
 *  9       1296
 * 10       2025
 * 11       3025
 * 12       4356
 * 13       6084
 * 14       8281
 * 15      11025
 * 16      14400
 * Thus, p = 10, i.e. aSubNoX = 5 and aSubNoY = 5 already generates
 * 2025 queries for every grid point on the main grid!
 * The total set of queries then consists of the union of all queries 
 * possible for each point on the main grid.
 * For every query in the total set of queries, the aEstimator is called 
 * and the result produced is compared with the correct result
 */


class TestRegular2dim {
  public:
   typedef unsigned int uint;
  public:
    TestRegular2dim(const Data2dim& aData2dim,
                    const rectangle_t& aRectangle, // bounding rectangle
                    const EstimatorBase2dim& aEstimator, 
                    const uint aNoX, 
                    const uint aNoY, 
                    const uint aSubNoX,
                    const uint aSubNoY,
                    EstimateEvaluator& aEstimateEvaluator);
    // constructor without bounding rectangle,
    // callers must be sure that bounding rectangle has area > 0
    TestRegular2dim(const Data2dim& aData2dim,
                    const EstimatorBase2dim& aEstimator,
                    const uint aNoX,
                    const uint aNoY,
                    const uint aSubNoX,
                    const uint aSubNoY,
                    EstimateEvaluator& aEstimateEvaluator);

    ~TestRegular2dim();
  public:
    void run(const badquery_t& aPrintBadQueries);
    void runComplete(const badquery_t& aPrintBadQueries);
    const EstimateEvaluator& evaluator() const { return _evaluator; }
          EstimateEvaluator& evaluator()       { return _evaluator; }
  public:
    void generateQueriesForOneGridPoint(const uint aXmain, const uint aYmain, 
                                        const badquery_t&  aPrintBadQueries);
    void mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                     rectangle_t& r) const;
    uint getTrueFrequency(const uint xlo, const uint ylo,
                          const uint xhi, const uint yhi) const;
    void handleQuery(const uint xlo, const uint ylo,
                     const uint xhi, const uint yhi, 
                     const badquery_t& aPrintBadQueries);
  public:
    inline uint   noX() const { return _noX; }
    inline uint   noY() const { return _noY; }
    inline uint   subNoX() const { return _subNoX; }
    inline uint   subNoY() const { return _subNoY; }
    inline double mainWidthX() const { return _mainWidthX; }
    inline double mainWidthY() const { return _mainWidthY; }
    inline uint   noQueriesGenerated() const { return _noQueriesGenerated; }
    inline const RegularPartitioning2dim& rp2d() const { return _rp2d; }
  private:
    void init();
  private:
    uint   _noX;
    uint   _noY;
    uint   _subNoX;
    uint   _subNoY;
    uint   _noQueriesGenerated;
    double _mainWidthX; // width in X-direction of main grid
    double _mainWidthY; // width in Y-direction of main grid
    RegularPartitioning2dim  _rp2d; // fine grained (aNoX * aSubNoX) * (aNoY * aSubNoY) grid
    const EstimatorBase2dim& _estimator;
    EstimateEvaluator&       _evaluator;
    Matrix                   _cumfreq; // cumulated frequencies (summed from 0,0 .. i,j [all inclusive]) for fine grained grid
    const Data2dim&          _data;
};


} // end namespace


#endif
