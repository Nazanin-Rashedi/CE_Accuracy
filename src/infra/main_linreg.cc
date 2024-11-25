#include <iostream>
#include <iomanip>
#include <vector>

#include "LinearRegression.hh"

typedef unsigned int uint;

void
test0() {
  LinearRegression<double> lLinReg;
  lLinReg.init();
  lLinReg.step(1, 3);
  lLinReg.step(2, 5);
  lLinReg.step(3, 7);
  std::cout << lLinReg.alpha() << " * x " << " + " << lLinReg.beta() << std::endl;
  lLinReg.fin();
}

struct xy_t {
  double _x;
  double _y;
  xy_t(const double aX, const double aY) : _x(aX), _y(aY) {}
};

typedef std::vector<xy_t> xy_vt;


struct Fun {

  inline double operator()(const double x) const { return a * x + b; }
  // return transformed y
  inline double trans(const double x, const double y) const { return y - (*this)(x); }
  // members
  double a;
  double b;
};

void
test2() {
 xy_vt  lData1;
 lData1.push_back(xy_t( 5, 8));
 lData1.push_back(xy_t( 5,14));
 lData1.push_back(xy_t(11, 8));
 lData1.push_back(xy_t(11,14));

 xy_vt lData2;
 lData2.push_back(xy_t( 5, 8));
 lData2.push_back(xy_t( 6, 9));
 lData2.push_back(xy_t( 7,10));
 lData2.push_back(xy_t( 8, 9));
 lData2.push_back(xy_t( 9,10));
 lData2.push_back(xy_t(10,12));
 lData2.push_back(xy_t(11,14));

 xy_vt lData3;
 lData3.push_back(xy_t( 8, 8));
 lData3.push_back(xy_t( 8,14));
 lData3.push_back(xy_t(11, 8));
 lData3.push_back(xy_t(11,14));

 LinearRegression<double> lLinReg;
 lLinReg.init();
 for(uint i = 0; i < lData2.size(); ++i) {
    lLinReg.step(lData2[i]._x, lData2[i]._y);
 }
 lLinReg.fin();
 std::cout << lLinReg.alpha() << " * x " << " + " << lLinReg.beta() << std::endl;


 // apply
 Fun lFun;
 lFun.a = lLinReg.alpha();
 lFun.b = lLinReg.beta();
 std::cout << std::setw(3) << "x" << ' '
           << std::setw(3) << "y" << ' '
           << std::setw(8) << "ax+b" << ' '
           << std::setw(8) << "y-(ax+b)" << ' '
           << std::endl;
 for(uint i = 0; i < lData2.size(); ++i) {
   std::cout << std::setw(3) << lData2[i]._x << ' '
             << std::setw(3) << lData2[i]._y << ' '
             << std::setw(8) << lFun(lData2[i]._x) << ' '
             << std::setw(8) << lFun.trans(lData2[i]._x, lData2[i]._y) << ' '
             << std::endl;
 }
 std::cout << std::endl;

  std::cout << std::setw(3) << "x" << ' '
           << std::setw(3) << "y" << ' '
           << std::setw(8) << "ax+b" << ' '
           << std::setw(8) << "y-(ax+b)" << ' '
           << std::endl;
 for(uint i = 0; i < lData1.size(); ++i) {
   std::cout << std::setw(3) << lData1[i]._x << ' '
             << std::setw(3) << lData1[i]._y << ' '
             << std::setw(8) << lFun(lData1[i]._x) << ' '
             << std::setw(8) << lFun.trans(lData1[i]._x, lData1[i]._y) << ' '
             << std::endl;
 }
 std::cout << std::endl;

 std::cout << std::setw(3) << "x" << ' '
           << std::setw(3) << "y" << ' '
           << std::setw(8) << "ax+b" << ' '
           << std::setw(8) << "y-(ax+b)" << ' '
           << std::endl;
 for(uint i = 0; i < lData3.size(); ++i) {
   std::cout << std::setw(3) << lData3[i]._x << ' '
             << std::setw(3) << lData3[i]._y << ' '
             << std::setw(8) << lFun(lData3[i]._x) << ' '
             << std::setw(8) << lFun.trans(lData3[i]._x, lData3[i]._y) << ' '
             << std::endl;
 }



 
}


int
main() {
  test0();
  test2();
  return 0;
}


