#include "spsummaryline.hh"


spsummaryline_t::spsummaryline_t() : summaryline_t(), _noSamples(0) {}
spsummaryline_t::~spsummaryline_t() {}

std::ostream&
spsummaryline_t::print(std::ostream& os, const std::string& aLineMarker) const {
  summaryline_t::print(os, aLineMarker);
  os << ' ' << _noSamples;
  return os;
}



