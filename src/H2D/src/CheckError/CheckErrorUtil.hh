#ifndef HIST2DIM_CHECK_ERROR_UTIL_HH
#define HIST2DIM_CHECK_ERROR_UTIL_HH

#include "infra/measure.hh"

#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/data2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"
#include "TestFixed2dim.hh"
#include "TestRandom2dim.hh"
#include "TestRegular2dim.hh"

namespace H2D {


void
fAnalyzeEstimator(const Data2dim&          aData,
                  const EstimatorBase2dim& aEstimator,
                  const Cb&                aCb,
                  summaryline_t&           aSummaryLine,
                  const bool               aCheckShort = false,
                  const bool               aPrintSummaryLine = true);

void
fInitEstimateEvaluator(EstimateEvaluator& aEstimateEvaluator, const uint aSize);

double
fCheckError(const Data2dim&          aData,
            const EstimatorBase2dim& aEstimator,
                  EstimateEvaluator& aEstimateEvaluator,
            const Cb&                aCb,
            const bool               aCheckShort = false);

uint
fTestRandom(const Data2dim&          aData2dim, 
            const EstimatorBase2dim& aEstimator,
            const uint               aNoX, 
            const uint               aNoY, 
            const uint               aN, 
            const double             aQmax,
            const badquery_t&        aPrintBadQueries,
            EstimateEvaluator&       aEstimateEvaluator);


uint
fTestRegular(const Data2dim&          aData2dim,
             const EstimatorBase2dim& aEstimator,
             const uint               aNoX, 
             const uint               aNoY, 
             const uint               aSubNoX, 
             const uint               aSubNoY, 
             const badquery_t&        aPrintBadQueries,
             EstimateEvaluator&       aEstimateEvaluator);

uint
fTestFixed(const Data2dim&          aData2dim,
           const EstimatorBase2dim& aEstimator,
           const uint               aNoX,
           const uint               aNoY,
           const badquery_t&        aPrintBadQueries,
           EstimateEvaluator&       aEstimateEvaluator);

} // end namespace

#endif

