#include "unionfind.hh"

UnionFind::UnionFind(uint aNoVar) : _noVar(aNoVar), _arr(aNoVar) {
  for(uint i = 0; i < noVar(); ++i) {
    _arr[i] = i;
  }
}

UnionFind::~UnionFind() {
}

bool
UnionFind::mUnion(const uint x, const uint y) {
  assert(x < noVar());
  assert(y < noVar());
  const uint xx = mFind(x);
  const uint yy = mFind(y);
  if(xx == yy) { return false; }
  mLink(xx, yy);
  return true;
}

void
UnionFind::mLink(const uint x, const uint y) {
  assert(x < noVar());
  assert(y < noVar());
  assert(x != y);
  assert(arr(x) == x);
  assert(arr(y) == y);
  if(x < y) {
    _arr[y] = x;
  } else {
    _arr[x] = y;
  }
}

uint
UnionFind::mFind(const uint x) {
  assert(x < noVar());
  if(arr(x) != x) {
    _arr[x] = mFind(arr(x));
  }
  return arr(x);
}

std::ostream&
UnionFind::print(std::ostream& os) {
  for(uint x = 0; x < noVar(); ++x) {
    if(mFind(x) == x) {
      os << std::setw(3) << x << ' ';
      for(uint y = 0; y < noVar(); ++y) {
        if(x == y) { continue; }
        if(mFind(y) == x) {
          os << ' ' << y;
        }
      }
      os << std::endl;
    }
  }
  return os;
}

std::ostream&
UnionFind::print(std::ostream& os, const string_vt& aVarNames) {
  for(uint x = 0; x < noVar(); ++x) {
    if(mFind(x) == x) {
      os << std::setw(3) << aVarNames[x] << ' ';
      for(uint y = 0; y < noVar(); ++y) {
        if(x == y) { continue; }
        if(mFind(y) == x) {
          os << ' ' << aVarNames[y];
        }
      }
      os << std::endl;
    }
  }
  return os;
}

std::ostream& 
operator<<(std::ostream& os, UnionFind& aUf) {
  return aUf.print(os);
}


