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

; [Note] ds:si will point to the string in memory

start:

  ; Initialize the ds/es registers here
  mov ax, 0
  mov ds, ax
  mov es, ax

  ; Stack go brrrr
  mov ss, ax
  mov sp, 0x7C00

  ; Some BIOS'es can start us at 07C0:0000 instead of 0000:7C00
  push es ; Intended new cs
  push word .after ; Intended new ip
  retf ; This will pop the .after into ip and es into cs (Both are before save on the stack).
  ; Now, the code execution will start at cs:ip (0000: location of .after)

.after:

  mov [ebr_drive_number], dl ; Assigns the calue of dl(drive number, most prob. the first floppy disk), so we can read from it.

  ; Read from the drive
  push es
  mov ah, 08h
  int 13h
  jc floppy_read_error ; Will jump to floppy_read_error if CF is 1(dis read failed.)
  pop es

  ; Not relying on the data on the disk, because the disk could get corrupted.
  and cl, 0x3F ; Remove top 2 bits
  xor ch, ch
  mov [bdb_sectors_per_track], cx ; Sector count
  
  inc dh
  mov [bdb_heads], dh ; Head count

  ; Read the Root directory
  mov ax, [bdb_sectors_per_fat] ; Compute the LBA location of root directory. lba = Reserved + fats * sectors_per_fat
  mov bl, [bdb_fat_count]
  xor bh, bh
  mul bx ; This will do ax * bl and store the result into ax (ax = fats * sectors_per_fat)
  add ax, [bdb_reserved_sectors] ; now ax = Reserved + fats * sectors_per_fat
  push ax

  ; Compute size of the root directory = (32 * number_of_entries) / bytes_per_sector
  mov ax, [bdb_dir_entries_count]
  shl ax, 5 ; Multiplies ax by 32 (haha no imul ax, 32 because I need that 1 byte boiiiiiiiii)
  xor dx, dx ; haha no mov dx, 0 because I wanna save that 1 byte boi.(this is 2 bytes and that is 3 bytes)
  div word [bdb_bytes_per_sector] ; This will do ax / bytes_per_sector

  test dx, dx ; if dx != 0, add 1(Does a bitwise AND)
  jz .root_dir_after ; Jump id Zero flage is set(Will be set by test if dx == 0).
  inc ax ; If division remainder != 0, add 1

.root_dir_after:
  ; Read the root directory
  mov cl, al ; cl = no. of sectors to read(size of root directory).
  pop ax ; ax = LBA of root directory.
  mov dl, [ebr_drive_number] ; Drive number (Saved previously)
  mov bx, buffer ; es:bx = buffer
  call disk_read

  ; search for kernel.bin
  xor bx, bx ; We will use this to keep track of the current directory entry count.
  mov di, buffer

.search_kernel:
  mov si, file_kernel_bin
  mov cx, 11 ; Length of the 'KERNEL  BIN', which is always 11 characters (As per fat12 limitations)
  push di
  
  repe cmpsb ; Compares the bytes at ds:si and es:si, and does so repeatedly until they differ, or cx reaches 0

  pop di
  je .kernel_found ; jumps to the .kernel_found label if ZF is set.
  
  ; Kernel not found; starting the next iteration.
  add di, 32 ; Next directory entry.
  inc bx ; Inc bx accordingly.
  cmp bx, [bdb_dir_entries_count]
  jl .search_kernel ; Will jump to the starting if bx is less than the total dir_entries_count.

  ; Jump to kernel_not_found_aaaa if kernel couldn't be found.
  jmp kernel_not_found_aaaa

.kernel_found:
  
  ; di should have the address to the entry
  mov ax, [di + 26] ; First logical cluster is at offset 26
  mov [kernel_cluster], ax

  ; load the FAT onto memory
  mov ax, [bdb_reserved_sectors]
  mov bx, buffer
  mov cl, [bdb_sectors_per_fat]
  mov dl, [ebr_drive_number]
  call disk_read

  mov bx, KERNEL_LOAD_SEGMENT
  mov es, bx
  mov bx, KERNEL_LOAD_OFFSET


.load_kernel_loop:
  ; Read next cluster
  mov ax, [kernel_cluster]

  ; Hardcoded the value, will change in the future (Please send help)
  add ax, 31 ; first cluster  = (kernel_cluster - 2) * bdb_sectors_per_cluster * start_sector
             ; start sector  = Reserved + fats + root dir size = 1 + 18 + 134 = 33
  
  mov cl, 1
  mov dl, [ebr_drive_number]
  call disk_read

  add bx, [bdb_bytes_per_sector]

  ; Compute location of the next cluster
  mov ax, [kernel_cluster]
  mov cx, 3
  mul cx
  mov cx, 2
  div cx

  mov si, buffer
  add si, ax
  mov ax, [ds:si] ; Read entry from fat table at index ax

  or dx, dx
  jz .even ; jumps when ZF == 1

.odd:
  shr ax, 4
  jmp .next_cluster_after

.even:
  and ax, 0x0FFF

.next_cluster_after:

  cmp ax, 0x0FF8 ; Check if end of file.
  jae .read_finish ; Jump to .read_finish if ax is equal or above than 0xFF8
  mov [kernel_cluster], ax
  jmp .load_kernel_loop


.read_finish:
  ; jump to our kernel
  mov dl, [ebr_drive_number] ; boot device in dl

  mov ax, KERNEL_LOAD_SEGMENT ; set the segment registers
  mov ds, ax
  mov es, ax 

  jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

  jmp RebootOnKeypress ; Should never happen

  cli ; Disable interrupts, so any interrupt does not forces our proccessor out of the "halt" state.
  hlt

;------------------------
; Error handlers (YAY :D)
;------------------------

floppy_read_error:
  mov si, FloppyReadFailedMessage
  call puts
  jmp RebootOnKeypress

kernel_not_found_aaaa:
  mov si, kernelNotFoundMessage
  call puts
  jmp RebootOnKeypress

RebootOnKeypress:
  mov ah, 0
  int 16h ; Waits for keypress
  jmp 0FFFFh:0 ; Location where the BIOS starts, jumps to that, rebooting the system

.halt:
  cli ; Disable interrupts
  hlt

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
;WelcomeMessage: db 10, "|\\  |   //-\\    //-\\", 10, 13, "| \\ | - |   |    \\", 10, 13, "|  \\|   \\-// \\_//", 10, 10, 13,"Welcome to N-OS!", 10, 13, 0
FloppyReadingMessage: db "Reading from floppy..", 10, 13, 0
FloppyReadFailedMessage: db "Read from floppy failed!", 0
kernelNotFoundMessage: db "Kernel not found!", 0

kernel_cluster: dw 0
file_kernel_bin: db "KERNEL  BIN"

; This segment contains the most amount if of contiguos memory, and is the biggest, around 480 kb
; We can't use more than 1 MB because we are currently in 16 bit real mode.
KERNEL_LOAD_SEGMENT equ 0x2000 ; equ doesn't loads the constant values onto memory
KERNEL_LOAD_OFFSET equ 0

; |\\  |   //-\\    //-\\
; | \\ | - |   |    \\
; |  \\|   \\-// \\_//

times 510 - ($-$$) db 0
dw 0xAA55

buffer:
