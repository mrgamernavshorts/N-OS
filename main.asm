org 0x7C00
bits 16

start:
  jmp main

; [Note] ds:si will point to the string in memory

puts:
  ; Save si/ax onto the stack
  push si
  push ax

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
  pop ax
  pop si
  ret

main:
  ; Initialize the ds/es registers here
  mov ax, 0
  mov ds, ax
  mov es, ax

  ; Stack go brrrr
  mov ss, ax
  mov sp, 0x7C00

  ; Print the welcome message
  mov si, myMessage
  call puts

  mov si, myMessage2
  call puts

  hlt

.halt:
  jmp .halt

; [Note]: here, 10 in ASCII is LF, and 13 is CR (line feed and carriage return)
; [Todo]: Make ASCII art for startup.
myMessage: db "Welcome to N-OS!", 10, 13, 0
myMessage2: db "Welcome to N-OS! (again)", 10, 13, 0

times 510 - ($-$$) db 0
dw 0xAA55
