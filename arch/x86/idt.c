#include "idt.h"
#include "vga.h"
#include "util.h"
#include <stdint.h>

IDT_entry_t IDT[IDT_SIZE];
IDT_ptr_t idt_ptr;

void idt_init(void) {
  
  idt_ptr.size = (sizeof(IDT_entry_t) * IDT_SIZE) -1;
  idt_ptr.offset = (uint32_t)&IDT;

  // Initialize everything to zero
  memfill(&IDT, 0, sizeof(IDT_entry_t) * IDT_SIZE);

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
	write_port(0x21 , 0x04);
	write_port(0xA1 , 0x02);

	/* ICW4 - environment info */
  // Makes it operate in 8086/88 mode
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xFD); // Only allow irq1(keyboard) i.e 1111 1101
	write_port(0xA1 , 0xFF); // Block all 8 hardware lines i.e 1111 1111

  /* Interrupt Service Routine Setup */
  setIdtGate(0, (uint32_t)isr0, 0x08, 0x00, 0x0E);
  setIdtGate(1, (uint32_t)isr1, 0x08, 0x00, 0x0E);
  setIdtGate(2, (uint32_t)isr2, 0x08, 0x00, 0x0E);
  setIdtGate(3, (uint32_t)isr3, 0x08, 0x00, 0x0E);
  setIdtGate(4, (uint32_t)isr4, 0x08, 0x00, 0x0E);
  setIdtGate(5, (uint32_t)isr5, 0x08, 0x00, 0x0E);
  setIdtGate(6, (uint32_t)isr6, 0x08, 0x00, 0x0E);
  setIdtGate(7, (uint32_t)isr7, 0x08, 0x00, 0x0E);
  setIdtGate(8, (uint32_t)isr8, 0x08, 0x00, 0x0E);
  setIdtGate(9, (uint32_t)isr9, 0x08, 0x00, 0x0E);
  setIdtGate(10, (uint32_t)isr10, 0x08, 0x00, 0x0E);
  setIdtGate(11, (uint32_t)isr11, 0x08, 0x00, 0x0E);
  setIdtGate(12, (uint32_t)isr12, 0x08, 0x00, 0x0E);
  setIdtGate(13, (uint32_t)isr13, 0x08, 0x00, 0x0E);
  setIdtGate(14, (uint32_t)isr14, 0x08, 0x00, 0x0E);
  setIdtGate(15, (uint32_t)isr15, 0x08, 0x00, 0x0E);
  setIdtGate(16, (uint32_t)isr16, 0x08, 0x00, 0x0E);
  setIdtGate(17, (uint32_t)isr17, 0x08, 0x00, 0x0E);
  setIdtGate(18, (uint32_t)isr18, 0x08, 0x00, 0x0E);
  setIdtGate(19, (uint32_t)isr19, 0x08, 0x00, 0x0E);
  setIdtGate(20, (uint32_t)isr20, 0x08, 0x00, 0x0E);
  setIdtGate(21, (uint32_t)isr21, 0x08, 0x00, 0x0E);
  setIdtGate(22, (uint32_t)isr22, 0x08, 0x00, 0x0E);
  setIdtGate(23, (uint32_t)isr23, 0x08, 0x00, 0x0E);
  setIdtGate(24, (uint32_t)isr24, 0x08, 0x00, 0x0E);
  setIdtGate(25, (uint32_t)isr25, 0x08, 0x00, 0x0E);
  setIdtGate(26, (uint32_t)isr26, 0x08, 0x00, 0x0E);
  setIdtGate(27, (uint32_t)isr27, 0x08, 0x00, 0x0E);
  setIdtGate(28, (uint32_t)isr28, 0x08, 0x00, 0x0E);
  setIdtGate(29, (uint32_t)isr29, 0x08, 0x00, 0x0E);
  setIdtGate(30, (uint32_t)isr30, 0x08, 0x00, 0x0E);
  setIdtGate(31, (uint32_t)isr31, 0x08, 0x00, 0x0E);

  setIdtGate(0x21, (uint32_t)irq1, 0x08, 0x00, 0x0E);

	load_idt(&idt_ptr);
  
  kprint("[*] IDT initialization done\n");
}

void setIdtGate(uint32_t num, uint32_t offset, uint16_t segment_selector, uint8_t dpl, uint8_t gate_type) {
  IDT[num].offset_low = (offset & 0xFFFF);
  IDT[num].selector = segment_selector;
  IDT[num].zero = 0;
  IDT[num].flags = (gate_type & 0x0F);
  IDT[num].flags &= ~(1 << 4);
  IDT[num].flags |= (dpl & 0x03);
  IDT[num].flags |= (1 << 7);
  IDT[num].offset_high = (offset & 0xFFFF0000) >> 16;
}

void send_eoi(uint8_t irq) {
  if (irq >= 8) {
    write_port(0xA0, 0x20);
  }
  write_port(0x20, 0x20);
}

char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(InterruptRegisters_t *regs) {
  if (regs->int_no < 32) {
    kprint(exception_messages[regs->int_no]);
    kprint("\n");
    kprint("Exception occured. System halted!");
    for(;;);
  }
}

void *irq_routines[16] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};

void irq_register_handler(int irq, void(*handler)(InterruptRegisters_t)) {
  if (irq < 16) {
    irq_routines[irq] = handler;
  }
}

void irq_uninstall_handler(int irq) {
  if (irq < 16) {
    irq_routines[irq] = 0;
  }
}

void irq_handler(InterruptRegisters_t *regs) {
  void (*handler)(InterruptRegisters_t *regs);

  handler = irq_routines[regs->int_no - 32];

  if (handler) {
    handler(regs);
  }

  send_eoi(regs->int_no - 32);
}
