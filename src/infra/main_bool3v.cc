#include <iostream>
#include <iomanip>

#include "bool3v.hh"

typedef unsigned int uint;

void
testNot() {
  std::cout << "not" << std::endl;
  bool3v lVal[3] = {bool3v::False(), bool3v::True(), bool3v::Unknown() };

  for(uint i = 0; i < 3; ++i) {
    bool3v x = lVal[i];
    bool3v y = x.Not();
    std::cout << x << ' ' << x.Not() << std::endl;
  }
}

void
testAnd() {
  std::cout << "and" << std::endl;
  bool3v lVal[3] = {bool3v::False(), bool3v::True(), bool3v::Unknown() };
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      bool3v x = lVal[i];
      bool3v y = lVal[j];
      std::cout << x << ' ' << y << "  " << x.And(y) << std::endl;
    }
  } 
}


void
testOr() {
  std::cout << "or" << std::endl;
  bool3v lVal[3] = {bool3v::False(), bool3v::True(), bool3v::Unknown() };
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      bool3v x = lVal[i];
      bool3v y = lVal[j];
      std::cout << x << ' ' << y << "  " << x.Or(y) << std::endl;
    }
  } 
}

void
testXor() {
  std::cout << "xor" << std::endl;
  bool3v lVal[3] = {bool3v::False(), bool3v::True(), bool3v::Unknown() };
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      bool3v x = lVal[i];
      bool3v y = lVal[j];
      std::cout << x << ' ' << y << "  " << x.Xor(y) << std::endl;
    }
  }
}

void
testImpl() {
  std::cout << "impl" << std::endl;
  bool3v lVal[3] = {bool3v::False(), bool3v::True(), bool3v::Unknown() };
  for(uint i = 0; i < 3; ++i) {
    for(uint j = 0; j < 3; ++j) {
      bool3v x = lVal[i];
      bool3v y = lVal[j];
      std::cout << x << ' ' << y << "  " << x.Impl(y) << std::endl;
    }
  }
}

int
main() {
  testNot();
  testAnd();
  testOr();
  testXor();
  testImpl();

  return 0;
}


