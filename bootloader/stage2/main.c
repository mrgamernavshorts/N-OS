#include "stdint.h"
#include "stdio.h"

/*
 * CDECL calling convention:
 * Args: passed through the stack, pushed from left to right, and the caller removes params from stack.
 * Returns: ints, pointers -> EAX, floating point -> ST0.
 * Registers: EAX, ECX and EDX saved by the caller and rest saved by callee.
 * */

/*
|\\  ||   //==\\    //==\\
||\\ || - ||  ||    \\
|| \\|| - ||  ||     \\
||  \\|   \\==// \\==//
*/

void _cdecl cstart_(uint_16t bootDrive){
  puts("|\\\\  ||   //==\\\\    //==\\\\ \r\n||\\\\ || - ||  ||    \\\\ \r\n|| \\\\|| - ||  ||     \\\\ \r\n||  \\\\|   \\\\==// \\\\==// \r\n");
  puts("Welcome to N-OS! \r\n");
  for(;;);
}
