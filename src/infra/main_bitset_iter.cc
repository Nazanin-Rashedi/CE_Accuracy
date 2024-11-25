#include "infra/glob_infra_standard_includes.hh"
#include "infra/bitvectorsmall.hh"
#include "infra/bit_intrinsics.hh"
#include "infra/bit_subsets.hh"

/*
// reset the least sigificant bit set to '1'
inline uint
reset_lsb_set(const uint x) {
  return (x & (x - 1));
}

// get the least sigificant bit set to '1'
inline uint
get_lsb_set(const uint x) {
  // return (x & (x ^ (x - 1)));
  return (x & (-x));
}
*/

/*
 * short list of intel intrinsics for bit manipulations
   _andn_u32(a, b) :=  ~a & b;
   _bextr_u32(a, s, l) := a[s,s+l-1]
   _bextr2_u32(a, c)   := a[c[0,7],c[0,7]+c[8,15]]
   _bittest(a, b)      := a[b]
   _blsi_u32(a)        := (-a) & a
   _blsr_u32(a)        := (a - 1) & a  // reset lsb set
   _blsmsk_u32(a)      := (a - 1) ^ a  // set all bits from lsb to first set bit
   _bittestandcomplement(a,b) := { x = a[b]; a[b] = ~a[b]; return x; }
   _bittestandreset(a,b)      := { x = a[b]; a[b] = 0;     return x; }
   _bittestandset(a,b)        := { x = a[b]; a[b] = 1;     return x; }
   _bzhi(a,b)                 := { x = a; x[31:b] = 0;     return x; }
   _bswap(a) reverse byte order
 */
 
/*
 *  Iterate through bits sets from lsb to msb
 */

/*
template<typename Tuint>
class IterBitsSetAsc {
  public:
    IterBitsSetAsc(const Tuint x) : _state(x), _lsb(get_lsb_set(x)) {}
  public:
    inline bool isValid() const { return (0 != _state); }
    inline Tuint operator*() const { return _lsb; }
    inline IterBitsSetAsc&  operator++() {
                             _state ^= _lsb;
                             _lsb = get_lsb_set(_state);
                             return (*this);
                           }
  private:
    Tuint _state;
    Tuint _lsb;
};
*/


void
test0() {
  std::cout << "=== TEST 0 ===" << std::endl;
  for(uint x = 0; x < 32; ++x) {
    std::cout << std::setw(6) << Bitvector32(x) << ' '
              << std::setw(6) << Bitvector32(  get_lsb_set(x)) << ' '
              << std::setw(6) << Bitvector32(reset_lsb_set(x)) << ' '
              << std::endl;

  }
}

void
test1() {
  std::cout << "=== TEST 1 ===" << std::endl;

  const uint W = 6;
  std::string lIndent(W, ' ');
  for(uint x = 0; x < 32; ++x) {
    std::cout << std::setw(W) << Bitvector32(x) << ' '
              << std::setw(W) << Bitvector32(x) << ' '
              << std::endl;
    for(IterBitsSetAsc<uint> lIter(x); lIter.isValid(); ++lIter) {
      std::cout << lIndent << ' '
                << std::setw(W) << Bitvector32(*lIter) << ' '
                << std::endl;
    }
  }
}

int
main() {
  test0();
  test1();
  return 0;
}

