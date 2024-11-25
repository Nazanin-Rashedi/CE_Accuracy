#ifndef INFRA_TABULATION_HASH_HH
#define INFRA_TABULATION_HASH_HH

#include <inttypes.h>
#include <vector>
#include <random>


// Tmt should be mt19937 or mt19937_64
template<typename Tmt, uint32_t tNoBits>
class TabulationHash {
  public:
    typedef typename Tmt::result_type loc_uint_t;
    typedef std::vector<loc_uint_t>  loc_uint_vt;
    typedef std::vector<loc_uint_vt> table_t;
    typedef unsigned char byte_t;
  public:
    static constexpr uint32_t no_bits = tNoBits;
    // static constexpr uint32_t size_in_bits = sizeof(loc_uint_t) * 8;
    // static constexpr uint32_t mask  = (((loc_uint_t) 1) << tNoBits) - 1;
    // static constexpr uint32_t shift = size_in_bits / tNoBits;
    static constexpr uint32_t no_entries = (((loc_uint_t) 1) << tNoBits);
  public:
    TabulationHash(const uint32_t aNoTables, Tmt& aMt);
  public:
    inline uint32_t   no_tables() const { return _tables.size(); }
    inline loc_uint_t lookup(const byte_t x, const uint32_t aHtNo) const { return _tables[aHtNo][x]; }
  public:
    loc_uint_t hash(const byte_t* x, const uint32_t aSize) const;
  private:
    table_t _tables;
};


template<typename Tmt, uint32_t tNoBits>
TabulationHash<Tmt,tNoBits>::TabulationHash(const uint32_t aNoTables, Tmt& aMt) : _tables() {
  _tables.resize(aNoTables);
  for(uint32_t i = 0; i < aNoTables; ++i) {
    _tables[i].resize(no_entries);
    for(uint32_t j = 0; j < no_entries; ++j) {
      _tables[i][j] = aMt();
    }
  }
}

template<typename Tmt, uint32_t tNoBits>
typename TabulationHash<Tmt,tNoBits>::loc_uint_t
TabulationHash<Tmt,tNoBits>::hash(const byte_t* x, const uint32_t aSize) const {
  loc_uint_t lRes = lookup(x[0], 0);
  loc_uint_t lHi = 0;

  uint lHf = 1;
  for(uint i = 1; i < aSize; ++i) {
    lHi = lookup(x[lHf++], i % no_tables());
    // std::cout << "lHi = " << lHi << std::endl;
    lRes = ((lRes ^ lHi) ^ lookup(lRes + lHi, lHf++ % no_tables())); // from Thorup, Zhang 04
    // lRes = ((lRes ^ lHi) ^ ((lRes << 1) + lHi));
    // lRes = ((lRes ^ lHi) );
  }

  return lRes;
}


#endif
