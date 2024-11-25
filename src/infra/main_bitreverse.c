#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "cmeasure.h"

#define WITH_BSWAP

#include "bit_reverse.h"

#define ARSIZE   (50L * 1024L * 1024L)

void
printbv(uint32_t x) {
  for(uint i = 0; i < 32; ++i) {
    if(x & 0x1) {
      printf("1");
    } else {
      printf("0");
    }
    x >>= 1;
  }
}

void
test0() {
  uint32_t x = 0xACAD7A8F;

  uint32_t y = reverse32_basic(x);
  uint32_t z = reverse32_bswap(x);

  printf("x = "); printbv(x); printf("\n"); 
  printf("y = "); printbv(y); printf("\n"); 
  printf("z = "); printbv(z); printf("\n"); 

}


void
test1() {
  uint32_t* lArr = (uint32_t*) malloc(ARSIZE * sizeof(uint32_t));

  for(uint32_t i = 0; i < ARSIZE; ++i) {
    lArr[i] = rand();
  }


  struct cmeasure_t lMeasureBasic;
  cmeasure_start(&lMeasureBasic); 
  for(uint32_t i = 0; i < ARSIZE; ++i) {
    lArr[i] = reverse32_basic(lArr[i]);
  }
  cmeasure_stop(&lMeasureBasic); 

  cmeasure_print(&lMeasureBasic, "total time basic %10.3f");
  printf("\n");

  struct cmeasure_t lMeasureBSwap;
  cmeasure_start(&lMeasureBSwap);
  for(uint32_t i = 0; i < ARSIZE; ++i) {
    lArr[i] = reverse32_bswap(lArr[i]);
  }
  cmeasure_stop(&lMeasureBSwap);

  cmeasure_print(&lMeasureBSwap, "total time bswap %10.3f");
  printf("\n");


  printf("per elem time basic: %10.3f\n", (double) cmeasure_total_ns(&lMeasureBasic) / (double) ARSIZE);
  printf("per elem time bswap: %10.3f\n", (double) cmeasure_total_ns(&lMeasureBSwap) / (double) ARSIZE);
}
 

int
main() {
  test0();
  test1();
  return 0;
}

