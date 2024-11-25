#include <stdio.h>
#include <string.h>
#include "cstring_container.h"
#include "cfixedsize_container.h"
#include "cstring_dictionary.h"
#include "cvarsize_container.h"

/*
 * compile with
 *  g++ main_csc.c cvarsize_container.c cfixedsize_container.c cstring_container.c cstring_dictionary.c
 */

void
test0() {
  printf("=== TEST CSTRING CONTAINER\n");
  const char* lPtr = 0;

  struct csc_cstring_container* lContainer = csc_new(6);
  csc_print(lContainer);

  for(int i = 0; i < 2; ++i) {
    csc_clear(lContainer);
    lPtr = csc_insert_cstr(lContainer, "bla");
    if(0 == lPtr) { printf("bad 1\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "blubs");
    if(0 == lPtr) { printf("bad 2\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "babelu");
    if(0 == lPtr) { printf("bad 2\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "xxxxxxxxxx");
    if(0 == lPtr) { printf("bad 3\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "yyyyyyyyyy");
    if(0 == lPtr) { printf("bad 4\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "zzzzzzzzzz");
    if(0 == lPtr) { printf("bad 5\n"); }
    csc_print(lContainer);

    lPtr = csc_insert_cstr(lContainer, "ich bin der letzte string.");
    if(0 == lPtr) { printf("bad 5\n"); }
    csc_print(lContainer);
  }
  csc_delete(lContainer);
}

void
test1() {
  printf("=== TEST CFIXEDSIZE CONTAINER\n");
  const char* lElem = 0;
  struct cfsc_fixedsize_container_t* lContainer = cfsc_new(3, 5);
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "bla");
  if(lElem) { printf("bad 1\n"); }
  assert(0 == strncmp("bla", lElem, 3));

  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "BLU");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "xxx");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "YYY");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "ZZZ");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "AAA*");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);

  lElem = cfsc_insert(lContainer, "BBB*");
  if(0 == lElem) { printf("bad 1\n"); }
  cfsc_print(lContainer);


  cfsc_delete(lContainer);
}

void
test2() {
  printf("=== TEST CSTRING DICTIONARY\n");
  const struct csd_cstring_t* lElem;

  struct csd_cstring_dictionary_t* lDic = csd_new(5, 8, 8, (csd_hashfun_t*) 0);
  if(0 == lDic) { printf("bad new for cstring dictionary.\n"); }
  csd_print(lDic);
 
  lElem = csd_insert_cstr(lDic, "bla");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "blubs");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "BioBlubs");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "Horch");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "xxxxx");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "yyyyy");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "zzzz");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "ZSDSSS");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);
  
  lElem = csd_insert_cstr(lDic, "ksdlsfjsldk");
  if(0 == lElem) { printf("bad insert.\n"); } 
  csd_print(lDic);

  lElem = csd_find_cstr(lDic, "blubs");
  if(0 == lElem) {
    printf("blubs not found.\n");
  } else {
    printf("blubs found: %s\n", lElem->_cstring);
  }
 
  lElem = csd_find_cstr(lDic, "xxxx");
  if(0 == lElem) {
    printf("not found: xxxx\n");
  } else {
    printf("found: %s\n", lElem->_cstring);
  }

  lElem = csd_find_cstr(lDic, "xxxxx");
  if(0 == lElem) {
    printf("not found: xxxxx\n");
  } else {
    printf("found: %s\n", lElem->_cstring);
  }

  lElem = csd_find_cstr(lDic, "xxxxxx");
  if(0 == lElem) {
    printf("not found: xxxxxx\n");
  } else {
    printf("found: %s\n", lElem->_cstring);
  }
 
  csd_delete(lDic);
}

void
test3(int aLog2ChunkSize) {
  printf("=== TEST CVARSIZE CONTAINER [log_2(chunk_size) = %d]\n", aLog2ChunkSize);
  struct cvsc_varsize_container* x = cvsc_new(aLog2ChunkSize);
  cvsc_insert(x, "aaa",        3);
  cvsc_insert(x, "bb",         2);
  cvsc_insert(x, "ccccc",      5);
  cvsc_insert(x, "dddd",       4);
  cvsc_insert(x, "eeeeeeee",   8);
  cvsc_insert(x, "fffffff",    7);
  cvsc_insert(x, "gggggg",     6);
  cvsc_insert(x, "hhhhh",      5);
  cvsc_insert(x, "iiii",       4);
  cvsc_insert(x, "jjj",        3);
  cvsc_insert(x, "kk",         2);
  cvsc_insert(x, "Z",          1);
  cvsc_print(x);
  cvsc_delete(x);
}


int
main() {
  test0();  // test cstring container   (csc_*)
  test1();  // test cfixedsizecontainer (cfsc_*)
  test2();  // test cstringdictionary   (csd_*)
  test3(5); // test cvarsizecontainer   (cvsc_*)
  test3(6); // test cvarsizecontainer   (cvsc_*)
  return 0;
}

