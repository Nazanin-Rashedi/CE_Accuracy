#include "geocoord.hh"

GeoCoord::GeoCoord() : _valgxx(0) {}

GeoCoord::GeoCoord(double x, format_t aFormat) : _valgxx(0) {
  switch(aFormat) {
    case gxx: _valgxx = x;
              break;
    case gmx: _valgxx = gmx2gxx(x);
              break;
    case gms: _valgxx = gms2gxx(x); break;
    default:
              std::cerr << "XXX" << std::endl;
              break;
  }
}

GeoCoord::~GeoCoord() {}

double
GeoCoord::val(format_t aFormat) const {
  switch(aFormat) {
    case gxx: return valgxx();
              break;
    case gmx: return valgmx();
              break;
    case gms: return valgms();
              break;
    default:
              std::cerr << "XXX" << std::endl;
              break;
  }
  return 0.0;
}

double
GeoCoord::gms2gxx(const double x) {
  double lGrad = trunc(x / 10000.0);
  double lRest = (x - lGrad * 10000.0);
  double lMin  = trunc(lRest / 100.0);
  double lSec  = (lRest - lMin * 100.0);
  double lRes  = (lGrad + (((lSec / 60.0) + lMin) / 60.0));
/*
  std:: cout << "gms2gxx: " << std::endl
             << "  in  : " << x << std::endl
             << "  grad: " << lGrad << std::endl
             << "  rest: " << lRest << std::endl
             << "  min : " << lMin << std::endl
             << "  sec : " << lSec << std::endl
             << "  res : " << lRes << std::endl;
*/
  return lRes;
}

double
GeoCoord::gxx2gms(const double x) {
  double lGrad = trunc(x);
  double lRest = (x - lGrad) * 60.0;
  double lMin  = trunc(lRest);
  double lSec  = ((lRest - lMin) * 60.0);
  double lRes  = (lGrad * 10000.0 + lMin * 100.0 + lSec);
/*
  std:: cout << "gxx2gms: " << std::endl
             << "  in  : " << x << std::endl
             << "  grad: " << lGrad << std::endl
             << "  rest: " << lRest << std::endl
             << "  min : " << lMin << std::endl
             << "  sec : " << lSec << std::endl
             << "  res : " << lRes << std::endl;
*/
  return lRes;
}

double
GeoCoord::gmx2gxx(const double x) {
  double lGrad = trunc(x / 100.0);
  double lMin  = (x - (lGrad * 100.0));
  double lRes  = lGrad + (lMin / 60.0);
/*
  std:: cout << "gmx2gxx: " << std::endl
             << "  in  : " << x << std::endl
             << "  grad: " << lGrad << std::endl
             << "  min : " << lMin << std::endl
             << "  res : " << lRes << std::endl;
*/
  return lRes;
}

double
GeoCoord::gxx2gmx(const double x) {
  double lGrad = trunc(x);
  double lRest = (x - lGrad) * 60.0;
  double lMin  = trunc(lRest);
  double lSec  = lRest - lMin; 
  double lRes = (lGrad * 100.0 + lMin + lSec);
/*
  std:: cout << "gxx2gmx: " << std::endl
             << "  in  : " << x << std::endl
             << "  grad: " << lGrad << std::endl
             << "  min : " << lMin << std::endl
             << "  sec : " << lSec << std::endl
             << "  res : " << lRes << std::endl;
*/
  return lRes;
}

GeoCoord::format_t
GeoCoord::format(const char* x) {
  if('s' == x[2]) {
    return gms;
  } else
  if('m' == x[1]) {
    return gmx;
  }
  return gxx;
}

double GeoPoint::_pi = atan2(1,1) * 4.0;

GeoPoint::GeoPoint() : _latitude(), _longitude(), _altitude(0) {}

GeoPoint::GeoPoint(const GeoCoord& aLatitude, 
                   const GeoCoord& aLongitude, 
                   double aAltitude) 
         : _latitude(aLatitude), 
           _longitude(aLongitude),
           _altitude(aAltitude) {
}

GeoPoint::~GeoPoint() {}

double
GeoPoint::distanceInKilometer(const GeoPoint& x) const {
  double lLatitude1  = (latitude().valgxx() / 180.0) * pi();
  double lLongitude1 = (longitude().valgxx() / 180.0) * pi();
  double lLatitude2  = (x.latitude().valgxx() / 180.0) * pi();
  double lLongitude2 = (x.longitude().valgxx() / 180.0) * pi();

  const double lEarthRadius = 6367.0; // in km
/*
  double lDiff = fabs(lLongitude1 - lLongitude2);

  if(lDiff > pi()) {
    lDiff -= 2 * pi();  // XXX
  }

  double lFac = (3958.6 * 1.6);
  double lDist = acos(  sin(lLatitude1) * sin(lLatitude2)
                      + cos(lLatitude1) * cos(lLatitude2) * cos(lDiff) ) * lFac;
*/

  double lDiffLon = lLongitude2 - lLongitude1;
  double lDiffLat = lLatitude2 - lLatitude1;
  double a = (sin(lDiffLat / 2.0) * sin(lDiffLat / 2.0)) 
             + cos(lLatitude1) * cos(lLatitude2) 
               * (sin(lDiffLon / 2.0) * sin(lDiffLon / 2.0));
  double lDist = 2 * atan2(sqrt(a), sqrt(1 - a));

  return lDist * lEarthRadius;
}

double
GeoPoint::distanceInMile(const GeoPoint& x) const {
  return (distanceInKilometer(x) / 1.6);
}

std::ostream&
GeoCoord::print(std::ostream& os, GeoCoord::format_t aFormat) const {
  os << val(aFormat);
  return os;
}

std::ostream&
GeoPoint::print(std::ostream& os, GeoCoord::format_t aFormat) const {
  latitude().print(os, aFormat);
  os << " ";
  longitude().print(os, aFormat);
  os << " ";
  os << altitude();
  return os;
}

