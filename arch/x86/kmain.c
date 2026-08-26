#include "input/keyboard.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"

void kmain(void) {
  init_Gdt();
  idt_init();

  keyboard_init();

  kprint("Everything set");
  clear_screen();
  while(1);
}
