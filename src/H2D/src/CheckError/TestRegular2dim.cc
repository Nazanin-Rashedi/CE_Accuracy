#include "TestRegular2dim.hh"


namespace H2D {

TestRegular2dim::TestRegular2dim(const Data2dim& aData2dim,
                                 const rectangle_t& aRectangle,
                                 const EstimatorBase2dim& aEstimator, 
                                 const uint aNoX, 
                                 const uint aNoY, 
                                 const uint aSubNoX,
                                 const uint aSubNoY,
                                 EstimateEvaluator& aEstimateEvaluator)
               : _noX(aNoX), _noY(aNoY), _subNoX(aSubNoX), _subNoY(aSubNoY),
                 _noQueriesGenerated(0), 
                 _mainWidthX(0), _mainWidthY(0),
                 _rp2d(aRectangle, aNoX * aSubNoX, aNoY * aSubNoY, aData2dim),
                 _estimator(aEstimator),
                 _evaluator(aEstimateEvaluator),
                 _cumfreq(),
                 _data(aData2dim) {
  init();
}

TestRegular2dim::TestRegular2dim(const Data2dim& aData2dim,
                                 const EstimatorBase2dim& aEstimator,
                                 const uint aNoX,
                                 const uint aNoY,
                                 const uint aSubNoX,
                                 const uint aSubNoY,
                                 EstimateEvaluator& aEstimateEvaluator)
               : _noX(aNoX), _noY(aNoY), _subNoX(aSubNoX), _subNoY(aSubNoY),
                 _noQueriesGenerated(0),
                 _mainWidthX(0), _mainWidthY(0),
                 _rp2d(aNoX * aSubNoX, aNoY * aSubNoY, aData2dim),
                 _estimator(aEstimator),
                 _evaluator(aEstimateEvaluator),
                 _cumfreq(),
                 _data(aData2dim) {
  init();
}

TestRegular2dim::~TestRegular2dim() {
}


void
TestRegular2dim::init() {
  const bool lTrace = false;

  _mainWidthX = (_rp2d.maxX() - _rp2d.minX()) / (double) _noX;
  _mainWidthY = (_rp2d.maxY() - _rp2d.minY()) / (double) _noY;

  _cumfreq = _rp2d.matrix();
  _cumfreq.accumulateInclusive();


  if(lTrace) {
    std::cout << "Frequency matrix: " << std::endl;
    _rp2d.printWithRowColSums(std::cout, 5);
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
TestRegular2dim::run(const badquery_t& aPrintBadQueries) {
  // _evaluator.init(); must be initialized before
  // run over main grid, remember each grid point considered must cover a 2x2 tile area of the main grid
  if(20 < noX() && 20 < noY()) {
    std::cout << "run with major grid points." << std::endl;
    for(uint lXmain = 0; lXmain < (noX() - 2); ++lXmain) {
      for(uint lYmain = 0; lYmain < (noY() - 2); ++lYmain) {
        generateQueriesForOneGridPoint(lXmain, lYmain, aPrintBadQueries);
      }
    }
  } else {
    runComplete(aPrintBadQueries);
  }
}

void
TestRegular2dim::runComplete(const badquery_t& aPrintBadQueries) {
  for(uint xlo = 0; xlo < rp2d().noRows(); ++xlo) {
    for(uint xhi = xlo + 2; xhi < rp2d().noRows(); ++xhi) {
      for(uint ylo = 0; ylo < rp2d().noCols(); ++ylo) {
        for(uint yhi = ylo + 2; yhi < rp2d().noCols(); ++yhi) {
          handleQuery(xlo, ylo, xhi, yhi, aPrintBadQueries);
        }
      }
    }
  }
}

void
TestRegular2dim::generateQueriesForOneGridPoint(const uint aXmain, const uint aYmain, 
                                                const badquery_t& aPrintBadQueries) {
  const bool lTrace = false;
  if(lTrace) {
    std::cout << "TestRegular2dim::generateQueriesForOneGridPoint" << std::endl;
  }

  uint xlo, ylo, xhi, yhi;

  for(uint lXLoIdx = 0; lXLoIdx < subNoX(); ++lXLoIdx) {
    for(uint lXHiIdx = lXLoIdx + 2; lXHiIdx < subNoX(); ++lXHiIdx) {
      for(uint lYLoIdx = 0; lYLoIdx < subNoY(); ++lYLoIdx) {
        for(uint lYHiIdx = lYLoIdx + 2; lYHiIdx < subNoY(); ++lYHiIdx) {
          xlo = (aXmain * subNoX()) + lXLoIdx;
          xhi = (aXmain * subNoX()) + lXHiIdx;
          ylo = (aYmain * subNoY()) + lYLoIdx;
          yhi = (aYmain * subNoY()) + lYHiIdx;
          handleQuery(xlo, ylo, xhi, yhi, aPrintBadQueries);

        }
      }
    }
  }
  // _evaluator.fin(); // must be finalized outside
}



void
TestRegular2dim::handleQuery(const uint xlo, const uint ylo, 
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


     #ifdef CHECKCHECK_X
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
TestRegular2dim::getTrueFrequency(const uint xlo, const uint ylo, const uint xhi, const uint yhi) const {
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
TestRegular2dim::mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                            rectangle_t& r) const {
  r._pll.x = _rp2d.minX() + xlo * _rp2d.tileWidthX();
  r._pll.y = _rp2d.minY() + ylo * _rp2d.tileWidthY();
  r._pur.x = _rp2d.minX() + xhi * _rp2d.tileWidthX();
  r._pur.y = _rp2d.minY() + yhi * _rp2d.tileWidthY();
}


} // end namespace

