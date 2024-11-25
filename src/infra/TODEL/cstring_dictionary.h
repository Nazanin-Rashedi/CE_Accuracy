#ifndef CSTRING_DICTIONARY_HH
#define CSTRING_DICTIONARY_HH

#include <inttypes.h>

/*
 * csd_cstring_dictionary
 * is an insert-only hash-based dictionary
 * where the hash-value is materialized in struct csd_cstring_ref
 * it uses csc_cstring_container
 * the hash function is given by pointer
 */



/* the main struct for the dictionary */

struct csd_cstring_dictionary_t;

/* struct for string reference */

struct csd_cstring_t {
  const char* _cstring;
  uint32_t    _length;
  uint32_t    _hash_value;
};

/* typedef for hashfunction */

typedef uint32_t (csd_hashfun_t)(const char* aBegin, const char* aEnd);

/* functions for creation, deletion, and insertion, lookup */

struct csd_cstring_dictionary_t* 
csd_new(const uint32_t aHtSize, // hashtable size
        const uint32_t aLog2ChunkSizeCollisionChainContainer,
        const uint32_t aLog2ChunkSizeStringContainer,
        csd_hashfun_t* aHashFun);
void   csd_delete(struct csd_cstring_dictionary_t*);

const struct csd_cstring_t*  csd_insert(struct csd_cstring_dictionary_t*, const char* aBegin, const char* aEnd);
const struct csd_cstring_t*  csd_insert_len(struct csd_cstring_dictionary_t*, const char* aBegin, const uint32_t aLen);
const struct csd_cstring_t*  csd_insert_cstr(struct csd_cstring_dictionary_t*, const char* aBegin);
const struct csd_cstring_t*  csd_find(struct csd_cstring_dictionary_t*, const char* aBegin, const char* aEnd);
const struct csd_cstring_t*  csd_find_len(struct csd_cstring_dictionary_t*, const char* aBegin, const uint32_t aLen);
const struct csd_cstring_t*  csd_find_cstr(struct csd_cstring_dictionary_t*, const char* aBegin);

void csd_print(struct csd_cstring_dictionary_t*);


/* some string hash functions are provided */
uint32_t cstring_hash_larson(const char* aBegin, const char* aEnd);


#endif
