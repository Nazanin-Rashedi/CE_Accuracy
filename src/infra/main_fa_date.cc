#include "fa_date.hh"

#include <iomanip>
#include <limits>
#include <assert.h>

void
test0(const int n) {
  int lPrint = 0;
  for(fa::fa_date_t::rep_t i = 0; i < n; ++i) {
    fa::fa_date_t lDate1(i);
    std::string   lStr = lDate1.to_string();
    fa::fa_date_t lDate2(lStr);
    fa::fa_date_t::rep_t j = lDate2.jd();
    if(i == lPrint) {
      std::cout         << std::setw( 8) << i
                << "  " << std::setw(12) << lStr
                << "  " << std::setw( 8) << j
                << std::endl;
      ++lPrint;
      if(10 < lPrint) { lPrint *= 2; }
    }
    assert(i == j);
  }
  int32_t lMx = std::numeric_limits<int32_t>::max();
  lPrint = 0;
  int lCnt = 0;
  for(fa::fa_date_t::rep_t i = lMx; i > (lMx - n); --i) {
    fa::fa_date_t lDate1(i);
    std::string   lStr = lDate1.to_string();
    fa::fa_date_t lDate2(lStr);
    fa::fa_date_t::rep_t j = lDate2.jd();
    if(lCnt++ == lPrint) {
      std::cout         << std::setw( 8) << i
                << "  " << std::setw(12) << lStr
                << "  " << std::setw( 8) << j
                << std::endl;
      ++lPrint;
      if(10 < lPrint) { lPrint *= 2; }
    }
    assert(i == j);
  }
}

using namespace fa;

void
test1() {
  fa_date_t lTodayLoc = fa_date_t::today_local();
  fa_date_t lTodayUtc = fa_date_t::today_utc();

  std::cout << "heute: " << lTodayLoc << " oder " << lTodayUtc << std::endl;
}

void
testA() {
  double x = std::numeric_limits<double>::max();
  double d = 24.0L * 60.0L * 60.0L * 1000.0L * 1000.0L * 1000.0L;
  std::cout << "x/s = " << x/d << std::endl;
}

int
main() {
  // test0(77777777);
  test1();
  testA();
  return 0;
}

