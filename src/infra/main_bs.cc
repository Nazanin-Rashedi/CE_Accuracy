#include "glob_infra_standard_includes.hh"

extern "C" {
  #include "cmeasure.h"
}

#include "binary_search.hh"
#include "vector_ops.hh"
#include "random_permutation.hh"

void
test0() {
  const int f = 4;
  int_vt arr(10);
  for(int i = 0; i < 10; ++i) {
    arr[i] = f * i;
  }


  std::cout << "idx  ";
  for(int i = 0; i < 10; ++i) {
    std::cout << ' ' << std::setw(3) << i;
  }
  std::cout << std::endl;
  std::cout << "arr = " << std::setw(3) << arr << std::endl;



  int idx = 0;
  std::cout << "  i idx arr" << std::endl;
  for(int i = -2; i < 25; ++i) {
    idx = binary_search(i, arr.data(), 10);
    std::cout << std::setw(3) << i << ' '
              << std::setw(3) << idx << ' '
              << std::setw(3) << arr[idx] << ' '
              << std::endl;
  }
}



void
test1(const int aLog2N) {
  const int n = (1 << aLog2N);
  std::mt19937 lRg;

  int_vt arr(n);
  for(int i = 0; i < n; ++i) {
    arr[i] = i;
  }
  random_permutation(lRg, arr);

  int lSum = 0;
  const int B = std::max(1, ((1000 * 1000 * 100) / n) / aLog2N);
  cmeasure_t lCm;
  cmeasure_start(&lCm);
  for(int b = 0; b < B; ++b) {
    for(int i = 0; i < n; ++i) {
      lSum += binary_search(i, arr.data(), n);
    }
  }
  cmeasure_stop(&lCm);
  cmeasure_print(&lCm, "# runtime: %4.3f");
  std::cout << std::endl;
  std::cout << "# sum = " << lSum << std::endl;

  std::cout << aLog2N  << ' ' 
            << n << ' ' 
            << cmeasure_div_ns(&lCm, (double) n * (double) B) << ' '
            << cmeasure_div_ns(&lCm, (double) n * (double) B * (double) aLog2N)
            << std::endl;

  
}


int
main() {
  test0();

  for(int l = 10; l <= 28; ++l) {
    test1(l);
  }

  return 0;
}

