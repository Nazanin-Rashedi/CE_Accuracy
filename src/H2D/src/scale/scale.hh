#ifndef QCOMPR_SCALE_HH
#define QCOMPR_SCALE_HH

#include <iostream>
#include <iomanip>
#include <vector>
#include <assert.h>
#include <inttypes.h>
#include <limits>

#include "infra/q.hh"
#include "infra/tmath.hh"

namespace q {


/*
 * Scale_S
 * materialized compression/decompression scheme for small numbers
 * use for compression schemes with 2 to 4 bits (both inclusive)
 */

class Scale_S {
  public:
    typedef std::vector<uint32_t> uint_vt;
    typedef std::vector<double>   double_vt;
  public:
    Scale_S();
  public:
    inline uint32_t compress(const uint32_t i) const;
    inline uint32_t decompress(const uint32_t i) const;
    inline double   decompressDouble(const uint32_t i) const;
    inline uint32_t noBits() const { return _noBits; }
    inline uint32_t maxNo() const { return _compr.size() - 1; }
    inline uint32_t limit() const { return _compr.size() - 1; }
    inline uint32_t limit1() const { return _limit1; }
    inline uint32_t limit2() const { return _limit2; }
  public:
    void   autofill_2_1();
    void   autofill_2_2();
    void   autofill_3_1();
    void   autofill_3_2();
    void   autofill_3_3();
    void   autofill_3_4();
    void   autofill_4_1();
    void   autofill_4_2();
    void   autofill_5_1();
    void   autofill_5_2();
  public:
    void   init(const uint aNoBits, const uint_vt& aBoundaries);
    void   initPartial(const uint_vt& aBoundaries);
  public:
    double qerror(const uint32_t aTheta = 1) const; // calculates q-error of scale, ignores entry leq theta
    double qerrorDouble(const uint32_t aTheta = 1) const; 
    double uniformityMeasure() const;
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    uint32_t  _noBits;  // number of bits used to store a single number
    uint_vt   _compr;   // for compression
    uint_vt   _decompr; // for decompression
    double_vt _decomprDouble; // for decompression
    uint32_t  _limit1;  // limit if 1111 is reserved
    uint32_t  _limit2;  // limit if 1111 and 1110 are reserved
};




uint32_t
Scale_S::compress(const uint32_t i) const {
  if(i >= _compr.size()) {
    return _compr[maxNo()];
  }
  return _compr[i];
}

uint32_t
Scale_S::decompress(const uint32_t i) const {
  return _decompr[i];
}

double
Scale_S::decompressDouble(const uint32_t i) const {
  return _decomprDouble[i];
}

/*
 *  Scale_6
 *  special scale for 6 bits
 */

class Scale_6 {
  public:
    typedef std::vector<uint32_t> uint_vt;
    typedef std::vector<double>   double_vt;
  public:
    Scale_6(const double aBase);
  public:
    inline uint32_t noBits() const { return _noBits; }
    inline uint32_t maxCode() const { return _maxCode; }
    inline uint32_t limit() const { return _limit; }
    inline double   base() const { return _base; }
    inline int      diff() const { return _diff; }
  public:
    inline uint32_t compress(const uint32_t aNumber) const;
    inline uint32_t decompress(const uint32_t aCode) const;
    inline double   decompressDouble(const uint32_t aCode) const;
  public:
    double qerror(const uint32_t aBegin, const uint32_t aEnd) const;
  private:
    void autofill();
  public:
    std::ostream& print(std::ostream& os, const uint32_t aBegin, const uint32_t aEnd) const;
  private:
    uint32_t   _noBits;  // number of bits used to store a single number
    uint32_t   _maxCode; // max code
    uint32_t   _limit;   // maximally compressable number
    double     _base;    // base for log
    int        _diff;    // saved stuff
    uint_vt    _decompr; // for decompression
    double_vt  _decomprD; // for decompression
};

uint32_t
Scale_6::compress(const uint32_t aNumber) const {
  if(10 > aNumber) {
    return aNumber;
  }
  return q::qcompress<double>(aNumber, base(), maxCode() + diff()) - diff();
}


uint32_t
Scale_6::decompress(const uint32_t aCode) const {
  if(10 > aCode) {
    return aCode;
  }
  return _decompr[aCode];
}

double
Scale_6::decompressDouble(const uint32_t aCode) const {
  if(10 > aCode) {
    return (double) aCode;
  }
  return _decomprD[aCode];
}



/*
 * ScaleM
 * use for compression schemes with 5 to 8 bits (both inclusive)
 */

class ScaleM {
  public:
    typedef std::vector<uint32_t> uint_vt;
    typedef std::vector<double>   double_vt;
  public:
    ScaleM(const uint32_t aNoBits, const double aBase);
  public:
    inline uint32_t noBits() const { return _noBits; }
    inline uint32_t maxCode() const { return _maxCode; }
    inline uint32_t limit() const { return _limit; }
    inline double   base() const { return _base; }
    inline int      diff() const { return _diff; }
  public:
    inline uint32_t compress(const uint32_t aNumber) const;
    inline uint32_t decompress(const uint32_t aCode) const;
    inline double   decompressDouble(const uint32_t aCode) const;
  public:
    double qerror(const uint32_t aBegin, const uint32_t aEnd) const;
    
  public:
    std::ostream& print(std::ostream& os, const uint32_t aBegin, const uint32_t aEnd) const;
  private:
    void autofill();
  private:
    uint32_t   _noBits;  // number of bits used to store a single number
    uint32_t   _maxCode; // max code
    uint32_t   _limit;   // maximally compressable number
    double     _base;    // base for log
    double     _diff;    // difference between needed and required
    uint_vt    _decompr; // for decompression
    double_vt  _decomprD; // for decompression
};



uint32_t
ScaleM::compress(const uint32_t aNumber) const {
  if(10 > aNumber) {
    return aNumber;
  }       
  return q::qcompress<double>(aNumber, base(), maxCode() + diff()) - diff();
}         
         
uint32_t
ScaleM::decompress(const uint32_t aCode) const {
  if(10 > aCode) {
    return aCode;
  } 
  return _decompr[aCode];
}
    
double 
ScaleM::decompressDouble(const uint32_t aCode) const {
  if(10 > aCode) {
    return (double) aCode;
  }
  return _decomprD[aCode];
}



/*
 * Scale_L
 * use to compress/decompress with 7/8 bits
 * materializes only decompression
 */

class Scale_L {
  public:
    typedef std::vector<uint32_t> uint_vt;
    typedef std::vector<double>   double_vt;
  public:
    Scale_L(const uint32_t aNoBits, const double aBase, const uint32_t aNoMat);
  public:
    inline uint32_t noBits() const { return _noBits; }
    inline uint32_t maxCode() const { return _maxCode; }
    inline uint32_t limit() const { return _decompr[maxCode()]; }
    inline double   limitDouble() const { return _decomprD[maxCode()]; }
    inline double   base() const { return _base; }
  public:
    inline uint32_t compress(const uint32_t aNumber) const;
    inline uint32_t decompress(const uint32_t aCode) const;
    inline double   decompressDouble(const uint32_t aCode) const;
  public:
    double qerror(const uint32_t aBegin, const uint32_t aEnd) const;
  private:
    uint32_t  _noBits;
    uint32_t  _maxCode;
    uint32_t  _limit;
    double    _base;
    uint_vt   _compr;
    uint_vt   _decompr;
    double_vt _decomprD;
};    


uint32_t
Scale_L::compress(const uint32_t aNumber) const {
  if(aNumber < _compr.size()) {
    return _compr[aNumber];
  }
  return q::qcompress<double>(aNumber, base(), maxCode());
}


uint32_t
Scale_L::decompress(const uint32_t aCode) const {
  return _decompr[aCode];
}

double
Scale_L::decompressDouble(const uint32_t aCode) const {
  return _decomprD[aCode];
}


/*
 *  class ScaleMgr contains all scales, singleton
 */


class ScaleMgr {
  public:
    enum scale_et {
       SCALE_S_2_1 =  0,
       SCALE_S_2_2 =  1,
       SCALE_S_3_1 =  2,
       SCALE_S_3_2 =  3,
       SCALE_S_3_3 =  4,
       SCALE_S_3_4 =  5,
       SCALE_S_4_1 =  6,
       SCALE_S_4_2 =  7,
       SCALE_S_5_1 =  8,
       SCALE_S_5_2 =  9,
       SCALE_6_6_1 = 10,
       SCALE_6_6_2 = 11,
       SCALE_6_6_3 = 12,
       SCALE_L_7_1 = 13,
       SCALE_L_7_2 = 14,
       SCALE_L_8_1 = 15,
       SCALE_L_8_2 = 16,
       SCALE_NO_SCALE = 17
    };
  private:
    ScaleMgr();
  public:
    static ScaleMgr* instance();
  public:                                                              // q-error
    inline const Scale_S* scale_S_2_1() const { return _scale_S_2_1; } // 1.44
    inline const Scale_S* scale_S_2_2() const { return _scale_S_2_2; } // 2.0
    inline const Scale_S* scale_S_3_1() const { return _scale_S_3_1; } // 1.11
    inline const Scale_S* scale_S_3_2() const { return _scale_S_3_2; } // 1.2
    inline const Scale_S* scale_S_3_3() const { return _scale_S_3_3; } // 1.3
    inline const Scale_S* scale_S_3_4() const { return _scale_S_3_4; } // 1.XXX
    inline const Scale_S* scale_S_4_1() const { return _scale_S_4_1; } // 1.1
    inline const Scale_S* scale_S_4_2() const { return _scale_S_4_2; } // 1.15
    inline const Scale_S* scale_S_5_1() const { return _scale_S_5_1; } // 1.1
    inline const Scale_S* scale_S_5_2() const { return _scale_S_5_2; } // 1.18
    inline const Scale_6* scale_6_6_1() const { return _scale_6_6_1; } // base 1.20
    inline const Scale_6* scale_6_6_2() const { return _scale_6_6_2; } // base 1.30
    inline const Scale_6* scale_6_6_3() const { return _scale_6_6_3; } // base 1.40
    inline const Scale_L* scale_L_7_1() const { return _scale_L_7_1; } // base 1.10
    inline const Scale_L* scale_L_7_2() const { return _scale_L_7_2; } // base 1.20
    inline const Scale_L* scale_L_8_1() const { return _scale_L_8_1; } // base 1.08
    inline const Scale_L* scale_L_8_2() const { return _scale_L_8_2; } // base 1.10
  private:
    Scale_S* _scale_S_2_1;
    Scale_S* _scale_S_2_2;
    Scale_S* _scale_S_3_1;
    Scale_S* _scale_S_3_2;
    Scale_S* _scale_S_3_3;
    Scale_S* _scale_S_3_4;
    Scale_S* _scale_S_4_1;
    Scale_S* _scale_S_4_2;
    Scale_S* _scale_S_5_1;
    Scale_S* _scale_S_5_2;
    Scale_6* _scale_6_6_1;
    Scale_6* _scale_6_6_2;
    Scale_6* _scale_6_6_3;
    Scale_L* _scale_L_7_1;
    Scale_L* _scale_L_7_2;
    Scale_L* _scale_L_8_1;
    Scale_L* _scale_L_8_2;
  private:
    static ScaleMgr* _instance;
};

} // end namespace

#endif
