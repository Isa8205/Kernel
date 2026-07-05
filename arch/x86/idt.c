#include "idt.h"
#include "vga.h"
#include <stdint.h>

extern void irq1_keyboard_handler();
extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t val);

IDT_entry_t IDT[IDT_SIZE];
IDT_ptr_t idt_ptr;

void idt_init(void) {
  
  idt_ptr.size = (sizeof(IDT_entry_t) * IDT_SIZE) -1;
  idt_ptr.offset = (uint32_t)&IDT;

  setIdtGate(0x21, (uint32_t)irq1_keyboard_handler, 0x08, 0x8E, 0);

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xfd);
	write_port(0xA1 , 0xff);

	load_idt(&idt_ptr);
  
  // kprint("[*] IDT initialization done");
}

void setIdtGate(uint32_t num, uint32_t offset, uint16_t segment_selector, uint8_t gate_type, uint8_t dpl) {
  IDT[num].offset_low = (offset & 0xFFFF);
  IDT[num].selector = segment_selector;
  IDT[num].zero = 0;
  IDT[num].gate_type = gate_type;
  IDT[num].gate_type |= (dpl << 5);
  IDT[num].gate_type |= (1 << 7);
  IDT[num].offset_high = (offset & 0xFFFF0000) >> 16;
}

void send_eoi(uint8_t irq) {
  if (irq >= 8) {
    write_port(0xA0, 0x20);
  }
  write_port(0x20, 0x20);
}
