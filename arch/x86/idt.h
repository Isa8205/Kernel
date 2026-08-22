#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
}__attribute__((packed)) IDT_entry_t;

typedef struct {
  uint16_t size;
  uint32_t offset;
}__attribute__((packed)) IDT_ptr_t; 

typedef struct {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
} InterruptRegisters_t;

void idt_init(void);
void setIdtGate(uint32_t num, uint32_t offset, uint16_t segment_selector, uint8_t dpl, uint8_t gate_type);
extern void load_idt(IDT_ptr_t* idt_ptr);
void send_eoi(uint8_t irq);
void isr_handler(InterruptRegisters_t *regs);

void irq_register_handler(int irq, void(*handler)(InterruptRegisters_t));
void irq_uninstall_handler(int irq);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
