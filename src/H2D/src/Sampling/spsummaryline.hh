#ifndef SAMPLING_SUMMARY_LINE_HH
#define SAMPLING_SUMMARY_LINE_HH

#include "infra/summaryline.hh"

// summary line for sampling
struct spsummaryline_t : public summaryline_t {
  uint _noSamples;

  spsummaryline_t();
  virtual ~spsummaryline_t();
  virtual std::ostream&  print(std::ostream& os, const std::string& aLineMarker) const;
};



#endif

