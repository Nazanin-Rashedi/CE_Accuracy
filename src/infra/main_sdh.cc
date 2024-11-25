#include <iostream>
#include <iomanip>

#include <unordered_map>
#include <functional>

#include "StringDictionaryHashed.hh"

void
test0() {
  const char* x1 = "anton";
  const char* x2 = "anna";
  const char* x3 = "guido";
  const char* x4 = "mama";
  const char* x5 = "anton";

  typedef std::unordered_map<char*,int>  hmap_t;
  hmap_t lMap;
  lMap.insert(std::pair<char*,int>((char*) x1,1));
  lMap.insert(std::pair<char*,int>((char*) x2,2));
  lMap.insert(std::pair<char*,int>((char*) x3,3));
  lMap.insert(std::pair<char*,int>((char*) x4,4));
  lMap.insert(std::pair<char*,int>((char*) x5,5));
  lMap.insert(std::pair<char*,int>((char*) x2,2));
  lMap.insert(std::pair<char*,int>((char*) x3,3));
  lMap.insert(std::pair<char*,int>((char*) x4,4));
  lMap.insert(std::pair<char*,int>((char*) x5,5));

  for(hmap_t::const_iterator iter = lMap.begin(); iter != lMap.end(); ++iter) {
    std::cout << (*iter).second << "  " << (*iter).first << std::endl;
  }
}


void
test1() {
  char x[] = "antonannaguidochristina";

  StringDictionaryHashed lSdh(10, 10);
  const int n = strlen(x);
  // std::hash<std::string> StrHashFn1;
  // std::hash<const char*> StrHashFn2;
  for(int i = 0; i < n; ++i) {
    for(uint j = i+1; j < n; ++j) {
       lSdh.insert(x + i, x + j);
    }
  }
  lSdh.print(std::cout);
}

int
main() {
  test0();
  test1();

  return 0;
}

