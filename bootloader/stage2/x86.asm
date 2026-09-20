bits 16

section _TEXT class=CODE

; int 10h ah=0Eh
; args: char, page

global _x86_Video_WriteChar
_x86_Video_WriteChar:
  ; Make a new call frame.
  push bp ; Save old call frame.
  mov bp, sp ; Initialize new call frame.

  push bx

  ; [bp + 0] the old call frame.
  ; [bp + 2] will have the return address. For small memory model, ~ 2 bytes.
  ; [bp + 4] first arg (char); bytes are converted to words (you can't push a single byte onto the stack.)
  ; [bp + 6] Second arg (page)

  mov ah, 0Eh
  mov al, [bp + 4] ; Char to print
  mov bh, [bp + 6] ; Page

  int 10h

  pop bx
  
  ; Restore old call frame.
  mov sp, bp
  pop bp
  ret
