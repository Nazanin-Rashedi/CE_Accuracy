#ifndef TERMINAL_COLOR_H
#define TERMINAL_COLOR_H


enum terminal_color_et {
  kTerminalColorDefault     =  0,
  kTerminalColorBlack       =  1,
  kTerminalColorDarkRed     =  2,
  kTerminalColorDarkGreen   =  3,
  kTerminalColorOrange      =  4, 
  kTerminalColorDarkBlue    =  5,
  kTerminalColorDarkMagenta =  6,
  kTerminalColorDarkCyan    =  7,
  kTerminalColorLightGray   =  8,
  kTerminalColorDarkGray    =  9,
  kTerminalColorRed         = 10,
  kTerminalColorGreen       = 11,
  kTerminalColorYellow      = 12, 
  kTerminalColorBlue        = 13,
  kTerminalColorMagenta     = 14,
  kTerminalColorCyan        = 15,
  kTerminalColorWhite       = 16,
  kNoTerminalColor          = 17,
};

const char* set_terminal_color(int);
const char* reset_terminal_color();

#endif
