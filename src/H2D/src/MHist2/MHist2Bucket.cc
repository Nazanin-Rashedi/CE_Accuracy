#include "MHist2Bucket.hh"

namespace H2D {


MHist2Bucket::MHist2Bucket(const Data2dim& aData) : _rect(), _c(aData.total()) {

  aData.getBoundingRectangle(_rect);

}

MHist2Bucket::~MHist2Bucket() {

}

uint
MHist2Bucket::size() {
  // return (4 * 4) + 4; // 4 * domValSize() + freqValSize()
  return (4 * 4) + 1; // 4 * domValSize() + freqValSize() q-compressed
}


void 
MHist2Bucket::estimate(double& aEstimate, const rectangle_t& aRect) const {
  rectangle_t lRect;
  lRect.isec(_rect, aRect);

  if ( ! lRect.isEmpty() ) {

    if (lRect == _rect) {
      aEstimate += _c;
    } else if(_rect.xhi() == _rect.xlo()) {
      aEstimate += _c / (_rect.yhi() - _rect.ylo()) * (lRect.yhi() - lRect.ylo());
    } else if(_rect.yhi() == _rect.ylo()) {
      aEstimate += _c / (_rect.xhi() - _rect.xlo()) * (lRect.xhi() - lRect.xlo());
    } else {
      aEstimate += _c / _rect.area() * lRect.area();
    }

  }

}

} // end namespace


