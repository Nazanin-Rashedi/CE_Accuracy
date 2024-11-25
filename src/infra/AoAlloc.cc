#include "AoAlloc.hh"


AoAlloc::AoAlloc(const size_t aChunkSize, bool aSetZero)
        : _chunk_curr_begin(0), _chunk_curr_end(0), 
          _chunks_used(), _chunks_free(), 
          _chunk_size(aChunkSize), _chunk_init(aSetZero) {
}

AoAlloc::~AoAlloc() {
  dealloc_all();
}


byte_t*
AoAlloc::allocate(const size_t aSize) {
  assert(aSize < _chunk_size);
  byte_t* x = _chunk_curr_begin + aSize;
  if(x >= _chunk_curr_end) {
     allocate_new_chunk();
  }
  x = _chunk_curr_begin;
  _chunk_curr_begin += aSize;
  return x;
}

void
AoAlloc::allocate_new_chunk() {
  byte_t* x = 0;
  if(0 < _chunks_free.size()) {
    x = _chunks_free[_chunks_free.size() - 1];
    _chunks_free.resize(_chunks_free.size() - 1);
  } else {
    x = (byte_t*) malloc(_chunk_size);
  }
  assert(0 != x);
  _chunk_curr_begin = x;
  _chunk_curr_end   = x + _chunk_size;
  _chunks_used.push_back(x);
  if(_chunk_init) {
    memset(x, 0, _chunk_size);
  }
}

void
AoAlloc::free_all() {
  for(uint i = 0; i < _chunks_used.size(); ++i) {
    _chunks_free.push_back(_chunks_used[i]);
  }
  _chunks_used.resize(0);
}


void
AoAlloc::dealloc_all() {
  for(uint i = 0; i < _chunks_used.size(); ++i) {
    free(_chunks_used[i]);
  }
  _chunks_used.resize(0);
  for(uint i = 0; i < _chunks_free.size(); ++i) {
    free((void*) _chunks_free[i]);
  }
  _chunks_free.resize(0);
}


