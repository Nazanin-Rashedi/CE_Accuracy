#include "scale.hh"


namespace q {


Scale_S::Scale_S() : _noBits(0), _compr(), _decompr(), _decomprDouble(), _limit1(0), _limit2(0) {}


// initializes everything 
void
Scale_S::init(const uint32_t aNoBits, const uint_vt& s) {
  const size_t   lIdxLast = s.size() - 1;
  const uint32_t lDecompSize = ((uint32_t) 1 << aNoBits);
  assert((s.size() - 1) == lDecompSize);
  _noBits = aNoBits;
  _compr.resize(s[lIdxLast]);
  _decompr.resize(lDecompSize);
  _decomprDouble.resize(lDecompSize);

  // initialize _compr
  for(size_t i = 1; i < s.size(); ++i) {
    const uint32_t lBegin = s[i-1];
    const uint32_t lEnd   = s[i];
    const uint32_t lEnc   = i - 1;  // the encoding for the interval [lBegin,lEnd[
    for(uint32_t j = lBegin; j < lEnd; ++j) {
      _compr[j] = lEnc;
    }
  }
 
  // initialize _decompDouble
  for(size_t i = 0; i < lDecompSize; ++i) {
    _decomprDouble[i] = mt::sqrtt<double>(((double) s[i+1] - 1) * ((double) s[i]));
  }

  // initialize _decomp 
  for(size_t i = 0; i < lDecompSize; ++i) {
    _decompr[i] = (uint32_t) mt::roundt<double>(_decomprDouble[i]);
  }
}

// initialize only the beginning of _compr and _decompr
void
Scale_S::initPartial(const uint_vt& s) {
  const uint32_t lDecompSize = s.size() - 1;

  // initialize _compr
  for(size_t i = 1; i < s.size(); ++i) {
    const uint32_t lBegin = s[i-1];
    const uint32_t lEnd   = s[i];
    const uint32_t lEnc   = i - 1;  // the encoding for the interval [lBegin,lEnd[
    for(uint32_t j = lBegin; j < lEnd; ++j) {
      _compr[j] = lEnc;
    }
  }

  // initialize _decompDouble
  for(size_t i = 0; i < lDecompSize; ++i) {
    _decomprDouble[i] = mt::sqrtt<double>(((double) s[i+1] - 1) * ((double) s[i]));
  }

  // initialize _decomp
  for(size_t i = 0; i < lDecompSize; ++i) {
    _decompr[i] = (uint32_t) mt::roundt<double>(_decomprDouble[i]);
  }
}


// minimizes qerror, but has smaller maximal number to be compressable
// qerror <= 2.0 for small numbers,
// qerror <= 1.5 for larger numbers
// maximal compressable number of 39

void
Scale_S::autofill_2_1() {
  // s = 0 1 5 17 40
  const uint32_t lNoBits = 2;
  const uint_vt& s = {0, 1, 5, 17, 40};
  init(lNoBits, s);
  _limit1 = 10;
  _limit2 = 2;
}


void
Scale_S::autofill_2_2() {
  const uint32_t lNoBits = 2;
  const uint_vt& s = {0, 1, 5, 21, 85};
  init(lNoBits, s);
  _limit1 = 10;
  _limit2 = 2;
}


void
Scale_S::autofill_3_4() {
  const uint32_t lNoBits = 3;
  const uint_vt& s = {0, 1, 4, 17, 37, 82, 159, 310, 608 };
  init(lNoBits, s);
  _limit1 = _decompr[6];
  _limit2 = _decompr[5];
}


// keep error small for larger numbers
// max qerror <= 2
// above 10: <= 1.5
// above 28: <= 1.3

void
Scale_S::autofill_3_3() {
  const uint32_t lNoBits = 3;
  const uint_vt& s = {0, 1, 4, 13, 29, 49, 82, 137, 232 };
  init(lNoBits, s);
  _limit1 = _decompr[6];
  _limit2 = _decompr[5];
}

void
Scale_S::autofill_3_2() {
  const uint32_t lNoBits = 3;
  const uint_vt& s = {0, 1, 4, 13, 20, 29, 43, 62, 89 };
  init(lNoBits, s);
  _limit1 = _decompr[6];
  _limit2 = _decompr[5];
}

// keep error small for larger numbers
// max qerror <= 2
// above 10: <= 1.7143
// above 20: <= 1.2069
void
Scale_S::autofill_3_1() {
  const uint32_t lNoBits = 3;
  const uint_vt& s = {0, 1, 4, 9, 16, 25, 34, 42, 51 };
  init(lNoBits, s);
  _limit1 = _decompr[6];
  _limit2 = _decompr[5];
}

// most precise
//  qerror fuer theta= 0: 1.25
//  qerror fuer theta=10: 1.18182
//  qerror fuer theta=15: 1.11429
// maximal number: 97
void
Scale_S::autofill_4_1() {
  const uint32_t lNoBits = 4;
  const uint_vt& s = { 0,  
                       1,  2,  3,  4, 
                       7, 10, 14, 18, 
                      23, 28, 35, 44, 
                      54, 66, 80, 98 };
  init(lNoBits, s);
  _limit1 = 80;
  _limit2 = 66;
}



// most precise
//  qerror fuer theta= 0: 2
//  qerror fuer theta=10: 1.42857
//  qerror fuer theta=15: 1.22222
//  qerror fuer theta=20: 1.1875
//  qerror fuer theta=30: 1.1875
//  qerror fuer theta=40: 1.15385
// maximal number: 497
void
Scale_S::autofill_4_2() {
  const uint32_t lNoBits = 4;
  const uint_vt& s = {  0,  
                        1,   2,   5,  11, 
                       18,  27,  39,  52, 
                       70,  92, 123, 163, 
                      215, 284, 375, 498 };
  init(lNoBits, s);
  _limit1 = 375;
  _limit2 = 284;
}


void
Scale_S::autofill_5_1() {
  _noBits = 5;
  _decompr.resize(32);
  _decomprDouble.resize(32);

  const uint32_t lLimit = 1965;
  _compr.resize(lLimit);

  const uint_vt& s = {  0,  
                        1,   2,   3,   4, 
                        7,  10,  14,  18, 
                       23,  28,  35,  44, 
                       54,  66,  80,  98 };
  initPartial(s);

  uint32_t j    = 98;
  uint32_t x    = 98;
  uint32_t lLim =  0;
  for(uint32_t i = 16; i < 32; ++i) {
    // std::cout << "A: i = " << i << ", j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;
    x = (uint32_t) floor( (double) x * (double) 1.1 );
    _decompr[i] = x;
    lLim = (uint32_t) floor( (double) x * (double) 1.1 );
    while(j < lLim && j < lLimit) {
      _compr[j++] = i;
    }
    x  = lLim;
    // std::cout << "E: i = " << i << ", j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;
  }
  // std::cout << "O: j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;

  for(size_t i = 0; i < _decompr.size(); ++i) {
    _decomprDouble[i] = _decompr[i];
  }

  _limit1 = (uint) floor(((double) limit()) / 1.1);
  _limit2 = (uint) floor(((double) _limit1) / 1.1);

}

void
Scale_S::autofill_5_2() {
  _noBits = 5;
  _decompr.resize(32);
  _decomprDouble.resize(32);

  const uint32_t lLimit = 18905;
  _compr.resize(lLimit);

  const uint_vt& s = {  0,  
                        1,   2,   3,   4, 
                        7,  10,  14,  18, 
                       23,  28,  35,  44, 
                       54,  66,  80,  98 };
  initPartial(s);

  uint32_t j    = 98;
  uint32_t x    = 98;
  uint32_t lLim =  0;
  for(uint32_t i = 16; i < 32; ++i) {
    // std::cout << "A: i = " << i << ", j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;
    x = (uint32_t) floor( (double) x * (double) 1.18 );
    _decompr[i] = x;
    lLim = (uint32_t) floor( (double) x * (double) 1.18 );
    while(j < lLim && j < lLimit) {
      _compr[j++] = i;
    }
    x  = lLim;
    // std::cout << "E: i = " << i << ", j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;
  }
  // std::cout << "Scale_S::autofill_5_2: O: j = " << j << ", x = " << x << ", lLim = " << lLim << std::endl;

  for(size_t i = 0; i < _decompr.size(); ++i) {
    _decomprDouble[i] = _decompr[i];
  }

  _limit1 = (uint) floor(((double) limit()) / 1.18);
  _limit2 = (uint) floor(((double) _limit1) / 1.18);

}

double
Scale_S::qerror(const uint32_t aTheta) const {
  double lRes = 0;
  for(uint32_t i = aTheta; i < _compr.size(); ++i) {
    const double x = i;
    const double y = decompress(compress(i));
    const double lQError = (0 == i) ? 1.0 : q::qerrorT1<double>(x,y);
    if(lRes < lQError) {
      lRes = lQError;
    }
  }
  return lRes;
}

double
Scale_S::qerrorDouble(const uint32_t aTheta) const {
  double lRes = 0;
  for(uint32_t i = aTheta; i < _compr.size(); ++i) {
    const double x = i;
    const double y = decompressDouble(compress(i));
    const double lQError = (0 == i) ? 1.0 : q::qerrorT1<double>(x,y);
    if(lRes < lQError) {
      lRes = lQError;
    }
  }
  return lRes;
}

double
Scale_S::uniformityMeasure() const {
  double lSum1 = 0;
  double lSum2 = 0;
  for(uint32_t i = 1; i < _compr.size(); ++i) {
    lSum1 += i;
    lSum2 += decompress(compress(i));
  }
  std::cout << "uniformityMeasure: lSum1 = " << lSum1 << ", lSum2 = " << lSum2 << std::endl;
  return q::qerrorT1<double>(lSum1, lSum2);
}

std::ostream&
Scale_S::print(std::ostream& os) const {
  std::cout << "max qerror: " << qerror() << std::endl
            << "uniformity: " << uniformityMeasure() << std::endl
            << std::endl;

  for(uint32_t i = 0; i < _compr.size(); ++i) {
    const double x = i;
    const double y = decompress(compress(i));
    const double lQError = (0 == i) ? 1.0 : q::qerrorT1<double>(x, y);
    os << std::setw(4) << i << ' '
       << std::setw(4) << compress(i) << ' '
       << std::setw(8) << y << ' '
       << std::setw(12) << (round(lQError * 100.0) / 100.0)
       << std::endl;
   
  }

  return os;
}

/*
 * start of Scale_6
 */

Scale_6::Scale_6(const double aBase) 
       : _noBits(6), _maxCode((1 << 6) - 1), _limit(0), _base(aBase), _diff(0),
         _decompr(1 << 6), _decomprD(1 << 6) {
  autofill();
}




void
Scale_6::autofill() {
  for(uint32_t i = 0; i < 10; ++i) {
    _decompr[i]  = i;
    _decomprD[i] = i;
  }

  
  const int lCodeOf10 = (int) q::qcompress<double>(10.0, base(), maxCode());
  // std::cout << "code of 10: " << lCodeOf10 << std::endl;
  _diff = lCodeOf10 - 10;

  _limit = pow(base(), maxCode() + diff());

  for(uint32_t c = 10; c <= maxCode(); ++c) {
    const double x = q::qdecompress<double>(c + diff(), base());
    _decompr[c]  = (uint32_t) mt::roundt<double>(x);
    _decomprD[c] = x;

    // std::cout << std::setw( 4) <<  c << ' '
    //           << std::setw(12) << x << ' '
    //           << std::setw(12) << pow(base(), c + diff()) << ' '
    //           << std::endl;

  }
}


double
Scale_6::qerror(const uint32_t aBegin, const uint32_t aEnd) const {
  double lRes = 0;
  // std::cout << "q6 "
  //           << std::setw(8) << "in" << ' '
  //           << std::setw(5) << "code" << ' '
  //           << std::setw(12) << "out" << ' '
  //           << std::setw(12) << "qerr" << ' '
  //           << std::endl;
  for(uint32_t i = aBegin; i < aEnd; ++i) {
     const double   x = (double) i;
     const uint32_t c = compress(i);
     const double   y = decompressDouble(c);
     const double   lQError = q::qerrorT1<double>(x,y);
     if(lRes < lQError) {
       lRes = lQError;
     }
     // std::cout << "q6 "
     //           << std::setw(8) << x << ' '
     //           << std::setw(5) << c << ' '
     //           << std::setw(12) << y << ' '
     //           << std::setw(12) << lQError << ' '
     //           << std::endl;
  }
  return lRes;
}


/*
 * start of ScaleM
 */


ScaleM::ScaleM(const uint32_t aNoBits, const double aBase) 
       : _noBits(aNoBits), 
         _maxCode((1 << aNoBits) - 1),
         _limit(0),
         _base(aBase), _diff(0), _decompr(1 << noBits()), _decomprD(1 << noBits()) {
  assert(3 < noBits());
  autofill();
}


void
ScaleM::autofill() {
  for(uint32_t i = 0; i < 10; ++i) {
    _decompr[i]  = i;
    _decomprD[i] = i;
  }

  const int lCodeOf10 = (int) q::qcompress<double>(10.0, base(), maxCode());
  // std::cout << "code of 10: " << lCodeOf10 << std::endl;
  _diff = lCodeOf10 - 10;

  _limit = pow(base(), maxCode() + diff());

  for(uint32_t c = 10; c <= maxCode(); ++c) {
    const double x = q::qdecompress<double>(c + diff(), base());
    _decompr[c]  = (uint32_t) mt::roundt<double>(x);
    _decomprD[c] = x;

    // std::cout << std::setw( 4) <<  c << ' '
    //           << std::setw(12) << x << ' '
    //           << std::setw(12) << pow(base(), c + diff()) << ' '
    //           << std::endl;

  }

}


double
ScaleM::qerror(const uint32_t aBegin, const uint32_t aEnd) const {
  double lRes = 0;
  for(uint32_t i = aBegin; i < aEnd; ++i) {
     const double   x = (double) i;
     const uint32_t c = compress(i);
     const double   y = decompress(c);
     const double   lQError = q::qerrorT1<double>(x,y);
     if(lRes < lQError) {
       lRes = lQError;
     }
  }
  return lRes;
}

std::ostream&
ScaleM::print(std::ostream& os, const uint32_t aBegin, const uint32_t aEnd) const {
  for(uint32_t i = aBegin; i < aEnd; ++i) {
    const double   x = (double) i;
    const uint32_t c = compress(i);
    const uint32_t y = decompress(c);
    const double lQError = q::qerrorT1<double>(x, (double) y);
    std::cout << std::setw(7) << i << ' '
              << std::setw(7) << c << ' '
              << std::setw(7) << y << ' '
              << std::setw(12) << lQError << ' '
              << std::endl;
  }

  return os;
}

/*
 * Stuff for Scale_L
 */

Scale_L::Scale_L(const uint32_t aNoBits, const double aBase, const uint32_t aNoMat) 
       : _noBits(aNoBits), 
         _maxCode((1 << aNoBits) - 1),
         _limit(0),
         _base(aBase), 
         _compr(aNoMat),
         _decompr(1 << aNoBits),
         _decomprD(1 << aNoBits) {
  const uint32_t lMax = std::numeric_limits<uint32_t>::max();
  const double   lMaxD = (double) lMax;

  // fill _compr
  for(uint32_t i = 0; i < _compr.size(); ++i) {
    _compr[i] = q::qcompress<double>(i, base(), maxCode());

    // const double x = (double) i;
    // const double c = _compr[i];
    // const double y = q::qdecompress(c, base());
    // std::cout << std::setw( 4) << i << ' '
    //           << std::setw(12) << c << ' '
    //           << std::setw(12) << x << ' '
    //           << std::setw(12) << y << ' '
    //           << std::setw(12) << q::qerrorT1<double>(x,y) << ' '
    //           << std::endl;
  }

  // fill _decompr and _decomprD
  for(uint32_t i = 0; i <= maxCode(); ++i) {
    const double x = q::qdecompress<double>(i, base());
    _decomprD[i] = x;
    const double y = round(x);
    if(y < lMaxD) {
      _decompr[i] = (uint32_t) y;
    } else {
      _decompr[i] = lMax;
    }
    // std::cout << std::setw( 4) << i << ' '
    //           << std::setw(12) << x << ' '
    //           << std::setw(12) << y << ' '
    //           << std::setw(12) << q::qcompress<double>(y, base(), maxCode()) << ' '
    //           << std::endl;
  }
}

double
Scale_L::qerror(const uint32_t aBegin, const uint32_t aEnd) const {
  double lRes = 0;
  for(uint32_t i = aBegin; i < aEnd; ++i) {
     const double   x = (double) i;
     const uint32_t c = compress(i);
     const double   y = decompressDouble(c);
     const double   lQError = q::qerrorT1<double>(x,y);
     if(lRes < lQError) {
       lRes = lQError;
     }
     // std::cout << 'q'
     //           << std::setw(12) << i << ' '
     //           << std::setw(12) << x << ' '
     //           << std::setw(12) << c << ' '
     //           << std::setw(12) << y << ' '
     //           << std::setw(12) << lQError << ' '
     //           << std::endl;
  }
  return lRes;
}

ScaleMgr::ScaleMgr()
         : _scale_S_2_1(0), _scale_S_2_2(0),
           _scale_S_3_1(0), _scale_S_3_2(0), _scale_S_3_3(0), _scale_S_3_4(0),
           _scale_S_4_1(0), _scale_S_4_2(0),
           _scale_S_5_1(0), _scale_S_5_2(0),
           _scale_6_6_1(0), _scale_6_6_2(0), _scale_6_6_3(0),
           _scale_L_7_1(0), _scale_L_7_2(0),
           _scale_L_8_1(0), _scale_L_8_2(0) {
  _scale_S_2_1 = new Scale_S; _scale_S_2_1->autofill_2_1();
  _scale_S_2_2 = new Scale_S; _scale_S_2_2->autofill_2_2();
  _scale_S_3_1 = new Scale_S; _scale_S_3_1->autofill_3_1();
  _scale_S_3_2 = new Scale_S; _scale_S_3_2->autofill_3_2();
  _scale_S_3_3 = new Scale_S; _scale_S_3_3->autofill_3_3();
  _scale_S_3_4 = new Scale_S; _scale_S_3_4->autofill_3_4();
  _scale_S_4_1 = new Scale_S; _scale_S_4_1->autofill_4_1();
  _scale_S_4_2 = new Scale_S; _scale_S_4_2->autofill_4_2();
  _scale_S_5_1 = new Scale_S; _scale_S_5_1->autofill_5_1();
  _scale_S_5_2 = new Scale_S; _scale_S_5_2->autofill_5_2();
  _scale_6_6_1 = new Scale_6(1.20);
  _scale_6_6_2 = new Scale_6(1.30);
  _scale_6_6_3 = new Scale_6(1.40);
  _scale_L_7_1 = new Scale_L(7, 1.10, 100);
  _scale_L_7_2 = new Scale_L(7, 1.20, 100);
  _scale_L_8_1 = new Scale_L(8, 1.08, 100);
  _scale_L_8_2 = new Scale_L(8, 1.10, 100);

}

ScaleMgr*
ScaleMgr::instance() {
  if(0 == _instance) {
    _instance = new ScaleMgr;
  }
  return _instance;
}

ScaleMgr* ScaleMgr::_instance = 0;


} // namespace


