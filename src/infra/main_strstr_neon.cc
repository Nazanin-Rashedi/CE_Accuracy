#include <iostream>
#include "gmstrstr_neon.hh"


/* compile with
g++ -mcpu=cortex-a15 -mtune=cortex-a15 -mfloat-abi=hard -mfpu=neon main_strstr_neon.cc
*/


void
test0() {
  char a[] = "lsjdkfjalksdjflsjkdflk;ajsl;kjdkkkksklwlkwkwdjsjs";
  char b[] = "jflsjkdf";

  const char* x1 = strstr(a, b);
  const char* x2 = strstr_mula(a, strlen(a), b, strlen(b));

  std::cout << (x1 - a) << ' ' << (x2 - a) << std::endl;
}

void
test1() {
  const char* A[] = { "lskdfsajflkldkdfjslsdjfikjklsskdjfiowowief",
                      "klslkdfslfkekekekeeelskdsjfklsdkfekeeesfjk",
                      "skkekekekekekslkekekelskskelsllslllslewejl",
                      "sleiijslkdjfliejlsijflijsilejfliewjlwejfls" };
  int  n = sizeof(A) / sizeof(char*);
  char b[] = "kekeke";

  const char*  a  = 0;
  const char*  x1 = 0;
  const char*  x2 = 0;
  for(uint i = 0; i < n; ++i) {
    a = A[i];
    x1 = strstr(a, b);
    x2 = strstr_mula(a, strlen(a), b, strlen(b));
    std::cout << strlen(a) << ' ' << (x1 - a) << ' ' << (x2 - a)  << ' '
              << (0 != x1) << (0 != x2)
              << std::endl;
    if((0 != x1) != (0 != x2)) {
      std::cout << "BAD BAD" << std::endl;
    }
    
  }
}


int
main() {
  test0();
  test1();
  return 0;
}

