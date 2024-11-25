#ifndef SRC_HIST2DIM_TYPES_HH
#define SRC_HIST2DIM_TYPES_HH

#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include <vector>
#include <assert.h>

#include "q.hh"


namespace H2D {


typedef unsigned int uint;
typedef std::vector<uint> uint_vt;


/*
 * enum for all kinds of algorithms
 */


enum H2D_kind_t {
  H2D_RegPart       = 0,
  H2D_EquiDepth     = 1,
  H2D_Sampling      = 2,
  H2D_GridTreeFixed = 3,
  H2D_GridTreeAuto  = 4,
  H2D_GridTreeBest  = 5,
  H2D_MHIST         = 6,
  H2D_VIHIST        = 7,
  H2D_HIRED         = 8,
  H2D_DiscreteCosineTransform = 9,
  H2D_DiscreteCosineTransformOnCumFreq = 10,
  H2D_GENHIST        = 11,
  H2D_NONOTREE       = 12,
  H2D_WAVELET_NONSTD = 13,
  H2D_HFT            = 14,
  H2D_PHASED         = 15,
  H2D_QTS            = 16,
  H2D_IQTS           = 17,
  H2D_FLEXTREE       = 18,
  H2D_GXTREE         = 19,
  H2D_EquiDepth2     = 20,
  H2D_MHIST2         = 21,
  H2D_EST_CONST      = 22,
  H2D_EST_AREA       = 23,
  H2D_ZHist          = 24,
  H2D_XGB	     = 25,
  H2D_EXGB           = 26,
  H2D_LWXGB         = 27,
  H2D_SAMPLALPHA     = 28,
  H2D_NXGB           = 29,
  H2D_INDEP          = 30,
  H2D_NREQD          =31,
  H2D_NO_KIND = 32
};


const std::string& h2d_kind_name(const H2D_kind_t aKind);

/*
 * specification for printing a bad query
 */

struct badquery_t {
  bool   _flag;  // do print bad queries
  double _qmax;  // maximal allowable q
  double _theta; // theta

  badquery_t() : _flag(false), _qmax(100), _theta(100) {}

  inline bool   flag()  const { return _flag; }
  inline double qmax()  const { return _qmax; }
  inline double theta() const { return _theta; }
};

/*
 *  points and rectangles
 */


struct point_t {
  double x;
  double y;
  point_t() : x(0), y(0) {}
  point_t(const double aX, const double aY) : x(aX), y(aY) {}
}
;
inline
std::ostream& operator<<(std::ostream& os, const point_t& p) {
  os << '(' << p.x << ", " << p.y << ')';
  return os;
}

struct line_t {
  double _colLo;
  double _colHi;

  line_t() : _colLo(), _colHi(){}
  line_t(const double aLow, const double aHi) : _colLo(aLow), _colHi(aHi){}


  inline bool containsValue(const double x) const {
                return (_colLo <= x && x <= _colHi);
              }
  inline bool containsHalfOpenValue(const double x) const {
                return (_colLo <= x && x < _colHi);
              }

  inline bool containsLine(const line_t l) const{
  return (_colLo <= l.colLo() && l.colHi()<=_colHi);
  }
  

  inline double colLo() const { return _colLo; }
  inline double colHi() const { return _colHi; }
  inline line_t& lisec(const line_t& a, const line_t& b) {
                        _colLo = std::max<double>(a._colLo, b._colLo);
                        _colHi = std::min<double>(a._colHi, b._colHi);
                        return (*this);
                      }
  inline bool hasZeroLength() const { return (_colLo >= _colHi); }
  inline double length() const { return (_colHi - _colLo); }
}
;


struct rectangle_t {
  point_t _pll; // point in the lower left corner
  point_t _pur; // point in the upper right corner

  rectangle_t() : _pll(), _pur() {}
  rectangle_t(const point_t& aPll, const point_t& aPur) : _pll(aPll), _pur(aPur) {}
  rectangle_t(const double aXlo, const double aYlo, const double aXhi, const double aYhi) : _pll(), _pur() {
     _pll.x = aXlo;
     _pll.y = aYlo;
     _pur.x = aXhi;
     _pur.y = aYhi;
  }
  rectangle_t(const char* aRectangle) : _pll(), _pur() {
    setTo(aRectangle);
  }
  


  inline bool isEmpty() const { return ((_pll.x > _pur.x) | (_pll.y > _pur.y)); }
  inline bool hasZeroArea() const { return ((_pll.x >= _pur.x) | (_pll.y >= _pur.y)); }
  // 00: area > 0, 10: x-width zero 01 y-width zero 11 both zero, i.e. rectangle is a single point
  inline uint theCase() const { return (((_pll.x >= _pur.x) << 1) | (_pll.y >= _pur.y)); }

  inline double xlo() const { return _pll.x; }
  inline double ylo() const { return _pll.y; }
  inline double xhi() const { return _pur.x; }
  inline double yhi() const { return _pur.y; }
  inline void   xlo(const double a) { _pll.x = a; }
  inline void   ylo(const double a) { _pll.y = a; }
  inline void   xhi(const double a) { _pur.x = a; }
  inline void   yhi(const double a) { _pur.y = a; }

  // set to intersection of two other rectangles
  inline rectangle_t& isec(const rectangle_t& a, const rectangle_t& b) {
                        _pll.x = std::max<double>(a._pll.x, b._pll.x);
                        _pll.y = std::max<double>(a._pll.y, b._pll.y);
                        _pur.x = std::min<double>(a._pur.x, b._pur.x);
                        _pur.y = std::min<double>(a._pur.y, b._pur.y);
                        return (*this);
                      }
  inline double area() const { return (_pur.x - _pll.x) * (_pur.y - _pll.y); }

  inline bool containsHalfOpen(const double x, const double y) const {
                return ((_pll.x <= x && _pll.y <= y) &&
                        (_pur.x >  x && _pur.y >  y));
              }


  inline bool contains(const rectangle_t& r) const {
                return ((_pll.x <= r._pll.x) &&
                        (_pll.y <= r._pll.y) &&
                        (_pur.x >= r._pur.x) &&
                        (_pur.y >= r._pur.y));
              }

  inline bool operator==(const rectangle_t& r) const {
                return ((_pll.x == r._pll.x) && 
                        (_pll.y == r._pll.y) && 
                        (_pur.x == r._pur.x) && 
                        (_pur.y == r._pur.y));
              }

  inline bool operator!=(const rectangle_t& r) const {
                 return ((_pll.x != r._pll.x) ||
                         (_pll.y != r._pll.y) ||
                         (_pur.x != r._pur.x) ||
                         (_pur.y != r._pur.y));
              }

  bool setTo(const char* aRectangle);
  bool read(std::istream& aIs);
  std::ostream& print(std::ostream& os) const;
};


struct query_t {
  uint32_t _no;
  uint32_t _card;
  H2D::rectangle_t _rectangle;

  query_t() : _no(), _card(), _rectangle() {}
  inline uint32_t no()   const { return _no; }
  inline uint32_t card() const { return _card; }
  inline const H2D::rectangle_t& rectangle() const { return _rectangle; }
};
typedef std::vector<query_t> query_vt;

inline std::ostream&
operator<<(std::ostream& os, const rectangle_t& x) {
  return x.print(os);
}

// the following struct provides the information needed to partition some data
// the width must be larger than zero
struct partitiondesc_t {
  double _min;
  double _max;
  double _width;
  uint   _anz;
  partitiondesc_t() : _min(0), _max(0), _width(0), _anz(0) {}
  inline void set(double aMin, double aMax, uint aAnz) {
         _min = aMin;
         _max = aMax;
         _anz = aAnz;
         _width = (_max - _min) / ((double) _anz);
  }
  inline uint idx(const double aVal) const {
    const uint lRes = (uint) floor( (aVal - _min) / _width );
    return lRes;
  }
  inline int idxCorrected(const double aVal) const {
    int lIdx = (int) (aVal >= _min) ? floor((aVal - _min) / _width) : -floor((_min - aVal) / _width);
    double lLo = boundary(lIdx);
    double lHi = boundary(lIdx + 1);
    if(lLo > aVal && 0 < lIdx) {
      --lIdx;
    }
    if(lHi <= aVal) {
      ++lIdx;
    }
    return lIdx;
  }

  inline double boundary(uint idx) const {
    return (_min + ((double) idx * _width));
  }
  inline double scaleToZeroMax(const double x, const double aMax) const {
    return ((x - min()) * (aMax / (aMax - min())));
  }
  inline double width() const { return _width; }
  inline double min() const { return _min; }
  inline double max() const { return _max; }
  inline double anz() const { return _anz; }
};



std::ostream& 
operator<<(std::ostream& os, const partitiondesc_t&);


struct partitiondescxy_t {
  partitiondesc_t _pdX;
  partitiondesc_t _pdY;

  partitiondescxy_t() : _pdX(), _pdY() {}
  partitiondescxy_t(const partitiondesc_t& aPdX, const partitiondesc_t& aPdY) : _pdX(aPdX), _pdY(aPdY) {}
  partitiondescxy_t(const rectangle_t& r, const uint nx, const uint ny) : _pdX(), _pdY() {
    set(r.xlo(), r.ylo(), r.xhi(), r.yhi(), nx, ny);
  }

  inline void init(const rectangle_t& r, const uint nx, const uint ny) {
    set(r.xlo(), r.ylo(), r.xhi(), r.yhi(), nx, ny);
  }

  inline void set(const double xlo, const double ylo,
                  const double xhi, const double yhi,
                  const uint nx, const uint ny) {
                _pdX.set(xlo, xhi, nx);
                _pdY.set(ylo, yhi, ny);
              }

  inline void set(const partitiondesc_t& aPdX, const partitiondesc_t& aPdY) {
                _pdX = aPdX;
                _pdY = aPdY;
              }

  inline const partitiondesc_t& pdX() const { return _pdX; }
  inline const partitiondesc_t& pdY() const { return _pdY; }

  // get tile rectangle
  inline void getRectangle(const uint i, const uint j, rectangle_t& aRectangle) const {
                aRectangle._pll.x = _pdX.boundary(i);
                aRectangle._pur.x = _pdX.boundary(i + 1); // inefficient but deterministic
                aRectangle._pll.y = _pdY.boundary(j);
                aRectangle._pur.y = _pdY.boundary(j + 1); // inefficient but deterministic
              }
  inline void getRectangle(const uint aXlo, const uint aXhi, const uint aYlo, const uint aYhi, rectangle_t& aRectangle) const {
                aRectangle._pll.x = _pdX.boundary(aXlo);
                aRectangle._pur.x = _pdX.boundary(aXhi);
                aRectangle._pll.y = _pdY.boundary(aYlo);
                aRectangle._pur.y = _pdY.boundary(aYhi);
              }
  inline void getBoundingRectangle(rectangle_t& aRectangle) const {
                aRectangle._pll.x = minX();
                aRectangle._pur.x = maxX();
                aRectangle._pll.y = minY();
                aRectangle._pur.y = maxY();
              }
  inline double boundaryX(const uint aIdxX) const {
                  return _pdX.boundary(aIdxX);
                }
  inline double boundaryY(const uint aIdxY) const {
                  return _pdY.boundary(aIdxY);
                }
  inline double minX() const { return _pdX.min(); }
  inline double minY() const { return _pdY.min(); }
  inline double maxX() const { return _pdX.max(); }
  inline double maxY() const { return _pdY.max(); }
  inline double widthX() const { return _pdX.width(); }
  inline double widthY() const { return _pdY.width(); }
  inline uint   idxX(const double x) const { return _pdX.idx(x); }
  inline uint   idxY(const double y) const { return _pdY.idx(y); }

  inline int  idxXcorrected(const double aValue) const {
                return _pdX.idxCorrected(aValue);
              }
  inline int  idxYcorrected(const double aValue) const {
                return _pdY.idxCorrected(aValue);
              }

  inline void printRectangle(std::ostream& os) const {
                os << "[(" << minX() << ", " << minY() << "), ("
                   << maxX() << ", " << maxY() << ")]";
              }

};

std::ostream& 
operator<<(std::ostream& os, const partitiondescxy_t&);

struct QC2d16x4 {
  uint64_t _data;

  QC2d16x4() : _data(0) {}
  QC2d16x4(const uint64_t aData) : _data(aData) {}
  QC2d16x4(const QC2d16x4& x) : _data(x._data) {}
  QC2d16x4& operator=(const QC2d16x4& x) { _data = x._data; return (*this); }

  static inline uint32_t shift(const uint32_t i, const uint32_t j) {
                           return (((i << 2) + j) << 2);
                         }
         inline uint32_t get(const uint32_t i, const uint32_t j) const {
                            return (uint32_t) (_data >> shift(i,j)) & 0xF;
                          }
         inline void set(const uint32_t i, const uint32_t j, const uint32_t x) {
                       const uint32_t lShift = shift(i,j);
                       const uint64_t lMask = ~(((uint64_t) 0xF) << lShift);
                       const uint64_t lContent = ((uint64_t) (x & 0xF)) <<  lShift;
                       _data = (_data & lMask) | lContent;
                      }
  inline uint32_t operator()(const uint32_t i, const uint32_t j) const {
                    return get(i,j);
                  }
  inline double getVal(const uint32_t i, const uint32_t j, const double aBase) const {
                  return q::qdecompress<double>(get(i,j), aBase);
                }
  inline void   setVal(const uint32_t i, const uint32_t j, const double aValue, const double aBase) {
                  const uint32_t lValue = q::qcompress<double>(aValue, aBase, 15);
                  set(i, j, lValue);
                }
};

} // end namespace

#endif
