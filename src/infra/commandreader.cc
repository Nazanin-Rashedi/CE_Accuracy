#include "commandreader.hh"

CommandReader::CommandReader(std::istream& aCis,
                             uint aLineBufferSize,
                             char aCommentChar) 
           : _linebuffersize(aLineBufferSize),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _buffer(0),
             _cis(aCis),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _noSplitMax(0),
             _noSplit(0),
             _splits(0) {
  _buffer = new char[linebuffersize()];
}

CommandReader::~CommandReader() {
  delete[] _buffer;
  if(0 != _splits) {
    delete[] _splits;
  }
}

bool
CommandReader::open() {
  if(!cis()) {
    throw SimpEx1("Input command stream is not o.k.");
  }

  _ok = true;
  _no_bytes_read = 0;
  getNonCommentLine();
  return (_ok);
}

bool
CommandReader::next() {
  getNonCommentLine();
  return ok();
}

void
CommandReader::close() {
}

void
CommandReader::getNonCommentLine() {
  char* x = _buffer;
  if(!ok()) { return; }
  while(!cis().eof()) {
    x = _buffer;
    cis().getline(_buffer, linebuffersize(), '\n');
    _linesize = cis().gcount() - 1;
    _no_bytes_read += (uint64_t) (_linesize + 1);
    if(1 > cis().gcount()) { // empty line
      continue;
    }
    ++_linecount;
    while(isspace(*x)) { ++x; }
    if(commentchar() == (*x)) {
      ++_commentlinecount;
      continue;
    }
    break;
  }
  if(cis().eof()) { _ok = false; }
}

/* Utilities */

bool
CommandReader::skipws(const char*& x) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while((x < e) && isspace(*x)) { ++x; }
  return (x < e);
}

bool
CommandReader::skipuntil(const char*& x, const char aCharToStopAt) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while(aCharToStopAt != (*x) && (x < e)) { ++x; }
  return (x == e) || (aCharToStopAt == *x);
}

bool
CommandReader::skipuntil(const char*& x, const char aCharToStopAt, const char aCharEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return true; }
  while((aCharToStopAt != (*x)) && (aCharEnd != (*x)) && (x < e)) { ++x; }
  return (x >= e) && (aCharEnd != (*x));
}

bool
CommandReader::read_string_set(const char*& x, string_st& aSetOut, const char aStringSep, const char aSep) {
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
CommandReader::read_char(const char*& x, char& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  out = *x;
  ++x;
  return true;
}

bool
CommandReader::read_uint(const char*& x, unsigned int& out) {
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
CommandReader::read_int(const char*& x, int& out) {
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
CommandReader::read_float(const char*& x, float& out) {
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
CommandReader::read_double(const char*& x, double& out) {
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
CommandReader::read_string(const char*& x, char aDelimiter, const char*& aBegin, const char*& aEnd) {
  const char* e = end();
  // std::cout << "LR 1" << std::endl;
  if(x < begin() || e <= x) { return false; }
  // std::cout << "LR 2" << std::endl;
  if(skipuntil(x, aDelimiter)) {
    // std::cout << "LR 3" << std::endl;
    ++x; // skip the delimiter
    const char* lBegin = x;
    if(skipuntil(x, aDelimiter)) {
      // std::cout << "LR 4" << std::endl;
      const char* lEnd = x;
      // ++x; // do not skip the delimiter
      aBegin = lBegin;
      aEnd   = lEnd;
      return true;
    }
  }
  return false;
}

bool
CommandReader::read_string_no_delim(const char*& x, 
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
CommandReader::read_ival(const char*& x, ival_t& out) {
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
CommandReader::read_uval(const char*& x, uval_t& out) {
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
CommandReader::read_fval(const char*& x, fval_t& out) {
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
CommandReader::read_dval(const char*& x, dval_t& out) {
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
CommandReader::read_stimestamp(const char*& x, STimestamp& out, char sep) {
  return out.set(x, sep);
}

char**
CommandReader::split(const char aSep, const bool aStrip) {
  uint lNoSplits = 0;
  if(0 == _splits) {
    _splits = new char*[100];
    _noSplitMax = 100;
  }
  const char* e = end();
  char* x = _buffer;
  while(x < e && '\0' != *x) {
    if(aSep == *x) {
      *x = '\0';
      ++lNoSplits;
      // XXX;
    }
  }

  return _splits;
}

