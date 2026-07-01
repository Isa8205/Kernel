#include "idt.h"
#include "display.h"
#include <stdint.h>

extern void irq1_keyboard_handler();

IDT_entry_t IDT[IDT_SIZE];
IDT_ptr_t idt_ptr;

void idt_init(void) {
  
  idt_ptr.size = (sizeof(IDT_entry_t) * IDT_SIZE) -1;
  idt_ptr.offset = (uint32_t)&IDT;

  setIdtGate(32, (uint32_t)irq1_keyboard_handler, 0x08, 0x0E, 0);

	load_idt(idt_ptr);
  
  kprint("[*] IDT initialization done");
}

void setIdtGate(uint32_t num, uint32_t offset, uint16_t segment_selector, uint8_t gate_type, uint8_t dpl) {
  IDT[num].offset_low = (offset & 0xFFFF);
  IDT[num].selector = segment_selector;
  IDT[num].zero = 0;
  IDT[num].gate_type = gate_type;
  IDT[num].gate_type |= (dpl << 5);
  IDT[num].gate_type |= (1 << 7);
  IDT[num].offset_high = (offset >> 16);
}
