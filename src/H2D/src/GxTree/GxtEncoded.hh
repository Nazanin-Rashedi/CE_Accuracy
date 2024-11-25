#ifndef H2D_GXTREE_GXT_ENCODED_HH
#define H2D_GXTREE_GXT_ENCODED_HH

#include <iostream>
#include <iomanip>


#include "infra/types.hh"

#include "GxtTypes.hh"
#include "Gxt_G_Node.hh"
#include "Gxt_L_Node.hh"
#include "Gxt_M_Node.hh"
#include "Gxt_S_Node.hh"

namespace H2D {

class GxTreeEncoded {
    typedef Gxt::node_type_t node_type_t;
  public:
    GxTreeEncoded();
  public:
    inline const rectangle_t& br() const { return _br; }
    inline const node_type_t rootType() const { return _rootType; }
    inline const uint32_t    rootIdx() const { return _rootIdx; }
  public:
    double estimate(const rectangle_t& aQueryRectangle) const;
    double estimate(const query_t& lQuery) const;
  private:
    double estimate(const rectangle_t& aQueryRectangle,
                    const rectangle_t& aBr,
                    const node_type_t& aNodeType,
                    const uint32_t     aIdx) const;
    double estimate_G(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aBr,
                      const uint32_t     aIdx) const;
    double estimate_L(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aBr,
                      const uint32_t     aIdx) const;
    double estimate_M(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aBr,
                      const uint32_t     aIdx) const;
    double estimate_S(const rectangle_t& aQueryRectangle,
                      const rectangle_t& aBr,
                      const uint32_t     aIdx) const;
  private:
    rectangle_t _br;  // bounding rectangle for all data points
    uint8_t*    _ptr[4]; // pointer to nodes of every kind
    uint16_t    _no[4];  // number  of nodes of every kind
    node_type_t _rootType; // node type of root node
    uint32_t    _rootIdx; // offset of root node (should be zero anyway)
};

} // end namespace

#endif
