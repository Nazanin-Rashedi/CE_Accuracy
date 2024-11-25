#include "cstring_container.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * csc_chunk is a chunk of memory of size 2^log2chunksize
 * _content contains the strings
 * _end     points to the end of the chunk
 * _curr    points to the first non-used chat in _content
 */


struct csc_chunk {
  struct csc_chunk* _next;
  char*             _end;
  char*             _curr;
  char              _content[8];
};


struct csc_cstring_container {
  struct csc_chunk*  _first;
  struct csc_chunk*  _free;
  uint32_t           _log2chunksize;
};


struct csc_cstring_container*
csc_new(const uint32_t aLog2ChunkSize) {
  assert(5 <= aLog2ChunkSize);
  struct csc_cstring_container* x = (struct csc_cstring_container*) malloc(sizeof(struct csc_cstring_container));
  if(0 == x) { fprintf(stderr, "malloc failed.\n"); return 0; }
  x->_log2chunksize = aLog2ChunkSize;
  x->_first = 0;
  x->_free  = 0;
  return x;
}

void
csc_delete(struct csc_cstring_container* x) {
  struct csc_chunk* c = x->_first;
  struct csc_chunk* z = 0;
  while(0 != c) {
    z = c->_next;
    free(c);
    c = z;
  }
  c = x->_free;
  while(0 != c) {
    z = c->_next;
    free(c);
    c = z;
  }
}

void
csc_clear(struct csc_cstring_container* x) {
  x->_free = x->_first;
  x->_first = 0;
  struct csc_chunk* c = x->_free;
  while(0 != c) {
    c->_curr = (char*) &(c->_content);
    c = c->_next;
  }
}

const char*
csc_insert(struct csc_cstring_container* x, const char* aBegin, const char* aEnd) {
  assert(aBegin <= aEnd);
  const uint32_t lLen = (aEnd - aBegin);
  const uint32_t lChunkSize = (1 << x->_log2chunksize);
  struct csc_chunk* c = 0;
  if((0 == x->_first) || ((x->_first->_end - x->_first->_curr) < (lLen + 1))) {
    if(0 == x->_free) {
      c = (struct csc_chunk*) malloc(lChunkSize);
      // printf("malloc\n");
      if(0 == c) { fprintf(stderr, "malloc failed.\n"); return 0; }
    } else {
      c = x->_free;
      x->_free = c->_next;
    }
    c->_next = x->_first;
    c->_curr = (char*) &(c->_content);
    c->_end  = ((char*) c) + lChunkSize;
    x->_first = c;
  }
  c = x->_first;
  assert(c);
  int lFree = (c->_end - c->_curr);
  // printf("strlen = %d, free = %d, container = 0x%lX, curr = 0x%lX, end = 0x%lX\n", 
  //        lLen, lFree, (long unsigned int) c, (long unsigned int) c->_curr, (long unsigned int) c->_end);
  assert(lLen < lFree); // that should be the case if container is large enough for string

  const char* lPtr = c->_curr;
  if(0 != lLen) {
    memcpy(c->_curr, aBegin, lLen);
  }
  *(c->_curr + lLen) = '\0';
  c->_curr += lLen + 1;
  return lPtr;
}


const char*
csc_insert_len(struct csc_cstring_container* x, const char* aBegin, const uint32_t aLen) {
  return csc_insert(x, aBegin, aBegin + aLen);
}

const char*
csc_insert_cstr(struct csc_cstring_container* x, const char* aBegin) {
  return csc_insert(x, aBegin, aBegin + strlen(aBegin));
}

void 
csc_print(struct csc_cstring_container* x) {
  printf("container(%d):\n", x->_log2chunksize);
  struct csc_chunk* c = x->_first;
  int lChunkNo = 0;
  while(c) {
    printf("  chunk %2d: ", lChunkNo);
    for(char* s = c->_content; s < c->_curr; ++s) {
      if(isprint(*s)) {
        printf("%c", *s);
      } else {
        printf("'0x%x'", *s);
      }
    }
    printf("\n");
    c = c->_next;
    ++lChunkNo;
  }
}

