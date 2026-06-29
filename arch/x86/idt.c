#include "idt.h"

IDT_entry_t IDT[IDT_SIZE];

void idt_init(void) {
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (IDT_entry_t) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}
