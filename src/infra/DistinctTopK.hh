#ifndef INFRA_DISTINCT_TOP_K_HH
#define INFRA_DISTINCT_TOP_K_HH

#include <iostream>
#include <iomanip>
#include <set>

#include "infra/PairingHeap.hpp"

// implements a class to keep the top k distinct elements

template<class Telem, typename Tcmp = std::less<Telem> >
class DistinctTopK {
  public:
    typedef Telem elem_t;
    typedef Tcmp  cmp_t;
    typedef PairingHeap<Telem, Tcmp> heap_t;
    typedef std::set<elem_t> set_t; // should be hash table
  public:
    DistinctTopK();
    DistinctTopK(const uint aK);
  public:
    inline bool empty() const { return _heap.empty(); }
    inline uint k() const { return _k; }
    inline uint size() const { return _heap.size(); }
    inline uint noEntries() const { return _set.size(); }
    inline const elem_t& top() const { return _heap.top(); }
    inline void pop() { _heap.pop(); }
    inline const cmp_t& cmp() const { return _cmp; }
    inline const set_t& get_set() const { return _set; }
  public:
    void insert(const elem_t& x);
  public:
    void init(const uint aK); // if constructor () is used only
    void clear();
  private:
    heap_t _heap;
    set_t  _set; // containing all elements for faster lookup
    uint   _k;
    cmp_t  _cmp;
};

template<class Telem, typename Tcmp>
DistinctTopK<Telem,Tcmp>::DistinctTopK() 
                 : _heap(), _set(), _k(0), _cmp() {
};

template<class Telem, typename Tcmp>
DistinctTopK<Telem,Tcmp>::DistinctTopK(const uint aK) 
                 : _heap(), _set(), _k(aK), _cmp() {
};

template<class Telem, typename Tcmp>
void
DistinctTopK<Telem, Tcmp>::insert(const elem_t& x) {
  // std::cout << "  insert " << x << std::endl;

  if(0 == size()) {
    // std::cout << "    noch leer" << std::endl;
    _heap.push(x);
    _set.insert(x);
  } else
  if(_set.end() == _set.find(x)) {
    // std::cout << "    noch nicht drin" << std::endl;
    if(k() > size()) {
      // std::cout << "      heap noch klein" << std::endl;
     _heap.push(x);
     _set.insert(x);
    } else {
      if(cmp()(x, top())) {
        // std::cout << "        x,top: " << x << ' ' << top() << std::endl;
        _set.erase(_set.find(top()));
        _heap.pop();
        _heap.push(x);
        _set.insert(x);
      }
    }
  } else {
    // std::cout << "    schon drin" << std::endl;
  }
}

template<class Telem, typename Tcmp>
void
DistinctTopK<Telem, Tcmp>::init(const uint aK) {
  _k = aK;
  clear();
}

template<class Telem, typename Tcmp>
void
DistinctTopK<Telem, Tcmp>::clear() {
  _heap.clear();
  _set.clear();
}
  

#endif

