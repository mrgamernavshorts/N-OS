#ifndef NLIBC_STDIO
#define NLIBC_STDIO
// clang-format off
#include "stdint.h"

void putc(char c, uint8_t fg, uint8_t bg); // Prints a char onto the screen thorugh VGA MMIO with a VGA fg and bg color.
void puts(char *str); // Prints a string onto the screen through VGA MMIO.
void sscanf(char *buff); // Takes input from the user and stores it into 'buff', and null-terminates it.
void clear_vga(); // Clears the VGA MMIO.

#endif
