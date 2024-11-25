#include "infra/glob_infra_standard_includes.hh"

#include "bitvectorgeneratorkofn.hh"
#include "BvGeneratorKofN.hh"
#include "bitvectorsmall.hh"

typedef BitvectorGeneratorKofN<uint32_t> bvgen1_t;
typedef BvGenKofN<uint32_t>              bvgen2_t;
typedef Bitvector32                      bv_t;
void
test0(const uint n, const uint k) {
  bvgen1_t lGen1(n,k);
  bvgen2_t lGen2(n,k);

  uint i = 0;
  while(lGen1.ok()) {
    std::cout << std::setw(n) << bv_t(*lGen1) << ' '
              << std::setw(n) << bv_t(*lGen2) 
              << std::endl;
    assert(*lGen1 == *lGen2);
    ++lGen1;
    ++lGen2;
    ++i;
  }
  std::cout << i << std::endl;
}


int
main() {
  test0(5,3);
  test0(6,3);
  test0(7,3);
  return 0;
}

