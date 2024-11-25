#include "datejd.hh"


void
test_a() {
  DateJd x(1968,01,01);
  uint y = 0;
  uint m = 0;
  uint d = 0;

  for(int i = 0; i < 20000; ++i) {
    x.get(y,m,d);
    DateJd  lDj(y,m,d);
    std::cout << x << ' ' 
              << y << '.' << m << '.' << d << ' '
              << x.jd() << ' ' << lDj << ' ' << lDj.jd() << std::endl;
    ++x;
  }

  // std::cout << "today: " << DateJd().set_to_current_date() << std::endl;
}

void
test_y() {
  DateJd x(1968,01,01);
  uint y = 0;
  uint m = 0;
  uint d = 0;
  for(int i = 0; i < 20000; ++i) {
    const uint y1 = x.year();
    x.get(y,m,d);
    if(y != y1 || i < 10 || i > 19990) {
      std::cout << x << ' ' << y1 << ' ' << y << std::endl;
    }
    ++x;
  }
}


int
main() {

  // test_a();
  test_y();

  return 0;
}

