#include "DsDesc.hh"

std::ostream&
DsDesc::tuple_t::print(std::ostream& os) const {
  os << sds()  << '|'
     << '\"' << ds() << '\"'  << '|'
     << card() << '|'
     << card_class() << '|'
     << nodv() << '|'
     << mxfreq();
  return os;
}

std::ostream&
operator<<(std::ostream& os, const DsDesc::tuple_t& x) {
  return x.print(os);
}

DsDesc::DsDesc() : _data() {}

const DsDesc::tuple_t*
DsDesc::find(const std::string& aSds, const std::string& aDs) const {
  for(uint i = 0; i < _data.size(); ++i) {
    if((_data[i].sds() == aSds) && (_data[i].ds() == aDs)) {
      return &(_data[i]);
    }
  }
  return nullptr;
}

uint
DsDesc::min_card_class() const {
  uint lRes = 77;
  for(const auto& x : _data) {
    if(lRes > x.card_class()) {
      lRes = x.card_class();
    }
  }
  return lRes;
}

uint
DsDesc::max_card_class() const {
  uint lRes = 0;
  for(const auto& x : _data) {
    if(lRes < x.card_class()) {
      lRes = x.card_class();
    }
  }
  return lRes;
}

bool
DsDesc::read(const std::string& aFilename) {
  LineReader lR(aFilename.c_str());
  const char  lSep = '|';
  const char* x = 0;
  const char* b = 0;
  const char* e = 0;
  tuple_t t;
  for(lR.open(); lR.ok(); lR.next()) {
    x = lR.begin();
    if(!lR.read_string_no_delim(x, lSep, b, e)) {
      std::cout << "Error: DsDesc::read (1)" << std::endl;
      return false;
    }
    if(!lR.eat(x, lSep)) {
      std::cout << "Error: DsDesc::read (1s)" << std::endl;
      return false;
    }
    t._sds = std::string(b,e);
    if(!lR.read_string(x, '\"', '\\', b, e)) {
      std::cout << "Error: DsDesc::read (2)" << std::endl;
      return false;
    }
    t._ds = std::string(b,e);
    if(!lR.eat(x, lSep)) {
      std::cout << "Error: DsDesc::read (2s)" << std::endl;
      return false;
    }
    if(!lR.read_uint(x, t._card)) {
      std::cout << "Error: DsDesc::read (3)" << std::endl;
      return false;
    }
    if(!lR.eat(x, lSep)) {
      std::cout << "Error: DsDesc::read (3s)" << std::endl;
      return false;
    }
    if(!lR.read_uint(x, t._nodv)) {
      std::cout << "Error: DsDesc::read (4)" << std::endl;
      return false;
    }
    if(!lR.eat(x, lSep)) {
      std::cout << "Error: DsDesc::read (4s)" << std::endl;
      return false;
    }
    if(!lR.read_uint(x, t._mxfreq)) {
      std::cout << "Error: DsDesc::read (5)" << std::endl;
      return false;
    }
    if(!lR.eat(x, lSep)) {
      std::cout << "Error: DsDesc::read (5s)" << std::endl;
      return false;
    }
    _data.emplace_back(t);
  }
  return true;
}


