#include "buckethistogram.hh"


BucketHistogram::BucketHistogram() : _boundary(0), _noinf(0), _nonan(0), _min(0), _max(0), _hist(0) {
  _boundary = 10;
  init();
}

BucketHistogram::BucketHistogram(int aBoundary) : _boundary(aBoundary), _noinf(0), _nonan(0), _min(0), _max(0), _hist(0) {
  init();
}

void
BucketHistogram::init() {
  _noinf = 0;
  _nonan = 0;
  _min = std::numeric_limits<double>::max();
  _max = std::numeric_limits<double>::min();
  _hist = new uint[size()];
  for(uint i = begin(); i < end(); ++i) {
    _hist[i] = 0;
  }
}

BucketHistogram::~BucketHistogram() {
  delete[] _hist;
}


uint
BucketHistogram::noEntries() const {
  uint lRes = 0.0;
  const uint b = begin();
  const uint e = end();
  for(uint i = b; i < e; ++i) {
    lRes += _hist[i];
  }
  return lRes;
}

void
BucketHistogram::insert(const double x) {
  if(std::isinf(x)) { ++_noinf; return; }
  if(std::isnan(x)) { ++_nonan; return; }
  if(x < ((double) (-boundary()))) { doInsert(- boundary()); }
  else if(x > boundary()) { doInsert(boundary()); }
  else { doInsert((int) floor(x)); }
  if(_max < x) {
    _max = x;
  }
  if(_min > x) {
    _min = x;
  }
}

void
BucketHistogram::doInsert(const int x) {
  ++_hist[boundary() + x];
}

std::ostream&
BucketHistogram::print(std::ostream& os, bool aPrintPositiveOnly) const {
  for(uint i = begin(); i < end(); ++i) {
    if(!aPrintPositiveOnly || 0 < value(i)) {
      os << std::setw(3) << value(i) << ' '
         << std::setw(10) << freq(i)
         << std::endl;
    }
  }
  os << "min " << min() << std::endl;
  os << "max " << max() << std::endl;
  return os;
}

void
BucketHistogram::printManyRelative(const BucketHistogram* aHistArr, uint aNoHist, 
                                   int aPrecision, std::ostream& os) {
  const uint b = aHistArr[0].begin();
  const uint e = aHistArr[0].end();
  const double N = (double) (aHistArr[0].noEntries() + aHistArr[0].nonan() + aHistArr[0].noinf());
  const int lOldPrecision = os.precision();
  const std::ios_base::fmtflags lOldFlags = os.flags();
  for(uint i = b; i < e; ++i) {
    os << std::setw(3) << aHistArr[0].value(i) << ' ';
    for(uint j = 0; j < aNoHist; ++j) {
      os << std::setw(5) << (round((((double) aHistArr[j].freq(i)) / N) * 1000.0) / 10.0) << ' ';
    }
    os << std::endl;
  }
  os << "min ";
  for(uint j = 0; j < aNoHist; ++j) {
    os << std::setw(4) << aHistArr[j].min() << ' ';
  }
  os << std::endl;
  os << "max ";
  for(uint j = 0; j < aNoHist; ++j) {
    os << std::setw(4) << aHistArr[j].max() << ' ';
  }
  os << std::endl;
  os << "inf ";
  for(uint j = 0; j < aNoHist; ++j) {
    os << std::setw(4) << (round((((double) aHistArr[j].noinf()) / N) * 1000.0) / 10.0)  << ' ';
  }
  os << std::endl;
  os << "nan ";
  for(uint j = 0; j < aNoHist; ++j) {
    os << std::setw(4) << (round((((double) aHistArr[j].nonan()) / N) * 1000.0) / 10.0)  << ' ';
  }
  os << std::endl;
  os.flags(lOldFlags);
  os.precision(lOldPrecision);
}


