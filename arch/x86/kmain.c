#include "vga.h"
#include "gdt.h"
#include "idt.h"

void kmain(void) {
  volatile unsigned short *vga = (volatile unsigned short *)0xB8000;
  vga[0] = (0x4F << 8) | 'X'; // Prints a white 'X' on a red background at top-left
  while (1);
}
