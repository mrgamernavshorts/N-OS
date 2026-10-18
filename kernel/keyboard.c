#ifndef NKERNEL_KEYBOARD
#define NKERNEL_KEYBOARD

#include "NLIBC/stdint.h"

// clang-format off
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);

#define PS2_DATA 0x60
#define PS2_STATUS 0x64

int keyboardPoll(uint8_t* scancode){
  if((inb(PS2_STATUS) & 1) == 0){
    return 1; // No key pressed :(
  }
  *scancode = inb(PS2_DATA);
  return 0;
}

vuint8_t shiftPressed = 0;

char scancodeToChar(uint8_t scancode){
  switch (scancode) {
    // Numbha's
    case 0x02: return shiftPressed ? '!' : '1';
    case 0x03: return shiftPressed ? '@' : '2';
    case 0x04: return shiftPressed ? '#' : '3';
    case 0x05: return shiftPressed ? '$' : '4';
    case 0x06: return shiftPressed ? '%' : '5';
    case 0x07: return shiftPressed ? '^' : '6';
    case 0x08: return shiftPressed ? '&' : '7';
    case 0x09: return shiftPressed ? '*' : '8';
    case 0x0A: return shiftPressed ? '(' : '9';
    case 0x0B: return shiftPressed ? ')' : '0';

    // AlphaBaticool
    case 0x10: return shiftPressed ? 'Q' : 'q';
    case 0x11: return shiftPressed ? 'W' : 'w';
    case 0x12: return shiftPressed ? 'E' : 'e';
    case 0x13: return shiftPressed ? 'R' : 'r';
    case 0x14: return shiftPressed ? 'T' : 't';
    case 0x15: return shiftPressed ? 'Y' : 'y';
    case 0x16: return shiftPressed ? 'U' : 'u';
    case 0x17: return shiftPressed ? 'I' : 'i';
    case 0x18: return shiftPressed ? 'O' : 'o';
    case 0x19: return shiftPressed ? 'P' : 'p';
    case 0x1E: return shiftPressed ? 'A' : 'a';
    case 0x1F: return shiftPressed ? 'S' : 's';
    case 0x20: return shiftPressed ? 'D' : 'd';
    case 0x21: return shiftPressed ? 'F' : 'f';
    case 0x22: return shiftPressed ? 'G' : 'g';
    case 0x23: return shiftPressed ? 'H' : 'h';
    case 0x24: return shiftPressed ? 'J' : 'j';
    case 0x25: return shiftPressed ? 'K' : 'k';
    case 0x26: return shiftPressed ? 'L' : 'l';
    case 0x2C: return shiftPressed ? 'Z' : 'z';
    case 0x2D: return shiftPressed ? 'X' : 'x';
    case 0x2E: return shiftPressed ? 'C' : 'c';
    case 0x2F: return shiftPressed ? 'V' : 'v';
    case 0x30: return shiftPressed ? 'B' : 'b';
    case 0x31: return shiftPressed ? 'N' : 'n';
    case 0x32: return shiftPressed ? 'M' : 'm';

    // Spashiool keys
    case 0x1C: return '\n';
    case 0x39: return ' ';
    case 0x0E: return '\b';
    case 0x2A: // Left shift press.
      shiftPressed = 1;
      return '\0';
    case 0xAA: // Left shift release.
      shiftPressed = 0;
      return '\0';
    case 0x36: // Right shift press.
      shiftPressed = 1;
      return '\0';
    case 0xB6: // Right shift release.
      shiftPressed = 0;
      return '\0';

    default: return '\0'; // Haha no key for you >:D
  }
}

#endif
