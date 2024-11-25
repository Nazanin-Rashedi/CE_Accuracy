#include "TestFixed2dim.hh"

namespace H2D {


TestFixed2dim::TestFixed2dim(const Data2dim& aData2dim,
                             const EstimatorBase2dim& aEstimator, 
                             EstimateEvaluator&             aEstimateEvaluator,
                             const uint aNoX, 
                             const uint aNoY, 
                             const RegularPartitioning2dim* aRpQuery, // aNoX, aNoY go here
                             const RegularPartitioning2dim* aRpToTest,
                             const bool aIgnoreClassL,
                             const uint aLimitClassL)
               : _noX(aNoX), _noY(aNoY),
                 _noQueriesGenerated(0), 
                 _mainWidthX(0), _mainWidthY(0),
                 _estimator(aEstimator),
                 _rpQuery(aRpQuery),
                 _rpToTest(aRpToTest),
                 _evaluator(aEstimateEvaluator),
                 _cumfreq(),
                 _ignoreClassL(aIgnoreClassL),
                 _limitClassL(aLimitClassL),
                 _rpQueryAllocatedQ(false),
                 _data(aData2dim)  {
  if(0 == aRpQuery) {
    _rpQuery = aRpQuery = new RegularPartitioning2dim(aNoX, aNoY, aData2dim);
    _rpQueryAllocatedQ = true;
  }
  init();
}

TestFixed2dim::~TestFixed2dim() {
  if(_rpQueryAllocatedQ) {
    delete _rpQuery;
  }
}


void
TestFixed2dim::init() {
  const bool lTrace = false;

  _mainWidthX = (rpQuery().maxX() - rpQuery().minX()) / (double) noX();
  _mainWidthY = (rpQuery().maxY() - rpQuery().minY()) / (double) noY();

  _cumfreq = rpQuery().matrix();
  _cumfreq.accumulateInclusive();

  if(lTrace) {
    std::cout << "Frequency matrix: " << std::endl;
    rpQuery().printWithRowColSums(std::cout, 5);
    std::cout << "Cumulative matrix: " << std::endl;
    _cumfreq.print(std::cout, 5);
    for(uint xlo = 0; xlo < 4; ++xlo) {
      for(uint ylo = 0; ylo < 4; ++ylo) {
        for(uint xhi = xlo + 1; xhi < 5; ++xhi) {
          for(uint yhi = ylo + 1; yhi < 5; ++yhi) {
            std::cout << xlo << ' ' << ylo << ' ' << xhi << ' ' << yhi << " | "
                      << getTrueFrequency(xlo, ylo, xhi, yhi) << std::endl;
          }
        }
      }
    }
  }
  
}



void
TestFixed2dim::run(const badquery_t& aPrintBadQueries) {
  runComplete(aPrintBadQueries);
}

void
TestFixed2dim::runComplete(const badquery_t& aPrintBadQueries) {
  for(uint xlo = 0; xlo < rpQuery().noRows(); ++xlo) {
    for(uint ylo = 0; ylo < rpQuery().noCols(); ++ylo) {
      if(ignoreClassL() && containedInClassLTile(xlo, ylo)) {
         break;
      }
      for(uint xhi = xlo + 2; xhi < rpQuery().noRows(); ++xhi) {
        for(uint yhi = ylo + 2; yhi < rpQuery().noCols(); ++yhi) {
          if(ignoreClassL() && containedInClassLTile(xhi, yhi)) {
             break;
          }
          handleQuery(xlo, ylo, xhi, yhi, aPrintBadQueries);
        }
      }
    }
  }
}

bool
TestFixed2dim::containedInClassLTile(const uint aQueryIdxX,
                                     const uint aQueryIdxY) const {
  // umrechnen xhi und yhi in _rpToTest indexes
  const double lValX = rpQuery().pdX().boundary(aQueryIdxX);
  const double lValY = rpQuery().pdY().boundary(aQueryIdxY);
  const uint   lIdxX = rpToTest().pdX().idx(lValX);
  const uint   lIdxY = rpToTest().pdY().idx(lValY);
  return (limitClassL() < rpToTest()(lIdxX, lIdxY));
}

void
TestFixed2dim::handleQuery(const uint xlo, const uint ylo, 
                           const uint xhi, const uint yhi, 
                           const badquery_t& aPrintBadQueries) {
  assert(xlo < xhi);
  assert(ylo < yhi);
  ++_noQueriesGenerated;
  rectangle_t lRectangle;
  const bool lTrace = false;
  mkRectangle(xlo, ylo, xhi, yhi, lRectangle);
  const uint32_t lTrueValInt = getTrueFrequency(xlo, ylo, xhi - 1, yhi - 1);
  const double   lTrueVal = (double) lTrueValInt;
  const double   lEstVal  = (double) _estimator.estimate(lRectangle);
  const double   lQError  = q::qerror<double>(std::max<double>(1.0, lTrueVal), 
                                              std::max<double>(1.0, lEstVal));

     #ifdef CHECKCHECK_Y
     // for checking
     const uint32_t lTrueValCounted = _data.countWithin(lRectangle);
     if(lTrueValCounted != lTrueValInt) {
       std::cout << "ERROR: c = " << lTrueValCounted << "  but  " 
                 << "      tv = " << lTrueValInt  << "  "
                 << "       e = " << lEstVal 
                 << std::endl;
     }
     assert((labs(lTrueValCounted - lTrueValInt) > 10) ||
            (labs(lTrueValCounted - lTrueValInt) <= 2 * std::min<uint32_t>(lTrueValCounted, lTrueValInt)));
     // end checking
     #endif

  _evaluator.step(lTrueVal, lEstVal);
  if((aPrintBadQueries.flag()) && (aPrintBadQueries.qmax() < lQError) 
      && (lTrueVal > aPrintBadQueries.theta() || lEstVal > aPrintBadQueries.theta())) {
    std::cout << "BAD RECTANGLE (" << lTrueVal << ", " << lEstVal << ") : "
              << lRectangle
              << std::endl;
  }
  if(lTrace) {
    std::cout << xlo << ' ' << ylo << ' ' << xhi << ' ' << yhi << "   "
              << lRectangle << "  ||"
              << lTrueVal << " / "
              << lEstVal << "||_q = "
              << lQError
              << std::endl;
  }
}




uint
TestFixed2dim::getTrueFrequency(const uint xlo, const uint ylo, const uint xhi, const uint yhi) const {
  assert(xlo < xhi);
  assert(ylo < yhi);
  uint lRes = _cumfreq(xhi, yhi);
  if(0 < xlo) {
    lRes -= _cumfreq(xlo - 1, yhi);
  }
  if(0 < ylo) {
    lRes -= _cumfreq(xhi, ylo - 1);
  }
  if(0 < xlo && 0 < ylo) {
    lRes += _cumfreq(xlo - 1, ylo - 1);
  }
  if(false && lRes > 10000) {
    uint x = _cumfreq(xhi, yhi);
    std::cout << "x1: " << x << std::endl;
    if(0 < xlo) {
      x -= _cumfreq(xlo - 1, yhi);
      std::cout << "x2: " << x << std::endl;
    }
    if(0 < ylo) {
      x -= _cumfreq(xhi, ylo - 1);
      std::cout << "x3: " << x << std::endl;
    }
    if(0 < xlo && 0 < ylo) {
      x += _cumfreq(xlo - 1, ylo - 1);
      std::cout << "x4: " << x << std::endl;
    }
    std::cout << "x: " << x << std::endl;
  }
  return lRes;
}


void
TestFixed2dim::mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                            rectangle_t& r) const {
  r._pll.x = rpQuery().minX() + xlo * rpQuery().tileWidthX();
  r._pll.y = rpQuery().minY() + ylo * rpQuery().tileWidthY();
  r._pur.x = rpQuery().minX() + xhi * rpQuery().tileWidthX();
  r._pur.y = rpQuery().minY() + yhi * rpQuery().tileWidthY();
}


} // end namespace



