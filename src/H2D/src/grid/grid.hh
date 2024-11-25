#include <iostream>
#include <iomanip>

#include <inttypes.h>


class Grid {
  public:
    Grid(const int aP, const int aFBits, const bool aHasIBits);
  public:
    inline int       p() const { return _p; }
    inline int       pp() const { return (p() * p()); }
    inline int       fbits() const { return _fbits; }
    inline int       ibits() const { return  hasIbits() * pp(); }
    inline int       gbits() const { return (fbits() * pp() + ibits()); }
    inline bool      hasIbits() const { return _hasIbits; }
    inline uint64_t  maxTileFreq() const { return _maxTileFreq[fbits()]; }
    inline int       size() const { return gbits(); }
  public:
    inline uint64_t uniCap() const { return pp() * maxTileFreq(); }
    inline uint64_t uniDvPb() const { return uniCap() / size(); }
    inline uint64_t uniDvPb(const int aSize) const { return uniCap() / aSize; }
    inline int      rest(const int aNoBits) const { return (aNoBits > size() ? (aNoBits - size()) : 0); }
    inline int      rest_div16(const int aNoBits) const { return (rest(aNoBits) / 16); }
    inline int      rest_mod16(const int aNoBits) const { return (rest(aNoBits) % 16); }
  public:
           std::ostream& printGridName(std::ostream& os) const;
    static std::ostream& printLatexHeader(std::ostream& os);
           std::ostream& printLatex(std::ostream& os) const;
    static std::ostream& printLatexFooter(std::ostream& os);
  private:
    int  _p;
    int  _fbits; 
    bool _hasIbits;
  public:
    static const int64_t  _maxTileFreq[7];
};


