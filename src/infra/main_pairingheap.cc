#include <iostream>
#include <iomanip>

#include "PairingHeap.hpp"

class LessInt {
  public:
    bool operator()(const int i, const int j) const { return (i < j); }
};

class GtInt {
  public:
    bool operator()(const int i, const int j) const { return (i > j); }
};

void
test0() {
  PairingHeap<int, LessInt> lHeap;
  for(int i = 0; i < 10; ++i) {
    lHeap.push(i);
  }
  std::cout << "HeapLess:";
  while(!lHeap.empty()) {
    std::cout << ' ' << lHeap.top();
    lHeap.pop();
  }
  std::cout << std::endl;
}

void
test1() {
  PairingHeap<int, GtInt> lHeap;
  for(int i = 0; i < 10; ++i) {
    lHeap.push(i);
  }
  std::cout << "HeapGt:";
  while(!lHeap.empty()) {
    std::cout << ' ' << lHeap.top();
    lHeap.pop();
  }
  std::cout << std::endl;
}


int
main() {
  test0();
  test1();
  return 0;
}

 
