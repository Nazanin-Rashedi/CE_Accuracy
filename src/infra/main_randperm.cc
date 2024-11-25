#include <iostream>
#include <random>
#include "random_permutation.hh"


void
print(std::vector<int> x) {
  for(unsigned int i = 0; i < x.size(); ++i) {
    std::cout << x[i] << ' ';
  }
  std::cout << std::endl;
}

int
main() {
  std::mt19937 lRng(17);
  std::vector<int> x;
  random_permutation(lRng, x, 0, 10);
  print(x);
  random_permutation(lRng, x, 0, 10);
  print(x);
  random_permutation(lRng, x, 0, 10);
  print(x);

  int a[10];
  for(int i = 0; i < 10; ++i) {
    a[i] = i;
  }
  random_permutation(lRng, a, 10);
  for(int i = 0; i < 10; ++i) {
    std::cout << a[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}
