#ifndef GRID_TT_HH
#define GRID_TT_HH

#include <inttypes.h>
#include <assert.h>


/*
 * Grid_TT
 * Grid Interpreter for encoded regular square grids of size pxp
 * where p is given as the template parameter Tgrid
 * and the number of bits per tile is given as the template parameter Tbits
 * the size in bytes (rounded up) it requires is given by NoBytes
 * the maximal encoded integer value is given by Mask
 * CAUTION:
 * the interpreter might read/write one byte more than necessary
 * the written parts remain unchanged.
 */


template<uint32_t Tgrid, uint32_t Tbits>
struct Grid_TT {
  static const uint32_t GridSize   = Tgrid; // Tgrid times Tgrid grid
  static const uint32_t NoTileBits = Tbits;
  static const uint32_t NoBytes = ((((Tgrid * Tgrid) * Tbits) + 7) / 8);
  static const uint32_t NoBits  = ((Tgrid * Tgrid) * Tbits);
  static const uint32_t Mask    = (1 << Tbits) - 1;
  static const uint32_t iBitMask = (1 << (Tbits - 1));

  uint8_t* _grid;

  Grid_TT();
  Grid_TT(uint8_t*);

  inline void attach(uint8_t* aGrid) { _grid = aGrid; }
  inline void detach() { _grid = 0; }

  void clear();

  inline uint32_t gridSize() const { return GridSize; };

  inline uint32_t get(const uint32_t x, const uint32_t y) const;
  inline void     set(const uint32_t x, const uint32_t y, const uint32_t aVal);

  inline uint32_t tileIdx(const uint32_t x, const uint32_t y) const { return ((Tgrid * x) + y); }
  inline uint32_t shift(const uint32_t aTileIdx) const { return (Tbits * aTileIdx) & 0x7; }
  inline uint32_t byteIdx(const uint32_t aTileIdx) const { return ((Tbits * aTileIdx) >> 3); }

  inline bool isIBitSet(const uint32_t x, const uint32_t y) const {
                return (0 != (get(x,y) & iBitMask));
              }

  inline uint noChildren() const; // counts number of cells with iBit set

  std::ostream& print(std::ostream& os) const;
};


template<uint32_t Tgrid, uint32_t Tbits>
Grid_TT<Tgrid,Tbits>::Grid_TT() : _grid(0) {
}

template<uint32_t Tgrid, uint32_t Tbits>
Grid_TT<Tgrid,Tbits>::Grid_TT(uint8_t* aGridBuffer) : _grid(aGridBuffer) {
}


template<uint32_t Tgrid, uint32_t Tbits>
void
Grid_TT<Tgrid,Tbits>::clear() {
  for(uint32_t i = 0; i < Tgrid; ++i) {
    for(uint32_t j = 0; j < Tgrid; ++j) {
      set(i, j, 0);
    }
  }
}

template<uint32_t Tgrid, uint32_t Tbits>
uint32_t
Grid_TT<Tgrid,Tbits>::get(const uint32_t x, const uint32_t y) const {
  const uint32_t lTileIdx = tileIdx(x,y);
  const uint32_t lByteIdx = byteIdx(lTileIdx);
  const uint32_t lRes = ((((_grid[lByteIdx + 1] << 8) | _grid[lByteIdx]) >> shift(lTileIdx)) & Mask);
  return lRes;
}

template<uint32_t Tgrid, uint32_t Tbits>
void
Grid_TT<Tgrid,Tbits>::set(const uint32_t x, const uint32_t y, const uint32_t aVal) {
  const uint32_t lVal = (aVal & 0x7F);
  const uint32_t lTileIdx = tileIdx(x,y);
  const uint32_t lByteIdx = byteIdx(lTileIdx);
  const uint32_t lShift   = shift(lTileIdx);
  const uint32_t lMask    = ~(Mask << lShift);
  const uint32_t lContent = ((((_grid[lByteIdx + 1] << 8) | _grid[lByteIdx]) & lMask) | (lVal << lShift));
  _grid[lByteIdx] = (lContent & 0xFF);
  _grid[lByteIdx + 1] = ((lContent >> 8) & 0xFF);
}

template<uint32_t Tgrid, uint32_t Tbits>
uint32_t
Grid_TT<Tgrid,Tbits>::noChildren() const {
  uint32_t lRes = 0;
  for(uint i = 0; i < Tgrid; ++i) {
    for(uint j = 0; j < Tgrid; ++j) {
      lRes += isIBitSet(i,j);
    }
  }
  return lRes;
}

template<uint32_t Tgrid, uint32_t Tbits>
std::ostream&
Grid_TT<Tgrid,Tbits>::print(std::ostream& os) const {
  for(uint32_t i = 0; i < Tgrid; ++i) {
    for(uint32_t j = 0; j < Tgrid; ++j) {
      os << std::setw(5) << get(i,j) << ' ';
    }
    os << std::endl;
  }
  os << std::endl;
  return os;
}



#endif

