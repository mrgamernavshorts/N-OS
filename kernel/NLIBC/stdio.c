#ifndef NLIBC_STDIO_C
#define NLIBC_STDIO_C

#include "stdio.h"
#include "stdint.h"
#include "../nkernel/io.c"
#include "string.c"
#include "stdlib.c"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

vuint16_t CurrVGARow = 0;
vuint16_t CurrVGAColumn = 0;
vuint8_t SSCANF_COMBINATION_HANDELED = 0;

void putc(char c, uint8_t fg, uint8_t bg) {
  if (c == '\n') {
    while(CurrVGAColumn >= VGA_WIDTH){
      CurrVGARow++;
      CurrVGAColumn -= VGA_WIDTH;
    }
    CurrVGARow++;
    CurrVGAColumn = 0;
  } else if(c == '\0') return;
  else if(c == '\b'){
    if(CurrVGAColumn == 0){
      return;
    } else {
      CurrVGAColumn--;
      io_write_char_vga(' ', fg, bg, (CurrVGARow * VGA_WIDTH) + CurrVGAColumn);
    }
  } else {
    io_write_char_vga(c, fg, bg, (CurrVGARow * VGA_WIDTH) + CurrVGAColumn);
    CurrVGAColumn++;
  }
}

void puts(char *str) {
  uint8_t fg;
  char buff[5];
  uint8_t bg = 0;
  uint8_t j = 0;
  for (vuint32_t i = 0; str[i] != '\0'; i++) {
    /*
    if(str[i] == '\033'){
      i++;
      if(str[i] == '['){
        i++;
        memcpycUntil(buff, str+i, 'm');
        itoa()
        }
      }
    }
    */
    putc(str[i], VGA_LIGHT_GRAY, VGA_BLACK);
  }
}

void sscanf(char *buff){
  uint8_t code;
  vuint32_t buffIndex = 0;
  volatile char c;
  vuint8_t CtrlPressed = 0;
  for(;;){
    SSCANF_COMBINATION_HANDELED = 0;
    if(!keyboardPoll(&code)){

      if(code == 0x1D){
        CtrlPressed = 1;
      } else if(code == 0x9D){ // Left control released.
          CtrlPressed = 0;
      }

      c = scancodeToChar(code);
      if(c == '\0') continue;
      else if(c == '\n'){
        putc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
        buff[buffIndex] = '\0';
        break;
      } else if(c == '\b'){
        if(buffIndex <= 0){
          buffIndex = 0;
          continue;
        }
        buffIndex--;
        buff[buffIndex] = '\0';
        putc(c, VGA_LIGHT_GRAY, VGA_BLACK);
      } else if(c == '\t'){
        for(vuint32_t i = 0; i < 2; i++){
          buff[buffIndex] = ' ';
          buffIndex++;
          putc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
        }
      } else if(CtrlPressed && c == 'l'){
        clear_vga();
        SSCANF_COMBINATION_HANDELED = 1;
        break;
      }
      else {
        buff[buffIndex] = c;
        buffIndex++;
        putc(c, VGA_LIGHT_GRAY, VGA_BLACK);
      }
    }
  }
}

void clear_vga(){
  for(uint32_t i = 0; i != VGA_WIDTH * VGA_HEIGHT; i++){
    io_write_char_vga(' ', VGA_LIGHT_GRAY, VGA_BLACK, i);
  }
  CurrVGAColumn = 0;
  CurrVGARow = 0;
}

#endif
