#ifndef HISTKDIM_DATA2DIM_HH
#define HISTKDIM_DATA2DIM_HH

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "../../../infra/aggregate.hh"
#include "../../../infra/variance.hh"

#include "types.hh"

namespace H2D {

struct xy_t {
  double x;
  double y;
  xy_t(const double xx, const double yy) : x(xx), y(yy) {}
  xy_t() : x(0), y(0) {}
};

std::ostream& operator<<(std::ostream& os, const xy_t& x);
std::istream& operator>>(std::istream& is, const xy_t& x);

struct xyc_t {
  double x;
  double y;
  uint   c; // count
  xyc_t(const double xx, const double yy, const uint cc) : x(xx), y(yy), c(cc) {}
  xyc_t() : x(0), y(0), c(0) {}
};

std::ostream& operator<<(std::ostream& os, const xyc_t& x);
std::istream& operator>>(std::istream& is, const xyc_t& x);


typedef std::vector<xy_t>  xy_vt;
typedef std::vector<xyc_t> xyc_vt;

class Data2dim {
  public:
    // for histograms in ONE dimension
    // needed for bounding rectangles with  width = 0 in exactly one direction
    struct histentry_t {
      double _lo; // lower bound of interval
      double _hi; // higher bound of interval
      uint   _cf; // cumulated frequency
      uint   _noDv; // number of distinct values in bucket

      histentry_t() : _lo(0), _hi(0), _cf(0), _noDv(0) {}
      histentry_t(const histentry_t& e) : _lo(e.lo()), _hi(e.hi()), _cf(e.cf()), _noDv(e.noDv()) {}

      inline double lo() const { return _lo; }
      inline double hi() const { return _hi; }
      inline uint   cf() const { return _cf; }
      inline uint   noDv() const { return _noDv; }
    };
    typedef std::vector<histentry_t>  histentry_vt;
    typedef std::vector<histentry_t>  hist1dim_t;
  public:
    class LessX { public: inline bool operator()(const xyc_t& a, const xyc_t& b) const { return (a.x < b.x); } };
    class LessY { public: inline bool operator()(const xyc_t& a, const xyc_t& b) const { return (a.y < b.y); } };
  public:
    Data2dim() : _data(),_colX(),_colY() {}
    Data2dim(const Data2dim&);
    Data2dim(const Data2dim& aData, const rectangle_t& aRectangle);
  public:
    void init();
    void step(const double& x, const double& y, const uint& c);
    void fin();
  public:
    inline void clear() { _data.clear(); }
    void fillCols(Data2dim& aData);
    inline std::vector<double> getColX() const {return _colX;}
    inline std::vector<double> getColY() const {return _colY;}
  public:
    uint total() const;
    uint total(const uint aBegin, const uint aEnd) const;
    uint maxFrequency() const;

    void readHistFile(const std::string& aHistFile, const size_t aLowLim = 0);
    void readValueFile(const std::string& aValueFile, const size_t aLowLim = 0);
  public:
    void getBoundingRectangle(rectangle_t& aRectangleOut) const; // goes through the data
    void getBoundingLine(line_t& aLineOut, uint dim) const;								 
    void getBoundingRectangle(rectangle_t& aRectangleOut, const uint aBegin, const uint aEnd) const; 
    void getPartitionDescX(partitiondesc_t&, const uint aNx) const;
    void getPartitionDescY(partitiondesc_t&, const uint aNy) const;
    bool getPartitionDescXY(partitiondescxy_t&, const uint aNx, const uint aNy) const;
    bool partition3x3(Data2dim lPartition[3][3], uint lCumFreq[3][3]) const; // returns false if bounding rectangle has area 0
    void partitionX(Data2dim&, Data2dim&, const double aBoundary) const;
    void partitionY(Data2dim&, Data2dim&, const double aBoundary) const;
  public:
    // must be sorted on either x or y
    inline double spreadX(const uint i) const { return (_data[i+1].x - _data[i].x); }
    inline double spreadY(const uint i) const { return (_data[i+1].y - _data[i].y); }
    inline double areaX  (const uint i) const { return (_data[i].c * spreadX(i)); }
    inline double areaY  (const uint i) const { return (_data[i].c * spreadY(i)); }
  public:
    uint countWithin(const rectangle_t& aRectangle) const; // number of points in rectangle
    uint countLine(const line_t& aLine, int dim) const;
  public:
    void split(Data2dim& aRegular, Data2dim& aOutlier, const uint aPhi) const;
    // all points with p.x <= aValue in left, others in right
    void splitX(Data2dim& aLeft, Data2dim& aRight, const double aValue) const;
    // all points with p.y <= aValue in left, others in right
    void splitY(Data2dim& aLeft, Data2dim& aRight, const double aValue) const;
  public:
    // constructing equi-width buckets in one dimension
    // helper: find bucket end
    uint eqdFindEnd(const uint  aBegin, // beginning of new bucket
                    const uint  aEnd,   // maximal end of new bucket (will not belong to bucket)
                    const uint  aCumFreqWish,  // wished frequency per bucket
                          uint& aCumFreqOut) const; // actual frequency within bucket
    // following two methods: build equi-depth histogram, destructive (sort of _data)
    void buildEqdHistX(const uint aBegin, const uint aEnd, const uint aNoBuckets, histentry_vt& aOut);
    void buildEqdHistY(const uint aBegin, const uint aEnd, const uint aNoBuckets, histentry_vt& aOut);
    // use aHist to estimate cardinality of range query [aLo,aHi]
    static double estimateClosed(const double aLo, const double aHi, const histentry_vt& aHist);
    void sortx(const uint aBegin, const uint aEnd);
    void sorty(const uint aBegin, const uint aEnd);
    void sortX(xyc_vt::iterator& aFirst, xyc_vt::iterator& aLast);
    void sortY(xyc_vt::iterator& aFirst, xyc_vt::iterator& aLast);
    uint maxDiffSpreadX(const uint aBegin, const uint aEnd, double& aMaxDiffSpreadX) const;
    uint maxDiffSpreadY(const uint aBegin, const uint aEnd, double& aMaxDiffSpreadX) const;
    uint maxDiffAreaX(const uint aBegin, const uint aEnd, double& aMaxDiffAreaX) const;
    uint maxDiffAreaY(const uint aBegin, const uint aEnd, double& aMaxDiffAreaY) const;
    // find point that minimizes the sum of the variances of left/right part
    uint minVarianceX(const uint aBegin, const uint aEnd, double& aVarianceX) const;
    uint minVarianceY(const uint aBegin, const uint aEnd, double& aVarianceY) const;
    static inline bool lowerIsCloser(uint aLo, uint aHi, uint aGoal) {
                         const uint lDiffLo = (aGoal - aLo);
                         const uint lDiffHi = (aHi - aGoal);
                         return (lDiffLo <= lDiffHi);
                       }
    double variance(const uint aBegin, const uint aEnd) const;
    double sse(const uint aBegin, const uint aEnd) const;
    static void printHist(std::ostream& os, const hist1dim_t& aHist);
  public:
    uint cumFreq(const uint aBegin, const uint aEnd) const;
  public:
    uint size() const { return _data.size(); }
    const xyc_t& operator[](const uint i) const { return _data[i]; }
          xyc_t& operator[](const uint i)       { return _data[i]; }
    const xyc_t& last() const { return _data[_data.size() - 1]; }
    Data2dim& push_back(const xyc_t&);
    Data2dim& push_back(const double, const double, const uint c = 1);
    inline void swap(uint i, uint j) { std::swap<xyc_t>(_data[i], _data[j]); }
  public:
    Data2dim& operator=(const Data2dim&);
  public:
    inline xyc_vt::iterator beginIter() {
      return _data.begin();
    }
    inline xyc_vt::const_iterator beginIter() const {
      return _data.begin();
    }
    inline xyc_vt::iterator endIter() {
      return _data.end();
    }
    inline xyc_vt::const_iterator endIter() const {
      return _data.end();
    }

  private:
    xyc_vt _data;
    std::vector<double> _colX;
    std::vector<double> _colY;
};


} // end namespace


#endif
