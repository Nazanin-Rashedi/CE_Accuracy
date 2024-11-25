#include "simpleexception.hh"

SimpleException::SimpleException(const char*  aFileName,
                                 unsigned int aLineNumber,
                                 const char*  aFunctionName,
                                 const std::string& aComment)
                : _file(aFileName),
                  _line(aLineNumber),
                  _func(aFunctionName),
                  _com(aComment) {
}

SimpleException::SimpleException(const SimpleException& x)
                : _file(x._file),
                  _line(x._line),
                  _func(x._func),
                  _com(x._com) {
}

SimpleException&
SimpleException::operator=(const SimpleException& x) {
  _file = x._file;
  _line = x._line;
  _func = x._func;
  _com  = x._com;
  return (*this);
}

std::ostream&
SimpleException::print(std::ostream& os) const {
  os << _file << ", "
     << _line  << ": "
     << _func << ": "
     << _com 
     << '.'
     << std::endl;
  return os;
};


