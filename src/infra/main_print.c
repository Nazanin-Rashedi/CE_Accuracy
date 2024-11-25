#include <stdio.h>

/*
on apollo compile with
armclang --target=armv7-arm-none-eabi -static -mfloat-abi=hard -c  main_print.c
then on bilbo link with
gcc main_print.o 
*/

void
test0() {
  printf("%s!\n","Hallo");
}

int
main() {
  test0();
  return 0;
}

