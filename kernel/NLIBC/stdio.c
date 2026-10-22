#ifndef NLIBC_STDIO_C
#define NLIBC_STDIO_C

#include "stdio.h"
#include "stdint.h"
#include "../nkernel/io.c"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

vuint16_t CurrVGARow = 0;
vuint16_t CurrVGAColumn = 0;

void putc(char c) {
  if (c == '\n') {
    CurrVGARow++;
    CurrVGAColumn = 0;
  } else if(c == '\0') return;
  else if(c == '\b'){
    if(CurrVGAColumn == 0){
      return;
    } else {
      CurrVGAColumn--;
      io_write_char_vga(' ', (CurrVGARow * VGA_WIDTH) + CurrVGAColumn);
    }
  } else {
    io_write_char_vga(c, (CurrVGARow * VGA_WIDTH) + CurrVGAColumn);
    CurrVGAColumn++;
  }
}

void puts(char *str) {
  for (vuint32_t i = 0; str[i] != '\0'; i++) {
    putc(str[i]);
  }
}

void sscanf(char *buff){
  uint8_t code;
  vuint32_t buffIndex = 0;
  volatile char c;
  for(;;){
    if(!keyboardPoll(&code)){
      c = scancodeToChar(code);
      if(c == '\0') continue;
      else if(c == '\n'){
        putc('\n');
        buff[buffIndex] = '\0';
        break;
      } else if(c == '\b'){
        if(buffIndex <= 0){
          buffIndex = 0;
          continue;
        }
        buffIndex--;
        buff[buffIndex] = '\0';
        putc(c);
      } else if(c == '\t'){
        for(vuint32_t i = 0; i < 2; i++){
          buff[buffIndex] = ' ';
          buffIndex++;
          putc(' ');
        }
      }
      else {
        buff[buffIndex] = c;
        buffIndex++;
        putc(c);
      }
    }
  }
}

void clear_vga(){
  for(uint32_t i = 0; i != VGA_WIDTH * VGA_HEIGHT; i++){
    io_write_char_vga(' ', i);
  }
  CurrVGAColumn = 0;
  CurrVGARow = 0;
}

#endif
