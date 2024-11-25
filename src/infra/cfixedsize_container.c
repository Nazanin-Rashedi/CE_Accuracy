#include "cfixedsize_container.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PADSZ (sizeof(void*) - sizeof(uint32_t))

/*
 * cfsc_chunk is a chunk of memory of size 2^log2chunksize
 * _content contains the strings
 * _end points to the end of the chunk
 * _curr points to the first non-used chat in _content
 */


struct cfsc_chunk {
  struct cfsc_chunk* _next;       // pointer to next chunk
  uint32_t           _no_elem;    // number of element it contains
  char               _pad[PADSZ]; // simple padding
  char               _content;    // start of elements
};


struct cfsc_fixedsize_container_t {
  struct cfsc_chunk*  _first;         // pointer to first chunk;
  uint32_t            _element_size;  // size of a single element stored here
  uint32_t            _log2chunksize; // log_2(chunksize)
  uint32_t            _capacity;      // capacity in number of elements
};


/* capacity in number of elements */
uint32_t
cfsc_capacity(struct cfsc_fixedsize_container_t* aC) {
  const uint32_t lChunkSize  = (1 << aC->_log2chunksize);
  const uint32_t lNoElements = ((lChunkSize - 2 * sizeof(void*)) / aC->_element_size);
  assert(0 < lNoElements);
  return lNoElements;
}

struct cfsc_fixedsize_container_t*
cfsc_new(const uint32_t aElementSize,
         const uint32_t aLog2ChunkSize) {
  assert(5 <= aLog2ChunkSize);
  struct cfsc_fixedsize_container_t* lC = (struct cfsc_fixedsize_container_t*) malloc(sizeof(struct cfsc_fixedsize_container_t));
  lC->_first = 0;
  if(0 == lC) { fprintf(stderr, "malloc failed.\n"); return 0; }
  lC->_element_size  = aElementSize;
  lC->_log2chunksize = aLog2ChunkSize;
  lC->_capacity      = cfsc_capacity(lC);
  return lC;
}

void
cfsc_delete(struct cfsc_fixedsize_container_t* aC) {
  struct cfsc_chunk* c = aC->_first;
  struct cfsc_chunk* z = 0;
  while(0 != c) {
    z = c->_next;
    free(c);
    c = z;
  }
}

const char*
cfsc_insert(struct cfsc_fixedsize_container_t* aC, const char* aItem) {
  const uint32_t lChunkSize = (1 << aC->_log2chunksize);
  struct cfsc_chunk* c = 0;
  if((0 == aC->_first) || (aC->_first->_no_elem >= aC->_capacity)) {
    struct cfsc_chunk* c = (struct cfsc_chunk*) malloc(lChunkSize);
    if(0 == c) { fprintf(stderr, "malloc failed.\n"); return 0; }
    c->_next    = aC->_first;
    c->_no_elem = 0;
    aC->_first = c;
  }
  c = aC->_first;
  assert(c);
  char* lEntry = &(c->_content) + c->_no_elem * (aC->_element_size);
  if(0 != aItem) {
    memcpy(lEntry, aItem, aC->_element_size);
  }
  ++(c->_no_elem);
  return lEntry;
}


void 
cfsc_print(struct cfsc_fixedsize_container_t* x) {
  printf("container(%d):\n", x->_log2chunksize);
  struct cfsc_chunk* c = x->_first;
  int lChunkNo = 0;
  while(c) {
    printf("  chunk %2d:\n", lChunkNo);
    const char* s = (char*) &(c->_content);
    for(uint32_t i = 0; i < c->_no_elem; ++i) {
      printf("    item %2d: ", i);
      for(uint32_t j = 0; j < x->_element_size; ++j, ++s) {
        if(isprint(*s)) {
          printf("%c", *s);
        } else {
          printf("[0x%x]", *s);
        }
      }
      printf("\n");
    }
    c = c->_next;
    ++lChunkNo;
  }
}

