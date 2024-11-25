#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <set>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

bool
digit_only(const std::string& s) {
  for(size_t i = 0; i < s.size(); ++i) {
    if(!isdigit(s[i])) {
      return false;
    }
  }
  return true;
}

int
to_number(const std::string& s) {
  char* x = 0;
  const int lRes = strtol(s.c_str(), &x, 10);
  if(x == s) {
    assert(0 == 1);
  }
  return lRes;
}

bool
is_rating(const std::string& s) {
  if('R' != s[0]) {
    return false;
  }
  for(size_t i = 1; i < s.size(); ++i) {
    if(!isdigit(s[i])) {
      return false;
    }
  }
  return true;
}

typedef std::map<std::string, uint32_t> map_t;
typedef std::set<int>                   int_st;

void
print(std::ostream& os, const map_t& aMap) {
  for(auto x: aMap) {
    os << std::setw(20) << x.first << "  "
       << std::setw( 4) << x.second
       << std::endl;
  }
}

void
read(std::istream& aIs, map_t& aMap, int_st& aSet) {
  std::string lString;
  bool lHasRating = true;
  while(aIs) {
    aIs >> lString;
    if(aIs.eof()) { break; }
    if(!digit_only(lString)) {
      aMap[lString]++;
      if(is_rating(lString)) {
        lHasRating = true;
      }
    } else {
      // number only
      // check for duplicate numbers
      const int lNo = to_number(lString);
      // if(aSet.contains(lNo)) 
      if(aSet.end() != aSet.find(lNo)) {
        std::cout << "E0: Bild Nummer doppelt: " << lNo << std::endl;
      }
      aSet.insert(lNo);
      // check presence of rating in previous line
      if(!lHasRating) {
        std::cout << "W0: Bild vor " << lString << " has no rating." << std::endl;
      }
      lHasRating = false;
    }
  }
}

void
test0(std::istream& aIs) {
  map_t  lMap;
  int_st lSet;
  read(aIs, lMap, lSet);
  print(std::cout, lMap);
}

  

int
main(const int argc, const char* argv[]) {
  if(2 != argc) {
    std::cout << "usage: word_count <filename>" << std::endl;
    return -1;
  }
  std::ifstream lIs(argv[1]);
  if(!lIs) {
    std::cout << "can't open file '" << argv[1] << "\'." << std::endl;
    return -1;
  }
  test0(lIs);
  return 0;
}



