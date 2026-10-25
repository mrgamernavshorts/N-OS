#include "NLIBC/stdint.h"
#include "NLIBC/stdio.c"
#include "NLIBC/stdlib.c"
#include "NLIBC/string.c"
#include "nkernel/io.h"
#include "nkernel/utifuncs.c"
#include "NLIBC/math.c"

void kernel_main(uint32_t magic, uint32_t addr){
  clear_vga();
  if(magic != 0x36d76289){
    utifuncs_nkernel_panic("N-OS only supports the GRUB bootloader with the Multiboot2 protocol,\n but N-OS couldn't find the Multiboot2 magic number.\n");
  }

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
    } else if(!SSCANF_COMBINATION_HANDELED) {
      puts("Command '");
      puts(cmd);
      puts("' not found.\n");
    }
    putc('\n', VGA_LIGHT_GRAY, VGA_BLACK);

    memsetc(cmd, '\0', 1024);
  }

  for (;;);
}
