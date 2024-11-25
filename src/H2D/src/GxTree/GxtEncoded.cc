#include "GxtEncoded.hh"

namespace H2D {

using namespace Gxt;

GxTreeEncoded::GxTreeEncoded() : _br(), _ptr(), _no(), _rootType(), _rootIdx() {
}



double
GxTreeEncoded::estimate(const rectangle_t& aQueryRectangle) const {
  double lRes = 0;
  switch(rootType()) {
    case N_G: lRes = estimate_G(aQueryRectangle, br(), rootIdx());
                   break;
    case N_L: lRes = estimate_L(aQueryRectangle, br(), rootIdx());
              break;
    case N_M: lRes = estimate_M(aQueryRectangle, br(), rootIdx());
              break;
    case N_S: lRes = estimate_S(aQueryRectangle, br(), rootIdx());
              break;
    default: assert(0 == 1);
             break;
  }
  return lRes;
}



double
GxTreeEncoded::estimate(const query_t& lQuery) const {
  rectangle_t aQueryRectangle = lQuery.rectangle();
  return estimate(aQueryRectangle);
}


double
GxTreeEncoded::estimate(const rectangle_t& aQueryRectangle,
                        const rectangle_t& aTr, // tile rectangle
                        const node_type_t& aNodeType,
                        const uint32_t     aIdx) const {
  double lRes = 0;
  switch(aNodeType) {
    case N_G: lRes = estimate_G(aQueryRectangle, aTr, aIdx);
              break;
    case N_L: lRes = estimate_L(aQueryRectangle, aTr, aIdx);
              break;
    case N_M: lRes = estimate_M(aQueryRectangle, aTr, aIdx);
              break;
    case N_S: lRes = estimate_S(aQueryRectangle, aTr, aIdx);
              break;
    default: assert(0 == 1);
             break;
  }
  return lRes;
}


double
GxTreeEncoded::estimate_G(const rectangle_t& aQueryRectangle,
                          const rectangle_t& aTr, 
                          const uint32_t     aIdx) const {
  const Gxt::Node_G_Generic* lNode = (const Node_G_Generic*) &(_ptr[N_G][aIdx]);


  double lRes = 0;
  const G_Node_Kind lGNodeKind = lNode->determineNodeKind();
  switch(lGNodeKind) {
    case  G_Inner_A_K:
          {
            // attach
            // estimate
            // detach
          }
          break;
    case  G_Inner_B_K:
          {
          }
          break;
    case  G_Inner_C_K:
          {
          }
          break;
    case  G_Inner_D_K:
          {
          }
          break;
    case  G_Leaf_A_K:
          {
          }
          break;
    case  G_Leaf_B_K:
          {
          }
          break;
    case  G_Leaf_D_K:
          {
          }
          break;
    case  G_Leaf_C_K:
          {
          }
          break;
    default: assert(0 == 1);
             break;
  
  };

  return lRes;
}



double
GxTreeEncoded::estimate_L(const rectangle_t& aQueryRectangle,
                          const rectangle_t& aTr,
                          const uint32_t     aIdx) const {
  const Node_G_Generic* lNode = (const Node_G_Generic*) &(_ptr[N_L][aIdx * nodeSize(N_L)]);


  double lRes = 0;


  return lRes;
}


double
GxTreeEncoded::estimate_M(const rectangle_t& aQueryRectangle,
                          const rectangle_t& aTr,
                          const uint32_t     aIdx) const {
  const Node_M_Generic* lNode = (const Node_M_Generic*) &(_ptr[N_M][aIdx * nodeSize(N_M)]);


  double lRes = 0;


  return lRes;
}


double
GxTreeEncoded::estimate_S(const rectangle_t& aQueryRectangle,
                          const rectangle_t& aTr,
                          const uint32_t     aIdx) const {
  const Node_S_Generic* lNode = (const Node_S_Generic*) &(_ptr[N_S][aIdx * nodeSize(N_S)]);


  double lRes = 0;


  return lRes;
}





} // end namespace H2D

