#include "grid.hh"



const int64_t
Grid::_maxTileFreq[7] = {         0, // 0
                                  2, // 1
                                 84, // 2
                                231, // 3
                                497, // 4
                              18904, // 5
                          585709328LL // 6
                        };

Grid::Grid(const int aP, const int aFBits, const bool aHasIBits)
     : _p(aP), _fbits(aFBits), _hasIbits(aHasIBits) {
}


std::ostream&
Grid::printGridName(std::ostream& os) const {
  os << "$G_{" << p() << ',' << fbits() << "}^{" << (hasIbits() ? '+' : '-' ) << "}$";
  return os;
}

std::ostream&
Grid::printLatexHeader(std::ostream& os) {
  os << "\\begin{center}" << std::endl;
  os << "\\begin{tabular}{crrrrrrr}" << std::endl;
  os << "grid & size & uniCap       &   uniDVpb & \\multicolumn{2}{c}{128} & \\multicolumn{2}{c}{256} \\\\\\hline" << std::endl;
  os << "     &      &              &           & rest &         uniDVpb & rest &         uniDVpb \\\\\\hline" << std::endl;
  return os;
}

std::ostream&
Grid::printLatexFooter(std::ostream& os) {
  os << "\\end{tabular}" << std::endl;
  os << "\\end{center}" << std::endl;
  return os;
}


std::ostream&
Grid::printLatex(std::ostream& os) const {
  printGridName(os) << " & "
                    << std::setw( 4) << size()    << " & "
                    << std::setw(12) << uniCap()  << " & "
                    << std::setw( 9) << uniDvPb() << " & ";
  if(rest(128)) {
    os << std::setw( 2) << rest_div16(128) << '/' << rest_mod16(128) << " & "
       << std::setw( 9) << uniDvPb(128);
  } else {
    os << "      &           ";
  }
  os << " & ";
  if(rest(256)) {
    os << std::setw( 2) << rest_div16(256) << '/' << rest_mod16(256) << " & "
       << std::setw( 9) << uniDvPb(256);
  } else {
    os << "      &           & ";
  }
    os << " \\\\" << std::endl;
  return os;
}



