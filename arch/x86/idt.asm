bits 32

global load_idt
global irq1_keyboard_handler
extern keyboard_callback

section .text

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	;sti 				;turn on interrupts
	ret

irq1_keyboard_handler:
  pusha
  call keyboard_callback
  popa
  iret
