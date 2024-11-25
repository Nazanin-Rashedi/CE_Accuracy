#include "TestRandom2dim.hh"

namespace H2D {


TestRandom2dim::TestRandom2dim(const Data2dim& aData2dim,
                               const EstimatorBase2dim& aEstimator, 
                               const uint aNoX, 
                               const uint aNoY, 
                               EstimateEvaluator& aEstimateEvaluator) 
               : _rp2d(aNoX, aNoY, aData2dim),
                 _estimator(aEstimator),
                 _evaluator(aEstimateEvaluator),
                 _cumfreq(),
                 _data(aData2dim),
                 _noQueriesGenerated(0) {
  init();
}

TestRandom2dim::~TestRandom2dim() {
}


void
TestRandom2dim::init() {
  const bool lTrace = false;

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
TestRandom2dim::run(const uint aN, const badquery_t& aPrintBadQueries) {
  srandom(20071962);
  const bool lTrace = false;

  if(lTrace) {
    std::cout << "TestRandom2dim::run" << std::endl;
  }

  rectangle_t lRectangle;
  uint xlo, ylo, xhi, yhi;

  // _evaluator.init(); // must be initialized outside
  for(uint i = 0; i < aN; ++i) {
     ++_noQueriesGenerated;

     getRandomCorners(xlo, ylo, xhi, yhi);
     mkRectangle(xlo, ylo, xhi, yhi, lRectangle);

     const uint32_t lTrueValInt = getTrueFrequency(xlo, ylo, xhi - 1, yhi - 1);
     const double   lTrueVal = (double) lTrueValInt;
     const double   lEstVal  = (double) _estimator.estimate(lRectangle);
     const double   lQError  = q::qerror<double>(std::max<double>(1.0, lTrueVal), 
                                               std::max<double>(1.0, lEstVal));

     #ifdef CHECKCHECK_Z
     // for checking
     const uint32_t lTrueValCounted = _data.countWithin(lRectangle);
     if(lTrueValCounted != lTrueValInt) {
       std::cout << "ERROR: c = " << std::setw(8) << lTrueValCounted << "  but  " 
                 << "      tv = " << std::setw(8) << lTrueValInt     << "  "
                 << "       e = " << std::setw(8) << lEstVal
                 ;
       if( (_rp2d.pdX().min() + xlo * _rp2d.pdX().width()) == lRectangle.xhi() ) {
          std::cout << " HIHIT X";
       }
       if( (_rp2d.pdY().min() + ylo * _rp2d.pdY().width()) == lRectangle.yhi() ) {
          std::cout << " HIHIT Y";
       }
       std::cout << std::endl;
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
  // _evaluator.fin(); // must be finalized outside
}



uint
TestRandom2dim::getTrueFrequency(const uint xlo, const uint ylo, const uint xhi, const uint yhi) const {
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
  return lRes;
}


void
TestRandom2dim::mkRectangle(const uint xlo, const uint ylo, const uint xhi, const uint yhi,
                            rectangle_t& r) const {
  r._pll.x = _rp2d.minX() + xlo * _rp2d.tileWidthX();
  r._pll.y = _rp2d.minY() + ylo * _rp2d.tileWidthY();
  r._pur.x = _rp2d.minX() + xhi * _rp2d.tileWidthX();
  r._pur.y = _rp2d.minY() + yhi * _rp2d.tileWidthY();
}

void
TestRandom2dim::getRandomCorners(uint& xlo, uint& ylo, uint& xhi, uint& yhi) const {
  const uint m = _rp2d.matrix().noRows();
  const uint n = _rp2d.matrix().noCols();
  xlo = random() % (m - 3) + 1;
  ylo = random() % (n - 3) + 1;
  xhi = xlo + 2 + random() % (m - xlo - 2);
  yhi = ylo + 2 + random() % (n - ylo - 2);
}

void
TestRandom2dim::getRandomRectangle(rectangle_t& r) const {
  uint lXlo = 0, lXhi = 0, lYlo = 0, lYhi = 0;
  getRandomCorners(lXlo, lXhi, lYlo, lYhi);
  mkRectangle(lXlo, lXhi, lYlo, lYhi, r);
}



} // end namespace

