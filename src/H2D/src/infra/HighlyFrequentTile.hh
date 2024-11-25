#ifndef H2D_INFRA_HIGHLY_FREQUEHT_TILE_HH
#define H2D_INFRA_HIGHLY_FREQUEHT_TILE_HH

#include <iostream>
#include <iomanip>
#include <vector>

#include "types.hh"

namespace H2D {

class HighlyFrequentTile {
  public:
    struct hft_t {
      int    _i;
      int    _j;
      double _f;
      hft_t() : _i(0), _j(0), _f(0) {}
      hft_t(const int aI, const int aJ, const double aF) : _i(aI), _j(aJ), _f(aF) {}
      hft_t(const hft_t& x) : _i(x._i), _j(x._j), _f(x._f) {}
    };
    typedef std::vector<hft_t> hft_vt;
  public:
    HighlyFrequentTile();
    HighlyFrequentTile(const rectangle_t& aBr, const uint aNx, const uint aNy);
    ~HighlyFrequentTile();
  public:
    void   init(const rectangle_t& aBr, const uint aNx, const uint aNy);
    void   init();
    void   step(int i, int j, double aFrequency);
    void   fin();
  public:
    // set bounding rectangle (if only hft_vt is available), also calculates partition descriptor
    void   setBoundingRectangle(const rectangle_t& aBoundingRectangle, const uint aNx, const uint aNy);
    // estimate functions
    double estimate(const rectangle_t& r) const;
    double estimateSlow(const rectangle_t& r) const;
  public:
    uint size() const;
  public:
    void          print(std::ostream& os, const uint k) const;
    std::ostream& print(std::ostream& os) const;
  public:
    inline const rectangle_t&       br() const { return _br; }
    inline const partitiondescxy_t& pd() const { return _pd; }
    inline const hft_vt&            hf() const { return _hfts; }
  private:
    rectangle_t       _br;
    partitiondescxy_t _pd;
    hft_vt            _hfts;
};

} // end namespace


#endif

