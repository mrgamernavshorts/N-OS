org 0x7C00
bits 16

jmp start

CODE_SEL equ 0x08

; Data address packet:
align 4
dap:
  db 0x10 ; Packet size (16 bytes)
  db 0 ; Reserved byte
  dw KERNEL_SECTOR_SIZE ; Sector count
  dw 0x7E00 ; Buffer offset; Place to load the kernel at.
  dw 0x0 ; Buffer segment.
  dq 1 ; Starting LBA

start:
  ; Setting up the stack:
  cli
  xor ax, ax
  mov ss, ax
  mov sp, 0x7C00
  mov ds, ax
  sti

  mov [BIOS_BOOT_DRIVE], dl ; Save boot drive number.

  mov si, LoadingKernelMessage
  call puts

  ; Load the kernel onto memory
  mov si, dap
  mov dl, [BIOS_BOOT_DRIVE]
  mov ah, 0x42
  int 0x13

  jc disk_read_error

  ; -------------------------------
  ; Entering 32-bit PROTECTED mode.
  ; -------------------------------

  cli

  ; Setting up A20(Fast-gate method)
  in al, 0x92 ; Hardware I/O port, where A20 resides.
  or al, 00000010b
  and al, 11111110b
  out 0x92, al

  ; Load the GDT address onto the gdt register.
  lgdt [gdt_descriptor]

  ; Enable protected mode
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  


  ; Jump to the kernel.
  jmp CODE_SEL:0x7E00

  cli
  hlt

; --------------
; Som functions:
; --------------
puts:
  push si
  push ax
  push bx

.loop:
  lodsb ; Will increment si(The current char), and load it's value onto al.
  or al, al ; Check if null-terminator; Will set the ZERO flag to 0 if yes.

  jz .ok

  mov ah, 0Eh
  mov bh, 0 ; Page number.
  int 0x10

  jmp .loop

.ok:
  pop bx
  pop ax
  pop si
  ret

; --------------------------
; Some error handling stuff.
; --------------------------
disk_read_error:
  mov si, KernelLoadFailedMessage
  call puts

  cli
  hlt

.halt:
  jmp .halt

; Creating a minimal GDT.
align 8
gdt:
    dq 0                    ; Null descriptor
    dq 0x00CF9A000000FFFF   ; 32-bit code.
    dq 0x00CF92000000FFFF   ; 32-bit data.
gdt_end:

gdt_descriptor:
  dw gdt_end - gdt - 1
  dd gdt

BIOS_BOOT_DRIVE: db 0

LoadingKernelMessage: db "[N-BOOT]: Loading the kernel...", 10, 13, 0
KernelLoadFailedMessage: db "[N-BOOT-ERROR]: Disk read error! Loading the kernel failed!", 10, 13, 0

times 510 - ($-$$) db 0
dw 0AA55h
