#ifndef INFRA_DL_LIST_HH
#define INFRA_DL_LIST_HH

#include "infra/reservoir.hh"

// double linked list with sentinel

template<class Tcontent>
class DL_List {
  DL_List(const DL_List&) = delete;
  DL_List& operator=(const DL_List&) = delete;
  public:
    typedef Tcontent content_t;
    struct item_t {
       item_t*   _prec;
       item_t*   _succ;
       content_t _content;

       const item_t* prec() const { return _prec; }
             item_t* prec()       { return _prec; }
       const item_t* succ() const { return _succ; }
             item_t* succ()       { return _succ; }
    };
    struct const_iterator_t {
      const item_t* _x;
      const item_t* x() const { return _x; }
      const_iterator_t(const item_t* aX) : _x(aX) {}
      const content_t& operator*() { return (_x->_content); }
      bool operator==(const const_iterator_t& i) { return (x() == i.x()); }
      bool operator!=(const const_iterator_t& i) { return (x() != i.x()); }
      const_iterator_t& operator++() { _x = _x->succ(); return (*this); }
    };
    struct iterator_t {
      item_t* _x;
      item_t* x() const { return _x; }
      iterator_t(item_t* aX) : _x(aX) {}
      content_t& operator*() { return (_x->_content); }
      bool operator==(const iterator_t& i) { return (x() == i.x()); }
      bool operator!=(const iterator_t& i) { return (x() != i.x()); }
      iterator_t& operator++() { _x = _x->succ(); return (*this); }
    };
    typedef Reservoir<item_t> rsv_t;
  public:
    DL_List() : _sentinel(), _rsv(new rsv_t), _new_rsv(true) { 
        _sentinel._succ = &_sentinel; 
        _sentinel._prec = &_sentinel; 
    }
    DL_List(rsv_t* aRsv) : _sentinel(), _rsv(aRsv), _new_rsv(false) {
        _sentinel._succ = &_sentinel; 
        _sentinel._prec = &_sentinel; 
    }
    ~DL_List() { if(_new_rsv) { delete _rsv; } else { _rsv->clear(); } }
  public:
    inline       iterator_t begin_nc() { return iterator_t(_sentinel.succ()); }
    inline       iterator_t end_nc() { return iterator_t(&_sentinel); }
    inline const_iterator_t begin() const { return const_iterator_t(_sentinel.succ()); }
    inline const_iterator_t end() const { return const_iterator_t(&_sentinel); }
    inline void push_back(const content_t& aContent) {
                  item_t* x = rsv().get_new_entry();
                  x->_content = aContent;
                  _sentinel.prec()->_succ = x;
                  x->_prec = _sentinel.prec();
                  x->_succ = &_sentinel;
                  _sentinel._prec = x;
                }
    inline void push_back(iterator_t& aIter) {
                  item_t* x = aIter._x;
                  _sentinel.prec()->_succ = x;
                  x->_prec = _sentinel.prec();
                  x->_succ = &_sentinel;
                  _sentinel._prec = x;
                }
    inline void remove(iterator_t& x) {
                  x._x->prec()->_succ = x._x->succ();
                  x._x->succ()->_prec = x._x->prec();
                }
  private:
    rsv_t& rsv() { return (*_rsv); }
  private:
    item_t _sentinel;
    rsv_t* _rsv;
    bool   _new_rsv;
};

#endif
