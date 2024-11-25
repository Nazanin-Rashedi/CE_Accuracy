#include "print_test_header.hh"

void
print_test_header(const char* aTestName, int aTot) {
  print_test_header(std::cout, aTestName, aTot);
}

void
print_test_header(std::ostream& os, const char* aTestName, int aTot) {
  const int lLen = strlen(aTestName) + 4; // 2 blanks links/rechts
  const int lTot = aTot;
  const int lLeft = ((lTot - lLen) / 2);
  const int lRight = lTot - lLeft - lLen;

  os << std::string(lTot, '=') << std::endl
     << std::string(lLeft, '=') << "  " << aTestName << "  " << std::string(lRight, '=') << std::endl
     << std::string(lTot, '=') << std::endl
     << std::endl;
}

