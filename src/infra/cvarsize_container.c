#include "cvarsize_container.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * cvsc_chunk is a chunk of memory of size 2^log2chunksize
 * _content contains the strings
 * _end points to the end of the chunk
 * _curr points to the first non-used chat in _content
 */


struct cvsc_chunk {
  struct cvsc_chunk* _next;
  char*              _end;
  char*              _curr;
  char               _content[8];
};


struct cvsc_varsize_container {
  struct cvsc_chunk* _first;
  uint32_t           _log2chunksize;
};


struct cvsc_varsize_container*
cvsc_new(const uint32_t aLog2ChunkSize) {
  assert(5 <= aLog2ChunkSize);
  struct cvsc_varsize_container* x = (struct cvsc_varsize_container*) malloc(sizeof(struct cvsc_varsize_container));
  if(0 == x) { fprintf(stderr, "malloc failed.\n"); return 0; }
  x->_log2chunksize = aLog2ChunkSize;
  x->_first = 0;
  return x;
}

void
cvsc_delete(struct cvsc_varsize_container* x) {
  struct cvsc_chunk* c = x->_first;
  struct cvsc_chunk* z = 0;
  while(0 != c) {
    z = c->_next;
    free(c);
    c = z;
  }
}

const char*
cvsc_insert(struct cvsc_varsize_container* x, const char* aBegin, const uint32_t aLen) {
  assert(0 < aLen);
  const uint32_t lChunkSize = (1 << x->_log2chunksize);
  struct cvsc_chunk* c = 0;
  if((0 == x->_first) || ((x->_first->_end - x->_first->_curr) < aLen)) {
    struct cvsc_chunk* c = (struct cvsc_chunk*) malloc(lChunkSize);
    if(0 == c) { fprintf(stderr, "malloc failed.\n"); return 0; }
    c->_next = x->_first;
    c->_curr = (char*) &(c->_content);
    c->_end  = ((char*) c) + lChunkSize;
    x->_first = c;
  }
  c = x->_first;
  assert(c);
  int lFree = (c->_end - c->_curr);
  // printf("strlen = %d, free = %d, container = 0x%lX, curr = 0x%lX, end = 0x%lX\n", 
  //        aLen, lFree, (long unsigned int) c, (long unsigned int) c->_curr, (long unsigned int) c->_end);
  if(aLen > lFree) {
    printf("len = %u, free = %d\n", aLen, lFree);
  }
  assert(aLen <= lFree); // that should be the case if container is large enough for string

  const char* lPtr = c->_curr;
  if(0 != aLen) {
    memcpy(c->_curr, aBegin, aLen);
  }
  c->_curr += aLen;
  return lPtr;
}

void
cvsc_print(struct cvsc_varsize_container* x) {
  printf("cvarsize_container(%d):\n", x->_log2chunksize);
  struct cvsc_chunk* c = x->_first;
  int lChunkNo = 0;
  while(c) {
    printf("  chunk %2d [rest%2ld]: ", lChunkNo, c->_end - c->_curr);
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


