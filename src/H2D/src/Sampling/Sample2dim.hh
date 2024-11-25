#ifndef SAMPLE_2DIM_HH
#define SAMPLE_2DIM_HH

#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <random>
#include <assert.h>

#include "../infra/types.hh"
#include "../infra/data2dim.hh"
#include "../infra/EstimatorBase2dim.hh"

namespace H2D {


class Sample2dim : public EstimatorBase2dim {
  public:
    Sample2dim(const Data2dim& aData,
               const uint      aSampleSize,
               const double    aQ,
               const double    aTheta,
               const bool      aTrace);
    virtual ~Sample2dim();
  public:
    virtual uint size() const;
    inline  uint   sampleSize() const { return _data.size(); }
    inline  double dataSize() const { return _dataSize; }
    inline  uint   noOutlier() const { return 0; }
    inline  bool   trace() const { return _trace; }
    inline  Data2dim samples() const {return _data;}
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual  double estimate(const query_t& lQuery) const;
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
  private:
    Data2dim _data;
    double   _dataSize;
    bool     _trace;
};


} // end namespace


#endif
