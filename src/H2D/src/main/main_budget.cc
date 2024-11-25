#include "infra/glob_infra_standard_includes.hh"
#include "infra/tmath.hh"

void
print_header() {
  std::cout << std::setw(2)  << "i" << ' '
            << std::setw(12) << "|R|" << ' '
            // << std::setw(12) << "0.01n" << ' '
            // << std::setw(12) << "8*|R|" << ' '
            << std::setw(12) << "|S|" << ' '
            << std::setw(12) << "|S| in %" << ' '
            << std::setw(12) << "b=8*|S|" << ' '
            << std::setw(12) << "nx/ny" << ' '
            << std::setw(12) << "theta" << ' '
            << std::setw(12) << "theta in %%" << ' '
            << std::endl;
}

void
print_line(const uint aExp, const uint aBase) {
    const double n = pow(aBase, aExp);       // cardinality
    const double m = floor(sqrt(n*log(n)));  // sample size |S|
    // const double s = n * 8;                  // 8*|R| = ||R||
    const double b = m * 8;                  // budget = 8*|S|
    const uint   nx = (uint) (std::ceil(std::sqrt(b)));
    const double theta = ceil(sqrt(n / log(n))); // theta
    const double thetaPr = ((1000 * theta) / n); // theta in promille
    std::cout << std::setw(2)  << aExp << ' '
              << std::setw(12) << ((uint64_t) n)          << ' '
              // << std::setw(12) << ((uint64_t) (0.01 * n)) << ' '
              // << std::setw(12) << ((uint64_t) s) << ' '
              << std::setw(12) << ((uint64_t) m) << ' '
              << std::setw(12) << mt::roundXXXt((100 * m) / n) << ' '
              << std::setw(12) << ((uint64_t) b) << ' '
              << std::setw(12) << nx << ' '
              << std::setw(12) << ((uint64_t) theta) << ' '
              << std::setw(12) << mt::roundXXXt(thetaPr) << ' '
              << std::endl;
}

void
test0() {
  print_header();
  std::cout << std::fixed;
  std::cout.precision(3);
  for(int i = 4; i < 10; ++i) {
    print_line(i, 10);
  }
  std::cout << std::endl;
  print_header();
  for(int i = 12; i < 29; ++i) {
    print_line(i, 2);
  }
  std::cout << std::endl;
}



int
main() {
  test0();
  return 0;
}

