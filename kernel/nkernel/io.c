#ifndef NKERNEL_IO_C
#define NKERNEL_IO_C

#include "io.h"

vuint16_t *vga = (uint16_t *)0xB8000;
vuint8_t shiftPressed = 0;
vuint8_t capsLockEnabled = 0;

void io_write_char_vga(char c, uint32_t index){
  vga[index] = c | (0x07 << 8);
}

int keyboardPoll(uint8_t* scancode){
  if((inb(PS2_STATUS) & 1) == 0){
    return 1; // No key pressed :(
  }
  *scancode = inb(PS2_DATA);
  return 0;
}

char scancodeToChar(uint8_t scancode){
  switch (scancode) {
    // Numbha's
    case 0x02: return (shiftPressed ^ capsLockEnabled) ? '!' : '1';
    case 0x03: return (shiftPressed ^ capsLockEnabled) ? '@' : '2';
    case 0x04: return (shiftPressed ^ capsLockEnabled) ? '#' : '3';
    case 0x05: return (shiftPressed ^ capsLockEnabled) ? '$' : '4';
    case 0x06: return (shiftPressed ^ capsLockEnabled) ? '%' : '5';
    case 0x07: return (shiftPressed ^ capsLockEnabled) ? '^' : '6';
    case 0x08: return (shiftPressed ^ capsLockEnabled) ? '&' : '7';
    case 0x09: return (shiftPressed ^ capsLockEnabled) ? '*' : '8';
    case 0x0A: return (shiftPressed ^ capsLockEnabled) ? '(' : '9';
    case 0x0B: return (shiftPressed ^ capsLockEnabled) ? ')' : '0';

    // AlphaBaticool
    case 0x10: return (shiftPressed ^ capsLockEnabled) ? 'Q' : 'q';
    case 0x11: return (shiftPressed ^ capsLockEnabled) ? 'W' : 'w';
    case 0x12: return (shiftPressed ^ capsLockEnabled) ? 'E' : 'e';
    case 0x13: return (shiftPressed ^ capsLockEnabled) ? 'R' : 'r';
    case 0x14: return (shiftPressed ^ capsLockEnabled) ? 'T' : 't';
    case 0x15: return (shiftPressed ^ capsLockEnabled) ? 'Y' : 'y';
    case 0x16: return (shiftPressed ^ capsLockEnabled) ? 'U' : 'u';
    case 0x17: return (shiftPressed ^ capsLockEnabled) ? 'I' : 'i';
    case 0x18: return (shiftPressed ^ capsLockEnabled) ? 'O' : 'o';
    case 0x19: return (shiftPressed ^ capsLockEnabled) ? 'P' : 'p';
    case 0x1E: return (shiftPressed ^ capsLockEnabled) ? 'A' : 'a';
    case 0x1F: return (shiftPressed ^ capsLockEnabled) ? 'S' : 's';
    case 0x20: return (shiftPressed ^ capsLockEnabled) ? 'D' : 'd';
    case 0x21: return (shiftPressed ^ capsLockEnabled) ? 'F' : 'f';
    case 0x22: return (shiftPressed ^ capsLockEnabled) ? 'G' : 'g';
    case 0x23: return (shiftPressed ^ capsLockEnabled) ? 'H' : 'h';
    case 0x24: return (shiftPressed ^ capsLockEnabled) ? 'J' : 'j';
    case 0x25: return (shiftPressed ^ capsLockEnabled) ? 'K' : 'k';
    case 0x26: return (shiftPressed ^ capsLockEnabled) ? 'L' : 'l';
    case 0x2C: return (shiftPressed ^ capsLockEnabled) ? 'Z' : 'z';
    case 0x2D: return (shiftPressed ^ capsLockEnabled) ? 'X' : 'x';
    case 0x2E: return (shiftPressed ^ capsLockEnabled) ? 'C' : 'c';
    case 0x2F: return (shiftPressed ^ capsLockEnabled) ? 'V' : 'v';
    case 0x30: return (shiftPressed ^ capsLockEnabled) ? 'B' : 'b';
    case 0x31: return (shiftPressed ^ capsLockEnabled) ? 'N' : 'n';
    case 0x32: return (shiftPressed ^ capsLockEnabled) ? 'M' : 'm';

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
    case 0x3A: // Haha caps lock go BRRRR
      capsLockEnabled = capsLockEnabled ? 0 : 1;
      return '\0';
    case 0x0F: return '\t';

    // Other Spaciool keys.
    case 0x33: return shiftPressed ? '<' : ',';
    case 0x34: return shiftPressed ? '>' : '.';
    case 0x35: return shiftPressed ? '?' : '/';
    case 0x27: return shiftPressed ? ':' : ';';
    case 0x28: return shiftPressed ? '"' : '\'';
    case 0x1A: return shiftPressed ? '{' : '[';
    case 0x1B: return shiftPressed ? '}' : ']';
    case 0x2B: return shiftPressed ? '|' : '\\';
    case 0x29: return shiftPressed ? '~' : '`';
    case 0x0C: return shiftPressed ? '_' : '-';
    case 0x0D: return shiftPressed ? '+' : '=';

    default: return '\0'; // Haha no key for you >:D
  }
}



#endif
