#ifndef CSTRING_CONTAINER_HH
#define CSTRING_CONTAINER_HH

#include <inttypes.h>
#include <assert.h>

/*
 *  csc_cstring_container
 *  is an insert only string container
 *  it stores strings in chunks of memory 
 *  all strings will be null-terminated by the container
 *  csc_new allocates a new container of size (1 << aLog2ChunkSize)
 *  csc_delete frees  the container
 *  csc_clear  clears the container (clears contents, but keeps memory)
 *  csc_insert_xxx insert a string into the container
 */

struct csc_cstring_container;

struct csc_cstring_container* 
csc_new(const uint32_t aLog2ChunkSize);

void        csc_delete(struct csc_cstring_container*);
void        csc_clear(struct csc_cstring_container*);
const char* csc_insert(struct csc_cstring_container*, const char* aBegin, const char* aEnd);
const char* csc_insert_len(struct csc_cstring_container*, const char* aBegin, const uint32_t aLen); 
const char* csc_insert_cstr(struct csc_cstring_container*, const char* aBegin);

void  csc_print(struct csc_cstring_container*);

#endif

