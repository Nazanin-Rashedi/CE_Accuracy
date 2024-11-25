#include <iostream>

int
main() {

  char c = -126;
  std::cout << "c: " << (int) c << std::endl;
  c = -127;
  std::cout << "c: " << (int) c << std::endl;
  c = -128;
  std::cout << "c: " << (int) c << std::endl;

  c = 126;
  std::cout << "c: " << (int) c << std::endl;
  c = 127;
  std::cout << "c: " << (int) c << std::endl;
  c = 128;
  std::cout << "c: " << (int) c << std::endl;

  return 0;
}
