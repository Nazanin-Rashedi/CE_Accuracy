#ifndef INFRA_FROM_STRING_HH
#define INFRA_FROM_STRING_HH

#include <inttypes.h>

template<typename T>
bool from_string(T& x, const std::string&);

template<>
bool from_string<int32_t>(int32_t& x, const std::string& s) {
  char* lCharPtr = 0;
  int32_t y = strtol(s.c_str(), &lCharPtr, 10);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<int64_t>(int64_t& x, const std::string& s) {
  char* lCharPtr = 0;
  int64_t y = strtol(s.c_str(), &lCharPtr, 10);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<uint32_t>(uint32_t& x, const std::string& s) {
  char* lCharPtr = 0;
  int32_t y = strtol(s.c_str(), &lCharPtr, 10);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<uint64_t>(uint64_t& x, const std::string& s) {
  char* lCharPtr = 0;
  int64_t y = strtol(s.c_str(), &lCharPtr, 10);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<float>(float& x, const std::string& s) {
  char* lCharPtr = 0;
  float y = strtof(s.c_str(), &lCharPtr);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<double>(double& x, const std::string& s) { 
  char* lCharPtr = 0;
  double y = strtof(s.c_str(), &lCharPtr);
  if(lCharPtr == s.c_str()) {
    return false;
  }
  x = y;
  return true;
}

template<>
bool from_string<char>(char& x, const std::string& s) {
  if(0 < s.size()) {
    x = s[0];
    return true;
  }
  return false;
}

template<>
bool from_string<bool>(bool& x, const std::string& s) {
  if((s == "true") || (s == "TRUE") || (s == "True")) {
    x = true;
    return true;
  } else
  if((s == "false") || (s == "FALSE") || (s == "False")) {
    x = false;
    return true;
  }

  return false;
}




#endif

