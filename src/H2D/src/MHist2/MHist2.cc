#include "MHist2.hh"


namespace H2D {


MHist2::MHist2() : _kind(k_invalid), _buckets() {
}

MHist2::MHist2(const uint aNumBuckets, const kind_t aKind, const Data2dim& aData) : _kind(aKind), _buckets() {
 // std::cout << "kind: " << aKind << std::endl;
  init(aNumBuckets, aKind, aData);
}

MHist2::~MHist2() {
}

uint
MHist2::size() const {
  return (_buckets.size() * MHist2Bucket::size());
}

void 
MHist2::init(const uint aNumBuckets, const kind_t aKind, const Data2dim& aData) {
  _kind = aKind;

  partialDistribution_vt lPvt;
  partialDistribution_t lP = partialDistribution_t(aData);
  switch (aKind) {
    case k_maxDiffSpread:
      maxDiffSpread(lP);
      break;
    case k_maxSpread:
      maxSpread(lP);
      break;
    case k_maxDiffCount:
      maxDiffCount(lP);
      break;
    case k_maxDiffArea:
      maxDiffArea(lP);
      break;
    case k_minVariance:
      minVariance(lP);
      break;
    default:
      assert(0>1);
  }
  lPvt.push_back(lP);

  while ( aNumBuckets >= lPvt.size() ) {
    partialDistribution_vt::iterator lExtreme = lPvt.end();

    for ( partialDistribution_vt::iterator lIter = (lPvt.end() - 1); lIter >= lPvt.begin(); --lIter ) {
      if ( lIter->dimension != ' ' ) {
        lExtreme = lIter;
        break;
      }
    }

    if (lExtreme == lPvt.end()) { // There is no single bucket that can be split
      break;
    }

    if (aKind == k_minVariance) {
      // Search for bucket with smaller diff
      for ( partialDistribution_vt::iterator lIter = (lExtreme - 1); lIter >= lPvt.begin(); --lIter ) {
        if ( lExtreme->extremeDiff > lIter->extremeDiff && lIter->dimension != ' ' ) {
          lExtreme = lIter;
        }
      }
    } else {
      // Search for bucket with greater diff
      for ( partialDistribution_vt::iterator lIter = (lExtreme - 1); lIter >= lPvt.begin(); --lIter ) {
        if ( lExtreme->extremeDiff < lIter->extremeDiff && lIter->dimension != ' ' ) {
          lExtreme = lIter;
        }
      }
    }

    Data2dim lLow;
    Data2dim lUp;
    if ( lExtreme->dimension == 'x' ) {
      // Split along x
      for ( xyc_vt::const_iterator lIter = lExtreme->data.beginIter(); lIter != lExtreme->data.endIter(); ++lIter ) {
        if ( lIter->x <= lExtreme->value) {
          lLow.step(lIter->x, lIter->y, lIter->c);
        } else {
          lUp.step(lIter->x, lIter->y, lIter->c);
        }
      }
    } else {
      // Split along y
      for ( xyc_vt::const_iterator lIter = lExtreme->data.beginIter(); lIter < lExtreme->data.endIter(); ++lIter ) {
        if ( lIter->y <= lExtreme->value) {
          lLow.step(lIter->x, lIter->y, lIter->c);
        } else {
          lUp.step(lIter->x, lIter->y, lIter->c);
        }
      }
    }

    *lExtreme = partialDistribution_t(lLow);
    lP = partialDistribution_t(lUp);

    switch (aKind) {
      case k_maxDiffSpread:
        maxDiffSpread(*lExtreme);
        maxDiffSpread(lP);
        break;
      case k_maxSpread:
        maxSpread(*lExtreme);
        maxSpread(lP);
        break;
      case k_maxDiffCount:
        maxDiffCount(*lExtreme);
        maxDiffCount(lP);
        break;
      case k_maxDiffArea:
        maxDiffArea(*lExtreme);
        maxDiffArea(lP);
        break;
      case k_minVariance:
        minVariance(*lExtreme);
        minVariance(lP);
        break;
      default:
        assert(0>1);
    }

    lPvt.push_back(lP);
  }

  for ( partialDistribution_vt::iterator lIter = lPvt.begin(); lIter != lPvt.end(); ++lIter ) {
    _buckets.push_back(MHist2Bucket(lIter->data));
  }

  // Allows faster estimate
  std::sort (_buckets.begin(), _buckets.end());
}


double
MHist2::estimate(const query_t& lQuery) const {
  rectangle_t aRect= lQuery.rectangle();
  return estimate(aRect);
}

double 
MHist2::estimate(const rectangle_t& aRect) const {

  double lEstimate = 0.0;

  for ( std::vector<MHist2Bucket>::const_iterator lIter = _buckets.begin(); lIter != _buckets.end(); ++lIter ) {
    if (lIter->xlo() > aRect.xhi()) // _bucket is sorted for xlo so we can skip if xhi is higher
      break;

    if (lIter->xhi() >= aRect.xlo() && lIter->yhi() >= aRect.ylo() && lIter->ylo() <= aRect.yhi())
      lIter->estimate(lEstimate, aRect);
  }

  return lEstimate;
}

std::ostream& 
MHist2::printSvg(std::ostream& os, const Data2dim& aData, const bool printDot) const {

  const int lXoff = 100;
  const int lYoff = 100;

  double lMinX = 0;
  double lMinY = 0;
  double lMaxX = 0;
  double lMaxY = 0;

  rectangle_t lRect;
  aData.getBoundingRectangle(lRect);

  if (lRect.xlo() < 0 ) lMinX = std::ceil(std::abs(lRect.xlo()));
  if (lRect.ylo() < 0 ) lMinY = std::ceil(std::abs(lRect.ylo()));
  if (lRect.xhi() > 0 ) lMaxX = std::ceil(lRect.xhi());
  if (lRect.yhi() > 0 ) lMaxY = std::ceil(lRect.yhi());

  os << "<?xml version=\"1.0\"?>" << std::endl;
  os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
  os << "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" height=\"" << lYoff * 2 + lMinY + lMaxY << "\" width=\"" << lXoff * 2 + lMinX + lMaxX << "\" fill=\"white\">" << std::endl;


  os << std::endl;
  os << std::endl;
  os << "  <!-- Hintergrund -->" << std::endl;
  os << "  <rect x=\"0\" y=\"0\" width=\"" << lXoff * 2 + lMinX + lMaxX << "\" height=\"" << lYoff * 2 + lMinY + lMaxY << "\" style=\"fill:white;\" />" << std::endl;


  os << std::endl;
  os << std::endl;
  os << "  <defs>" << std::endl;
  os << "    <style type=\"text/css\">" << std::endl;
  os << "    <![CDATA[" << std::endl;
  os << "      line {stroke:black; stroke-width:1;}" << std::endl;
  os << "      rect {fill:none; stroke:black; stroke-width:1;}" << std::endl;
  os << "      circle {fill:red;opacity:0.5;}" << std::endl;
  os << "    ]]>" << std::endl;
  os << "    </style>" << std::endl;
  os << std::endl;
  os << "    <marker id=\"pf\"" << std::endl;
  os << "      viewBox=\"0 0 20 10\" refX=\"0\" refY=\"5\"" << std::endl;
  os << "      markerWidth=\"30\" markerHeight=\"30\"" << std::endl;
  os << "      orient=\"auto\">" << std::endl;
  os << "      <path d=\"M 0,0 L 20,5 L 0,10 z\" />" << std::endl;
  os << "    </marker>" << std::endl;
  os << "  </defs>" << std::endl;


  os << std::endl;
  os << std::endl;
  os << "  <!-- Koordinatensystem -->" << std::endl;
  os << "  <line x1=\"" << lXoff + lMinX << "\" y1=\"" << lYoff * 1.5 + lMaxY + lMinY << "\" x2=\"" << lXoff + lMinX << "\" y2=\"" << lYoff * 0.5 << "\" marker-end=\"url(#pf)\" />" << std::endl;
  os << "  <line x1=\"" << lXoff * 0.5 << "\" y1=\"" << lYoff + lMaxY << "\" x2=\"" << lXoff * 1.5 + lMinX + lMaxX << "\" y2=\"" << lYoff + lMaxY << "\" marker-end=\"url(#pf)\" />" << std::endl;


  os << std::endl;
  os << std::endl;
  os << "  <!-- Rechtecke -->" << std::endl;
  for (std::vector<MHist2Bucket>::const_iterator lIter = _buckets.begin(); lIter != _buckets.end(); ++lIter) {
    os << "  <rect x=\"" << lXoff + lMinX + lIter->xlo() - 0.5 << "\" y=\"" << lYoff + lMaxY - lIter->yhi() - 0.5 << "\" width=\"" << lIter->xhi() - lIter->xlo() + 1 << "\" height=\"" << lIter->yhi() - lIter->ylo() + 1 << "\" />" << std::endl;
  }


  if ( printDot ) {
    os << std::endl;
    os << std::endl;
    os << "  <!-- Punkte -->" << std::endl;
    for (xyc_vt::const_iterator lIter = aData.beginIter(); lIter != aData.endIter(); ++lIter) {
      os << "  <circle cx=\"" << lXoff + lMinX + lIter->x << "\" cy=\"" << lYoff + lMaxY - lIter->y << "\" r=\"0.5\" />" << std::endl;
    }
  }

  os << "</svg>" << std::endl;

  return os;
}



void 
MHist2::maxDiffSpread(partialDistribution_t& aPd) const {
  xyc_vt::iterator lIterFirst = aPd.data.beginIter();
  xyc_vt::iterator lIterLast = aPd.data.endIter();
  xyc_vt::iterator lA, lB, lC;

  aPd.data.sortX(lIterFirst, lIterLast);
  lA = lIterFirst;
  lB = nextX(lA, lIterLast);
  lC = nextX(lB, lIterLast);

  while (lC < lIterLast) {
    const double lSpreadDiff = std::fabs((lB->x - lA->x) - (lC->x - lB->x));

    if (lSpreadDiff > aPd.extremeDiff) {
      aPd.extremeDiff = lSpreadDiff;
      aPd.value = lA->x;
      aPd.dimension = 'x';
    }

    lA = lB;
    lB = lC;
    lC = nextX(lB, lIterLast);
  }

  aPd.data.sortY(lIterFirst, lIterLast);
  lA = lIterFirst;
  lB = nextY(lA, lIterLast);
  lC = nextY(lB, lIterLast);

  while (lC < lIterLast) {
    const double lSpreadDiff = std::fabs((lB->y - lA->y) - (lC->y - lB->y));

    if (lSpreadDiff > aPd.extremeDiff) {
      aPd.extremeDiff = lSpreadDiff;
      aPd.value = lA->y;
      aPd.dimension = 'y';
    }

    lA = lB;
    lB = lC;
    lC = nextY(lB, lIterLast);
  }
}

void 
MHist2::maxSpread(partialDistribution_t& aPd) const {

  xyc_vt::iterator lIterFirst = aPd.data.beginIter();
  xyc_vt::iterator lIterLast = aPd.data.endIter();
  xyc_vt::iterator lA, lB;

  aPd.data.sortX(lIterFirst, lIterLast);
  lA = lIterFirst;
  lB = nextX(lA, lIterLast);

  while (lB < lIterLast) {
    const double lSpread = lB->x - lA->x;

    if (lSpread > aPd.extremeDiff) {
      aPd.extremeDiff = lSpread;
      aPd.value = lA->x;
      aPd.dimension = 'x';
    }

    lA = lB;
    lB = nextX(lA, lIterLast);
  }

  aPd.data.sortY(lIterFirst, lIterLast);
  lA = lIterFirst;
  lB = nextY(lA, lIterLast);

  while (lB < lIterLast) {
    const double lSpread = lB->y - lA->y;

    if (lSpread > aPd.extremeDiff) {
      aPd.extremeDiff = lSpread;
      aPd.value = lA->y;
      aPd.dimension = 'y';
    }

    lA = lB;
    lB = nextY(lA, lIterLast);
  }
}

void 
MHist2::maxDiffCount(partialDistribution_t& aPd) const {

  xyc_vt::iterator lIterFirst = aPd.data.beginIter();
  xyc_vt::iterator lIterLast = aPd.data.endIter();

  aPd.data.sortX(lIterFirst, lIterLast);
  xyc_vt::iterator lIter = lIterFirst;
  uint lC, lCLag = cntX(lIter, lIterLast);
  lIter = nextX(lIter, lIterLast);

  while ( lIter < lIterLast ) {
    lC = cntX(lIter, lIterLast);
    const double lDiff = std::fabs(lC - lCLag);

    if ( aPd.extremeDiff < lDiff ) {
      aPd.extremeDiff = lDiff;
      aPd.value = (lIter - 1)->x;
      aPd.dimension = 'x';
    }

    lCLag = lC;
    lIter = nextX(lIter, lIterLast);
  }


  aPd.data.sortY(lIterFirst, lIterLast);
  lIter = lIterFirst;
  lCLag = cntY(lIter, lIterLast);
  lIter = nextY(lIter, lIterLast);

  while ( lIter < lIterLast ) {
    lC = cntY(lIter, lIterLast);
    const double lDiff = std::fabs(lC - lCLag);

    if ( aPd.extremeDiff < lDiff ) {
      aPd.extremeDiff = lDiff;
      aPd.value = (lIter - 1)->y;
      aPd.dimension = 'y';
    }

    lCLag = lC;
    lIter = nextY(lIter, lIterLast);
  }

}

void 
MHist2::maxDiffArea(partialDistribution_t& aPd) const {
  xyc_vt::iterator lIterFirst = aPd.data.beginIter();
  xyc_vt::iterator lIterLast = aPd.data.endIter();
  xyc_vt::iterator lIter, lIterNext;
  double lAreaLag;

  aPd.data.sortX(lIterFirst, lIterLast);
  lIter = nextX(lIterFirst, lIterLast);
  lIterNext = nextX(lIter, lIterLast);
  lAreaLag = ((lIter->x - lIterFirst->x) * cntX(lIterFirst, lIterLast));


  while (lIterNext < lIterLast) {
    const double lArea = ((lIterNext->x - lIter->x) * cntX(lIter, lIterLast));
    const double lDiffSpread = std::fabs(lAreaLag - lArea);

    if (lDiffSpread > aPd.extremeDiff) {
      aPd.extremeDiff = lDiffSpread;
      aPd.value = (lIter - 1)->x;
      aPd.dimension = 'x';
    }

    lAreaLag = lArea;
    lIter = lIterNext;
    lIterNext = nextX(lIter, lIterLast);
  }


  aPd.data.sortY(lIterFirst, lIterLast);
  lIter = nextY(lIterFirst, lIterLast);
  lIterNext = nextY(lIter, lIterLast);
  lAreaLag = ((lIter->y - lIterFirst->y) * cntY(lIterFirst, lIterLast));

  while (lIterNext < lIterLast) {
    const double lArea = ((lIterNext->y - lIter->y) * cntY(lIter, lIterLast));
    const double lDiffSpread = std::fabs(lAreaLag - lArea);

    if (lDiffSpread > aPd.extremeDiff) {
      aPd.extremeDiff = lDiffSpread;
      aPd.value = (lIter - 1)->y;
      aPd.dimension = 'y';
    }

    lAreaLag = lArea;
    lIter = lIterNext;
    lIterNext = nextY(lIter, lIterLast);
  }

}

void 
MHist2::minVariance(partialDistribution_t& aPd) const {
  xyc_vt::iterator lIterFirst = aPd.data.beginIter();
  xyc_vt::iterator lIterLast = aPd.data.endIter();

  Variance<double> lVarLow;
  Variance<double> lVarUp;
  xyc_vt::iterator lIterA, lIterB;

  aPd.extremeDiff = std::numeric_limits<double>::max();

  aPd.data.sortX(lIterFirst, lIterLast);
  lVarLow.init();
  lVarUp.init();
  for ( xyc_vt::const_iterator lIter = lIterFirst; lIter < lIterLast; ++lIter ) {
    for(uint c = 0; c < lIter->c; ++c) {
      lVarUp.step(lIter->x);
    }
  }

  lIterA = lIterFirst;
  lIterB = nextX(lIterA, lIterLast);
  while(lIterB < lIterLast) {
    const uint lCnt = cntX(lIterA, lIterLast);
    for(uint c = 0; c < lCnt; ++c) {
      lVarLow.step(lIterA->x);
      lVarUp.reverseStep(lIterA->x);
    }

    const double lVariance = lVarLow.variance() + lVarUp.variance();
    if ( lVariance < aPd.extremeDiff ) {
      aPd.extremeDiff = lVariance;
      aPd.value = lIterA->x;
      aPd.dimension = 'x';
    }

    lIterA = lIterB;
    lIterB = nextX(lIterA, lIterLast);
  }



  aPd.data.sortY(lIterFirst, lIterLast);
  lVarLow.init();
  lVarUp.init();
  for ( xyc_vt::const_iterator lIter = lIterFirst; lIter < lIterLast; ++lIter ) {
    for(uint c = 0; c < lIter->c; ++c) {
      lVarUp.step(lIter->y);
    }
  }

  lIterA = lIterFirst;
  lIterB = nextY(lIterA, lIterLast);
  while(lIterB < lIterLast) {
    const uint lCnt = cntY(lIterA, lIterLast);
    for(uint c = 0; c < lCnt; ++c) {
      lVarLow.step(lIterA->y);
      lVarUp.reverseStep(lIterA->y);
    }

    const double lVariance = lVarLow.variance() + lVarUp.variance();
    if ( lVariance < aPd.extremeDiff ) {
      aPd.extremeDiff = lVariance;
      aPd.value = lIterA->y;
      aPd.dimension = 'y';
    }

    lIterA = lIterB;
    lIterB = nextY(lIterA, lIterLast);
  }
}

std::ostream&
MHist2::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_MHIST2) << ' '
            << kind() // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 

} // end namespace

