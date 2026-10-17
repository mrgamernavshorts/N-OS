bits 32

SECTION .text

global inb
global outb

inb:
  push ebp
  mov ebp, esp

  mov dx, [ebp + 8] ; Port
  xor eax, eax
  
  in al, dx ; Return value inside al/eax according to CDECL.

  pop ebp
  ret

outb:
  push ebp
  mov ebp, esp

  mov dx, [ebp + 8] ; Port
  mov al, [ebp + 12] ; Value
  out dx, al

  pop ebp
  ret
