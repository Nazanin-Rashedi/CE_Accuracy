#ifndef HISTKDIM_REGULARPARTITIONING_HH
#define HISTKDIM_REGULARPARTITIONING_HH

#include <iostream>
#include <iomanip>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>
#include <limits>

#include "infra/tmath.hh"
#include "infra/aggregate.hh"
#include "infra/matrix.hh"
#include "infra/q.hh"

#include "types.hh"
#include "util.hh"
#include "data2dim.hh"
#include "EstimatorBase2dim.hh"


namespace H2D {


/*
 * class RegularPartitioning2dim
 * calculates the 2-dimensional regular partitioning
 * of a given Data2dim object with nx partitions in x-direction and
 * ny partitions in y-direction.
 */

class RegularPartitioning2dim : public EstimatorBase2dim {
  private:
    RegularPartitioning2dim(const RegularPartitioning2dim&);
    RegularPartitioning2dim& operator=(const RegularPartitioning2dim&);
  public:
    RegularPartitioning2dim();
    RegularPartitioning2dim(const uint nx, const uint ny, const Data2dim&);
    // in case a bounding rectangle is known, use
    RegularPartitioning2dim(const rectangle_t& aRectangle, const uint nx, const uint ny, const Data2dim&);
    ~RegularPartitioning2dim();
  public:
    void initFromData2dim(const uint nx, const uint ny, const Data2dim&);
    void initFromData2dim(const rectangle_t& aRectangle, const uint nx, const uint ny, const Data2dim&);
    void initFromData2dim(const Data2dim&, const uint aBegin, const uint aEnd, const uint nx, const uint ny);
  public:
    void initFromMatrix(const partitiondesc_t& aPdX,
                        const partitiondesc_t& aPdY,
                        const uint             aTotal,
                        const Matrix&          aMatrix); // only part of the functionality can be used here
  public:
    // size of input relation (as calculated by sum of frequencies)
    inline uint total() const { return _total; }
    // estimate of tile frequency under attribute value independence assumption
    inline double avi_estimate(const uint i, const uint j) const {
                    return std::max<double>(1.0, round( ((double) (_vx[i] * _vy[j])) / total() ) );
                  }
    double max_avi_estimate_qerror() const;
    double max_avi_estimate_qerror(const double aTheta) const;
  public:
    double globalDistortion() const; // max_{t1,t2} ||F^+(t1) / F^+(t2)||_Q
    // calculate the distortion 
    // X := \{_m(i,j),...,_m(i+3,j_3)\}
    // distortion(i,j) := max_{x,y \in X} \Qnorm{x/y}
    // the next two should be a calculated 4x4 refinement of the RegP for which distortion is to be calculated
    double maxInnerDistortion(const uint aLimit = 32, uint* aNoGtLimitOut = 0) const; 
    double distortion(const uint i, const uint j, const uint aLimit = 32, bool* aIsGtLimitOut = 0) const;
    // more general version: each tile is subdivided into a kxl grid
    double distortion(const uint aIdxX, const uint aIdxY, const uint aSubX, const uint aSubY) const;
    // a tile in some pxo grid is subdivided into 4x4 subtiles (as for class M)
    // each subtile is subdivided into 2x2 subtiles to calculate the max distortion of the subtiles
    // here: a pxo grid is subdivided into aNoSubX times aNoSubY subtiles
    //       each subtile is subdivided into aNoSubSubX times aNoSubSuby subtiles
    double distortion(const uint aIdxX, const uint aIdxY, 
                      const uint aNoSubX, const uint aNoSubY,
                      const uint aNoSubSubX, const uint aNoSubSubY) const;
  public:
    inline uint noRows() const { return _m.noRows(); }
    inline uint noCols() const { return _m.noCols(); }
    inline uint nx() const { return _m.noRows(); }
    inline uint ny() const { return _m.noCols(); }
    inline double tileWidthX() const { return _descX._width; }
    inline double tileWidthY() const { return _descY._width; }
    inline double minX() const { return _descX._min; }
    inline double maxX() const { return _descX._max; }
    inline double minY() const { return _descY._min; }
    inline double maxY() const { return _descY._max; }
    inline const Matrix& matrix() const { return _m; }
    inline const partitiondesc_t& pdX() const { return _descX; }
    inline const partitiondesc_t& pdY() const { return _descY; }
    inline void  getPdXY(partitiondescxy_t& aPdXY) const {
                   aPdXY.set(pdX(), pdY());
                 }
           uint  max() const; // determine maximum tile frequency
           uint  min() const; // determine minimum tile frequency
           uint  noGtLimit(const uint aLimit) const;
           uint  noGtLimit(const uint aLimit, const uint aNxSub, const uint aNySub) const;
    inline uint  operator()(const uint i, const uint j) const { return (uint) _m(i,j); }
    inline double& operator()(const uint i, const uint j) { return _m(i,j); }
    inline void  set(const int i, const int j, const double x) { _m(i,j) = x; }
    inline void mkRectangle(const uint i, const uint j, rectangle_t& r) const {
                  partitiondescxy_t lPd(_descX, _descY);
                  lPd.getRectangle(i, j, r);
                }
    inline uint noZero() const { return matrix().countZeroElements(); }
  public:
    double maxEntry() const;
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimate(const query_t& lQuery) const;
    // ignore tiles with only a small area in common with query rectangle
    double estimateIgnore(const rectangle_t& r, const double aEpsilon) const;
  public:
    virtual uint size() const override;
  public:
    // half the number of tiles in both dimensions in the submatrix 0:m,0:n
    // invalidates partitiondescriptors, _vx, _vy
    void shrink2(const uint m, const uint n); 
  public:
    uint estimate3LevelTreeMemoryConsumption(const double aTheta) const; // experimental
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os, const int aFieldWidth = 4) const;
            std::ostream& print(std::ostream& os, const uint m, const uint n, const int aFieldWidth = 4) const;
            std::ostream& printMatrix(std::ostream& os, const int aFieldWidth = 4) const;
            std::ostream& printWithRowColSums(std::ostream& os, const int aFieldWidth = 4) const;
            std::ostream& printAnalysis(std::ostream& os,
                                        const uint aTheta,
                                        const int aFieldWidth = 4) const;
  private:
    void initPartitionDescXY(const uint nx, const uint ny, const Data2dim&);
    void initPartitionDescXY(const Data2dim&, const uint aBegin, const uint aEnd,
                                              const uint nx, const uint ny);
    void initPartitioning(const uint nx, const uint ny, const Data2dim&);
    void initPartitioning(const Data2dim&, const uint aBegin, const uint aEnd,
                                           const uint nx, const uint ny);
    void allocVxy(const uint nx, const uint ny);
    void deleteVxy();
  private:
    partitiondesc_t _descX;
    partitiondesc_t _descY;
    uint*  _vx; // cumulated frequencies along x-axis, partitioned into nx equi width intervals
    uint*  _vy; // cumulated frequencies along y-axis, partitioned into ny equi width intervals
    Matrix _m;  // 2-dimensional regular partitioning of frequency
    uint   _total; // size of relation
};


} // end namespace

#endif

