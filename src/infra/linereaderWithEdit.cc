#include "linereaderWithEdit.hh"

LineReaderWithEdit::LineReaderWithEdit(const char* aPrompt, char aCommentChar) 
           : _prompt(aPrompt),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _line(0),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _noSplit(0),
             _splits() {
}

LineReaderWithEdit::~LineReaderWithEdit() {
}

bool
LineReaderWithEdit::open() {
  getNonCommentLine();
  return ok();
}

bool
LineReaderWithEdit::next() {
  getNonCommentLine();
  return ok();
}

void
LineReaderWithEdit::close() {
}

void
LineReaderWithEdit::getNonCommentLine() {
  const char* x = "\0";

  do {
    _line = readline(prompt());

    // std::cout << "LINE: " << line() << std::endl;

    if(0 == _line) { 
      _ok = false; 
      return; 
    }

    ++_linecount;
    _linesize = strlen(line());

    if(0 == _linesize) { continue; }

    _no_bytes_read += (uint64_t) linesize();

    x = line();
    while(isspace(*x)) { ++x; }
    if(_commentchar == (*x)) {
      ++_commentlinecount;
    }
  } while((0 == linesize()) || ('\0' == *x) || (commentchar() == (*_line)));

  add_history(_line);
  _ok = true;
}

/* Utilities */

bool
LineReaderWithEdit::skipws(const char*& x) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while((x < e) && isspace(*x)) { ++x; }
  return (x < e);
}

bool
LineReaderWithEdit::skipuntil(const char*& x, const char aCharToStopAt) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while(aCharToStopAt != (*x) && (x < e)) { ++x; }
  return (x == e) || (aCharToStopAt == *x);
}

bool
LineReaderWithEdit::skipuntil(const char*& x, const char aCharToStopAt, const char aCharEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return true; }
  while((aCharToStopAt != (*x)) && (aCharEnd != (*x)) && (x < e)) { ++x; }
  return (x >= e) && (aCharEnd != (*x));
}

bool
LineReaderWithEdit::read_string_set(const char*& x, string_st& aSetOut, const char aStringSep, const char aSep) {
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
LineReaderWithEdit::read_char(const char*& x, char& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  out = *x;
  ++x;
  return true;
}

bool
LineReaderWithEdit::read_uint(const char*& x, unsigned int& out) {
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
LineReaderWithEdit::read_int(const char*& x, int& out) {
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
LineReaderWithEdit::read_float(const char*& x, float& out) {
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
LineReaderWithEdit::read_double(const char*& x, double& out) {
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
LineReaderWithEdit::read_string(const char*& x, char aDelimiter, const char*& aBegin, const char*& aEnd) {
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
LineReaderWithEdit::read_string_no_delim(const char*& x, 
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
LineReaderWithEdit::read_ival(const char*& x, ival_t& out) {
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
LineReaderWithEdit::read_uval(const char*& x, uval_t& out) {
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
LineReaderWithEdit::read_fval(const char*& x, fval_t& out) {
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
LineReaderWithEdit::read_dval(const char*& x, dval_t& out) {
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
LineReaderWithEdit::read_stimestamp(const char*& x, STimestamp& out, char sep) {
  return out.set(x, sep);
}

int
LineReaderWithEdit::split_line(const char aSep, const bool aStrip) {
  _splits.clear();
  const char* e = end();
  const char* x = line();
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // aSeps, blanks must have been striped
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    if('\"' == *x) {
      ++x; // skip '\"'
      lBegin = x;
      while('\"' != *x && x < e && '\0' != *x) ++x;
      // std::cerr << "*x = " << *x << std::endl;
      if('\"' == *x) {
        char* lEnd = (char*) x;
        // std::cerr << "*lEnd = " << *lEnd << std::endl;
        ++x; // skip '\"'
        if(aStrip) {
          while(lEnd >= lBegin && isspace(*(lEnd - 1))) {
            --lEnd;
            std::cerr << "*lEnd in loop: " << *lEnd << std::endl;
          }
          *lEnd = '\0';
          _splits.push_back((char*) lBegin);
          // std::cerr << "read: " << lBegin << std::endl;
        }
      } else {
        std::cerr << "unmatched \"." << std::endl;
        char* lEnd = (char*) x;
        *lEnd = '\0';
        _splits.push_back((char*) lBegin);
      }
    } else {
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
    }
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

