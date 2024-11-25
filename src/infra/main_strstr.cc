#include <iostream>
#include <iomanip>
#include <string.h>

#include "gmstrstr16.hh"
#include "gmstrstr16_42.hh"

int
main() {

  char p[16];
  memset(p, 0, 16);
  p[0] = 'J';
  p[1] = 'o';
  p[2] = 'e';
  p[3] = 'H';
  p[4] = '\0';

  const int lNoString = 5;
  const char* x[lNoString] = {
                               "lsAAAAAAAAAAkdfjlskJoeHannlsdkf",
                               "JXoeXHannlsdkf",
                               "JXXXXXXXXXXXXXXXXXXXXJoeHannlsdkf",
                               "JXXXXXXXXXXXXXXXXXXXXJoeannlsdkfXXXABC\0DEFGHIJKLMNOPQR",
                               "0123456789ABCJoeannlsdkfXXXABCDEFGHIJKLMNOPQR"
                             };

  for(int i = 0; i < lNoString; ++i) {
    std::cout << "pattern: " << p << std::endl;
    std::cout << "string : " << x[i] << std::endl;
 
    const char* y = gmstrstr16_42(x[i], p, strlen(p)); 
    std::cout << "result : " << ((0 == y) ? "NULL" : y) << std::endl;

  }

  const char* x0 = "KokoloresJaja";
  const char* x1 = "KokolXresJaja";
  const char* p0 = "Kokolores";

  const int lIdx0 = gmprefix16_42_inl(x0, p0, 9);
  std::cout << "idx 0: " << lIdx0 << std::endl;
  const int lIdx1 = gmprefix16_42_inl(x1, p0, 9);
  std::cout << "idx 1: " << lIdx1 << std::endl;

  return 0;
}

