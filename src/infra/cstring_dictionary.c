#include "cstring_dictionary.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cstring_container.h"
#include "cfixedsize_container.h"

/*
 *  csd_entry_t
 *  is an entry in the hash table directory as well as an element in the collision chain
 */

struct csd_entry_t {
  struct csd_cstring_t _csd_cstring;
  struct csd_entry_t*  _next;
};

struct csd_cstring_dictionary_t {
  struct csd_entry_t*                 _directory;  // hash directory
  struct cfsc_fixedsize_container_t*  _rsv_entry;  // container for collision chain entries
  struct csc_cstring_container*       _rsv_string; // container for strings
         csd_hashfun_t*               _hashfun;    // hash function
         uint32_t                     _htsize;     // hash table directory size
};





struct csd_cstring_dictionary_t* 
csd_new(const uint32_t aHtSize,
        const uint32_t aLog2ChunkSizeCollisionChainContainer,
        const uint32_t aLog2ChunkSizeStringContainer,
        csd_hashfun_t* aHashFun) {
  assert(5 <= aLog2ChunkSizeCollisionChainContainer);
  assert(5 <= aLog2ChunkSizeStringContainer);

  struct csd_cstring_dictionary_t* x = (struct csd_cstring_dictionary_t*) malloc(sizeof(struct csd_cstring_dictionary_t));
  if(0 == x) { fprintf(stderr, "malloc failed.\n"); return 0; }

  const size_t lDirSize = (size_t) aHtSize * sizeof(struct csd_entry_t);
  x->_directory  = (struct csd_entry_t*) malloc(lDirSize);
  if(0 == x->_directory) { printf("can't allocate directory.\n"); return 0; }
  memset(x->_directory, 0, lDirSize);

  x->_rsv_entry  = cfsc_new(sizeof(struct csd_entry_t), aLog2ChunkSizeCollisionChainContainer);
  if(0 == x->_rsv_entry) { printf("can't allocate entry reservoir.\n"); return 0; }

  x->_rsv_string = csc_new(aLog2ChunkSizeStringContainer);
  if(0 == x->_rsv_entry) { printf("can't allocate string reservoir.\n"); return 0; }

  x->_hashfun    = aHashFun;
  if(0 == x->_hashfun) {
    // printf("INFO: taking default hash function.\n");
    x->_hashfun = &cstring_hash_larson;
  }
  x->_htsize = aHtSize;
  return x;
}


void   
csd_delete(struct csd_cstring_dictionary_t* x) {
  if(0 == x) { printf("Warning: csd_delete has been given a nullptr.\n"); return; }
  free(x->_directory);
  cfsc_delete(x->_rsv_entry);
  csc_delete(x->_rsv_string);
}

struct csd_entry_t*
csd_find_entry(struct csd_entry_t* aEntry, const uint32_t aHashVal, const char* aBegin, const char* aEnd) {
  struct csd_entry_t* lEntry = aEntry;
  int lSame = 0;
  if(0 == lEntry->_csd_cstring._cstring) {
    assert(0 == lEntry->_next);
    return 0;
  }
  while(0 != lEntry) {
    lSame = 0;
    if(aHashVal == lEntry->_csd_cstring._hash_value) {
      const char* s1 = lEntry->_csd_cstring._cstring;
      lSame = 1;
      for(const char* s2 = aBegin; s2 < aEnd; ++s1, ++s2) {
        if(*s1 != *s2)  { lSame = 0; break; }
        if('\0' == *s1) { lSame = 0; break; }
      }
    }
    if(lSame) {
      break;
    }
    lEntry = lEntry->_next;
  }
  return lEntry;
}

const struct csd_cstring_t*  
csd_insert(struct csd_cstring_dictionary_t* x, const char* aBegin, const char* aEnd) {
  assert(aBegin <= aEnd);
  const uint32_t h = (x->_hashfun)(aBegin, aEnd); // hash value
  const uint32_t k = (h % (x->_htsize)); // director entry
  struct csd_entry_t* lEntry = x->_directory + k;
  if(0 == lEntry->_csd_cstring._cstring) {
    // directory entry is empty
    const char* s = csc_insert(x->_rsv_string, aBegin, aEnd); // stored string
    lEntry->_csd_cstring._cstring = s;
    lEntry->_csd_cstring._length  = (aEnd - aBegin);
    lEntry->_csd_cstring._hash_value = h;
    return &(lEntry->_csd_cstring);
  }
  // otherwise, see, whether we find the string
  struct csd_entry_t* lCCEntry = csd_find_entry(lEntry, h, aBegin, aEnd);
  if(0 != lCCEntry) {
    return &(lCCEntry->_csd_cstring);
  }

  // not found, need to add collision chain entry
  lCCEntry = (struct csd_entry_t*) cfsc_insert(x->_rsv_entry, 0);
  if(0 == lCCEntry) {
    printf("allocating new collision chain entry failed.");
  }
  lCCEntry->_next = lEntry->_next; 
  lCCEntry->_csd_cstring._cstring    = csc_insert(x->_rsv_string, aBegin, aEnd);
  lCCEntry->_csd_cstring._length     = (aEnd - aBegin);
  lCCEntry->_csd_cstring._hash_value = h;
  lEntry->_next = lCCEntry;

  return &(lCCEntry->_csd_cstring);
}

const struct csd_cstring_t*  
csd_insert_len(struct csd_cstring_dictionary_t* aCd, const char* aBegin, const uint32_t aLen) {
  return csd_insert(aCd, aBegin, aBegin + aLen);
}

const struct csd_cstring_t*  
csd_insert_cstr(struct csd_cstring_dictionary_t* aCd, const char* aBegin) {
  return csd_insert(aCd, aBegin, aBegin + strlen(aBegin));
}

const struct csd_cstring_t*  
csd_find(struct csd_cstring_dictionary_t* x, const char* aBegin, const char* aEnd) {
  assert(aBegin <= aEnd);
  const uint32_t h = (x->_hashfun)(aBegin, aEnd); // hash value
  const uint32_t k = (h % (x->_htsize)); // director entry
  struct csd_entry_t* lEntry = x->_directory + k;
  if(0 == lEntry->_csd_cstring._cstring) {
    return 0;
  }
  lEntry = csd_find_entry(lEntry, h, aBegin, aEnd);
  if(0 == lEntry) {
    return 0;
  }
  return &(lEntry->_csd_cstring);
}

const struct csd_cstring_t*  
csd_find_len(struct csd_cstring_dictionary_t* aCd, const char* aBegin, const uint32_t aLen) {
  return csd_find(aCd, aBegin, aBegin + aLen);
}

const struct csd_cstring_t*  
csd_find_cstr(struct csd_cstring_dictionary_t* aCd, const char* aBegin) {
  return csd_find(aCd, aBegin, aBegin + strlen(aBegin));
}

void
csd_print_entry(const struct csd_entry_t* aEntry) {
  const struct csd_entry_t* e = aEntry;
  while(0 != e) {
    if(0 == e->_csd_cstring._cstring) {
      printf(" <null>");
    } else {
      printf(" e[%s(%d:0x%x)]", e->_csd_cstring._cstring,
                                e->_csd_cstring._length,
                                e->_csd_cstring._hash_value);
    }
    e = e->_next;
  }
}

void 
csd_print(struct csd_cstring_dictionary_t* aCd) {
  printf("directory:\n");
  for(uint32_t i = 0; i < aCd->_htsize; ++i) {
    printf("  de%3d:", i);
    csd_print_entry(aCd->_directory + i);
    printf("\n");
  }
}





/* default hash function */

uint32_t 
cstring_hash_larson(const char* aBegin, const char* aEnd) {
  uint32_t h = 0x0; // 0xEBAE;
  const char* s = aBegin;
  while(s < aEnd) {
    h = h * 101 + (unsigned char) *s++;
  }
  return h;
}


