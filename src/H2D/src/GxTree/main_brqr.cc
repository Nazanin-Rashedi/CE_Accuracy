#include <iostream>
#include <iomanip>


#include "infra/types.hh"

using namespace H2D;

double
roundXXX(double x) {
  return round(x * 1000.0) / 1000.0;
}

// for a given n, bounding rectangle (br) and query rectangle (qr)
// calculate nxn partition desc and print
// 
void
printTiles(const H2D::uint n, const rectangle_t& aBr, const rectangle_t& aQr) {
  std::cout << " n = " << n   << std::endl
            << "br = " << aBr << std::endl
            << "qr = " << aQr << std::endl;

  partitiondescxy_t lPd(aBr, n, n);

  rectangle_t lTileRectangle;
  rectangle_t lTileIsec;
  rectangle_t lBrIsec;
  lBrIsec.isec(aBr, aQr);
  if(lBrIsec.hasZeroArea()) {
    std::cout << "isec(br,qr) = " << lBrIsec << " has zero area." << std::endl;
    return;
  }
  for(H2D::uint i = 0; i < n; ++i) {
    for(H2D::uint j = 0; j < n; ++j) {
      lPd.getRectangle(i, j, lTileRectangle);
      lTileIsec.isec(lTileRectangle, aQr);
      const double lArea = (lTileIsec.hasZeroArea() ? 0 : lTileIsec.area());
      const double lFraction = lArea / lTileRectangle.area();
      std::cout << std::setw(2) << i << ' '
                << std::setw(2) << j << "   "
                << std::setw(8) << roundXXX(lArea) << "   "
                << std::setw(8) << roundXXX(lFraction) << "   "
                << lTileRectangle << "   "
                << lTileIsec
                << std::endl;
    }
  }
}

int
main(const int argc, const char* argv[]) {
  if(4 != argc) {
    std::cout << "wrong number of arguments."
              << "usage: ./main_brqr <n> <br> <qr>"
              << std::endl;
    return -1;
  }

  char* x = 0;
  const int n = strtol(argv[1], &x, 10);
  if(x == argv[1]) {
    std::cout << "error while reading <n>" << std::endl;
  }

  rectangle_t lBr;
  rectangle_t lQr;

  if(!lBr.setTo(argv[2])) {
    std::cout << "can't parse <br>" << std::endl;
    return 0;
  }
  if(!lQr.setTo(argv[3])) {
    std::cout << "can't parse <qr>" << std::endl;
    return 0;
  }

  printTiles(n, lBr, lQr);
}
