#ifndef SRC_INFRA_INDENT_HH
#define SRC_INFRA_INDENT_HH


struct Indent {
  unsigned int _indent;
  Indent(const unsigned int aIndent) : _indent(aIndent) {}
};

inline std::ostream&
operator<<(std::ostream& os, const Indent& aIndent) {
  os << std::string(aIndent._indent, ' ');
  return os;
}


#endif
