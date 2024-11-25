#ifndef GEOCOORD_HH
#define GEOCOORD_HH

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <math.h>

class GeoCoord {
  public:
    enum format_t {
      gxx = 0,
      gmx = 1, 
      gms = 2
    };
  public:
    GeoCoord();
    GeoCoord(double x, format_t aFormat = gxx);
    ~GeoCoord();
  public:
           double val(format_t) const;
    inline double valgxx() const { return _valgxx; }
    inline double valgmx() const { return gxx2gmx(valgxx()); }
    inline double valgms() const { return gxx2gms(valgxx()); }
  public:
    inline void set_gxx(double x) { _valgxx = x; }
  public:
    static double gms2gxx(const double x);
    static double gxx2gms(const double x);
    static double gmx2gxx(const double x);
    static double gxx2gmx(const double x);
  public:
    static format_t format(const char*);
  public:
    std::ostream& print(std::ostream&, format_t = gxx) const;
  private:
    double _valgxx;
};

class GeoPoint {
  public:
    GeoPoint();
    GeoPoint(const GeoCoord& aLatitude, 
             const GeoCoord& aLongitude,
             double aAltitude = 0.0);
    ~GeoPoint();
  public:
    inline const GeoCoord& latitude() const { return _latitude; }
    inline const GeoCoord& longitude() const { return _longitude; }
    inline double altitude() const { return _altitude; }
  public:
    inline GeoCoord& latitude() { return _latitude; }
    inline GeoCoord& longitude() { return _longitude; }
    inline void altitude(double x) { _altitude = x; }
  public:
    double distanceInKilometer(const GeoPoint&) const; // ignores altitude
    double distanceInMile(const GeoPoint&) const;     // ignores altitude
  public:
    std::ostream& print(std::ostream&, GeoCoord::format_t = GeoCoord::gxx) const;
  private:
    GeoCoord _latitude;
    GeoCoord _longitude;
    double   _altitude;
  public:
    static double pi() { return _pi; }
  private:
    static double _pi;
};

inline std::ostream& 
operator<<(std::ostream& os, const GeoCoord& x) {
  return x.print(os);
}

inline std::ostream& 
operator<<(std::ostream& os, const GeoPoint& x) {
  return x.print(os);
}

#endif // GEOCOORD_HH

