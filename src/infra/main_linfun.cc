#include <iostream>

#include "linfun.hh"


void
test0(const double x1, const double y1, const double x2, const double y2, const double e) {
  LinFun lFun(x1, y1, x2, y2);
/*
  const double w = (x2 - x1) / 100;
  double x = x1;
  for(uint i = 0; i < 100; ++i) {
    std::cout << x << ' ' << lFun(x) << std::endl;
    x += w;
  }
*/
  std::cout << "Lin:" << std::endl;
  std::cout << "  x1 = " << x1 << std::endl
            << "  y1 = " << y1 << std::endl
            << "  x2 = " << x2 << std::endl
            << "  y2 = " << y2 << std::endl;
  std::cout << "  f(x1) = " << lFun(x1) << " =?= " << y1 << std::endl
            << "  f(x2) = " << lFun(x2) << " =?= " << y2 << std::endl;
  std::cout << "  f(x) =  " << lFun.a() << " * x + " << lFun.b() << std::endl;
  std::cout << "  #root(" << e << ") = " << lFun.root(e) << " f(r) = " << lFun(lFun.root(e)) << std::endl;
}

void
test1(const double x1, const double z1, const double x2, const double z2, const double e) {
  const double y1 = std::log(z1);
  const double y2 = std::log(z2);
  const double log_e = std::log(e);
  LogLinFun lFun(x1, y1, x2, y2);
/*
  const double w = (x2 - x1) / 100;
  double x = x1;
  for(uint i = 0; i < 100; ++i) {
    std::cout << x << ' ' << std::exp(lFun(x)) << std::endl;
    x += w;
  }
*/
  std::cout << "LogLin:" << std::endl;
  std::cout 
            << "        x1 = " << x1 << std::endl
            << "        z1 = " << z1 << std::endl
            << "        y1 = " << y1 << std::endl
            << "        x2 = " << x2 << std::endl
            << "        z2 = " << z2 << std::endl
            << "        y2 = " << y2 << std::endl
            << "        e  = " << e  << std::endl
            << "    log(e) = " << log_e << std::endl;
  std::cout << "     f(x1) = " << lFun(x1) << " =?= " << y1 << std::endl
            << "     f(x2) = " << lFun(x2) << " =?= " << y2 << std::endl
            << "  exp(f(x1)) = " << std::exp(lFun(x1)) << " =?= " << z1 << std::endl
            << "  exp(f(x2)) = " << std::exp(lFun(x2)) << " =?= " << z2 << std::endl;
  std::cout << "  f(x) =     (" << lFun.a() << " * log(x) + " << lFun.b() << ')' << std::endl;
  std::cout << "  f(x) =  exp(" << lFun.a() << " * log(x) + " << lFun.b() << ')' << std::endl;
  std::cout << "  #root(" << e << ") = " << (lFun.root(e)) << " f(r) = " << lFun(lFun.root(e))
            << std::endl;
}


int
main(const int argc, const char* argv[]) {
  if(6 != argc) {
    std::cout << "usage: " << argv[0] << " <x1> <y1> <x2> <y2> <e>" << std::endl
              << " approximate points by linear function and return x such that f(x) = e." << std::endl;
    return -1;
  }
  char* x = 0;
  const double x1 = strtod(argv[1], &x);
  const double y1 = strtod(argv[2], &x);
  const double x2 = strtod(argv[3], &x);
  const double y2 = strtod(argv[4], &x);
  const double e  = strtod(argv[5], &x);
  test0(x1, y1, x2, y2, e);
  test1(x1, y1, x2, y2, e);
}

