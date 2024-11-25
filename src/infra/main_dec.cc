#include <iostream>
#include <iomanip>

#include "decimal.hh"



int
main() {
  decimal32_2_t  d1(125, true);
  decimal32_2_t  d2(135, true);

  std::cout << "d1: " << d1 << std::endl;
  std::cout << "d2: " << d2 << std::endl;
  std::cout << "d1 + d2 = " << (d1 + d2) << std::endl;
 
  decimal32_2_t d3;


  const char* lNo = "-22.38ABC";
  const char* s = lNo;
  if(d3.read(s)) {
    std::cout << "d3: " << lNo << " = " << d3 << std::endl;
  } else {
    std::cout << "not valid: " << lNo << std::endl;
  }
  std::cout << "*s = " << (*s) << std::endl;

  decimal32_2_t d4(-3, true);

  std::cout << " -0.03 = " << d4 << std::endl; 
  d4 /= 2;
  std::cout << " -0.02 = " << d4 << std::endl;


  decimal32_2_t  d5;
  decimal32_2_t  d6;
  std::cout << "0|0| " << d5 << '|' << d6 << '|' << std::endl;

  decimal32_2_t  d7(15, true);
  decimal32_2_t  d8(17, true);

  std::cout << d7 << " * " << d8 << " = " << (d7 * d8) << std::endl;
 
  decimal32_2_t  d10(15, true);
  decimal32_2_t  d11(17, true);

  std::cout << d10 << " / "  << d11 << " = " << (d10 / d11) << std::endl;
  d10 = -d10;
  std::cout << d10 << " / "  << d11 << " = " << (d10 / d11) << std::endl;
  d11 = -d11;
  std::cout << d10 << " / "  << d11 << " = " << (d10 / d11) << std::endl;
  d10 = -d10;
  std::cout << d10 << " / "  << d11 << " = " << (d10 / d11) << std::endl;
}

