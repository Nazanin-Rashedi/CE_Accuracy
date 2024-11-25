
#include <fstream>
#include "lageparameter.hh"


int
main() {

  Lageparameter lLp0;
  lLp0 << 1 << 2 << 9;
  lLp0.fin();

  std::cout << "min:       " << lLp0.min() << std::endl
            << "max:       " << lLp0.max() << std::endl
            << "mean:      " << lLp0.mean() << std::endl
            << "median:    " << lLp0.median() << std::endl
            << "middle:    " << lLp0.middle() << std::endl
            << "geomiddle: " << lLp0.geomiddle() << std::endl
            << std::endl;


  std::cout << "mean  (" << lLp0.mean() << "): "
            << std::setw(7) << lLp0.Error1(lLp0.mean()) << ' '
            << std::setw(7) << lLp0.Error2(lLp0.mean()) << ' '
            << std::setw(7) << lLp0.ErrorI(lLp0.mean()) << ' '
            << std::setw(7) << lLp0.ErrorQ(lLp0.mean()) << ' '
            << std::endl;

  std::cout << "median(" << lLp0.median() << "): "
            << std::setw(7) << lLp0.Error1(lLp0.median()) << ' '
            << std::setw(7) << lLp0.Error2(lLp0.median()) << ' '
            << std::setw(7) << lLp0.ErrorI(lLp0.median()) << ' '
            << std::setw(7) << lLp0.ErrorQ(lLp0.median()) << ' '
            << std::endl;

  std::cout << "middle(" << lLp0.middle() << "): "
            << std::setw(7) << lLp0.Error1(lLp0.middle()) << ' '
            << std::setw(7) << lLp0.Error2(lLp0.middle()) << ' '
            << std::setw(7) << lLp0.ErrorI(lLp0.middle()) << ' '
            << std::setw(7) << lLp0.ErrorQ(lLp0.middle()) << ' '
            << std::endl;

  std::cout << "geomid(" << lLp0.geomiddle() << "): "
            << std::setw(7) << lLp0.Error1(lLp0.geomiddle()) << ' '
            << std::setw(7) << lLp0.Error2(lLp0.geomiddle()) << ' '
            << std::setw(7) << lLp0.ErrorI(lLp0.geomiddle()) << ' '
            << std::setw(7) << lLp0.ErrorQ(lLp0.geomiddle()) << ' '
            << std::endl;

/*
  std::ofstream os("errorplot.dat");

  Lageparameter::mfun_t  lFuns[4];
  lFuns[0] = &Lageparameter::Error1(double);
  lFuns[1] = &Lageparameter::Error2(double);
  lFuns[2] = &Lageparameter::ErrorI(double);
  lFuns[3] = &Lageparameter::ErrorQ(double);
*/

  return 0;
}
