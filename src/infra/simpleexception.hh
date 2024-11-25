#ifndef SIMPLE_EXCEPTION_HH
#define SIMPLE_EXCEPTION_HH

#include <iostream>
#include <iomanip>
#include <string>

class SimpleException {
  public:
    SimpleException(const char*  aFileName,
                    unsigned int aLineNumber,
                    const char*  aFunctionName,
                    const std::string& _com);
    SimpleException(const SimpleException&);
  public:
    const std::string& msg() const { return _com; }
  public:
    std::ostream& print(std::ostream& os) const;
    SimpleException& operator=(const SimpleException&);
  private:
    const char*  _file;
    unsigned int _line;
    const char*  _func;
    std::string  _com;
};

inline
std::ostream& operator<<(std::ostream& os, const SimpleException& x) {
  return x.print(os);
}


#define SimpEx1(c1) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__, c1)

#define SimpEx2(c1,c2) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__, std::string(c1) + std::string(c2))

#define SimpEx3(c1,c2,c3) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__,  \
                                          std::string(c1) + std::string(c2) + std::string(c3))

#define SimpEx4(c1,c2,c3,c4) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__,  \
                                             std::string(c1) + std::string(c2) \
                                             + std::string(c3) + std::string(c4))

#define SimpEx5(c1,c2,c3,c4,c5) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__,  \
                                                std::string(c1) + std::string(c2) \
                                                + std::string(c3) + std::string(c4) \
                                                + std::string(c5))

#define SimpEx6(c1,c2,c3,c4,c5,c6) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__,  \
                 std::string(c1) + std::string(c2) + std::string(c3) \
               + std::string(c4) + std::string(c5) + std::string(c6))

#define SimpEx7(c1,c2,c3,c4,c5,c6,c7) SimpleException(__FILE__, __LINE__, __PRETTY_FUNCTION__,  \
                 std::string(c1) + std::string(c2) + std::string(c3) + std::string(c4) \
               + std::string(c5) + std::string(c6) + std::string(c7))


#endif
