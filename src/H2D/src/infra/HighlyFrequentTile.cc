#include "HighlyFrequentTile.hh"


namespace H2D {


HighlyFrequentTile::HighlyFrequentTile() : _br(), _pd(), _hfts() {}
HighlyFrequentTile::HighlyFrequentTile(const rectangle_t& aBr, const uint aNx, const uint aNy) : _br(), _pd(), _hfts() {
  init(aBr, aNx, aNy);
}

HighlyFrequentTile::~HighlyFrequentTile() {
}

void
HighlyFrequentTile::init(const rectangle_t& aBr, const uint aNx, const uint aNy) {
  _br = aBr;
  _pd.init(br(), aNx, aNy);
  _hfts.clear();
}

void
HighlyFrequentTile::init() {
  _hfts.clear();
}

void
HighlyFrequentTile::step(const int i, const int j, const double f) {
  _hfts.push_back(hft_t(i, j, f));
}

void
HighlyFrequentTile::fin() {
}

void
HighlyFrequentTile::setBoundingRectangle(const rectangle_t& aBoundingRectangle, const uint aNx, const uint aNy) {
  _br = aBoundingRectangle;
  _pd.init(br(), aNx, aNy);
}


double
HighlyFrequentTile::estimateSlow(const rectangle_t& r) const {
  double lRes = 0;
  // indices describing subgrid A containing rectangle r
  const int lIdxXloA = pd().idxXcorrected(r.xlo());
  const int lIdxXhiA = pd().idxXcorrected(r.xhi()) + 1;
  const int lIdxYloA = pd().idxYcorrected(r.ylo());
  const int lIdxYhiA = pd().idxYcorrected(r.yhi()) + 1;

  // indices describing subgrid B contained in r
  // const int lIdxXloB = ((pd().boundaryX(lIdxXloA) == r.xlo()) ?  lIdxXloA : (lIdxXloA + 1));
  // const int lIdxXhiB = ((pd().boundaryX(lIdxXhiA) == r.xhi()) ?  lIdxXhiA : (lIdxXhiA - 1));
  // const int lIdxYloB = ((pd().boundaryY(lIdxYloA) == r.ylo()) ?  lIdxYloA : (lIdxYloA + 1));
  // const int lIdxYhiB = ((pd().boundaryY(lIdxYhiA) == r.yhi()) ?  lIdxYhiA : (lIdxYhiA - 1));

  rectangle_t lTileRectangle; // rectangle of tile (i,j)
  rectangle_t lIsec;          // intersection of query rectangle and tile rectangle
  const double lInverseTileArea = (((double) 1.0) / (pd().widthX() * pd().widthY()));
  for(hft_vt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    const int i = (*lIter)._i;
    const int j = (*lIter)._j;
    // if tile is not contained in A: continue
    if( ((lIdxXhiA < i) | (i < lIdxXloA)) ||
        ((lIdxYhiA < j) & (j < lIdxYloA)) ) {
      continue;
    }
    // o.k. may overlap
    pd().getRectangle(i, j, lTileRectangle);
    lIsec.isec(lTileRectangle, r);
    if(!lIsec.isEmpty()) {
      lRes += (lIsec.area() * lInverseTileArea) * (*lIter)._f;
    }
  }

  return lRes;
}

double
HighlyFrequentTile::estimate(const rectangle_t& aQueryRectangle) const {
  double lRes = 0;
  rectangle_t lTileRectangle;
  rectangle_t lIsec;
  const double lInverseTileArea = (((double) 1.0) / (pd().widthX() * pd().widthY()));
  for(hft_vt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    const int i = (*lIter)._i;
    const int j = (*lIter)._j;
    pd().getRectangle(i, j, lTileRectangle);
    lIsec.isec(lTileRectangle, aQueryRectangle);
    if(!lIsec.isEmpty()) {
      lRes += (lIsec.area() * lInverseTileArea) * (*lIter)._f;
    }
  }
  return lRes;
}


uint
HighlyFrequentTile::size() const {
  return hf().size() * 4;  // 12 bit i, 12 bit j, 8 bit f
}


void
HighlyFrequentTile::print(std::ostream& os, const uint k) const {
  const int    i = hf()[k]._i;
  const int    j = hf()[k]._j;
  const double f = hf()[k]._f;
  os << "  [" << i << ',' << j << ',' << f << ']';
}

std::ostream&
HighlyFrequentTile::print(std::ostream& os) const {
  std::cout << "HFT: "
            << "  bounding rectangle: " << br() << std::endl
            << "  partition descriptor: " << pd() << std::endl
            << "  highly frequent tiles: " << std::endl;
  for(hft_vt::const_iterator lIter = hf().begin(); lIter != hf().end(); ++lIter) {
    const int    i = (*lIter)._i;
    const int    j = (*lIter)._j;
    const double f = (*lIter)._f;
    os << "  [" << i << ',' << j << ',' << f << ']';
  }
  os << std::endl;
  return os;
}


} // end namespace 

