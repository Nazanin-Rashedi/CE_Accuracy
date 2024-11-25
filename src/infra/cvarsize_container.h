#ifndef CVSC_VARSIZE_CONTAINER_HH
#define CVSC_VARSIZE_CONTAINER_HH

#include <inttypes.h>
#include <assert.h>

/*
 *  cvsc_varsize_container
 *  is an insert only container for variable sized objects
 *  it stores them in chunks of memory 
 *  cvsc_new allocates a new container with chunks of size (1 << aLog2ChunkSize)
 *  cvsc_delete free's the container's chunks
 *  cvsc_insert inserts an object into the container
 */

struct cvsc_varsize_container;

struct cvsc_varsize_container* 
cvsc_new(const uint32_t aLog2ChunkSize);

void        cvsc_delete(struct cvsc_varsize_container*);
const char* cvsc_insert(struct cvsc_varsize_container*, const char* aBegin, const uint32_t aLen); 

/* just for debugging purposes */

void cvsc_print(struct cvsc_varsize_container*);

#endif

