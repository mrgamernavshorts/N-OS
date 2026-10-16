#ifndef NLIBC_STDIO_C
#define NLIBC_STDIO_C
// clang-format off

#include "stdio.h"
#include "stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

vuint16_t CurrVGARow = 0;
vuint16_t CurrVGAColumn = 0;
vuint16_t *vga = (uint16_t *)0xB8000;

void putc(char c) {
  if (c == '\n') {
    CurrVGARow++;
    CurrVGAColumn = 0;
  } else {
    vga[(CurrVGARow * VGA_WIDTH) + CurrVGAColumn] = c | (0x07 << 8);
    CurrVGAColumn++;
  }
}

void puts(char *str) {
  for (vuint32_t i = 0; str[i] != '\0'; i++) {
    putc(str[i]);
  }
}

void clear_vga(){
  for(uint32_t i = 0; i != VGA_WIDTH * VGA_HEIGHT; i++){
    vga[i] = ' ' | (0x07 << 8);
  }

  CurrVGAColumn = 0;
  CurrVGARow = 0;
}

#endif
