#include "vga.h"
#include <stdint.h>

void memfill(void* dest, char val, uint32_t count) {
  char *temp = (char *)dest;
  for (; count != 0; count--) {
    *temp++ = val;
  }
}

void print_hex(char *val, char *label) {
  char* hex = "0x00";
  char* hex_chars = "0123456789ABCDEF";
  hex[2] = hex_chars[(*val >> 4) & 0xF];
  hex[3] = hex_chars[*val & 0xF];
  kprint(label);
  kprint(hex);
  kprint("\n");
}
