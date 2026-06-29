#ifndef IDT_H
#define IDT_H

#define IDT_SIZE 256

typedef struct {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higerbits;
} IDT_entry_t;

void idt_init(void);
extern void load_idt(unsigned long *idt_ptr);

#endif
