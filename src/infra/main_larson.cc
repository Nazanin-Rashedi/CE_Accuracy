#include "Larson.hh"

void
test0() {
  const bool lTrace = false;
  eqstr<uint64_t> lEqStr;
  const uint  lLength = 20;
  std::string lString0;
  std::string lString1;
  std::string lString2;
  std::set<uint64_t> lSet;
  uint64_t    lHashValue;
  const uint n = (1 << 25);
  for(uint i = 0; i < n; ++i) {
    lString1 = std::to_string(i);
    lString0 = std::string(lLength - lString1.size(), '0');
    lString2 = lString0 + lString1;
    lHashValue = lEqStr(lString2.c_str());
    lSet.insert(lHashValue);
    if(lTrace) {
      std::cout << std::setw(8) << i << "  "
                << std::setw(2) << lString2.size() << "  "
                << lString2  << "  "
                << std::setw(15) << lHashValue
                << std::endl;
    }
  }
  std::cout << n << ' ' << lSet.size() << std::endl;
}


int
main() {
  test0();
  return 0;
}


