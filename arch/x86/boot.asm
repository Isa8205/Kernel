;;kernel.asm
bits 32

MBALIGN  equ  1 << 0                   ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1                   ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO        ; Multiboot 'flag' field
MAGIC    equ  0x1BADB002               ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)          ; checksum required by multiboot spec

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

global read_port
global write_port
extern kmain

section .text
global _start
_start:
    cli
    ; Set up the stack
    mov esp, stack_top
    call kmain
    hlt

read_port:
    mov edx, [esp+4]
    in al, dx
    ret

write_port:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4]
    out dx, al
    ret
    
section .bss
align 16
stack_bottom:
  resb 16834 ; 16Kb Stack
stack_top:
