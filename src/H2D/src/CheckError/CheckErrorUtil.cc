#include "CheckErrorUtil.hh"



namespace H2D {


void
fInitEstimateEvaluator(EstimateEvaluator& aEstimateEvaluator, const uint aSize) {
  aEstimateEvaluator.push_back(aSize / 100);
  aEstimateEvaluator.push_back(aSize /  50);
  aEstimateEvaluator.push_back(aSize /  33);
  aEstimateEvaluator.push_back(aSize /  20);
  aEstimateEvaluator.push_back(aSize /  10);
}

uint
fTestRandom(const Data2dim&          aData2dim, 
            const EstimatorBase2dim& aEstimator,
            const uint               aNoX, 
            const uint               aNoY, 
            const uint               aN, 
            const badquery_t&        aPrintBadQuery,
            EstimateEvaluator&       aEstimateEvaluator) {
  std::cout << "Test random ...";
  std::cout.flush();
  TestRandom2dim lTestRandom2dim(aData2dim, aEstimator, aNoX, aNoY, aEstimateEvaluator);
  lTestRandom2dim.run(aN, aPrintBadQuery);
  std::cout << "no queries generated: " << lTestRandom2dim.noQueriesGenerated() 
            << " current max qerr@1%: " << aEstimateEvaluator.aggregate(0).max() 
            << std::endl;
  return lTestRandom2dim.noQueriesGenerated();
}


uint
fTestRegular(const Data2dim&          aData2dim,
             const EstimatorBase2dim& aEstimator,
             const uint               aNoX, 
             const uint               aNoY, 
             const uint               aSubNoX, 
             const uint               aSubNoY, 
             const badquery_t&        aPrintBadQuery,
             EstimateEvaluator&       aEstimateEvaluator) {
  TestRegular2dim lTestRegular2dim(aData2dim, aEstimator, aNoX, aNoY, aSubNoX, aSubNoY, aEstimateEvaluator);
  lTestRegular2dim.run(aPrintBadQuery);
  std::cout << "test regular: " << aNoX << 'x' << aNoY << ": " 
            << "no queries generated: " << lTestRegular2dim.noQueriesGenerated() 
            << " current max qerr@1%: " << aEstimateEvaluator.aggregate(0).max() 
            << std::endl;
  return lTestRegular2dim.noQueriesGenerated();
}

uint
fTestFixed(const Data2dim&          aData2dim,
           const EstimatorBase2dim& aEstimator,
           const uint               aNoX,
           const uint               aNoY,
           const badquery_t&        aPrintBadQuery,
           EstimateEvaluator&       aEstimateEvaluator) {
  TestFixed2dim lTestFixed2dim(aData2dim, aEstimator, aEstimateEvaluator, 
                               aNoX, aNoY, 0, 0, false, 512);
  lTestFixed2dim.run(aPrintBadQuery);
  std::cout << "test fixed: " << aNoX << 'x' << aNoY << ": "
            << "no queries generated: " << lTestFixed2dim.noQueriesGenerated()
            << " current max qerr@1%: " << aEstimateEvaluator.aggregate(0).max() 
            << std::endl;
  return lTestFixed2dim.noQueriesGenerated();
}


double
fCheckError(const Data2dim&          aData,
            const EstimatorBase2dim& aEstimator,
                  EstimateEvaluator& aEstimateEvaluator,
            const Cb&                aCb,
            const bool               aCheckShort) {
  bool lCheckLong = true;
  if(aCheckShort) {
    lCheckLong = false;
  }

  double lRes = 0.0; // check time per query, returned

  if(aCb.checkError()) {
    uint lNoQueries = 0;
    std::cout << "check error" << std::endl;
    Measure lMeasureCheckTime;
    lMeasureCheckTime.start();
    if(lCheckLong) {
      lNoQueries += fTestRandom(aData, aEstimator, 1017, 1017, 1000*1000, 
                                aCb.printBadQueries(),
                                aEstimateEvaluator);
    } else {
      lNoQueries += fTestRandom(aData, aEstimator, 1017, 1017, 10*1000, 
                                aCb.printBadQueries(),
                                aEstimateEvaluator);
    }

    if(lCheckLong) {
      lNoQueries += fTestRegular(aData, aEstimator,  7,  7, 3, 3, aCb.printBadQueries(), aEstimateEvaluator);
      lNoQueries += fTestRegular(aData, aEstimator,  7,  7, 5, 5, aCb.printBadQueries(), aEstimateEvaluator);
      lNoQueries += fTestRegular(aData, aEstimator, 11, 11, 5, 5, aCb.printBadQueries(), aEstimateEvaluator);
      lNoQueries += fTestRegular(aData, aEstimator, 15, 15, 3, 3, aCb.printBadQueries(), aEstimateEvaluator);
      lNoQueries += fTestRegular(aData, aEstimator, 16, 16, 3, 3, aCb.printBadQueries(), aEstimateEvaluator);
      lNoQueries += fTestRegular(aData, aEstimator, 17, 17, 3, 3, aCb.printBadQueries(), aEstimateEvaluator);
    }

    lNoQueries += fTestFixed(aData, aEstimator,  7,  7, aCb.printBadQueries(), aEstimateEvaluator);
    lNoQueries += fTestFixed(aData, aEstimator, 11, 11, aCb.printBadQueries(), aEstimateEvaluator);
    lNoQueries += fTestFixed(aData, aEstimator, 13, 13, aCb.printBadQueries(), aEstimateEvaluator);
    lNoQueries += fTestFixed(aData, aEstimator, 19, 19, aCb.printBadQueries(), aEstimateEvaluator);


    lMeasureCheckTime.stop();

    lRes = ((lMeasureCheckTime.mCpuTime() * 1000.0 * 1000.0) / lNoQueries);

    std::cout << "check time: " << lMeasureCheckTime.mCpuTime() << ' '
              << ", total number of queries: " << lNoQueries << ' '
              << ", time per query: " << (lMeasureCheckTime.mCpuTime() / lNoQueries) * 1000.0 * 1000.0
              << "[mues]"
              << std::endl;
    aEstimateEvaluator.fin();
    aEstimateEvaluator.print(std::cout);
  }

  return lRes;
}

void
fAnalyzeEstimator(const Data2dim&          aData,
                  const EstimatorBase2dim& aEstimator,
                  const Cb&                aCb,
                  summaryline_t&           aSummaryLine,
                  const bool               aCheckShort,
                  const bool               aPrintSummaryLine) {
  EstimateEvaluator lEstimateEvaluator;
  fInitEstimateEvaluator(lEstimateEvaluator, aData.total());
  lEstimateEvaluator.init();

  aSummaryLine._checkTimePerQuery = fCheckError(aData, 
                                                aEstimator, 
                                                lEstimateEvaluator, 
                                                aCb,
                                                aCheckShort);

  if(aPrintSummaryLine) {
    lEstimateEvaluator.printOneLine(std::cout, "++");
    std::cout << std::endl;
  }

  // std::cout << "fAnalyzeEstimator:" << std::endl;
  for(uint i = 0; i < 5; ++i) {
    // std::cout << "   max = " << lEstimateEvaluator.aggregate(i).max() << std::endl;
    // std::cout << "   avg = " << lEstimateEvaluator.aggregate(i).avg() << std::endl;
    // std::cout << "   cnt = " << lEstimateEvaluator.aggregate(i).count() << std::endl;
    // std::cout << "   sum = " << lEstimateEvaluator.aggregate(i).sum() << std::endl;

    aSummaryLine._max[i] = lEstimateEvaluator.aggregate(i).max();
    aSummaryLine._maxNZ[i] = lEstimateEvaluator.aggregateNZ(i).max();
    aSummaryLine._avg[i] = lEstimateEvaluator.aggregate(i).avg();
  }
  for(uint i = 0; i < 4; ++i) {
    aSummaryLine._maxFixed[i] = lEstimateEvaluator.aggrFixed(i).max();
    aSummaryLine._maxFixedNZ[i] = lEstimateEvaluator.aggrFixedNZ(i).max();
    aSummaryLine._cntBadFixedNZ[i] = lEstimateEvaluator.countBadFixedNZ(i);
  }
}



} // end namespace


