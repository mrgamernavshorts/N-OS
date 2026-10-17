#include "NLIBC/stdint.h"
#include "NLIBC/stdio.c"
#include "NLIBC/string.c"
// clang-format off

void kernel_main(void) {
  clear_vga();
  puts("|\\\\  ||   //==\\\\    //==\\\\ \n||\\\\ || - ||  ||    \\\\ \n|| \\\\|| - ||  ||     \\\\ \n||  \\\\|   \\\\==// \\\\==// \n");
  puts("Welcome to the NEW and IMPROVED: N-OS!\n");
  puts("\nType 'help' to get started.\n");

  char cmd[1024];
  for(;;){
    puts("$ ");
    sscanf(cmd);
    if(!strcmp(cmd, "help")){
      puts("nosfetch - Prints the N-OS ASCII art.\nclear - well; it uh... clears :D\n");
    } else if(!strcmp(cmd, "nosfetch")){
      puts("|\\\\  ||   //==\\\\    //==\\\\ \n||\\\\ || - ||  ||    \\\\ \n|| \\\\|| - ||  ||     \\\\ \n||  \\\\|   \\\\==// \\\\==// \n");
    } else if(!strcmp(cmd, "clear")){
      clear_vga();
    } else {
      puts("Command '");
      puts(cmd);
      puts("' not found.\n");
    }
    putc('\n');

    memsetc(cmd, '\0', 1024);
  }

  for (;;);
}
