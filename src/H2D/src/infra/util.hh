#ifndef HIST2DIM_UTIL_HH
#define HIST2DIM_UTIL_HH

#include <iostream>
#include <iomanip>
#include <string.h>


#include "types.hh"

namespace H2D {


const char* gbasename(const char* x);
void printNxy(std::ostream& os, const char aLineMarker, const uint nx, const uint ny);

inline
double
roundX(const double x) {
  return (round(x * 10.0) / 10.0);
}


} // end namespace


#endif


