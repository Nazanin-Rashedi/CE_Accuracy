#include <iostream>
#include <iomanip>

#include "PairingHeap.hpp"

typedef PairingHeap<int> heap_t;

int
main() {

  heap_t lHeap;

  int a[] = {5, 6, 4, 7, 3, 8, 2, 9, 1, 10, 0};

  for(int i = 0; i < 10; ++i) {
    if(5 > lHeap.size()) {
      lHeap.push(a[i]);
    } else {
      if(a[i] < lHeap.top()) {
        lHeap.push(a[i]);
        std::cout << "popd: " << lHeap.top() << std::endl;
        lHeap.pop();
      } else {
        std::cout << "not pushed: " << a[i] << std::endl;
      }
    }
    std::cout << "push(" << std::setw(2) << a[i] 
              << "): top: " << std::setw(2) << lHeap.top() 
              << "  size: " << lHeap.size()
              << std::endl;
  }

  std::cout << "The heap: " << std::endl;
  while(!lHeap.empty()) {
    std::cout << std::setw(2) << lHeap.top() << std::endl;
    lHeap.pop();
  }
}
