#include <iostream>
#include <iomanip>

#include "types.hh"

struct badcase_t {
  partitiondescxy_t pd;
  int nx, ny;
  rectangle_t tr; // tile rectangle, constructed from partitiondesc
  point_t     p; // point
  int idxX, idxY;

  badcase_t(const rectangle_t& aBr, const int aNx, const int aNy, 
            const point_t& aPoint, const int aIdxX, const int aIdxY)
           : pd(aBr, aNx, aNy), nx(aNx), ny(aNy), 
             p(aPoint), idxX(aIdxX), idxY(aIdxY) {
  }

  void print(std::ostream& os) const;
  void printXBoundaries(std::ostream& os) const;
  void printYBoundaries(std::ostream& os) const;
};

void
badcase_t::printXBoundaries(std::ostream& os) const {
  double lX = pd.minX();
  for(int i = 0; i <= nx; ++i) {
    os << std::setw(3) << i << "  "
       << std::setw(10) << lX 
       << std::endl;
    lX += pd.widthX();
  }
}

void
badcase_t::printYBoundaries(std::ostream& os) const {
  double lY = pd.minY();
  for(int i = 0; i <= ny; ++i) {
    os << std::setw(3) << i << "  "
       << std::setw(10) << lY
       << std::endl;
    lY += pd.widthY();
  }
}

void
badcase_t::print(std::ostream& os) const {
  os << "bounding rectangle: ";
  pd.printRectangle(os);
  os << std::endl
     << "nx, ny : " << nx << ", " << ny << std::endl
     << "wx, wy : " << pd.widthX() << ", " << pd.widthY() << std::endl
     << "idxX, idxY : " << idxX << ", " << idxY << std::endl
     << std::endl;

  os << "point: " << p << std::endl;
 
  os << "derived indices: " << pd.idxX(p.x) << " , " << pd.idxY(p.y) << std::endl;
 
  rectangle_t lTileRectangle;
  pd.getRectangle(idxX, idxY, lTileRectangle);
  os << "tile rectangle: " << lTileRectangle << std::endl;
  os << std::endl;

  if(lTileRectangle.containsHalfOpen(p.x, p.y)) {
    os << "point contained in tile rectangle" << std::endl;
  } else {
    os << "point NOT contained in tile rectangle" << std::endl;

    // check x boundaries
    if(p.x < lTileRectangle.xlo()) {
      os << "p.x < xlo : " << p.x << " < " << lTileRectangle.xlo() 
         << std::endl;
      os << "boundaries for x: " << std::endl;
      printXBoundaries(os);
    }
    if(p.x >= lTileRectangle.xhi()) {
      os << "p.x >= xhi : " << p.x << " >= " << lTileRectangle.xhi() 
         << std::endl;
      os << "boundaries for x: " << std::endl;
      printXBoundaries(os);
    }

    
    // check y boundaries
    if(p.y < lTileRectangle.ylo()) {
      os << "p.y < ylo : " << p.y << " < " << lTileRectangle.ylo() 
         << std::endl;
      os << "boundaries for y: " << std::endl;
      printYBoundaries(os);
    }
    if(p.y >= lTileRectangle.yhi()) {
      os << "p.y >= yhi : " << p.y << " >= " << lTileRectangle.yhi() 
         << std::endl;
      os << "boundaries for y: " << std::endl;
      printYBoundaries(os);
    }

  }

}


/*
 * first bad case:
 * y-boundaries of the bounding rectangle are 28.8 and 32.4
 * if partitioned into 18 subintervals, we get
 * 32.4 - 28.8 = 3.6 and thus widthY = 0.2
 * however, this does not work!
 */


void
test1() {
  const rectangle_t br(1859, 28.8, 2525.333333333, 32.4);
  const point_t p(2391, 29);
  const int nx = 4;
  const int ny = 18;
  const int idxX = 3;
  const int idxY = 0;

  badcase_t c(br, nx, ny, p, idxX, idxY);
  c.print(std::cout);

  double wy = ((32.4 - 28.8) / (double) 18);
  std::cout << "wy: " << wy << std::endl;

  double lIdxYD =            ((29.0 - 28.8) / wy);
  int    lIdxYF = (int) floor((29.0 - 28.8) / wy);
  int    lIdxYI = (int)      ((29.0 - 28.8) / wy);

  std::cout << "here it is: " 
            << "((29.0 - 28.8) / ((32.4 - 28.8) / 18)) = "
            << ((29.0 - 28.8) / ((32.4 - 28.8) / 18)) << " = "
            << ((29.0 - 28.8) / wy) << " = " 
            << (int) ((29.0 - 28.8) / wy) << " = " 
            << lIdxYF << " = "
            << lIdxYI
            << " sic!" 
            << std::endl;
  std::cout << "difference: " << ((double) 1.0 - lIdxYD) << std::endl;
}




int
main() {
  // std::cout.precision(10);
  // std::cout.setf( std::ios::fixed, std::ios::floatfield );
  test1();
}

