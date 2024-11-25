#ifndef H2D_COMPARE_EXP_RES_AGGR_HH
#define H2D_COMPARE_EXP_RES_AGGR_HH
#pragma once

#include "cmp_util.hh"
#include "SynopsisProfile.hh"

/*
 * class ExpResAggr
 * is used to aggregate the raw experimental results for a single synopsis
 * it is an intermediate representation to finally produce instances of SynopsisProfile
 * it aggregates the experimental results into the form
 * <card_class> <sel_class> <theta> <qerr_class> --> <cnt>
 * in its member _data
 * the storage format of _data is
 * - top level: one vector element for each theta
 * - second level: one vector for each cardinality class
 * - therein: an array_t sel_class X qerr_class
 * at first, only the top level is allocated, lower levels remain uninitialized
 */

class ExpResAggr {
  public:
    using data_t   = array_t;
    using data_vt  = std::vector<data_t>;
    using data_vvt = std::vector<data_vt>;
  public:
    ExpResAggr();
    ExpResAggr(const uint aSynNo, const uint aMinCardClass, const uint aMaxCardClass);
  public:
    inline uint no_cc()  const { return ((_max_card_class - _min_card_class) + 1); }
    inline uint syn_no() const { return _syn_no; }
    inline uint min_cc() const { return _min_card_class; }
    inline uint max_cc() const { return _max_card_class; }
  public:
    void init(const uint aSynNo, const uint aMinCardClass, const uint aMaxCardClass);
    void insert(const uint aTheta, const uint aCardClass, const array_t& aArray);
  public:
    void get_profile_max(SynopsisProfile&) const;
  public:
    std::ostream& print(std::ostream& os) const;
    std::ostream& print_rel(std::ostream& os) const;
  private:
    data_vvt _data;
    uint     _syn_no; // synopsis number
    uint     _min_card_class;
    uint     _max_card_class;
};

using exp_res_aggr_vt = std::vector<ExpResAggr>;

#endif

