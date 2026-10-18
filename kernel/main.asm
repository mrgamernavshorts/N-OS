MAGIC equ 0xe85250d6 ; Multiboot2 magic number.
ARCH equ 0 ; 0 means 32-bit protected mode.
HEADER_LENGTH equ (header_end - header_start)
CHECKSUM equ -(MAGIC + ARCH + HEADER_LENGTH)

section .multiboot
header_start:
  dd MAGIC
  dd ARCH
  dd HEADER_LENGTH
  dd CHECKSUM

  ; End tag
  align 8
  dw 0 ; Type (tag)
  dw 0 ; Flags
  dd 8 ; size
header_end:

section .text

bits 32
global _start
extern kernel_main

_start:
  cli
  mov esp, stack_top
  push ebx ; Multiboot2 info
  push eax ; Multiboot2 Magic
  call kernel_main

.halt:
  cli
  hlt
  jmp .halt

; Haha stack go brrrr.

section .bss
align 16 ; Make the stack 16-bit aligned, like C expects.
stack_bottom:
  resb 16384 ; 16KB
stack_top:
