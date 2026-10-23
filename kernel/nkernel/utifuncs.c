#ifndef NKERNEL_UTIFUNCS_C
#define NKERNEL_UTIFUNCS_C

#include "utifuncs.h"

// A kernle panic message :O
/*
/--\ /--\ |\   | --- /---  |
|  | |  | | \  |  |  |     |
|--| |--| |  \ |  |  |     |
|    |  | |   \| --- \---  O
*/

void utifuncs_nkernel_panic(char *message){
  clear_vga();
  puts("//==\\\\ //==\\\\ ||\\\\   || ====  //===  ||\n");
  puts("||  || ||  || || \\\\  ||  ||  ||      ||\n");
  puts("||==|| ||==|| ||  \\\\ ||  ||  ||      ||\n");
  puts("||     ||  || ||   \\\\|| ====  \\\\===  :(\n");
  puts("Something went wrong, and the kernel panicked.\nPanic message:\n\n");
  puts(message);
  freeze();
}

#endif
