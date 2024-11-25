#include "RegularPartitioning2dim.hh"

namespace H2D {


/*
 * members for RegularPartitioning2dim
 */

RegularPartitioning2dim::RegularPartitioning2dim() : _descX(), _descY(), _vx(0), _vy(0), _m(), _total(0) {}

RegularPartitioning2dim::RegularPartitioning2dim(const uint nx, const uint ny, const Data2dim& aData2dim) 
                        : _descX(), _descY(), _vx(0), _vy(0), _m(), _total(0) {
  initFromData2dim(nx, ny, aData2dim);
}

RegularPartitioning2dim::RegularPartitioning2dim(const rectangle_t& aRectangle,
                                                 const uint nx, const uint ny, 
                                                 const Data2dim& aData2dim) 
                        : _descX(), _descY(), _vx(0), _vy(0), _m(), _total(0) {
  initFromData2dim(aRectangle, nx, ny, aData2dim);
}

RegularPartitioning2dim::~RegularPartitioning2dim() {
  deleteVxy();
}


void
RegularPartitioning2dim::allocVxy(const uint nx, const uint ny) {
  deleteVxy();
  _vx = new uint[nx];
  _vy = new uint[ny];
  memset(_vx, 0, sizeof(uint) * nx);
  memset(_vy, 0, sizeof(uint) * ny);
  _m.rescale(nx, ny);
}

void
RegularPartitioning2dim::deleteVxy() {
  if(0 != _vx) {
    delete[] _vx;
  }
  if(0 != _vy) {
    delete[] _vy;
  }
  _vx = _vy = 0;
}

void
RegularPartitioning2dim::initPartitionDescXY(const uint nx, const uint ny, const Data2dim& aData2dim) {
  assert(0 < aData2dim.size());
  rectangle_t lRectangle;
  aData2dim.getBoundingRectangle(lRectangle);
  _descX.set(lRectangle.xlo(), lRectangle.xhi(), nx);
  _descY.set(lRectangle.ylo(), lRectangle.yhi(), ny);
  // std::cout << "partition desc X: " << _descX << std::endl;
  // std::cout << "partition desc Y: " << _descY << std::endl;
}

void
RegularPartitioning2dim::initPartitionDescXY(const Data2dim& aData,
                                             const uint aBegin,
                                             const uint aEnd,
                                             const uint aNx,
                                             const uint aNy) {
  assert(aBegin < aEnd);
  rectangle_t lRectangle;
  aData.getBoundingRectangle(lRectangle, aBegin, aEnd);
  _descX.set(lRectangle.xlo(), lRectangle.xhi(), aNx);
  _descY.set(lRectangle.ylo(), lRectangle.yhi(), aNy);
}


// _descX and _descY must have been set
void
RegularPartitioning2dim::initPartitioning(const uint nx, const uint ny, const Data2dim& aData2dim) {
  allocVxy(nx, ny);
  _total = 0;

  for(uint i = 0; i < aData2dim.size(); ++i) {
    const xyc_t& e  = aData2dim[i];
    if(e.x < minX() || e.x >= maxX() || e.y < minY() || e.y >= maxY()) {
      continue;
    }

    const int lIdxX = _descX.idxCorrected(e.x);
    const int lIdxY = _descY.idxCorrected(e.y);

    // ignore those on the outer boarders or outside BR
    if(lIdxX < 0 || lIdxX >= (int) nx) { continue; }
    if(lIdxX < 0 || lIdxY >= (int) ny) { continue; }
    // end ignore
    assert((uint) lIdxX < nx && (uint) lIdxY < ny);

    _vx[lIdxX] += e.c;
    _vy[lIdxY] += e.c;
    _m(lIdxX, lIdxY) += e.c;
    _total += e.c;
  }
}

// _descX and _descY must have been set
void
RegularPartitioning2dim::initPartitioning(const Data2dim& aData,
                                          const uint      aBegin,
                                          const uint      aEnd,
                                          const uint      nx, 
                                          const uint      ny) {
  allocVxy(nx, ny);
  _total = 0;

  for(uint i = aBegin; i < aEnd; ++i) {
    const xyc_t& e  = aData[i];
    if(e.x < minX() || e.x >= maxX() || e.y < minY() || e.y >= maxY()) {
      continue;
    }

    uint idxX = _descX.idx(e.x);
    uint idxY = _descY.idx(e.y);

    // ignore those on the outer boarders 
    if(idxX >= nx) { continue; }
    if(idxY >= ny) { continue; }
    // end ignore
    assert(idxX < nx && idxY < ny);

    // check for rounding/imprecision  errors
    rectangle_t lRectangle;
    mkRectangle(idxX, idxY, lRectangle);
    if(!lRectangle.containsHalfOpen(e.x, e.y)) {
       // correct
       if(e.x < lRectangle.xlo()) {
         --idxX;
       } else
       if(e.x >= lRectangle.xhi()) {
         ++idxX;
       }

       if(e.y < lRectangle.ylo()) {
         --idxY;
       } else
       if(e.y >= lRectangle.yhi()) {
         ++idxY;
       }
       mkRectangle(idxX, idxY, lRectangle);
 
       // this assertion will eventually fail:
       assert(lRectangle.containsHalfOpen(e.x, e.y));

       // ignore those on the outer boarders 
       if(idxX >= nx) { continue; }
       if(idxY >= ny) { continue; }
       // end ignore
       assert(idxX < nx && idxY < ny);
    }

    _vx[idxX] += e.c;
    _vy[idxY] += e.c;
    _m(idxX, idxY) += e.c;
    _total += e.c;
  }
}


void
RegularPartitioning2dim::initFromData2dim(const uint nx, const uint ny, const Data2dim& aData2dim) {
  initPartitionDescXY(nx, ny, aData2dim);
  initPartitioning(nx, ny, aData2dim);
}

void
RegularPartitioning2dim::initFromData2dim(const rectangle_t& aRectangle,
                                          const uint nx, const uint ny, const Data2dim& aData2dim) {
  _descX.set(aRectangle.xlo(), aRectangle.xhi(), nx);
  _descY.set(aRectangle.ylo(), aRectangle.yhi(), ny);
  initPartitioning(nx, ny, aData2dim);
}

void
RegularPartitioning2dim::initFromData2dim(const Data2dim& aData,
                                          const uint      aBegin,
                                          const uint      aEnd,
                                          const uint      aNx,
                                          const uint      aNy) {
  initPartitionDescXY(aData, aBegin, aEnd, aNx, aNy);
  initPartitioning(aData, aBegin, aEnd, aNx, aNy);
}


void
RegularPartitioning2dim::initFromMatrix(const partitiondesc_t& aPdX,
                                        const partitiondesc_t& aPdY,
                                        const uint             aTotal,
                                        const Matrix&          aMatrix) {
  _descX = aPdX;
  _descY = aPdY;
  _total = aTotal;
  _m     = aMatrix;
};

double
RegularPartitioning2dim::globalDistortion() const {
  uint lMin = std::numeric_limits<uint>::max();
  uint lMax = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      lMin = std::min<uint>(lMin, _m(i,j));
      lMax = std::max<uint>(lMax, _m(i,j));
    }
  }
  if(1 > lMin) { lMin = 1; }
  return (((double) lMax) / ((double) lMin));
}

double
RegularPartitioning2dim::maxInnerDistortion(const uint aLimit, uint* aNoGtLimitOut) const {
  double lMaxDistortion = 0.0;


  bool lIsGtLimit = false;
  uint lNoGtLimit = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double lDistortion = distortion(4 * i, 4 * j, aLimit, &lIsGtLimit);
      if(lMaxDistortion < lDistortion) {
        lMaxDistortion = lDistortion;
      }
      lNoGtLimit += lIsGtLimit;
      lIsGtLimit = false;
    }
  }
  if(0 != aNoGtLimitOut) {
    (*aNoGtLimitOut) = lNoGtLimit;
  }
  return lMaxDistortion;
}

double
RegularPartitioning2dim::distortion(const uint aIdxX, 
                                    const uint aIdxY, 
                                    const uint aLimit,
                                    bool* aGtLimitOut) const {
  if(_m.noRows() < (aIdxX + 4)) { return 0; }
  if(_m.noCols() < (aIdxY + 4)) { return 0; }

  double lMax = 0.0;
  double lMin = std::numeric_limits<double>::max();
  double lSum = 0;
  for(uint i = 0; i < 4; ++i) {
    for(uint j = 0; j < 4; ++j) {
       const double lFreq = _m(aIdxX + i, aIdxY + j);
       lSum += lFreq;
       if(lFreq < lMin) {
         lMin = lFreq;
       }
       if(lFreq > lMax) {
         lMax = lFreq;
       } 
    }
  }
  if((0 != aGtLimitOut) && (aLimit < lSum)) {
    (*aGtLimitOut) = true;
  }

  if(1.0 > lMin) {
    lMin = 1.0;
  }
  const double lDistortion = (lMax / lMin);
  return lDistortion;
}

double
RegularPartitioning2dim::distortion(const uint aIdxX,
                                    const uint aIdxY,
                                    const uint aSubX,
                                    const uint aSubY) const {
  const uint lEndi = std::min<uint>(_m.noRows(), aIdxX + aSubX);
  const uint lEndj = std::min<uint>(_m.noCols(), aIdxY + aSubY);

  double lMax = 0.0;
  double lMin = std::numeric_limits<double>::max();
  for(uint i = aIdxX; i < lEndi; ++i) {
    for(uint j = aIdxY; j < lEndj; ++j) {
       const double lFreq = _m(i, j);
       if(lFreq < lMin) {
         lMin = lFreq;
       }
       if(lFreq > lMax) {
         lMax = lFreq;
       }
    }
  }
  if(1.0 > lMin) { lMin = 1.0; }
  const double lDistortion = (lMax / lMin);
  return lDistortion;
}

double
RegularPartitioning2dim::distortion(const uint aIdxX, const uint aIdxY, 
                                    const uint aNoSubX, const uint aNoSubY,
                                    const uint aNoSubSubX, const uint aNoSubSubY) const {
  const uint lStepX = aNoSubX * aNoSubSubX;
  const uint lStepY = aNoSubY * aNoSubSubY;
  const uint lBeginSubTileX = aIdxX * lStepX;
  const uint lEndSubTileX   = lBeginSubTileX + lStepX;
  const uint lBeginSubTileY = aIdxY * lStepY;
  const uint lEndSubTileY   = lBeginSubTileY + lStepY;
  double lRes = 0.0;
  for(uint i = lBeginSubTileX; i < lEndSubTileX; i += aNoSubSubX) {
    for(uint j = lBeginSubTileY; j < lEndSubTileY; j += aNoSubSubY) {
      const double lDistortionSubTile = distortion(i, j, aNoSubSubX, aNoSubSubY);
      if(lRes < lDistortionSubTile) {
        lRes = lDistortionSubTile;
      }
    }
  }
  return lRes;
}


uint
RegularPartitioning2dim::max() const {
  uint lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(matrix()(i,j) > lRes) {
         lRes = matrix()(i,j);
      }
    }
  }
  return lRes;
}

uint
RegularPartitioning2dim::min() const {
  uint lRes = matrix()(0,0);
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(matrix()(i,j) < lRes) {
         lRes = matrix()(i,j);
      }
    }
  }
  return lRes;
}


uint
RegularPartitioning2dim::noGtLimit(const uint aLimit) const {
  const double lLimit = aLimit;
  uint lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(lLimit < matrix()(i,j)) {
        ++lRes;
      }
    }
  }
  return lRes;
}


/*
 * as noGtLimit, but assumes that partitioining is finer than that for which
 * noGtLimit has to be calculated.
 */

uint
RegularPartitioning2dim::noGtLimit(const uint aLimit, const uint aNxSub, const uint aNySub) const {
  uint lRes = 0;
  for(uint k = 0; k < noRows(); k += aNxSub) {
    for(uint l = 0; l < noCols(); l += aNySub) {
      uint lSum = 0;
      for(uint i = 0; i < aNxSub; ++i) {
        for(uint j = 0; j < aNySub; ++j) {
          lSum += _m(k + i, l + j);
        }
      }
      if(aLimit < lSum) {
        ++lRes;
      }
    }
  }
  return lRes;
}





double
RegularPartitioning2dim::estimate(const query_t& lQuery) const {
  const bool lTrace = false;
  rectangle_t aRectangle = lQuery.rectangle();
  rectangle_t lRectangle = aRectangle;

  if(lTrace) {
    std::cout << "estimate R: " << aRectangle << ' ';
  }

  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 1" << std::endl;
    }
    return 0.0; 
  }
  if(lRectangle.xlo() < minX()) { lRectangle.xlo(minX()); }
  if(lRectangle.ylo() < minY()) { lRectangle.ylo(minY()); }
  if(lRectangle.xhi() > maxX()) { lRectangle.xhi(maxX()); }
  if(lRectangle.yhi() > maxY()) { lRectangle.yhi(maxY()); }
  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 2" << std::endl;
    }
    return 0.0; 
  }

  const uint lXmin = (uint) floor( (lRectangle.xlo() - minX()) / tileWidthX());
  const uint lYmin = (uint) floor( (lRectangle.ylo() - minY()) / tileWidthY());
  const uint lXmax = std::min<uint>(noRows() - 1, (uint) floor ( (lRectangle.xhi() - minX()) / tileWidthX()));
  const uint lYmax = std::min<uint>(noCols() - 1, (uint) floor ( (lRectangle.yhi() - minY()) / tileWidthY()));
 
  if(lTrace) { 
    std::cout << "XYmin/max: " << lXmin << '-' << lXmax << '/' << lYmin << '-' << lYmax << std::endl;
  }

  double lRes = 0.0;
  const double lTileArea = tileWidthX() * tileWidthY();
  rectangle_t lRectTile;

  lRectTile.xlo(minX() + lXmin * tileWidthX());
  lRectTile.xhi(lRectTile.xlo() + tileWidthX());

  rectangle_t lRectIsec;
  for(uint i = lXmin; i <= lXmax; ++i) {
    lRectTile.ylo(minY() + lYmin * tileWidthY());
    lRectTile.yhi(lRectTile.ylo() + tileWidthY());
    for(uint j = lYmin; j <= lYmax; ++j) {
      lRectIsec.isec(lRectangle, lRectTile);
      if(lTrace) {
        std::cout << "T = " << lRectTile << ", I = " << lRectIsec << std::endl;
      }
      if(!(lRectIsec.isEmpty())) {
        lRes += (lRectIsec.area() / lTileArea) * _m(i, j);
      }
      lRectTile._pll.y += tileWidthY();
      lRectTile._pur.y += tileWidthY();
    }
    lRectTile._pll.x += tileWidthX();
    lRectTile._pur.x += tileWidthX();
  }
  return lRes;
}





double
RegularPartitioning2dim::estimate(const rectangle_t& aRectangle) const {
  const bool lTrace = false;
  rectangle_t lRectangle = aRectangle;

  if(lTrace) {
    std::cout << "estimate R: " << aRectangle << ' ';
  }

  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 1" << std::endl;
    }
    return 0.0; 
  }
  if(lRectangle.xlo() < minX()) { lRectangle.xlo(minX()); }
  if(lRectangle.ylo() < minY()) { lRectangle.ylo(minY()); }
  if(lRectangle.xhi() > maxX()) { lRectangle.xhi(maxX()); }
  if(lRectangle.yhi() > maxY()) { lRectangle.yhi(maxY()); }
  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 2" << std::endl;
    }
    return 0.0; 
  }

  const uint lXmin = (uint) floor( (lRectangle.xlo() - minX()) / tileWidthX());
  const uint lYmin = (uint) floor( (lRectangle.ylo() - minY()) / tileWidthY());
  const uint lXmax = std::min<uint>(noRows() - 1, (uint) floor ( (lRectangle.xhi() - minX()) / tileWidthX()));
  const uint lYmax = std::min<uint>(noCols() - 1, (uint) floor ( (lRectangle.yhi() - minY()) / tileWidthY()));
 
  if(lTrace) { 
    std::cout << "XYmin/max: " << lXmin << '-' << lXmax << '/' << lYmin << '-' << lYmax << std::endl;
  }

  double lRes = 0.0;
  const double lTileArea = tileWidthX() * tileWidthY();
  rectangle_t lRectTile;

  lRectTile.xlo(minX() + lXmin * tileWidthX());
  lRectTile.xhi(lRectTile.xlo() + tileWidthX());

  rectangle_t lRectIsec;
  for(uint i = lXmin; i <= lXmax; ++i) {
    lRectTile.ylo(minY() + lYmin * tileWidthY());
    lRectTile.yhi(lRectTile.ylo() + tileWidthY());
    for(uint j = lYmin; j <= lYmax; ++j) {
      lRectIsec.isec(lRectangle, lRectTile);
      if(lTrace) {
        std::cout << "T = " << lRectTile << ", I = " << lRectIsec << std::endl;
      }
      if(!(lRectIsec.isEmpty())) {
        lRes += (lRectIsec.area() / lTileArea) * _m(i, j);
      }
      lRectTile._pll.y += tileWidthY();
      lRectTile._pur.y += tileWidthY();
    }
    lRectTile._pll.x += tileWidthX();
    lRectTile._pur.x += tileWidthX();
  }
  return lRes;
}

double
RegularPartitioning2dim::estimateIgnore(const rectangle_t& aRectangle, const double aEpsilon) const {
  const bool lTrace = false;
  rectangle_t lRectangle = aRectangle;

  if(lTrace) {
    std::cout << "estimate R: " << aRectangle << ' ';
  }

  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 1" << std::endl;
    }
    return 0.0; 
  }
  if(lRectangle.xlo() < minX()) { lRectangle.xlo(minX()); }
  if(lRectangle.ylo() < minY()) { lRectangle.ylo(minY()); }
  if(lRectangle.xhi() > maxX()) { lRectangle.xhi(maxX()); }
  if(lRectangle.yhi() > maxY()) { lRectangle.yhi(maxY()); }
  if(lRectangle.isEmpty()) { 
    if(lTrace) {
      std::cout << "empty 2" << std::endl;
    }
    return 0.0; 
  }

  const uint lXmin = (uint) floor( (lRectangle.xlo() - minX()) / tileWidthX());
  const uint lYmin = (uint) floor( (lRectangle.ylo() - minY()) / tileWidthY());
  const uint lXmax = std::min<uint>(noRows() - 1, (uint) floor ( (lRectangle.xhi() - minX()) / tileWidthX()));
  const uint lYmax = std::min<uint>(noCols() - 1, (uint) floor ( (lRectangle.yhi() - minY()) / tileWidthY()));
 
  if(lTrace) { 
    std::cout << "XYmin/max: " << lXmin << '-' << lXmax << '/' << lYmin << '-' << lYmax << std::endl;
  }

  double lRes = 0.0;
  const double lTileArea = tileWidthX() * tileWidthY();
  rectangle_t lRectTile;

  lRectTile.xlo(minX() + lXmin * tileWidthX());
  lRectTile.xhi(lRectTile.xlo() + tileWidthX());

  rectangle_t lRectIsec;
  for(uint i = lXmin; i <= lXmax; ++i) {
    lRectTile.ylo(minY() + lYmin * tileWidthY());
    lRectTile.yhi(lRectTile.ylo() + tileWidthY());
    for(uint j = lYmin; j <= lYmax; ++j) {
      lRectIsec.isec(lRectangle, lRectTile);
      if(lTrace) {
        std::cout << "T = " << lRectTile << ", I = " << lRectIsec << std::endl;
      }
      if(!(lRectIsec.isEmpty())) {
        const double lFraction = (lRectIsec.area() / lTileArea);
        if(aEpsilon <= lFraction) {
          lRes += (lRectIsec.area() / lTileArea) * _m(i, j);
        }
      }
      lRectTile._pll.y += tileWidthY();
      lRectTile._pur.y += tileWidthY();
    }
    lRectTile._pll.x += tileWidthX();
    lRectTile._pur.x += tileWidthX();
  }
  return lRes;
}




double
RegularPartitioning2dim::max_avi_estimate_qerror() const {
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double lEstimate  = avi_estimate(i,j);
      const double lTrueValue = std::max<double>(1.0, _m(i,j));
      const double lQError    = q::qerror(lEstimate, lTrueValue);
      if(lRes < lQError) {
        lRes = lQError;
      }
    }
  }
  return lRes;
}

double
RegularPartitioning2dim::max_avi_estimate_qerror(const double aTheta) const {
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      const double lEstimate  = avi_estimate(i,j);
      const double lTrueValue = std::max<double>(1.0, _m(i,j));
      const double lQError    = q::qerror(lEstimate, lTrueValue);
      if((aTheta < lTrueValue) || (aTheta < lEstimate)) {
        if(lRes < lQError) {
          lRes = lQError;
        }
      }
    }
  }
  return lRes;
}

std::ostream&
RegularPartitioning2dim::print(std::ostream& os, const int aFieldWidth) const {
  return _m.print(os, aFieldWidth);
}

std::ostream&
RegularPartitioning2dim::print(std::ostream& os, const uint m, const uint n, const int aFieldWidth) const {
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      os << std::setw(aFieldWidth) << _m(i,j) << ' ';
    }
    os << std::endl;
  }
  return os;
}

std::ostream&
RegularPartitioning2dim::printMatrix(std::ostream& os, const int aFieldWidth) const {
  return _m.print(os, aFieldWidth);
}

std::ostream&
RegularPartitioning2dim::printWithRowColSums(std::ostream& os, const int aFieldWidth) const {
  const uint m = _m.noRows(); const uint n = _m.noCols();


  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      os << std::setw(aFieldWidth) << _m(i,j) << ' ';
    }
    os << " | " << _vx[i];
    os << std::endl;
  }
  os << " --- col sum ---" << std::endl;
  uint lTotal = 0;
  for(uint j = 0; j < n; ++j) {
    os << std::setw(aFieldWidth) << _vy[j] << ' ';
    lTotal += _vy[j];
  }
  os << " | " << std::setw(aFieldWidth) << lTotal << std::endl;
  return os;
}

double
RegularPartitioning2dim::maxEntry() const {
  double lRes = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
       if(_m(i,j) > lRes) {
         lRes = _m(i,j);
       }
    }
  }
  return lRes;
}

void
RegularPartitioning2dim::shrink2(const uint m, const uint n) {
  for(uint i = 0; i < m; i += 2) {
    for(uint j = 0; j < n; j += 2) {
      _m(i >> 1, j >> 1) = _m(i,j) + _m(i,j+1) + _m(i+1,j) + _m(i+1,j+1);
    }
  }
}


uint
RegularPartitioning2dim::estimate3LevelTreeMemoryConsumption(const double aTheta) const {
  // step 1: determine number of medium sized tiles (M) and third level very rough estimate
  uint lNoM = 0;
  uint lNoL2 = 0;
  uint lResLevel2b = 0;
  for(uint i = 0; i < noRows(); ++i) {
    for(uint j = 0; j < noCols(); ++j) {
      if(aTheta < _m(i,j)) {
        if(_m(i,j) < 16*aTheta) {
          ++lNoM;
        } else {
          const uint x1 = (uint) round(sqrt((double) _m(i,j) / (16.0 * aTheta)));
          const uint x2 = (uint) (round(mt::logbaset<double>((double) _m(i,j) / (double) aTheta, 4.0)));
                uint x = std::min<uint>(x1, x2);
          if(2 > x) { x = 2; }
          lResLevel2b += x*x;
          lNoL2 += 0.55 * x * x;
        }
      }
    }
  }
  uint lRes = 0;
  lRes += noRows() * noCols(); // Level 1, one byte per entry in noX * noY array
  lRes += lNoM * 8;            // Level 2, part 1: lNoM entries, each 8 bytes large (for 4x4 subtiling and QC16x4)
  lRes += lResLevel2b * 2; // array, 2 bytes for each entry (one does not suffice for child pointers)
  lRes += lNoL2 * 8;       // true risky part lNoL2 entries, each 8 bytes large (for 4x4 subtiling and QC16x4)
  return lRes;
}

uint
RegularPartitioning2dim::size() const {
  return (4 * nx() * ny());
}

std::ostream&
RegularPartitioning2dim::printAnalysis(std::ostream& os,
                                       const uint aTheta,
                                       const int aFieldWidth) const {
  // const bool lPrintMatrices = (30 > noCols() && 20 > noRows());
  const bool lPrintMatrices = false;

  if(lPrintMatrices) {
    os << "Frequency matrix: " << std::endl;
    printWithRowColSums(os, aFieldWidth);
  }
  os << std::endl;

  const uint m = _m.noRows();
  const uint n = _m.noCols();

  uint lNoZero = 0;
  uint lNoLtTheta = 0;
  uint lNoGt16Theta = 0;
  uint lMaxFreq = 0;
  uint lCumFreqSM = 0; // S = small (F <= theta), M = medium (theta < F <= 16*theta)
  uint lCumFreqL  = 0; // L = large (16*theta < F)
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      if(0 == _m(i,j)) {
        ++lNoZero;
      }
      if(aTheta >= _m(i,j)) {
        ++lNoLtTheta;
      }
      if((16 * aTheta) < _m(i,j)) {
        ++lNoGt16Theta;
        lCumFreqL += _m(i,j);
      } else {
        lCumFreqSM += _m(i,j);
      }
      if(lMaxFreq < _m(i,j)) {
        lMaxFreq = _m(i,j);
      }
    }
  }
  os << "Number of: empty tiles: " << lNoZero << ", "
     << "lt theta: " << lNoLtTheta << ", "
     << "gt 16 theta: " << lNoGt16Theta << ", "
     << "max freq: " << lMaxFreq << ", "
     << std::endl;

  if(false && lPrintMatrices) {
    os << "Estimate matrix: " << std::endl;
    for(uint i = 0; i < m; ++i) {
      for(uint j = 0; j < n; ++j) {
        const double lEstimate = avi_estimate(i,j);
        os << std::setw(aFieldWidth) << lEstimate << ' ';
      }
      os << std::endl;
    }
    os << std::endl;
  }

  if(false && lPrintMatrices) {
    os << "Error matrix: " << std::endl;
  }


  Aggregate<double> lAggrError;
  Aggregate<double> lAggrErrorTheta;

  const double lTheta = (double) aTheta;
  for(uint i = 0; i < m; ++i) {
    for(uint j = 0; j < n; ++j) {
      const double lEstimate = avi_estimate(i,j);
      // os << std::setw(aFieldWidth) << lEstimate << ' ';
      const double lTrueValue = (double) _m(i,j);
      const double lPError = q::p_error(lEstimate, lTrueValue);
      if(false && lPrintMatrices) {
        os << std::setw(aFieldWidth) << roundX(lPError) << ' ';
      }
      lAggrError.step(lPError);
      if((lTheta < lTrueValue) && (lTheta < lEstimate)) {
        lAggrErrorTheta.step(lPError);
      }
    }
    if(false && lPrintMatrices) {
      os << std::endl;
    }
  }
  os << std::endl;
  os << "p-error summary: " << lAggrError << std::endl;
  os << std::endl;

  const double lNoZeroPercent = (((double) lNoZero) / ((double) (m*n))) * 100.0;
  os << '@' << m << 'x' << n << ' ';
  if(10 > n) {
    os << ' ';
  }
  if(10 > m) {
    os << ' ';
  }
  os << '@'
     << std::setw(6) << lNoZero << ' '
     << std::setw(6) << roundX(lNoZeroPercent) << " | "
     << std::setw(4) << lNoLtTheta << ' '
     << std::setw(4) << lNoGt16Theta << ' '
     << std::setw(4) << lMaxFreq << " | "
     << std::setw(8) << lCumFreqSM << ' '
     << std::setw(8) << lCumFreqL << " | "
     << std::setw(6) << roundX(lAggrError.min()) << ' '
     << std::setw(6) << roundX(lAggrError.max()) << ' '
     << std::setw(6) << roundX(lAggrError.avg()) << " | "
     << std::setw(6) << roundX(lAggrErrorTheta.min()) << ' '
     << std::setw(6) << roundX(lAggrErrorTheta.max()) << ' '
     << std::setw(6) << roundX(lAggrErrorTheta.avg()) << " | "
     << std::setw(6) << (distortion(0,0)) << ' '
     << std::endl;
  return os;
}

std::ostream&
RegularPartitioning2dim::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_RegPart) << ' '
            << 0 // subkind
            << '(' 
            << nx() << ',' << ny() 
            << ')'
            << std::endl;
  return os;
}


} // end namespace



