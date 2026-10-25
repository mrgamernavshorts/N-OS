#ifndef NKERNEL_UTIFUNCS
#define NKERNEL_UTIFUNCS

#include "../NLIBC/stdio.c"

extern void freeze(); // Freezes the entire system.

// -------------------------------------------------------------------------------
// This header file contains various utility functions used by the Kernel or LIBC.
// -------------------------------------------------------------------------------


// Prints a Kernel panic message.
void utifuncs_nkernel_panic(char* message);

// Reboots the entire system.
void utifuncs_nkernel_reboot();

// Poweroff's the entire system.
void utifuncs_nkernel_poweroff();

#endif
