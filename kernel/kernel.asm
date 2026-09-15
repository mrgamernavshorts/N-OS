org 0x0
bits 16

; [Note] ds:si will point to the string in memory

start:

  ; Print the welcome message
  mov si, WelcomeMessage
  call puts

.halt:
  cli
  hlt


puts:
  ; Save si/ax onto the stack
  push si
  push ax
  push bx

.loop:
  lodsb ; This will load the a char of the string ds:si into al register, and increments si.
  or al, al ; Just check of al is null, if yes, it will set the Zero flag to, well, Zero :D

  jz .done ; Jump to done only if al is NULL(Conditional jump).

  ; Print dat boi to screen(tanku BIOS(Totally not going insane :D))
  mov ah, 0x0e ; Tell the BIOS that "Oi, I am printing a ASCII character".
  mov bh, 0 ; Set the page to 0 because yes.
  int 0x10 ; Tell the BIOS that "Oi, I am doing a video interrupt, see the ah register to see what Video thing I wanna do."


  jmp .loop

.done:
  ; FILO go brrrrrrr
  pop bx
  pop ax
  pop si
  ret


; [Note]: here, 10 in ASCII is LF, and 13 is CR (line feed and carriage return)
; [Todo]: Make ASCII art for startup.
WelcomeMessage: db 10, "|\\  |   //-\\    //-\\", 10, 13, "| \\ | - |   |    \\", 10, 13, "|  \\|   \\-// \\_//", 10, 10, 13,"Welcome to N-OS!", 10, 13, 0

; |\\  |   //-\\    //-\\
; | \\ | - |   |    \\
; |  \\|   \\-// \\_//

