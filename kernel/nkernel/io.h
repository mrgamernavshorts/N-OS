#ifndef NKERNEL_IO
#define NKERNEL_IO

#include "../NLIBC/stdint.h"

// ATA/IDE drive hardware ports.
#define ATA_PRIMARY_DATA 0x1F0
#define ATA_PRIMARY_SECTOR_COUNT 0x1F2
#define ATA_PRIMARY_LBA_LOW 0x1F3
#define ATA_PRIMARY_LBA_MID 0x1F4
#define ATA_PRIMARY_LBA_HIGH 0x1F5
#define ATA_PRIMARY_DEVICE 0x1F6
#define ATA_PRIMARY_COMMAND 0x1F7
#define ATA_PRIMARY_STATUS 0x1F7

// PS/2 keyboard ports.
#define PS2_DATA 0x60
#define PS2_STATUS 0x64

// For VGA foreground/background colors.
enum {
  VGA_BLACK = 0x0,
  VGA_BLUE = 0x1,
  VGA_GREEN = 0x2,
  VGA_CYAN = 0x3,
  VGA_RED = 0x4,
  VGA_PURPLE = 0x5,
  VGA_BROWN = 0x6,
  VGA_LIGHT_GRAY = 0x7,
  VGA_DARK_GRAY = 0x8,
  VGA_LIGHT_BLUE = 0x9,
  VGA_LIGHT_GREEN = 0xA,
  VGA_LIGHT_CYAN = 0xB,
  VGA_LIGHT_RED = 0xC,
  VGA_LIGHT_MAGENTA = 0xD,
  VGA_YELLOW = 0xE,
  VGA_WHITE = 0xF
};

extern uint8_t inb(uint16_t port); // Recive from a hardware port.
extern void outb(uint16_t port, uint8_t value); // Send to a hardware port.

// Writes a char onto the VGA MMIO at an Index
void io_write_char_vga(char c, uint8_t fg, uint8_t bg, uint32_t index); 

// Polls the keyboard for a scancode, and if one is found,
// writes the scancode into `scancode`(Passed in by Refrence).
// Returns 0 if Successful, and 1 if not.
int keyboardPoll(uint8_t* scancode);

// Takes a PS/2 keyboard scan code, and returns a Char related to it.
char scancodeToChar(uint8_t scancode);

#endif
