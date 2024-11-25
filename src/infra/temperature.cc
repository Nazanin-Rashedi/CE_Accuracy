#include "temperature.hh"

Temperature::Temperature() : _tempInC(0) {}
Temperature::Temperature(double x, scale s) : _tempInC(celsius(x, s)) {}
Temperature::~Temperature() {}

double
Temperature::fahrenheit() const {
  return (((celsius() * 9.0) / 5.0) + 32.0);
}

double
Temperature::kelvin() const {
  return celsius() + 273.15;
}


double
Temperature::celsius(double x, scale s) {
  double lRes = 0;
  switch(s) {
    case Kelvin:
         lRes = x - 273.15;
         break;
    case Celsius:
         lRes = x;
         break;
    case Fahrenheit:
         lRes = (((x - 32.0) * 5.0)/9.0);
         break;
  }
  return lRes;
}


