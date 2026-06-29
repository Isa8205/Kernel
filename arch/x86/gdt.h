#ifndef GDT_T
#define GDT_T
#include <stdint.h>

struct GDT_entry {
  uint16_t limit;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access;
  uint8_t flags;
  uint8_t base_high;
}__attribute__((packed));

struct GDT_ptr {
  uint16_t limit;
  uint32_t base;
}__attribute__((packed));

void init_Gdt(void);
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
extern void gdt_flush(struct GDT_ptr *gdt_ptr);

#endif // !GDT_T
