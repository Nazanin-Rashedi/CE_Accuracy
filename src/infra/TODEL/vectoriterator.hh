#ifndef INFRA_VECTOR_ITERATOR_HH
#define INFRA_VECTOR_ITERATOR_HH

#include <vector>

class VectorIterator {
  public:
    typedef unsigned int uint;
    typedef std::vector<uint> uint_vt;
  public:
    VectorIterator(const uint_vt& aVt);
    ~VectorIterator();
  public:
    VectorIterator& operator++();
    inline bool ok() const { return _ok; }
    inline const uint_vt& operator*() const { return _vec; }
    inline const uint_vt& vec() const { return _vec; }
    inline uint  vec(const uint i) const { return _vec[i]; }
    inline uint  size() const { return _vec.size(); }
    inline const uint_vt& lim() const { return _lim; }
    inline uint lim(const uint i) const { return _lim[i]; }
    inline uint curr() const { return _curr; }
  private:
    inline uint_vt& vec() { return _vec; }
  private:
    const uint_vt& _lim;
          uint_vt  _vec;
	  uint     _curr;
	  bool     _ok;
};


#endif
