bits 32

DATA_SEL equ 0x10

global _start
extern kernel_main

section .text
_start:
  cli
  mov ax, DATA_SEL ; GDT data selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov esp, 0x90000 ; Top of a reserved space for the stack.
  and esp, 0xFFFFFFF0 ; Align the stack before jumping to the C-code.

  call kernel_main

.halt:
  cli
  hlt
  jmp .halt
