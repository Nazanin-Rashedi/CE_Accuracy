#include "infra/glob_infra_standard_includes.hh"
#include "bit_intrinsics.hh"
#include "bitvectorsmall.hh"

void
test0() {
  uint r1, r2, r3;
  for(uint x = 0; x < 32; ++x) {
     r1 = set_trailing_zeros(x);
     r2 = _blsmsk_u32(x);
     r3 = (x ^ (x - 1));
     std::cout << std::setw(6) << Bitvector32(x ) << "  "
               << std::setw(6) << Bitvector32(r1) << "  "
               << std::setw(6) << Bitvector32(r2) << "  "
               << std::setw(6) << Bitvector32(r3)
               << std::endl;
  }
}


int
main() {
  test0();
  return 0;
}

