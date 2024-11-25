#include <iostream>
#include <iomanip>

#include "TopK.hh"



void
test0() {
  typedef TopK<int> topk_t;
  topk_t lTopK(10);

  for(int i = 1; i < 100; ++i) {
    lTopK.insert(i);
  }
  std::cout << "lTopK.size() = " << lTopK.size() << std::endl;
  while(!lTopK.empty()) {
    std::cout << ' ' << lTopK.top();
    lTopK.pop();
  }
  std::cout << std::endl;
}


int
main() {

  test0();

  return 0;
}

