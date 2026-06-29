#include "display.h"
#include "gdt.h"

void kmain(void) {
  // init_Gdt();

  clear_screen();
  kprint("[*] GDT Initialization done!");
}
