#ifndef NKERNEL_IO
#define NKERNEL_IO

#include "../NLIBC/stdint.h"

#define PS2_DATA 0x60
#define PS2_STATUS 0x64

extern uint8_t inb(uint16_t port); // Recive from a hardware port.
extern void outb(uint16_t port, uint8_t value); // Send to a hardware port.

// Writes a char onto the VGA MMIO at an Index
void io_write_char_vga(char c, uint32_t index); 

// Polls the keyboard for a scancode, and if one is found,
// writes the scancode into `scancode`(Passed in by Refrence).
// Returns 0 if Successful, and 1 if not.
int keyboardPoll(uint8_t* scancode);

#endif
