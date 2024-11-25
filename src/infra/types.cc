#include "types.hh"

std::ostream& 
print_string_vec(std::ostream& os, const string_vt& aStringVec) {
  for(auto x : aStringVec) {
    os << ' ' << x;
  }
  return os;
}

std::ostream&
operator<<(std::ostream& os, const string_vt& aStringVec) {
  return print(os, aStringVec);
}

