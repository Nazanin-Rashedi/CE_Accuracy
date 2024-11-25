#include "linereader.hh"

LineReader::LineReader(const char* aFileName, 
                       const uint  aLineBufferSize,
                       const char  aCommentChar) 
           : _streamgiven(false),
             _filename(aFileName),
             _linebuffersize(aLineBufferSize),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _buffer(0),
             _ifs(0),
             _is(std::cin),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _no_bytes_read_comment(0),
             _noSplit(0),
             _splits(),
             _splitsEnd(),
             _subsplitsBegin(),
             _subsplitsEnd() {
  _buffer = new char[linebuffersize() + 3];
}

LineReader::LineReader(std::istream& aIs,
                       const uint    aLineBufferSize,
                       const char    aCommentChar)
           : _streamgiven(true),
             _filename("no-file"),
             _linebuffersize(aLineBufferSize),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _buffer(0),
             _ifs(0),
             _is(aIs),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _no_bytes_read_comment(0),
             _noSplit(0),
             _splits(),
             _splitsEnd(),
             _subsplitsBegin(),
             _subsplitsEnd()  {
  _buffer = new char[linebuffersize()];
}

LineReader::~LineReader() {
  if(0 != _ifs) {
    delete _ifs;
  }
  delete[] _buffer;
}

bool
LineReader::open(const bool aSkipCommentLine) {
  if(_streamgiven) {
    if(_is) {
      _ok = true;
    } else {
      _ok = false;
    }
  } else {
    if(0 != _ifs) {
      throw SimpEx3("The file \'", filename(), "\' has already been opened");
    }
    _ifs = new std::ifstream(filename());
    if(!ifs()) {
      throw SimpEx3("Cannot open file \'", filename(), "\'");
    }
    _ok = true;
    _no_bytes_read = 0;
  }
  if(aSkipCommentLine) {
    getNonCommentLine();
  } else {
    getNextLine();
  }
  return (_ok);
}

bool
LineReader::next(const bool aSkipCommentLine) {
  if(aSkipCommentLine) {
    getNonCommentLine();
  } else {
    getNextLine();
  }
  return ok();
}

void
LineReader::close() {
  if(0 != _ifs) {
    delete _ifs;
    _ifs = 0;
  }
}

bool
LineReader::isCommentLine() const {
  const char* x = _buffer;
  if(skipws(x) && commentchar() == (*x)) {
    return true;
  }
  return false;
}

void
LineReader::getNextLine() {
  if(!ok()) { return; }
  bool lNonEmptyLineRead = false;
  while(!ifs().eof() && !lNonEmptyLineRead) {
    ifs().getline(_buffer, linebuffersize(), '\n');
    size_t lGCount = ifs().gcount();
    _linesize = lGCount - 1; // '\n' also counts as one byte read
    _no_bytes_read += (uint64_t) lGCount;
    if(1 > ifs().gcount()) {
      continue;
    }
    if(lGCount >= (linebuffersize() - 1)) {
      std::cout << "Error: buffer too small for line " << linecount() << ':' << std::endl;
      throw SimpEx3("Buffer too small for file \'", filename(), "\'");
    }
    ++_linecount;
    if(isCommentLine()) {
      ++_commentlinecount;
      _no_bytes_read_comment += (uint64_t) (_linesize + 1);
    }
    break;
  }
  if(ifs().eof()) { _ok = false; }
}

void
LineReader::getNonCommentLine() {
  if(!ok()) { return; }
  while(!ifs().eof()) {
    ifs().getline(_buffer, linebuffersize(), '\n');
    size_t lGCount = ifs().gcount();
    _linesize = lGCount - 1; // '\n' also counts as one byte read
    _no_bytes_read += (uint64_t) (_linesize + 1);
    if(1 > ifs().gcount()) { // empty line
      continue;
    }
    if(lGCount >= (linebuffersize() - 1)) {
      std::cout << "LineReader::getNonCommentLine: Error: buffer too small for line " << linecount() << ':' << std::endl;
      throw SimpEx3("Buffer too small for file \'", filename(), "\'");
    }
    ++_linecount;
    const char* x = _buffer;
    if(skipws(x) && commentchar() == (*x)) {
      ++_commentlinecount;
      _no_bytes_read_comment += (uint64_t) (_linesize + 1);
      continue;
    }
    if(x == end()) { // line contains only blanks
      continue;
    }
    break;
  }
  if(ifs().eof()) { _ok = false; }
}

/* Utilities */

bool
LineReader::eat(const char*& x, const char c) const {
  if(c == *x) {
    ++x;
    return true;
  }
  return false;
}

bool
LineReader::skipws(const char*& x) const {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while((x < e) && isspace(*x)) { ++x; }
  return (x < e);
}

bool
LineReader::skipuntil(const char*& x, const char aCharToStopAt) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while(aCharToStopAt != (*x) && (x < e)) { ++x; }
  return (x == e) || (aCharToStopAt == *x);
}

bool
LineReader::skipuntil(const char*& x, const char aCharToStopAt, const char aCharEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return true; }
  while((aCharToStopAt != (*x)) && (aCharEnd != (*x)) && (x < e)) { ++x; }
  return (x >= e) && (aCharEnd != (*x));
}

bool
LineReader::read_string_set(const char*& x, string_st& aSetOut, const char aStringSep, const char aSep) {
  const char* e = end();
  const char* lBegin = 0;
  const char* lEnd = 0;
  if(x < begin() || e <= x) { return false; }
  while((aSep != *x) && (x < e)) {
    if(aStringSep == (*x)) { ++x; continue; }
    lBegin = x;
    skipuntil(x, aStringSep, aSep);
    lEnd = x;
    if(lBegin < lEnd) {
      aSetOut.insert(std::string(lBegin, lEnd));
    }
  }
  return true;
}



bool
LineReader::read_char(const char*& x, char& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  out = *x;
  ++x;
  return true;
}

bool
LineReader::read_uint(const char*& x, unsigned int& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || lRead < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_uint64(const char*& x, uint64_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int64_t lRead = strtoll(x, &lEndPtr, 10);
  if(lEndPtr == x || lRead < 0) {
    return false;
  }
  out = (uint64_t) lRead;
  x = lEndPtr;
  return true;
}



bool
LineReader::read_int(const char*& x, int& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_float(const char*& x, float& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  float lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_double(const char*& x, double& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  double lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_bool(const char*& x, bool& out) {
  if( (0 == strncmp("true", x, 4)) ||
      (0 == strncmp("TRUE", x, 4)) ||
      (0 == strncmp("True", x, 4))) {
    out = true;
    x += 4;
    return true;
  }
  if( (0 == strncmp("false", x, 5)) ||
      (0 == strncmp("FALSE", x, 5)) ||
      (0 == strncmp("False", x, 5))) {
    out = false;
    x += 5;
    return true;
  }
  return false;
}

bool
LineReader::read_string(const char*& x, const char aDelimiter, 
                        const char*& aBegin, const char*& aEnd) {
  const char* e = end();
  // std::cout << "LR 1" << std::endl;
  if(x < begin() || e <= x) { 
    aBegin = 0;
    aEnd   = 0;
    return false; 
  }
  // std::cout << "LR 2" << std::endl;
  if(skipuntil(x, aDelimiter)) {
    // std::cout << "LR 3" << std::endl;
    ++x; // skip the delimiter
    const char* lBegin = x;
    if(skipuntil(x, aDelimiter)) {
      // std::cout << "LR 4" << std::endl;
      const char* lEnd = x;
      ++x; // do not skip the delimiter
      aBegin = lBegin;
      aEnd   = lEnd;
      return true;
    }
  }
  return false;
}

// string: [aBegin,aEnd[
// resulting x: first char after the string
bool
LineReader::read_string(const char*& aStart, const char aDelimiter, const char aEscape,
                        const char*& aBegin, const char*& aEnd) {
  char* x = 0;
  char* y = 0;
  const char* e = end();
  const char* lBegin = 0;
  const char* lEnd = 0;
  // std::cout << "LR 1" << std::endl;
  if(aStart < begin() || e <= aStart) { return false; }
  // std::cout << "LR 2" << std::endl;
  if(skipuntil(aStart, aDelimiter)) {
    // std::cout << "LR 3" << std::endl;
    x = const_cast<char*>(aStart);
    ++x; // skip the delimiter
    y = x;
    lBegin = x;
    while(x < e) {
      // std::cout << "LR 4: " << (*x) << ' ' << (*y) << std::endl;
      if(aEscape == *x) {
        ++x;
        *y = *x;
        ++x; ++y;
      } else
      if(aDelimiter == *x) {
        lEnd = y;
        break;
      } else {
        if(x != y) {
          *y = *x;
        }
        ++x; ++y;
      }
    }
    if(aDelimiter == *x) {
      // std::cout << "LR 5a: " << (*x) << ' ' << (*y) << std::endl;
      ++x; // skip the delimiter
      // std::cout << "LR 5b: " << (*x) << ' ' << (*y) << std::endl;
      aBegin = lBegin;
      aEnd   = lEnd;
      aStart = x;
      return true;
    } else {
      // std::cout << "LR 6: " << (*x) << ' ' << (*y) << std::endl;
    }
  }
  return false;
}


bool
LineReader::read_string_no_delim(const char*& x, 
                                 char aSep, 
                                 const char*& aBegin, 
                                 const char*& aEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  const char* lBegin = x;
  if(skipuntil(x, aSep)) {
    const char* lEnd = x;
    // ++x; // do not skip the delimiter
    aBegin = lBegin;
    aEnd   = lEnd;
    return true;
  }
  return false;
}


bool
LineReader::read_ival(const char*& x, ival_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || lRead < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_uval(const char*& x, uval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || lRead < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_fval(const char*& x, fval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  float lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_dval(const char*& x, dval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  double lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_stimestamp(const char*& x, STimestamp& out, char sep) {
  return out.set(x, sep);
}

int
LineReader::split_line(const char aSep, const bool aStrip) {
  _splits.clear();
  const char* e = end();
  const char* x = _buffer;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    while(aSep != *x && x < e && '\0' != *x) ++x;
    // if(('\0' == *x) || (e <= x)) {
    //   break;
    // }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _splits.push_back((char*) lBegin);

    ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

int
LineReader::split_line(const char aSep, const char aDelim, const bool aStrip) {
  _splits.clear();
  _splitsEnd.clear();
  const char* e = end();
  const char* x = _buffer;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if((aDelim == (*x) && (aDelim == (*(x + 1))))) {
        x += 2;
      } else {
        if(   (aDelim == (*x))
           && (aDelim != (*(x + 1)))
           && ('\\' != (*(x - 1)))
          ) {
          lInString = !lInString;
          // std::cout << "TOGGLE" << std::endl;
        }
        ++x;
      }
    }
    if(lInString) {
      std::cerr << "LineReader::split_line: Error; unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _splits.push_back((char*) lBegin);
    _splitsEnd.push_back((char*) lEnd);
     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

int
LineReader::split(const char* aBegin, const char* aEnd,
                  const char aSep, const char aDelim, const bool aStrip) {
  _subsplitsBegin.clear();
  _subsplitsEnd.clear();
  const char* e = aEnd;
  const char* x = aBegin;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if(aDelim == (*x)) {
        lInString = !lInString;
        // std::cout << "TOGGLE" << std::endl;
      }
      ++x;
    }
    if(lInString) {
      std::cerr << "LineReader::split: Error: unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _subsplitsBegin.push_back((char*) lBegin);
    _subsplitsEnd.push_back((char*) lEnd);

     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}



int
LineReader::splitDoubleDelim(const char* aBegin, const char* aEnd,
                             const char aSep, const char aDelim, const bool aStrip) {
  _subsplitsBegin.clear();
  _subsplitsEnd.clear();
  const char* e = aEnd;
  const char* x = aBegin;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if(aDelim == (*x)) {
        if(aDelim == (*(x+1))) {
          // std::cout << "TOGGLE" << std::endl;
          lInString = !lInString;
          ++x;
        }
      }
      ++x;
    }
    if(lInString) {
      std::cerr << "LineReader::splitDoubleDelim: Error: unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _subsplitsBegin.push_back((char*) lBegin);
    _subsplitsEnd.push_back((char*) lEnd);

     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

