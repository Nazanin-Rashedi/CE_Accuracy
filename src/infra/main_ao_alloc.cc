#include "AoAlloc.hh"
#include <string>

typedef std::vector<std::string> string_vt;

class A {
  public:
    A() : _a(0) {}
    A(int x) : _a(x) {}
    virtual ~A() {}
  public:
    inline int a() const { return _a; }
    inline void a(int x) { _a = x; }
    virtual int get_min() const { return _a; }
  protected:
    int _a;
};

class B : public A {
  public:
    B() : A(), _b(0) {}
    B(int x, int y) : A(x), _b(y) {}
    virtual ~B() {}
  public:
    inline int b() const { return _b; }
    inline void b(int x) { _b = x; }
    virtual int get_min() const { return std::min<int>(a(), b()); }
  protected:
    int _b;
};

void
test0(uint aChunkSize) {
  std::cout << "=== test0 (" << aChunkSize << ") ===" << std::endl;
  string_vt S = {
    "Anton",
    "Berta",
    "Christina",
    "Dorothea",
    "Emil",
    "Friedrich",
    "Gerhard",
    "Harald",
    "Inge",
    "Joachim",
    "Karl",
    "Leopold",
    "Mozart",
    "Norbert"
  };

  AoAlloc lAoAlloc(aChunkSize, true);

  for(uint i = 0; i < S.size(); ++i) {
    byte_t* s = lAoAlloc.allocate(S[i].size());
    memcpy(s, S[i].c_str(), S[i].size());
  }

  for(uint i = 0; i < lAoAlloc.no_chunks_used(); ++i) {
    byte_t* c = lAoAlloc.get_chunk_used(i);
    std::cout << "  " << std::setw(2) << i << ": ";
    for(uint j = 0; j < aChunkSize; ++j) {
      if(isprint(c[j])) {
        std::cout << c[j];
      } else {
        std::cout << '.';
      }
    }
    std::cout << std::endl;
  }
}

void
test1() {
  std::cout << "=== test1 ===" << std::endl;
  const uint lChunkSize = 100;
  AoAlloc lAoAlloc(lChunkSize);

  std::vector<A*> lVec;

  for(uint i = 0; i < 20; ++i) {
    A* a = new(lAoAlloc.allocate(sizeof(A))) A;
    a->a(i);
    lVec.push_back(a);
    B* b = new(lAoAlloc.allocate(sizeof(B))) B;
    b->a(7);
    b->b(i);
    lVec.push_back(b);
  }

  for(uint i = 0; i < lVec.size(); ++i) {
    std::cout << i << ' ' << lVec[i]->get_min() << std::endl;
  }
}

void
test2() {
  std::cout << "=== test2 ===" << std::endl;
  const uint lChunkSize = 100;
  AoAlloc lAoAlloc(lChunkSize);

  std::vector<A*> lVec;

  for(uint i = 0; i < 20; ++i) {
    A* a = new(lAoAlloc.allocate(sizeof(A))) A(i);
    lVec.push_back(a);
    B* b = new(lAoAlloc.allocate(sizeof(B))) B(7,i);
    lVec.push_back(b);
  }

  for(uint i = 0; i < lVec.size(); ++i) {
    std::cout << i << ' ' << lVec[i]->get_min() << std::endl;
  }
}


template<typename T>
T*
get_new(AoAlloc& aAoAlloc) {
  return new(aAoAlloc.allocate(sizeof(T))) T;
}

void
test3() {
  std::cout << "=== test3 ===" << std::endl;
  const uint lChunkSize = 100;
  AoAlloc lAoAlloc(lChunkSize);

  std::vector<A*> lVec;

  for(uint i = 0; i < 20; ++i) {
    A* a = get_new<A>(lAoAlloc);
    a->a(i);
    lVec.push_back(a);
    B* b = get_new<B>(lAoAlloc);
    b->a(7);
    b->b(i);
    lVec.push_back(b);
  }

  for(uint i = 0; i < lVec.size(); ++i) {
    std::cout << i << ' ' << lVec[i]->get_min() << std::endl;
  }
}

int
main() {
  test0(11);
  test0(17);
  test0(42);
  test1();
  test2();
  return 0;
}
 
