#include "infra/glob_infra_standard_includes.hh"
#include "infra/hasht.hh"

#include <set>

/*
 *   CRC32
 */

typedef ht::HashCrc32<uint32_t> CRC32_32;
typedef ht::HashCrc32<uint64_t> CRC32_64;

void
test0(const uint64_t n) {
  // const uint32_t lSeed = 0xae08fe14;
  CRC32_32 lCrc;
  std::set<uint32_t> lSet;
  uint32_t h = 0;
  std::cout << std::hex;
  for(uint64_t i = 0; i < n; ++i) {
    // lCrc.seed(lSeed);
    // lSet.insert(h = lCrc.hash(i));
    lSet.insert(h = lCrc((uint32_t) i));
    if(10 > i) {
      std::cout << std::setw(8) << i << ' ' << std::setw(8) << h << std::endl;
    }
  }
  std::cout << std::dec;
  std::cout << n << " =?= " << lSet.size() << std::endl;
}

void
test1(const uint64_t n) {
  // const uint64_t lSeed = 0x6ca554372a5a9d8e;
  CRC32_64 lCrc;
  std::set<uint64_t> lSet;
  uint64_t h = 0;
  std::cout << std::hex;
  for(uint64_t i = 0; i < n; ++i) {
    // lCrc.seed(lSeed);
    // lSet.insert(h = lCrc.hash(i));
    lSet.insert(h = lCrc((uint64_t) i));
    if(10 > i) {
      std::cout << std::setw(16) << i << ' ' << std::setw(16) << h << std::endl;
    }
  }
  std::cout << std::dec;
  std::cout << n << " =?= " << lSet.size() << std::endl;
}



int
main() {
  const uint64_t n = 1000 * 1000;
  test0(n);
  test1(n);
  return 0;
}

