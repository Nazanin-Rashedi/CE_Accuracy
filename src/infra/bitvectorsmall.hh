#ifndef BITVECTORSMALL_HH
#define BITVECTORSMALL_HH

#include <stdint.h>
#include <string.h> // for size_t 
#include <inttypes.h>

#include <ostream>  // for ostream (operator <<)
#include <istream>  // for istream (operator >>)


/*
 * not used: bit scan forward bsf ist intel befehl, der
 * fuer ein gesetztes bit die position rausfindet
 */

/*
  src/prime/bitmap.hh
  src/prime/bitmap.cc
  Sonstiges/Primes/bitmap.cc
  Sonstiges/Primes/bitmap.hh

  src/opt/infra/bits.cc

  ./src/opt/DynProg/DpAlgorithms/bitvect.h
  ./src/opt/DynProg/DpBridges/bitvect.h
  ./src/prime/bitmap.cc
  ./src/prime/bitmap.hh
  ./src/tneumann/chisigmajoin/bitset.cpp
  ./src/tneumann/chisigmajoin/bitset.hpp
  ./src/tneumann/chisigma/bitset.cpp
  ./src/tneumann/chisigma/bitset.hpp
  ./sys/tools/Bit/bits.cc
  ./AODB/src/cts/cts_plangen/cts_plangen_a/cts_pga_bitvector.hh

  /usr/include/g++/bitset

*/

/*
  uint_t must be "unsigned t" with
  t for example char, short int, long int, long long int
*/


template<class uint_t> 
class BitVectorSmall {
  public:
    typedef unsigned int element_t;
    typedef uint_t       bitvector_t;
  public:
    BitVectorSmall() : _bitvector(0) {}
    BitVectorSmall(const BitVectorSmall& x) : _bitvector(x._bitvector) {}
    BitVectorSmall(bitvector_t x) : _bitvector(x) {}
    BitVectorSmall(const char* x) : _bitvector(0) { this->initFromCstring(x); }
    ~BitVectorSmall() {}
  public:
    inline bitvector_t bitvector() const { return _bitvector; }
    inline bitvector_t content() const { return _bitvector; }
  public:
    inline bool is_empty() const { return (((bitvector_t) 0) == _bitvector); }
    inline bool is_not_empty() const { return (((bitvector_t) 0) != _bitvector); }
  public:
    inline BitVectorSmall& reset() { _bitvector = 0; return (*this); }
    inline BitVectorSmall& clear() { _bitvector = 0; return (*this); }
    inline BitVectorSmall& operator++() { ++_bitvector; return (*this); }
    inline BitVectorSmall& operator-() { _bitvector = -_bitvector; return (*this); }
    inline BitVectorSmall& operator~() { _bitvector = ~_bitvector; return (*this); }
  public:
    static inline size_t   capacity() { return sizeof(bitvector_t) * 8; }

    static inline unsigned int cardinality(bitvector_t x);
           inline unsigned int cardinality() const { return cardinality(_bitvector); }

    int element(element_t& aElementOut) const; /* out: an element of the set,  */
                                               /* return: -1: set empty */
                                               /*          0: set is singleton */
                                               /*          1: cardinality() > 1 */

    inline bool 
    test(element_t i) const { return (((((bitvector_t) 1) << i) & _bitvector) != 0); }

    inline BitVectorSmall&  
    set(element_t i) { _bitvector |= (((bitvector_t) 1) << i); return (*this); }

    inline BitVectorSmall&  
    set_only(element_t i) { _bitvector = (((bitvector_t) 1) << i); return (*this); }

    inline BitVectorSmall&
    reset(element_t i) { _bitvector &= ~(((bitvector_t) 1) << i); return (*this); }

    inline BitVectorSmall&
    set_first_n(element_t i) { _bitvector = ((((bitvector_t) 1) << i) - 1); return (*this); }
    
    inline BitVectorSmall&
    set_last_n(element_t i) { 
      _bitvector = ~((((bitvector_t) 1) << ((sizeof(bitvector_t) * 8) - i)) - 1); 
      return (*this); 
    }
    
    inline BitVectorSmall& 
    operator=(const BitVectorSmall& x) { _bitvector = x._bitvector; return (*this); }

    inline BitVectorSmall& 
    operator=(const bitvector_t& x) { _bitvector = x; return (*this); }

    inline BitVectorSmall& 
    operator|=(const bitvector_t& x) { _bitvector |= x; return (*this); }

    inline BitVectorSmall& 
    operator^=(const bitvector_t& x) { _bitvector ^= x; return (*this); }

    inline BitVectorSmall& 
    operator^=(const BitVectorSmall& x) { _bitvector ^= x._bitvector; return (*this); }

    inline BitVectorSmall& 
    operator+=(const element_t i) { _bitvector |= (1 << i); return (*this); }

    inline BitVectorSmall&
    operator+=(const BitVectorSmall& x) { _bitvector += x._bitvector; return (*this); }

    inline BitVectorSmall& 
    operator-=(const element_t i) { _bitvector &= ~(1 << i); return (*this); }

    inline BitVectorSmall&  
    union_with(const BitVectorSmall& x) { _bitvector |= x._bitvector; return (*this); }

    inline BitVectorSmall&  
    union_of(const BitVectorSmall& x, const BitVectorSmall& y) { 
             _bitvector = (x._bitvector | y._bitvector); return (*this); }

    inline BitVectorSmall&  
    operator|=(const BitVectorSmall& x) { _bitvector |= x._bitvector; return *this; }

    inline BitVectorSmall
    operator&(const BitVectorSmall& x) const { return BitVectorSmall(_bitvector & x._bitvector); }

    inline BitVectorSmall
    operator|(const BitVectorSmall& x) const { return BitVectorSmall(_bitvector | x._bitvector); }

    inline BitVectorSmall
    operator^(const BitVectorSmall& x) const { return BitVectorSmall(_bitvector ^ x._bitvector); }

    inline BitVectorSmall&  
    intersect_with(const BitVectorSmall& x) { _bitvector &= x._bitvector; return *this; }

    inline BitVectorSmall&
    intersection_of(const BitVectorSmall& x, const BitVectorSmall& y) { 
        _bitvector = x._bitvector & y._bitvector; 
        return *this; 
    }

    static inline BitVectorSmall
    the_intersection_of(const BitVectorSmall& x, const BitVectorSmall& y) { 
        return BitVectorSmall(x._bitvector & y._bitvector);
    }

    inline BitVectorSmall&  
    operator&=(const BitVectorSmall& x) { _bitvector &= x._bitvector; return *this; }

    inline BitVectorSmall&
    set_difference(const BitVectorSmall& x) { _bitvector &= (~(x._bitvector)); return *this; }

    inline BitVectorSmall&
    set_to_difference(const BitVectorSmall& x, const BitVectorSmall& y) {
        _bitvector = (x._bitvector & (~(y._bitvector))); return *this; }

    inline BitVectorSmall&
    complement(int aCapacity) {
      _bitvector = ((1 << aCapacity) - 1) & (~_bitvector); return *this; }
   
    inline BitVectorSmall&  
    complement(const BitVectorSmall& x, int aCapacity) {
      _bitvector = ((1 << aCapacity) - 1) & (~x._bitvector); return *this; }

    inline BitVectorSmall&  
    complement(const BitVectorSmall& x) { _bitvector = (~x._bitvector); return *this; }

           inline bitvector_t lowest_bit() const { return lowest_bit(_bitvector); }
    static inline bitvector_t lowest_bit(bitvector_t x) { return (x & (-x)); }
           inline element_t   lowest_bit_index() const { return lowest_bit_index(_bitvector); }
    static inline element_t   lowest_bit_index(bitvector_t x) { 
                                return (cardinality(lowest_bit(x) - 1)); }

    inline element_t log2() const { return log2(_bitvector); }
    static element_t log2(bitvector_t x);

    inline element_t highest_bit() const { return highest_bit(_bitvector); }
    static element_t highest_bit(bitvector_t x);
    inline element_t highest_bit_index() const { return log2(highest_bit()); }

  public:
    // arighmetics
    inline BitVectorSmall& add(const bitvector_t x) { _bitvector += x; return (*this); }
  public:
    inline bool operator==(const BitVectorSmall& x) const { return (_bitvector == x._bitvector); }
    inline bool operator!=(const BitVectorSmall& x) const { return (_bitvector != x._bitvector); }
    inline bool operator<(const BitVectorSmall& x) const { return (_bitvector < x._bitvector); }
  public:
    inline bool overlap(const BitVectorSmall& x) const { return (0 != (_bitvector & x._bitvector)); }
    inline bool intersects_with(const BitVectorSmall& x) const { return (0 != (_bitvector & x._bitvector)); }
    inline bool intersects(const BitVectorSmall& x) const { return (0 != (_bitvector & x._bitvector)); }
    inline bool disjoint(const BitVectorSmall& x) const { return (0 == (_bitvector & x._bitvector)); }
    inline bool disjoint_with(const BitVectorSmall& x) const { return (0 == (_bitvector & x._bitvector)); }
    inline bool contains(const BitVectorSmall& x) const { return (_bitvector == (_bitvector | x._bitvector)); }
    inline bool notContains(const BitVectorSmall& x) const { return (_bitvector != (_bitvector | x._bitvector)); }
  public:
    inline unsigned int hashvalue() const;
  public:
    class IteratorElement {
        friend class BitVectorSmall;
        IteratorElement(bitvector_t x) : _bitvector(x), 
                                         _element(BitVectorSmall::lowest_bit_index(x)) {}
      public:
        IteratorElement() : _bitvector(0), _element(0) {}
        ~IteratorElement() {}
      public:
        element_t operator*() { return _element; }
        inline IteratorElement& operator++() { 
                                    _bitvector &= ~(1 << _element); 
                                    _element = BitVectorSmall::lowest_bit_index(_bitvector);
                                    return *this; }

        inline IteratorElement& operator=(const IteratorElement& x) { 
                                   _bitvector = x._bitvector; 
                                   _element = x._element; 
                                   return *this; }

        inline bool operator==(const IteratorElement& x) const { 
                               return (_bitvector == x._bitvector); }
        inline bool operator!=(const IteratorElement& x) const { 
                               return (_bitvector != x._bitvector); }
        inline bool isValid() const { return (0 != _bitvector); }
      private:
        bitvector_t _bitvector;
        element_t   _element;
    };
    IteratorElement begin() const { return IteratorElement(_bitvector); }
    IteratorElement end()   const { return IteratorElement(); }
  public:
    class IteratorBit { // iterates through all bits of a set
        friend class BitVectorSmall;
        IteratorBit(bitvector_t x) : _bitvector(x),
                                            _elementbit(BitVectorSmall::lowest_bit(x)) {}
      public:
        IteratorBit() : _bitvector(0), _elementbit(0) {}
        ~IteratorBit() {}
      public:
        bitvector_t operator*() { return _elementbit; }
        inline IteratorBit& operator++() {
                                      _bitvector &= ~(_elementbit);
                                      _elementbit = BitVectorSmall::lowest_bit(_bitvector);
                                      return *this; }

        inline IteratorBit& operator=(const IteratorBit& x) {
                                     _bitvector = x._bitvector;
                                     _elementbit = x._elementbit;
                                     return *this; }

        inline bool operator==(const IteratorBit& x) const {
                               return (_bitvector == x._bitvector); }
        inline bool operator!=(const IteratorBit& x) const {
                               return (_bitvector != x._bitvector); }
        inline bool isValid() const { return (0 != _bitvector); }
      private:
        bitvector_t _bitvector;
        bitvector_t _elementbit;
    };
    inline IteratorBit beginBit() const { return IteratorBit(_bitvector); }
    inline IteratorBit endBit()   const { return IteratorBit(); }

  public:
    class IteratorSubset { // iterates through all subsets, excluding empty set and set itself 
        friend class BitVectorSmall;
        IteratorSubset(bitvector_t x, bitvector_t y) : _bitvector(x), _current(y) {}
      public:
        IteratorSubset() : _bitvector(0), _current(0) {}
        IteratorSubset(bitvector_t x) : _bitvector(x), _current(x & (-x)) {}
        ~IteratorSubset() {}
      public:
        inline void init(bitvector_t x) {
                      _bitvector = (x); 
                      _current   = (x & (-x));
                    }
        inline void reset() {
                      _current = (_bitvector & (-_bitvector));
                    }
        BitVectorSmall operator*() { return BitVectorSmall(_current); }
        bitvector_t get() const { return _current; }
        bitvector_t getFullSet() const { return _bitvector; }
        inline IteratorSubset& operator++() {
                                    _current = (_bitvector & (_current - _bitvector));
                                    return (*this); }

        inline IteratorSubset& operator=(const IteratorSubset& x) { 
                                   _bitvector = x._bitvector;
                                   _current = x._current;
                                   return (*this); }

        inline bool operator==(const IteratorSubset& x) const {
                               return (_current == x._current); }
        inline bool operator!=(const IteratorSubset& x) const {
                               return (_current != x._current); }
        inline bool isValid() const { return (_bitvector != _current); }
      private:
        bitvector_t _bitvector; // const
        bitvector_t _current;
    };
    IteratorSubset beginSub() const { return IteratorSubset(_bitvector); }
    IteratorSubset endSub()   const { return IteratorSubset(_bitvector,_bitvector); }

  public:

#define SUPERCAP(x,i) ((x) | (~((1 << (i)) - 1)))
#define SUPERMASK(i)  ((1 << (i)) - 1)

    class IteratorSuperset { /* iterates through all supersets, excluding the set itself */
        friend class BitVectorSmall;
      public:
        IteratorSuperset() : _bitvector(0), _current(0), _mask(~((uint_t) 0)) {}
      private:
        IteratorSuperset(bitvector_t x, bitvector_t m) : _bitvector(x), _current((~x) & (-(~x))),
                                                         _mask(m) {}
      public:
        ~IteratorSuperset() {}
      public:
        BitVectorSmall operator*() { 
                         if((uint_t) _bitvector ==  (uint_t) (~_current)) { return BitVectorSmall(_mask); } 
                         else { return BitVectorSmall(~_current & _mask); }
                       }
        inline IteratorSuperset& operator++() {
                                 _current = (~_bitvector & ( _current - (~_bitvector)));
                                 return (*this); }

        inline IteratorSuperset& operator=(const IteratorSuperset& x) {
                                   _bitvector = x._bitvector;
                                   _current = x._current;
                                   return (*this); }

        inline bool operator==(const IteratorSuperset& x) const {
                               return (_current == x._current); }
        inline bool operator!=(const IteratorSuperset& x) const {
                               return (_current != x._current); }
        inline bool isValid() const { return (0 != _current); }
      private:
        bitvector_t _bitvector;
        bitvector_t _current;
        const bitvector_t _mask;
    };
    IteratorSuperset beginSuper(int aCapacity /* < (!) (sizeof(uint_t) * 8) */ ) const { 
                  return IteratorSuperset(SUPERCAP(_bitvector,aCapacity), SUPERMASK(aCapacity)); }
    IteratorSuperset beginSuper() const { return IteratorSuperset(_bitvector, ~(uint_t)0); }
    IteratorSuperset endSuper()   const { return IteratorSuperset(); }
#undef SUPERCAP
#undef SUPERMASK
  public:
    void
    initFromCstring(const char* x);
    std::ostream& 
    print(std::ostream& os, unsigned int aCapacity = (sizeof(uint_t) * 8)) const;

    std::ostream&
    printReverse(std::ostream& os, unsigned int aCapacity = (sizeof(uint_t) * 8)) const;

    std::ostream& 
    printAsSet(std::ostream& os, unsigned int aCapacity = (sizeof(uint_t) * 8)) const;

    std::ostream& 
    printSetBitIdx(std::ostream& os, unsigned int aCapacity = (sizeof(uint_t) * 8), const char aSep = ' ') const;
  private:
    bitvector_t _bitvector;
};

typedef uint8_t  bitvector8_t;
typedef uint16_t bitvector16_t;
typedef uint32_t bitvector32_t;
typedef uint64_t bitvector64_t;

typedef BitVectorSmall<bitvector8_t>  Bitvector8;
typedef BitVectorSmall<bitvector16_t> Bitvector16;
typedef BitVectorSmall<bitvector32_t> Bitvector32;
typedef BitVectorSmall<bitvector64_t> Bitvector64;

template<class uint_t>
inline std::ostream&
operator<<(std::ostream& os, const BitVectorSmall<uint_t>& x) { 
  if(0 < os.width()) {
    const int lWidth = os.width();
    os.width(1);
    return x.print(os, lWidth);
  }
  return x.print(os); 
}

template<class uint_t>
int 
BitVectorSmall<uint_t>::element(element_t& aElementOut) const {
  if(0 == _bitvector) { return -1; }
  bitvector_t x = lowest_bit();
  aElementOut = log2(x);
  if(x == _bitvector) { return 0; }
  return 1;
}


template<class uint_t, bool large>
class BitVectorSmall_ANY {
};

template<class uint_t>
class BitVectorSmall_ANY<uint_t, false> {
  public:
    static inline unsigned int
    log2(uint_t x) {
      x |= (x >> 1);
      x |= (x >> 2);
      x |= (x >> 4);
      if((sizeof(uint_t) * 8) > 8) {
        x |= (x >> 8);
      }
      if((sizeof(uint_t) * 8) > 16) {
        x |= (x >> 16);
      }
      return (BitVectorSmall<uint_t>::cardinality(x) - 1);
    }
    static inline unsigned int
    hashvalue(uint_t value) {
      return static_cast<unsigned>(value);
    }
};

template<class uint_t>
class BitVectorSmall_ANY<uint_t, true> {
  public:
    static inline unsigned int
    log2(uint_t x) {
      x |= (x >> 1);
      x |= (x >> 2);
      x |= (x >> 4);
      x |= (x >> 8);
      x |= (x >> 16);
      x |= (x >> 32);
      return (BitVectorSmall<uint_t>::cardinality(x) - 1);
    }
    static inline unsigned int
    hashvalue(uint_t value) {
      return static_cast<unsigned>(value >> 8*sizeof(unsigned)) 
             ^ static_cast<unsigned>(value);
    }
};

template<class uint_t>
typename BitVectorSmall<uint_t>::element_t 
BitVectorSmall<uint_t>::log2(bitvector_t x) {
  return BitVectorSmall_ANY<uint_t, (sizeof(uint_t) > sizeof(unsigned))>::log2(x);
}


/*
template<class uint_t>
typename BitVectorSmall<uint_t>::element_t
BitVectorSmall<uint_t>::log2(bitvector_t x) {
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   if((sizeof(bitvector_t) * 8) > 8) {
     x |= (x >> 8);
   }
   if((sizeof(bitvector_t) * 8) > 16) {
     x |= (x >> 16);
   }
   if((sizeof(bitvector_t) * 8) > 32) {
     x |= (x >> 32);
   }
   return (cardinality(x) - 1);
}

*/

/* out: an element of the set,  */
/* return: -1: set empty */
/*          0: set is singleton */
/*          1: cardinality() > 1 */

#define TWO(c)     (0x1u << (c))
#define MASK(c)    (((-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

template<class uint_t>
unsigned int
BitVectorSmall<uint_t>::cardinality(bitvector_t x) {
  x = COUNT(x,0);
  x = COUNT(x,1);
  x = COUNT(x,2);
  if((sizeof(bitvector_t) * 8) > 8) {
    x = COUNT(x,3);
  }
  if((sizeof(bitvector_t) * 8) > 16) {
    x = COUNT(x,4);
  }
  if((sizeof(bitvector_t) * 8) > 32) {
    x = COUNT(x,5);
  }
  if((sizeof(bitvector_t) * 8) > 64) {
    x = COUNT(x,6);
  }
  // if((sizeof(bitvector_t) * 8) > 128) {
  //   x = COUNT(x,7);
  // }
  return (unsigned int) (x);
}

#undef TWO
#undef MASK
#undef COUNT

template<class uint_t>
typename BitVectorSmall<uint_t>::element_t
BitVectorSmall<uint_t>::highest_bit(bitvector_t x) {
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  if(capacity() > 8) {
    x |= (x >> 8);
  }
  if(capacity() > 16) {
    x |= (x >> 16);
  }
  if(capacity() > 32) {
    x |= (x >> 32);
  }
  return (element_t) (x & ~(x >> 1));
}


// least significant bit has to be to the left of the string

template<class uint_t>
void
BitVectorSmall<uint_t>::initFromCstring(const char* x) {
  unsigned int i = 0;
  while('\0' != (*x)) { 
    if('1' == (*x)) {
      set(i);
    }
    ++x;
    ++i;
  }
}

/*
// same as above but reversed (least significant bit is to the right)

template<class uint_t>
void
BitVectorSmall<uint_t>::initFromCstring(const char* x) {
  unsigned int i = 0;
  const char* z = x;
  while((*z) != '\0') { ++z;}
  --z;
  --x;
  do {
    if('1' == (*z)) {
      set(i);
    }
    --z;
    ++i;
  } while(z != x);
}
*/

template<class uint_t>
std::ostream&
BitVectorSmall<uint_t>::print(std::ostream& os, unsigned int aCapacity) const {
  for(unsigned int i = 0; i < aCapacity; ++i) {
    os << (test(i) ? '1' : '0');
  }
  return os;
}

template<class uint_t>
std::ostream&
BitVectorSmall<uint_t>::printReverse(std::ostream& os, unsigned int aCapacity) const {
  unsigned int i = aCapacity;
  do {
    --i;
    os << (test(i) ? '1' : '0');
  } while(i > 0);

  return os;
}

template<class uint_t>
std::ostream&
BitVectorSmall<uint_t>::printAsSet(std::ostream& os, unsigned int aCapacity) const {
  os << '{';
  bool lFirst = true;
  for(unsigned int i = 0; i < aCapacity; ++i) {
    if(test(i)) {
      if(lFirst) {
        lFirst = false;
      } else {
        os << ',';
      }
      os << i;
    }
  }
  os << '}';
  return os;
}

template<class uint_t>
std::ostream&
BitVectorSmall<uint_t>::printSetBitIdx(std::ostream& os, unsigned int aCapacity, const char aSep) const {
  bool lFirst = true;
  for(unsigned int i = 0; i < aCapacity; ++i) {
    if(test(i)) {
      if(lFirst) {
        lFirst = false;
      } else {
        os << aSep;
      }
      os << i;
    }
  }
  return os;
}




template<class uint_t>
unsigned int
BitVectorSmall<uint_t>::hashvalue() const { 
  return BitVectorSmall_ANY<uint_t, (sizeof(uint_t) > sizeof(unsigned))>::hashvalue(bitvector());
}


template<class Tbv>
class BvHash {
  public:
    inline std::size_t operator()(const Tbv& aBv) const {
                         return aBv.hashvalue();
                       };
};



#endif

