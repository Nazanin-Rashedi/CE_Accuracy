#include "linereaderWithEdit.hh"


void
test0() {
  LineReaderWithEdit lLr("> ", '#');

  const char* x = 0;
  const char* b = 0;
  const char* e = 0;

  for(lLr.open(); lLr.ok(); lLr.next()) {
    std::cout << "line " << lLr.linecount() << ": " << lLr.line() << std::endl;
    x = lLr.line();
    lLr.skipws(x);
    lLr.read_string_no_delim(x, ' ', b, e);
    std::cout << "the command is: " << std::string(b,e) << std::endl;
  }
  lLr.close();
}



int
main() {
  test0();
  return 0;
}

