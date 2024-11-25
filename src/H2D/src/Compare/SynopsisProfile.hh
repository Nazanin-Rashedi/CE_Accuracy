#ifndef H2D_COMPARE_SYNOPSIS_PROFILE_HH
#define H2D_COMPARE_SYNOPSIS_PROFILE_HH
#pragma once

#include "cmp_util.hh"


/*
 * class SynopsisProfile
 * for each theta, one array with 
 * dimension no_cc() X no_sc() is stored
 * no_cc(): number of cardinality classes
 * no_sc(): number of selectivity classes
 * the raw experimental data is aggregated
 */

class SynopsisProfile {
  public:
    enum kind_t {
       kMax    = 0,
       kNoKind = 1
    };
  public:
    using data_t   = array_t;
    using data_vt  = std::vector<data_t>;
  public:
    SynopsisProfile();
    SynopsisProfile(const uint   aSynNo,
                    const kind_t aKind,
                    const uint   aMinCardClass,
                    const uint   aMaxCardClass);
  public:
    void init(const uint   aSynNo,
              const kind_t aKind,
              const uint   aMinCardClass,
              const uint   aMaxCardClass);
  public:
    inline uint no_cc()  const { return ((_max_card_class - _min_card_class) + 1); }
    inline uint syn_no() const { return _syn_no; }
    inline uint min_cc() const { return _min_card_class; }
    inline uint max_cc() const { return _max_card_class; }
    inline uint no_sc()  const { return gNoSelClass; }
    inline uint get(const uint aTheta, const uint aCc, const uint aSc) const;
  public:
    bool operator<=(const SynopsisProfile&) const;
    bool operator<(const SynopsisProfile&) const;
    std::tuple<bool,uint,uint> compare_1(const SynopsisProfile&) const;
    std::tuple<bool,uint,uint,uint> compare_n(const SynopsisProfile&, const uint lTheta) const;

  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& print(std::ostream& os, const uint aTheta) const;
  private:
    friend class ExpResAggr;
    inline void set(const uint aTheta, const uint aCc, const uint aSc, const uint aQc);
  private:
    data_vt _data; // one entry per theta
    uint    _syn_no;
    kind_t  _kind;
    uint    _min_card_class;
    uint    _max_card_class;
};

uint
SynopsisProfile::get(const uint aTheta, const uint aCc, const uint aSc) const {
  return _data[aTheta](aCc, aSc);
}

void
SynopsisProfile::set(const uint aTheta, 
                     const uint aCc,
                     const uint aSc,
                     const uint aQc) {
  _data[aTheta](aCc, aSc) = aQc;
}


#endif
