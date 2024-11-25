#ifndef INFRA_TOP_K_HH
#define INFRA_TOP_K_HH

#include <iostream>
#include <iomanip>
#include <set>

#include "infra/PairingHeap.hpp"

// implements a class to keep the top k elements

template<class Telem, typename Tcmp = std::less<Telem> >
class TopK {
  public:
    typedef unsigned int uint;
    typedef Telem elem_t;
    typedef Tcmp  cmp_t;
    typedef PairingHeap<Telem, Tcmp> heap_t;
    typedef std::set<elem_t> set_t; // should be hash table
  public:
    TopK(const uint aK);
  public:
    inline bool empty() const { return _heap.empty(); }
    inline uint k() const { return _k; }
    inline uint size() const { return _heap.size(); }
    inline uint noEntries() const { return _heap.size(); }
    inline const elem_t& top() const { return _heap.top(); }
    inline void pop() { _heap.pop(); }
    inline const cmp_t& cmp() const { return _cmp; }
  public:
    void insert(const elem_t& x);
  private:
    heap_t _heap;
    uint   _k;
    cmp_t  _cmp;
};

template<class Telem, typename Tcmp>
TopK<Telem,Tcmp>::TopK(const uint aK) 
                 : _heap(), _k(aK), _cmp() {
};

template<class Telem, typename Tcmp>
void
TopK<Telem, Tcmp>::insert(const elem_t& x) {
  // std::cout << "  insert " << x << std::endl;

  if(0 == size()) {
    // std::cout << "    noch leer" << std::endl;
    _heap.push(x);
  } else
  if(k() > size()) {
    // std::cout << "      heap noch klein" << std::endl;
   _heap.push(x);
  } else {
    if(cmp()(x, top())) {
      // std::cout << "        x,top: " << x << ' ' << top() << std::endl;
      _heap.pop();
      _heap.push(x);
    }
  }
}


#endif

