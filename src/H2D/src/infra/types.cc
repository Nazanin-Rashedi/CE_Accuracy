#include "types.hh"

namespace H2D {


const std::string&
h2d_kind_name(const H2D_kind_t aKind) {
  static std::string lName[H2D_NO_KIND+1] = {"RegP",     //  0
                                             "EqD",      //  1
                                             "SAMPL",    //  2
                                             "GTF",      //  3
                                             "GTA",      //  4
                                             "GTB",      //  5
                                             "MHIST",    //  6
                                             "VIHIST",   //  7
                                             "HIRED",    //  8
                                             "DCT",      //  9
                                             "DCTcf",    // 10
                                             "GENHIST",  // 11
                                             "NONO",     // 12
                                             "WVLTNSTD", // 13
                                             "HFT",      // 14
                                             "PHSD",     // 15
                                             "QTS",      // 16
                                             "IQTS",     // 17
                                             "FXT",      // 18
                                             "GXT",      // 19
                                             "EqD2",     // 20
                                             "MHist2",   // 21
                                             "ESTCONST", // 22
                                             "ESTAREA",  // 23
                                             "ZHIST",    // 24
					     "XGB",      //25
					     "EXGB",     //26  
                                             "EBOXGB",     //27 
				             "SAMPLALPHA",  //28 
				             "NXGB",        //29 
				             "INDEP",       //30
				             "NREQD",	     //31 
                                             "H2D_NO_KIND"};

  // std::cout << "h2d_kind_name(" << aKind << ")" << std::endl;
  if(H2D_NO_KIND < aKind) {
    // std::cout << "oops, kind too large" << std::endl;
    return lName[H2D_NO_KIND];
  }
  return lName[aKind];
}

// setTo of rectangle

bool
rectangle_t::setTo(const char* aRectangle) {
  const char* x = aRectangle;
  char* lPtr = 0;
  double lXlo = 0;
  double lYlo = 0;
  double lXhi = 0;
  double lYhi = 0;
  if('[' != (*x)) {
    return false;
  }
  ++x;
  if('(' != (*x)) {
    return false;
  }
  ++x;

  lXlo = strtod(x, &lPtr);
  if(x == lPtr) {
    return false;
  }
  x = lPtr;

  if(',' != (*x)) {
    return false;
  }
  ++x;  

  lYlo = strtod(x, &lPtr);
  if(x == lPtr) {
    return false;
  }
  x = lPtr;

  if(')' != (*x)) {
    return false;
  }
  ++x;
  if(',' != (*x)) {
    return false;
  }
  ++x;
  if('(' != (*x)) {
    return false;
  }
  ++x;

  lXhi = strtod(x, &lPtr);
  if(x == lPtr) {
    return false;
  }
  x = lPtr;

  if(',' != (*x)) {
    return false;
  }
  ++x;

  lYhi = strtod(x, &lPtr);
  if(x == lPtr) {
    return false;
  }
  x = lPtr;

  _pll.x = lXlo;
  _pll.y = lYlo;
  _pur.x = lXhi;
  _pur.y = lYhi;

  return true;
}

bool
rectangle_t::read(std::istream& aIs) {
  char   c = 0;
  double lXlo = 0;
  double lYlo = 0;
  double lXhi = 0;
  double lYhi = 0;

  aIs >> c;
  if('[' != c) { return false; }
  aIs >> c;
  if('(' != c) { return false; }
  aIs >> lXlo;
  aIs >> c;
  if(',' != c) { return false; }
  aIs >> lYlo;
  aIs >> c;
  if(')' != c) { return false; }
  aIs >> c;
  if(',' != c) { return false; }
  aIs >> c;
  if('(' != c) { return false; } 
  aIs >> lXhi;
  aIs >> c;
  if(',' != c) { return false; }
  aIs >> lYhi;
  aIs >> c;
  if(')' != c) { return false; }
  aIs >> c;
  if(']' != c) { return false; }

  _pll.x = lXlo;
  _pll.y = lYlo;
  _pur.x = lXhi;
  _pur.y = lYhi;

  return true;
}


// print operator for rectangles ands partitiondescriptors

std::ostream&
rectangle_t::print(std::ostream& os) const {
  os << "[(" << xlo() << ',' << ylo() << "),(" << xhi() << ',' << yhi() << ")]";
  return os;
}

std::ostream&
operator<<(std::ostream& os, const partitiondesc_t& x) {
  os << "min: "   << x._min << ", "
     << "max: "   << x._max << ", "
     << "width: " << x._width;
  return os; 
}

std::ostream&
operator<<(std::ostream& os, const partitiondescxy_t& x) {
  os << "pdX: " << x.pdX() << ", pdY: " << x.pdY();
  return os;
}



} // end namespace



