#ifndef INFRA_FROM_CHARPTR_HH
#define INFRA_FROM_CHARPTR_HH

#include <inttypes.h>

template<typename T>
bool from_charptr(T& x, const char*);

template<>
bool from_charptr<int32_t>(int32_t& x, const char* s) {
  char* lCharPtr = 0;
  int32_t y = strtol(s, &lCharPtr, 10);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<int64_t>(int64_t& x, const char* s) {
  char* lCharPtr = 0;
  int64_t y = strtol(s, &lCharPtr, 10);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<uint32_t>(uint32_t& x, const char* s) {
  char* lCharPtr = 0;
  int32_t y = strtol(s, &lCharPtr, 10);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<uint64_t>(uint64_t& x, const char* s) {
  char* lCharPtr = 0;
  int64_t y = strtol(s, &lCharPtr, 10);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<float>(float& x, const char* s) {
  char* lCharPtr = 0;
  float y = strtof(s, &lCharPtr);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<double>(double& x, const char* s) { 
  char* lCharPtr = 0;
  double y = strtod(s, &lCharPtr);
  if(lCharPtr == s) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_charptr<char>(char& x, const char* s) {
  if(0 != (*s)) {
    x = *s;
    return true;
  }
  return false;
}

template<>
bool from_charptr<bool>(bool& x, const char* s) {
  if((0 == strcmp(s, "true")) || (0 == strcmp(s, "TRUE")) || (0 == strcmp(s, "True"))) {
    x = true;
    return true;
  } else
  if((0 == strcmp(s, "false")) || (0 == strcmp(s, "FALSE")) || (0 == strcmp(s, "False"))) {
    x = false;
    return true;
  }

  return false;
}




#endif

