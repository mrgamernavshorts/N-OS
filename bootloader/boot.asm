org 0x7C00
bits 16

; --------------------------------
; Fat 12 header [Please help me]
; --------------------------------
jmp short start ; Jump to our start function
nop ; The above line is 1-2 bytes, but BIOS expectes the BIOS paramter block to start at 3 bytes, so empty instruction here. [BPB tells the layout of the disk]

bdb_oem:                    db "MSWIN4.1" ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880       ; 2880 * 512 = 1.44 MB
bdb_media_descriptor_type:  db 0F0h       ; F0 = 3.5" floppy disk
bdb_sectors_per_fat:        dw 9          ; 9 sectors/fat
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

; Extendet boot record (Still have to do this AAAAAAAAAAAAAAA)
ebr_drive_number:           db 0                    ; 0x00 floppy, 0x80 hdd, useless but still here ig.
db 0                                                ; Reserved because yes.
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h   ; Serial number, value doesn't matter.
ebr_volume_label:           db "N-OS       "        ; 11 bytes, needs to be exactly 11 bytes, so padded with spaces.
ebr_system_id:              db "FAT12   "           ; 8 bytes, also padded with spaces.


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
  mov si, WelcomeMessage
  call puts

  cli ; Disable interrupts, so any interrupt does not forces our proccessor out of the "halt" state.
  hlt

floppy_read_error:
  mov si, FloppyReadFailedMessage
  call puts
  jmp RebootOnKeypress

RebootOnKeypress:
  mov ah, 0
  int 16h ; Waits for keypress
  jmp 0FFFFh ; Location where the BIOS starts, jumps to that, rebooting the system

.halt:
  cli ; Disable interrupts
  hlt

; Params:
; ax = LBA value
; Return Values:
; cx [0-5 bits]: Sector number
; cx [6-15 bits]: cylinder
; dh: head
lba_to_chs:
  ; Save ax and dx onto the stack
  push ax
  push dx

  xor dx, dx ; dx = 0
  div word [bdb_sectors_per_track] ; ax = LBA / Sectors per track, dx = LBA % Sectors per track (Remainder of the previous division)

  ; Sector:
  inc dx ; dx = (LBA % Sectors per track) + 1  just increment 1 from the value of dx calculated from the previous calculation.
  mov cx, dx ; cx = sector
  
  xor dx, dx
  div word [bdb_heads] ; ax = (ax( which is LBA / Sectors per track now)) / Heads = cylinder
  ; now dx will have (LBA / Sectors per track) % Heads = head
  
  mov dh, dl ; dl = head
  mov ch, al ; ch = cylinder (lower 8 bits)
  shl ah, 6 ; shifts ah 6 bytes to the left
  or cl, ah ; upper 2 bits of cylinder in CL

  pop ax ; Saves the value of dx saved onto the stack before into ax.
  mov dl, al ; Moves the lower 8 bits of ax to dl.
  pop ax ; Saves the old LBA value to ax.
  ret

; Params:
; ax: LBA address
; cl: Number of sectors to read (upto 128)
; dl: Drive number
; es:bx : Memory address to store the read data.
disk_read:
  ; Save all of the registers we will modify
  push ax
  push bx
  push cx
  push dx
  push di

  push cx ; Save CL temporarily, because lba_to_chs overwrites it
  call lba_to_chs
  pop ax  ; now al = cl (No. of sectors to read)

  mov ah, 02h ; Tells BIOS the interrupt will be for reading from the disk

  mov di, 3 ; Sets the loop register to 3, to atleast retry reading 3 times

.retry:
  pusha ; Save all of the registers onto memory, We don't know what registers BIOS will change.
  stc ; Set the carry flag, some BIOS'es don't do it automatically.
  int 13h ; Disk interrupt. if successful, will clear the carry flag.
  jnc .done ; jump to .done label if carry flag is cleared.

  ; Read failed
  popa
  call disk_reset

  ; Restart the loop
  dec di
  test di, di ; Check if count is 0
  jnz .retry

.fail:
  ; When all attempts fail
  jmp floppy_read_error

.done:
  popa

  pop di
  pop dx
  pop cx
  pop bx
  pop ax
  ret

; Params
; dl - drive number
disk_reset:
  pusha
  mov ah, 0
  stc
  int 13h
  jc floppy_read_error
  popa
  ret

; [Note]: here, 10 in ASCII is LF, and 13 is CR (line feed and carriage return)
WelcomeMessage: db 10, "|\\  |   //-\\    //-\\", 10, 13, "| \\ | - |   |    \\", 10, 13, "|  \\|   \\-// \\_//", 10, 10, 13,"Welcome to N-OS!", 10, 13, 0
FloppyReadFailedMessage: db "Read from floppy failed. Press any key to reboot.", 0

; |\\  |   //-\\    //-\\
; | \\ | - |   |    \\
; |  \\|   \\-// \\_//

times 510 - ($-$$) db 0
dw 0xAA55
