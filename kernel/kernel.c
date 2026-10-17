#include "NLIBC/stdint.h"
#include "NLIBC/stdio.c"
#include "keyboard.c"
// clang-format off

void kernel_main(void) {
  clear_vga();
  puts("|\\\\  ||   //==\\\\    //==\\\\ \n||\\\\ || - ||  ||    \\\\ \n|| \\\\|| - ||  ||     \\\\ \n||  \\\\|   \\\\==// \\\\==// \n");
  puts("Welcome to the NEW and IMPROVED: N-OS!\n");

  uint8_t code;
  volatile char buff;
  for(;;){
  
    if(!keyboardPoll(&code)){
      buff = scancodeToChar(code);
      putc(buff);
    }
  }

  for (;;);
}
