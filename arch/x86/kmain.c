#include "gdt.h"
#include "idt.h"

void kmain(void) {
  init_Gdt();
  idt_init();
}
