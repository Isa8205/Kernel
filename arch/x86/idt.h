#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t gate_type;
    uint16_t offset_high;
}__attribute__((packed)) IDT_entry_t;

typedef struct {
  uint16_t size;
  uint32_t offset;
}__attribute__((packed)) IDT_ptr_t; 

void idt_init(void);
void setIdtGate(uint32_t num, uint32_t offset, uint16_t segment_selector, uint8_t gate_type, uint8_t dpl);
extern void load_idt(IDT_ptr_t* idt_ptr);
void send_eoi(uint8_t irq);

#endif
