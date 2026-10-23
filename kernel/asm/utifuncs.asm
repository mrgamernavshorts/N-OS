; Asm file used by the utifuncs kernel lib.
bits 32

section .text

global freeze

; Freezes the whole system.
freeze:
        cli
        hlt
