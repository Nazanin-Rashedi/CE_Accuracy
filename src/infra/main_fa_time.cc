#include "infra/glob_infra_standard_includes.hh"
#include <ctime>
#include <chrono>
#include "fa_time.hh"
#include "fa_date.hh"
#include "fa_timestamp.hh"

using namespace std::literals;

void
test0() {
  // using clock_t      = std::chrono::steady_clock;
  using clock_t      = std::chrono::system_clock;
  using duration_t   = clock_t::duration;
  using time_point_t = std::chrono::time_point<clock_t,duration_t>;
  using ns_t         = std::chrono::nanoseconds;
  time_point_t  lNow  = clock_t::now();
  duration_t    lDura = lNow.time_since_epoch();
  // std::time_t   lTime = clock_t::to_time_t(lNow); // this works, but is bad (resolution loss)
  std::cout << "standard c++ classes:" << std::endl;
  std::cout << "  sizeof(time_point_t) = " << sizeof(time_point_t) << std::endl;
  std::cout << "  sizeof(duration_t)   = " << sizeof(duration_t)   << std::endl;
  std::cout << "  sizeof(time_t)       = " << sizeof(time_t)       << std::endl;

  const auto lTicksPerSec = clock_t::period();
  std::cout << "  clock resolution     = " <<  lTicksPerSec.num << '/' << lTicksPerSec.den   << std::endl;
  std::cout << "  now in C++           = " <<  std::chrono::duration_cast<ns_t>(lDura).count() << std::endl;


}

using namespace fa;

void
test2() {
   std::cout << "sizeof(fa_time_t) = " << sizeof(fa_time_t) << std::endl;

   fa_time_t lTime(17, 44, 13);
   std::cout << "17:44:13     = " << lTime.val()
             << " = " << lTime
             << std::endl;

   lTime += 1388_ms;
   std::cout << "17:44:13 + x = " << lTime.val()
             << " = " << lTime
             << std::endl;

   lTime = "19:31:15.3"_ft;
   std::cout << "19:31:15.3   = " << lTime.val()
             << " = " << lTime
             << std::endl;

   lTime.time_of_day_local();
   std::cout << "time local   = " << lTime.val()
             << " = " << lTime
             << std::endl;

   lTime.time_of_day_utc();
   std::cout << "time utc     = " << lTime.val()
             << " = " << lTime
             << std::endl;


   std::cout << "ns/day: " << fa_time_t::ns_per_d() << std::endl;
}

void
test3() {
  fa_timestamp_t lTimestamp;
  lTimestamp.set_to_current_time_utc();
  std::cout << "now (utc): " << lTimestamp << std::endl;
  lTimestamp.set_to_current_time_local();
  std::cout << "now (loc): " << lTimestamp << std::endl;
  std::cout << "   sizeof(fa_date_t)      = " << std::setw(2) << sizeof(fa_date_t) << std::endl;
  std::cout << "   sizeof(fa_time_t)      = " << std::setw(2) << sizeof(fa_time_t) << std::endl;
  std::cout << "   sizeof(fa_timestamp_t) = " << std::setw(2) << sizeof(fa_timestamp_t) << std::endl;
}
  
int
main() {
  test0();
  test2();
  test3();
  return 0;
}

