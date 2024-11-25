#include "GxtTypes.hh"


namespace H2D {

  namespace Gxt {

    std::ostream& 
    printBrCode(std::ostream& os, const uint16_t aBrCode) {
      uint16_t lBrCode = aBrCode;
      for(int i = 0; i < 4; ++i) {
        const int8_t lVal = (lBrCode & 0xF);
        const char   lChar = (lVal < 10) ? '0' + lVal : 'A' + (lVal - 10);
        os << lChar;
        lBrCode >>= 4;
      }
      return os;
    }




    const uint NodeSizes[5] = {28, 16, 8, 4, 0};
    const char NodeNames[5] = {'G', 'L', 'M', 'S', 'X'};

    void 
    NodeType::set(node_type_t aNt, Node_G_Inner_T aSk) {
      _nodeType = aNt;
      _isLeaf   = false;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_G_Leaf_T aSk) {
      _nodeType = aNt;
      _isLeaf   = true;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_L_Inner_T aSk) {
      _nodeType = aNt;
      _isLeaf   = false;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_L_Leaf_T aSk) {
      _nodeType = aNt;
      _isLeaf   = true;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_M_Inner_T aSk) {
      _nodeType = aNt;
      _isLeaf   = false;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_M_Leaf_T aSk) {
      _nodeType = aNt;
      _isLeaf   = true;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_S_Inner_T aSk) {
      _nodeType = aNt;
      _isLeaf   = false;
      _subkind  = aSk;
    }

    void 
    NodeType::set(node_type_t aNt, Node_S_Leaf_T aSk)  {
      _nodeType = aNt;
      _isLeaf   = true;
      _subkind  = aSk;
    }


    std::ostream&
    NodeType::print(std::ostream& os) const {
      os << nodeName((node_type_t) _nodeType) << '.';
      if(isLeaf()) {
        os << "Leaf" << '.';
        if(24 >= subkind()) {
           os << (char) ('A' + subkind());
        } else {
           os << ((int) 'A' + subkind());
        }
      } else {
        os << "Inner" << '.';
        if(28 >= subkind()) {
           os << (char) ('A' + subkind() - 1);
        } else {
           os << ((int) 'A' + subkind() - 1);
        }
      }
      return os;
    }



  } // end namespace Gxt


} // end namespace H2D



