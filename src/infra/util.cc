#include "util.hh"

namespace H2D {


const char*
gbasename(const char* x) {
  const char* z = x + strlen(x);
  while('/' != *z && z > x) { --z; }
  if('/' == *z) {
    ++z;
  }
  return z;
}


void
printNxy(std::ostream& os, const char aLineMarker, const uint nx, const uint ny) {
  os << aLineMarker << nx << 'x' << ny << ' ';
  if(10 > nx) {
    std::cout << "  ";
  } else
  if(100 > nx) {
    std::cout << " ";
  }

  if(10 > ny) {
    std::cout << "  ";
  } else
  if(100 > ny) {
    std::cout << " ";
  }
}

} // end namespace


