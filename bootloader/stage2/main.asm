bits 16

section _ENTRY class=CODE ; Tell nasm to place the code in _ENTRY section.

extern _cstart_ ; Entry point from c
global entry ; Global makes the label accessible outside this asm file.

entry:
  cli
  ; Setup stack.
  mov ax, ds
  mov ss, ax
  mov sp, 0
  mov bp, sp
  sti

  ; Boot drive in dl, xor the upper 8 bits.
  xor dh, dh
  push dx
  call _cstart_

  cli
  hlt
