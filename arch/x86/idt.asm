bits 32

global load_idt

section .text

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret
