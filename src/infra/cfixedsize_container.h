#ifndef CFIXEDSIZE_CONTAINER_HH
#define CFIXEDSIZE_CONTAINER_HH

#include <inttypes.h>
#include <assert.h>

/*
 *  cfsc_fixedsize_container
 *  is an insert only container for fixed size elements
 *  all strings will be null-terminated by the container
 *  cfsc_new allocates a new container of size (1 << aLog2ChunkSize) bytes
 *  cfsc_delete free's the container
 *  cfsc_insert insert an element into the container
 */

struct cfsc_fixedsize_container_t;

struct cfsc_fixedsize_container_t* 
cfsc_new(const uint32_t aElementSize,
         const uint32_t aLog2ChunkSize);

void         cfsc_delete(struct cfsc_fixedsize_container_t*);
const char*  cfsc_insert(struct cfsc_fixedsize_container_t*, const char* aItem); // aItem maybe 0

void cfsc_print(struct cfsc_fixedsize_container_t*);

#endif

