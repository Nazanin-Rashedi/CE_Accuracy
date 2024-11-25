#ifndef H2D_COMPARE_DSDESC_HH
#define H2D_COMPARE_DSDESC_HH
#pragma once

#include "cmp_util.hh"

/*
 * class DsDesc
 * contains statistics for each data set
 * it is read from a data set descriptor file
 * (gDsDescFileName in cmp_util.cc)
 */

class DsDesc {
  public:
    struct tuple_t {
      std::string _sds;
      std::string _ds;
      uint        _card;
      uint        _nodv;
      uint        _mxfreq;
      tuple_t() : _sds(), _ds(), _card(0), _nodv(0), _mxfreq(0) {}
      inline const std::string& sds() const { return _sds; }
      inline const std::string& ds()  const { return _ds; }
      inline       uint         card()  const { return _card; }
      inline       uint         card_class() const { return cardinality_class(card()); }
      inline       uint         nodv()  const { return _nodv; }
      inline       uint         mxfreq()  const { return _mxfreq; }
      std::ostream& print(std::ostream& os) const;
    };
    using tuple_vt = std::vector<tuple_t>;
  public:
    DsDesc();
  public:
    bool read(const std::string& aFilename);
  public:
    const tuple_t* find(const std::string& aSds, const std::string& aDs) const;
    uint min_card_class() const;
    uint max_card_class() const;
  public:
    tuple_vt _data;
};

std::ostream& operator<<(std::ostream& os, const DsDesc::tuple_t& x);

#endif
