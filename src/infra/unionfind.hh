#ifndef INFRA_UNION_FIND_HH
#define INFRA_UNION_FIND_HH
#pragma once

#include "infra/glob_infra_standard_includes.hh"
#include <string>

typedef std::vector<std::string> string_vt;

class UnionFind {
  private:
    UnionFind(const UnionFind&);
    UnionFind& operator=(const UnionFind&);
  public:
    UnionFind(uint aNoVar);
    ~UnionFind();
  public:
    inline uint noVar() const { return _noVar; }
    inline uint arr(uint i) const { return _arr[i]; }
           bool mUnion(const uint x, const uint y); // returns true, falls nicht bereits in gleicher Menge
           uint mFind(const uint x);
    inline bool mEqual(const uint x, const uint y) { return (mFind(x) == mFind(y)); }
    inline bool isRepresentative(const uint x) const { return (_arr[x] == x); }
  public:
    std::ostream& print(std::ostream& os);
    std::ostream& print(std::ostream& os, const string_vt& aVarNames);
  private:
    void mLink(const uint x, const uint y);
  private:
    uint    _noVar;
    uint_vt _arr;
};

std::ostream& operator<<(std::ostream& os, UnionFind& aUf);

#endif
