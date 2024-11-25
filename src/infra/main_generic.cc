#include <iostream>

#include "genericfield.hh"
#include "generictuple.hh"


void
test0() {
  GenericField a((int) 5);
  GenericField b((uint) 5);
  std::cout << "a: " << a << ' ' << a.typ() << std::endl;
  std::cout << "b: " << b << ' ' << b.typ() << std::endl;
}

void
test1() {
  GenericTuple lTuple(5);
  std::cout << lTuple << std::endl;
}


void
test2() {
  GenericSchema lSchema("eqcentenniallatlon");
  lSchema.push_back("lat", GenericField::double_e);
  lSchema.push_back("lon", GenericField::double_e);
  // lSchema.push_back("lon", GenericField::double_e); 

  for(uint i = 0; i < lSchema.schema().size(); ++i) {
    std::cout << std::setw(2) << i << ' '
              << lSchema.typ(i) << ' '
              << lSchema.map().size() << ' '
              << std::endl;
  }
  std::cout << lSchema << std::endl;

  GenericTuple lTuple(lSchema.schema());
  std::cout << lTuple << std::endl;

  const char* lFileName = "/home/moer/qexp/Dim2/eqcentennial.LatLon";

  LineReader lLineReader(lFileName);
  SimpleCharContainer lCharContainer(1000);

  if(!lLineReader.open()) {
    throw SimpEx3("Could' not open LineReader for file '", lFileName, "'.");
  }
  for(; lLineReader.ok(); lLineReader.next()) {
    lTuple.read(lLineReader, ' ', lSchema.schema(), lCharContainer);
    std::cout << lTuple << std::endl;
  }

}

int
main() {
  try {
    // test0();
    // test1();
    test2();
  } 
  catch(const SimpleException& lException) {
    std::cerr << "Exception caught: " << std::endl
              << lException << std::endl;
  }
  
}

