#include "simplecharcontainer.hh"
#include "assert.h"

SimpleCharContainer::SimpleCharContainer(uint aChunkSize) 
                    : _chunkSize(aChunkSize),
                      _currentBegin(0),
                      _currentEnd(0),
                      _chunks() {
}

SimpleCharContainer::~SimpleCharContainer() {
  for(chunk_vt::iterator iter = _chunks.begin(); iter != _chunks.end(); ++iter) {
    delete[] (*iter);
  }
}

const char*
SimpleCharContainer::insert(const char* aBegin, const char* aEnd) {
  const long int len = aEnd - aBegin; // for additional appended '\0';
  const long int lenX = len + 1;
  const long int cap = _currentEnd - _currentBegin;
  assert(lenX < (long int) _chunkSize);
  if(cap < lenX) {
    char* lChunk = new char[_chunkSize];
    _currentBegin = lChunk;
    _currentEnd   = lChunk + _chunkSize;
    _chunks.push_back(lChunk);
  }
  memcpy(_currentBegin, aBegin, len);
  _currentBegin[len] = '\0';
  const char* lRes = _currentBegin;
  _currentBegin += lenX;
  return lRes;
}  
  
  
