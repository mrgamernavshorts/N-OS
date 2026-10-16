#ifndef NLIBC_STDIO
#define NLIBC_STDIO

void putc(char c);    // Prints a char onto the screen thorugh VGA MMIO.
void puts(char *str); // Prints a string onto the screen through VGA MMIO.
void clear_vga();     // Clears the VGA MMIO.

#endif
