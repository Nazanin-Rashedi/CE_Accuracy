#include <iostream>
#include <iomanip>
#include "../../../infra/bitvectorsmall.hh"


struct hdr_t {
  uint16_t a : 1;
  uint16_t m : 14;
  uint16_t z : 1;
};


void
test0() {
  union {
    hdr_t    _hdr;
    uint16_t _num;
    struct {
      uint8_t _a;
      uint8_t _b;
    }  _ab;
  } lHdr;
  lHdr._num = 0;

  Bitvector16 lBv(lHdr._num);
  lBv.printReverse(std::cout);
  std::cout << std::endl;

  lHdr._hdr.a = 1;
  lBv = lHdr._num;
  lBv.printReverse(std::cout);
  std::cout << std::endl;

  lHdr._num = 0;
  lBv = lHdr._num;
  lBv.printReverse(std::cout);
  std::cout << std::endl;

  lHdr._ab._a = 0x1;
  lBv = lHdr._num;
  lBv.printReverse(std::cout);
  std::cout << std::endl;


}

int
main() {
  test0();

  return 0;
}

