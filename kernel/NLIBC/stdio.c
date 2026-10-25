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
  char mode = '7'; // HAHA light gray go BRRRRR.
  for (vuint32_t i = 0; str[i] != '\0'; i++) {
    if(str[i] == '\033'){
      i += 2;
      if(str[i] == '3'){
        i++;
        switch(str[i]){
          case '0': mode = '0'; break;
          case '1': mode = '1'; break;
          case '2': mode = '2'; break;
          case '3': mode = '3'; break;
          case '4': mode = '4'; break;
          case '5': mode = '5'; break;
          case '6': mode = '6'; break;
          case '7': mode = '7'; break;
          case '8': mode = '8'; break;
          case '9': mode = '9'; break;
        }
        i += 2;
      }
    }
    
    switch(mode){
      case '0': putc(str[i], VGA_BLACK, VGA_BLACK); break;
      case '1': putc(str[i], VGA_RED, VGA_BLACK); break;
      case '2': putc(str[i], VGA_GREEN, VGA_BLACK); break;
      case '3': putc(str[i], VGA_BROWN, VGA_BLACK); break;
      case '4': putc(str[i], VGA_BLUE, VGA_BLACK); break;
      case '5': putc(str[i], VGA_PURPLE, VGA_BLACK); break;
      case '6': putc(str[i], VGA_CYAN, VGA_BLACK); break;
      case '7': putc(str[i], VGA_LIGHT_GRAY, VGA_BLACK); break;
      case '8': putc(str[i], VGA_YELLOW, VGA_BLACK); break;
      case '9': putc(str[i], VGA_WHITE, VGA_BLACK); break;
    }
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
