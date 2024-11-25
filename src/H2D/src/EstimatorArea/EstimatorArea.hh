#ifndef H2D_ESTIMATOR_AREA_HH
#define H2D_ESTIMATOR_AREA_HH

#include "infra/types.hh"
#include "infra/data2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include <tuple>
#include <cmath>

namespace H2D {


class EstimatorArea : public EstimatorBase2dim {
  public:
    EstimatorArea(const Data2dim& aData,
                  const double aTheta);
    virtual ~EstimatorArea();
  public:
    virtual uint size() const;
    inline  const rectangle_t br() const { return _br; }
    inline  double card() const { return _card; }
    inline  double theta() const { return _theta; }
  public:
    double estimate(const rectangle_t& aQueryRectangle) const;
    double estimate(const query_t& lQuery) const;
    std::tuple<double,double> minEbo(const rectangle_t& aQueryRectangle) const;
/*
    std::tuple<double,double> selXY(const rectangle_t& aQueryRectangle) const;
    double minsel(const std::tuple<double,double> selXY) const;
    double ebo(const std::tuple<double,double> selXY) const;
    double avi(const std::tuple<double,double> selXY) const;
*/
public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
  private:
    rectangle_t _br;
    double      _card;
    double      _theta;
    Data2dim    _data;
};


} // end namespace


#endif

