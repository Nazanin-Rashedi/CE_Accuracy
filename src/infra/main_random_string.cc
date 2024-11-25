#include <iostream>
#include <iomanip>
#include "random_string.hh"


void
printString(const char* x, const size_t n) {
  std::cout << "string: ";
  for(size_t i = 0; i < n; ++i) {
    std::cout << x[i];
  }
  std::cout << std::endl;
}

int
main() {
  char x[100];
  std::mt19937 lRng;

  std::cout << "random string: " << std::endl;

  for(int i = 0; i < 10; ++i) {
    fRandomString(x, 10);
    printString(x, 10);
  }

  std::cout << "random string 01: p=2:" << std::endl;
  for(uint i = 0; i < 10; ++i) {
    fRandomString01(x, 10, 2);
    printString(x, 10);
  }

  std::cout << "random string 01: p=4:" << std::endl;
  for(uint i = 0; i < 10; ++i) {
    fRandomString01(x, 10, 4);
    printString(x, 10);
  }

  std::cout << "numbers from 1 to 10:" << std::endl;
  for(uint i = 1; i <= 10; ++i) {
    fUint2Char(x, 10, i);
    printString(x, 10);
  }

  std::cout << "random numbers:" << std::endl;
  for(uint i = 0; i < 10; ++i) {
    fUint2Char(x, 32, lRng());
    printString(x, 32);
  }

  std::cout << "random numbers in Hex:" << std::endl;
  std::cout << std::hex;
  for(uint i = 0; i < 10; ++i) {
    std::cout << "0x" << lRng() << std::endl; 
  }
  std::cout << std::dec;
    
}


