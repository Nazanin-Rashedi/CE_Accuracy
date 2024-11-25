#include <stdio.h>
#include "infra/terminal_color.h"

void
test0() {
  char c = 'A';
  for(int i = 0; i < kNoTerminalColor; ++i) {
    printf("|%s%c%s|", set_terminal_color(i), c, reset_terminal_color());
  }
  printf("\n");
}

int
main() {
  test0();
  return 0;
}

