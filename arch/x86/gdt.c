#include "gdt.h"
#include "display.h"
#include <stdint.h>

struct GDT_entry gdt_entries[5];
struct GDT_ptr gdt_ptr;

void init_Gdt(void) {
  gdt_ptr.limit = (sizeof(struct GDT_entry) * 5) -1;
  gdt_ptr.base = (uint32_t)gdt_entries;

  setGdtGate(0, 0, 0, 0, 0); // Null segment
  setGdtGate(1, 0, 0xFFFFF, 0x9A, 0xC); // Kernel code segment
  setGdtGate(2, 0, 0xFFFFF, 0x92, 0xC); // Kernel data segment
  setGdtGate(3, 0, 0xFFFFF, 0xFA, 0xC); // User code segment
  setGdtGate(4, 0, 0xFFFFF, 0xF2, 0xC); // User data segment

  gdt_flush(&gdt_ptr);

  kprint("[*] GDT Initialization done!");
}

void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_mid = (base >> 16) & 0xFF;
  gdt_entries[num].base_high = (base >> 24) & 0xFF;

  gdt_entries[num].limit = (limit & 0xFFFF);
  gdt_entries[num].flags = (limit >> 16) & 0x0F;
  gdt_entries[num].flags |= ((gran << 4) & 0xF0);

  gdt_entries[num].access = access;
}
