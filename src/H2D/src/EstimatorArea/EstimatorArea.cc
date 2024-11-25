#include "EstimatorArea.hh"

namespace H2D {


EstimatorArea::EstimatorArea(const Data2dim& aData,
                            const double    aTheta) 
              : _br(), _card(aData.total()), _theta(aTheta),_data(aData) {
  aData.getBoundingRectangle(_br);
}

EstimatorArea::~EstimatorArea() {
}


uint
EstimatorArea::size() const {
  return 4*4 + 4;
}

double
EstimatorArea::estimate(const rectangle_t& aQueryRectangle) const {
  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, br());
  if(lIsec.hasZeroArea()) {
    return theta();
  }
  const double lFraction = lIsec.area() / br().area();
  // std::cout << " EstimatorArea::estimate: lFraction = " << lFraction << std::endl;
  return std::max<double>(theta(), lFraction * card());
}
/*

std::tuple<double,double>
EstimatorArea::minEbo(const rectangle_t& aQueryRectangle) const {
  rectangle_t xRect = rectangle_t(aQueryRectangle._pll.x,br()._pll.y , aQueryRectangle._pur.x,br()._pur.y);
  rectangle_t yRect = rectangle_t(br()._pll.x,aQueryRectangle._pll.y , br()._pur.x,aQueryRectangle._pur.y);
  double selX = _data.countWithin(xRect)/card();
  double selY = _data.countWithin(yRect)/card();
  //double avi = selX * selY * card();
  double minsel = std::min(selX , selY) * card();
  double ebo = std::min(selX , selY) * std::sqrt(std::max(selX , selY)) * card();
  return {minsel, ebo};

}
*/

std::tuple<double,double>
EstimatorArea::minEbo(const rectangle_t& aQueryRectangle) const {
  rectangle_t xRect = rectangle_t(aQueryRectangle._pll.x,br()._pll.y , aQueryRectangle._pur.x,br()._pur.y);
  rectangle_t yRect = rectangle_t(br()._pll.x,aQueryRectangle._pll.y , br()._pur.x,aQueryRectangle._pur.y);
  double selX = xRect.area()/br().area();
  double selY = yRect.area()/br().area();
  //double avi = selX * selY * card();
  double minsel = std::min(selX , selY) * card();
  double ebo = std::min(selX , selY) * std::sqrt(std::max(selX , selY)) * card();
  return {minsel, ebo};

}


/*
double
EstimatorArea::minsel(const std::tuple<double,double> selXY) const{
//std::cout<<"minsel:"<< (std::min<double>(std::get<0>(selXY),std::get<1>(selXY))) <<std::endl;
return std::max<double>(theta(), (std::min<double>(std::get<0>(selXY),std::get<1>(selXY))) * card());

}

double
EstimatorArea::ebo(const std::tuple<double,double> selXY) const{
double ebo = std::min<double>(std::get<0>(selXY),std::get<1>(selXY))*std::sqrt(std::max<double>(std::get<0>(selXY),std::get<1>(selXY)));
return std::max<double>(theta(), ebo * card());

}

double
EstimatorArea::avi(const std::tuple<double,double> selXY) const{
double avi = (std::get<0>(selXY))*(std::get<1>(selXY));
return std::max<double>(theta(), avi * card());

}
*/





double
EstimatorArea::estimate(const query_t& lQuery) const {
  rectangle_t aQueryRectangle = lQuery.rectangle();
  return estimate(aQueryRectangle);
}

std::ostream&
EstimatorArea::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_EST_AREA) << ' '
            << 0 // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 



} // end namespace

