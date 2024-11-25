
typedef unsigned int uint;

#include "GxtTypes.hh"
#include "Gxt_Node_G.hh"
#include "Gxt_Node_L.hh"
#include "Gxt_Node_M.hh"
#include "Gxt_Node_S.hh"

using namespace H2D;
using namespace H2D::Gxt;

int
main() {
  std::cout << "L-hdr size: " << sizeof(H2D::Gxt::Node_L_Generic::hdr_t)
            << std::endl;

  std::cout << "(sizeof(Node_G_Generic) * 8) = "
            <<  (sizeof(Node_G_Generic) * 8)
            << std::endl;
  std::cout << "(sizeof(Node_L_Generic) * 8) = "
            <<  (sizeof(Node_L_Generic) * 8)
            << std::endl;
  std::cout << "(sizeof(Node_M_Generic) * 8) = "
            <<  (sizeof(Node_M_Generic) * 8)
            << std::endl;
  std::cout << "(sizeof(Node_S_Generic) * 8) = "
            <<  (sizeof(Node_S_Generic) * 8)
            << std::endl;

  return 0;
}


