#ifndef AO_ALLOC_HH
#define AO_ALLOC_HH

#include <stdlib.h>
#include <string.h>

#include "infra/glob_infra_standard_includes.hh"

/*
 *  Append Only Allocator
 */


class AoAlloc {
  public:
    typedef std::vector<byte_t*> byte_pvt;
  private:
    AoAlloc(const AoAlloc&) = delete;
    AoAlloc& operator=(const AoAlloc&) = delete;
  public:
    AoAlloc(const size_t aChunkSize, bool aSetZero = false);
    ~AoAlloc();
  public:
    byte_t* allocate(const size_t aSize);
    void    free_all();    // move all chunks to free chunks
    void    dealloc_all(); // deallocate chunks
  public:
    // only for testing
    inline size_t  no_chunks_used() const { return _chunks_used.size(); }
    inline byte_t* get_chunk_used(const size_t i) const { return _chunks_used[i]; }
  private:
    void allocate_new_chunk();
  private:
    byte_t*  _chunk_curr_begin;
    byte_t*  _chunk_curr_end;
    byte_pvt _chunks_used;
    byte_pvt _chunks_free;
    size_t   _chunk_size;
    bool     _chunk_init;
};

#endif
