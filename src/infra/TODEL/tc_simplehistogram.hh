#ifndef TC_SIMPLEHISTOGRAM_HH
#define TC_SIMPLEHISTOGRAM_HH

#include <iostream>
#include <vector>
#include <map>
#include "types.hh"

template<class x_t, class y_t>
class TcSimpleHistogram {
  public:
    typedef std::map<x_t, y_t> map_t;
    typedef typename map_t::const_iterator const_iterator;
  public:
    TcSimpleHistogram() : _map() {}
    TcSimpleHistogram(const std::vector<x_t>& v) : _map() {
      for(typename std::vector<x_t>::const_iterator iter = v.begin();
          iter != v.end();
          ++iter) {
        insert(*iter);
      }
    }
    ~TcSimpleHistogram() {}
  public:
    void init() { _map.erase(_map.begin(), _map.end()); }
    void clear() { _map.erase(_map.begin(), _map.end()); }
    void insert(x_t x) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,y_t>(x, 1));
           } else {
             ++(iter->second);
           }
         }
    void insert(x_t x, y_t y) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,y_t>(x, y));
           } else {
             iter->second += y;
           }
         }
    void fin() {}
  public:
    const_iterator begin() const { return _map.begin(); }
    const_iterator end() const { return _map.end(); }
    const_iterator find(x_t x) const { return _map.find(x); }
  public:
    uint size() const { return _map.size(); }
  public:
    std::ostream& print(std::ostream& os) const {
                    for(const_iterator iter = begin(); iter != end(); ++iter) {
                      os << iter->first << ' '
                         << iter->second
                         << std::endl;
                    }
                    return os;
                  }
  private:
    map_t _map;
};

template<class x_t, class y_t>
class TcDoubleHistogram {
  public:
    struct entry_t {
      entry_t() : c() { c[0] = c[1] = 0; }
      entry_t(y_t x1, y_t x2) : c() { c[0] = x1; c[1] = x2; }
      y_t c[2];
    };
    typedef std::map<x_t,entry_t> map_t;
    typedef typename map_t::const_iterator const_iterator;
  public:
    TcDoubleHistogram() : _map() {}
    ~TcDoubleHistogram() {}
  public:
    void init() { _map.erase(_map.begin(), _map.end()); }
    void insert0(const x_t& x) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,entry_t>(x, entry_t(0,0)));
           }
         }
    void insert1(const x_t& x) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,entry_t>(x, entry_t(1,0)));
           } else {
             ++(iter->second.c[0]);
           }
         }
    void insert2(const x_t& x) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,entry_t>(x, entry_t(0,1)));
           } else {
             ++(iter->second.c[1]);
           }
         }
    void insert(const x_t& x, const y_t y1, const y_t y2) {
           typename map_t::iterator iter = _map.find(x);
           if(_map.end() == iter) {
             _map.insert(std::pair<x_t,entry_t>(x, entry_t(y1,y2)));
           } else {
             iter->second.c[0] += y1;
             iter->second.c[1] += y2;
           }
         }
    void fin() {}
  public:
    const_iterator begin() const { return _map.begin(); }
    const_iterator end() const { return _map.end(); }
  public:
    uint size() const { return _map.size(); }
  private:
    map_t _map;
};


#endif


