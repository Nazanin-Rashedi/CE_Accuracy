#include "terminal_color.h"

static const char*
gTerminalColorForeground[kNoTerminalColor + 1] = {
  "\033[39m", // default
  "\033[30m", // black
  "\033[31m", // dark red
  "\033[32m", // dark green
  "\033[33m", // dark yellow looks like orange
  "\033[34m", // dark blue
  "\033[35m", // dark magenta
  "\033[36m", // dark cyan
  "\033[37m", // light gray
  "\033[90m", // dark gray
  "\033[91m", // red
  "\033[92m", // green
  "\033[93m", // orange looks like yellow
  "\033[94m", // blue
  "\033[95m", // magenta
  "\033[96m", // cyan
  "\033[97m", // white
  "\033[39m"  // default
};


const char*
set_terminal_color(int i) {
  if(i < kNoTerminalColor) {
    return gTerminalColorForeground[i];
  }
  return gTerminalColorForeground[kTerminalColorDefault];
}

const char*
reset_terminal_color() {
  return gTerminalColorForeground[kTerminalColorDefault];
}


