#ifndef H2D_GXTREE_GXT_TYPES_HH
#define H2D_GXTREE_GXT_TYPES_HH

#include "infra/types.hh"
#include "../scale/scale.hh"

namespace H2D {

  namespace Gxt {

    std::ostream& printBrCode(std::ostream& os, const uint16_t aBrCode);

    enum node_type_t {
      N_G = 0,
      N_L = 1,
      N_M = 2,
      N_S = 3,
      N_NO_TYPE = 4
    };

    extern const uint  NodeSizes[5]; 
    extern const char  NodeNames[5]; 

    inline uint nodeSize(node_type_t x) { return NodeSizes[x]; }
    inline char nodeName(node_type_t x) { return NodeNames[x]; }

    inline std::ostream&
    operator<<(std::ostream& os, const node_type_t& aNodeType) {
      os << nodeName(aNodeType);
      return os;
    }

    // kinds for inner G-Nodes
    enum Node_G_Inner_T {
      N_G_I_INVALID = 0,
      N_G_I_A  = 1, // one child pointer
      N_G_I_B  = 2, // two child pointer
      N_G_I_C  = 3, // three child pointer
      N_G_I_D  = 4, // four child pointer
      N_G_I_NO = 5
    };

    // kinds for leaf G-Nodes
    enum Node_G_Leaf_T {
      N_G_L_A  = 0,
      N_G_L_B  = 1,
      N_G_L_C  = 2,
      N_G_L_D  = 3,
      N_G_L_NO = 4
    };

    // kinds for inner L-Nodes
    enum Node_L_Inner_T {
      N_L_I_INVALID = 0,
      N_L_I_A  = 1, // one child pointer
      N_L_I_B  = 2, // two child pointer
      N_L_I_C  = 3, // three child pointer
      N_L_I_NO = 4
    };

    // kinds for leaf L-Nodes
    enum Node_L_Leaf_T {
      N_L_L_A  = 0,
      N_L_L_B  = 1,
      N_L_L_C  = 2,
      N_L_L_D  = 3,
      N_L_L_NO = 4
    };


    // kinds for inner M-Nodes
    enum Node_M_Inner_T {
      N_M_I_INVALID = 0,
      N_M_I_A1  = 1,
      N_M_I_A2  = 2,
      N_M_I_B2  = 3,
      N_M_I_NO  = 4,
    };

    // kinds for leaf M-Nodes
    enum Node_M_Leaf_T {
      N_M_L_A  = 0,
      N_M_L_B  = 1,
      N_M_L_C  = 2,
      N_M_L_NO = 3,
    };

    // kinds for inner M-Nodes
    enum Node_S_Inner_T {
      N_S_I_INVALID = 0,
      N_S_I_A  = 1,
      N_S_I_NO = 2
    };

    // kinds for leaf M-Nodes
    enum Node_S_Leaf_T {
      N_S_L_A = 0,
      N_S_L_B = 1,
      N_S_L_NO = 3
    };





    // detailed description of node kind
    struct NodeType {
      typedef q::ScaleMgr::scale_et scale_et;
      uint8_t _nodeType;
      uint8_t _isLeaf;
      uint8_t _subkind;

      NodeType() : _nodeType(0xFF), _isLeaf(0xFF), _subkind(0xFF) {}

      void set(node_type_t, Node_G_Inner_T);
      void set(node_type_t, Node_G_Leaf_T);
      void set(node_type_t, Node_L_Inner_T);
      void set(node_type_t, Node_L_Leaf_T);
      void set(node_type_t, Node_M_Inner_T);
      void set(node_type_t, Node_M_Leaf_T);
      void set(node_type_t, Node_S_Inner_T);
      void set(node_type_t, Node_S_Leaf_T);

      void set_G_Inner(Node_G_Inner_T x);
      void set_G_Leaf (Node_G_Leaf_T x);
      void set_L_Inner(Node_L_Inner_T x);
      void set_L_Leaf (Node_L_Leaf_T x);
      void set_M_Inner(Node_M_Inner_T x);
      void set_M_Leaf (Node_M_Leaf_T x);
      void set_S_Inner(Node_S_Inner_T x);
      void set_S_Leaf (Node_S_Leaf_T x);

      inline node_type_t    nodeType() const { return (node_type_t) _nodeType; }
      inline bool           isLeaf() const { return (0 != _isLeaf); }
      inline uint           subkind() const { return _subkind; }

      inline bool           is_G_L_C() const { return (isLeaf() && 
                                                       (N_G == _nodeType) &&
                                                       (N_G_L_C == _subkind)); }


      inline Node_G_Inner_T get_G_Leaf()  const { return (Node_G_Inner_T) _subkind; }
      inline Node_G_Leaf_T  get_G_Inner() const { return (Node_G_Leaf_T)  _subkind; }
      inline Node_L_Inner_T get_L_Inner() const { return (Node_L_Inner_T) _subkind; }
      inline Node_L_Leaf_T  get_L_Leaf()  const { return (Node_L_Leaf_T)  _subkind; }
      inline Node_M_Inner_T get_M_Inner() const { return (Node_M_Inner_T) _subkind; }
      inline Node_M_Leaf_T  get_M_Leaf()  const { return (Node_M_Leaf_T)  _subkind; }
      inline Node_S_Inner_T get_S_Inner() const { return (Node_S_Inner_T) _subkind; }
      inline Node_S_Leaf_T  get_S_Leaf()  const { return (Node_S_Leaf_T)  _subkind; }

      std::ostream& print(std::ostream& os) const;
    };

    inline
    std::ostream& operator<<(std::ostream& os, const NodeType& x) {
      return x.print(os);
    }

  } // end namespace Gxt

} // end namespace H2D



#endif
