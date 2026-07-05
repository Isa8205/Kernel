#include <stdint.h>
#include "../vga.h"
#include "../idt.h"

extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t val);

void keyboard_callback(void) {
  uint8_t scancode = read_port(0x60);

  // For now, just print the scancode
  char* hex = "0x00";
  char* hex_chars = "0123456789ABCDEF";
  hex[2] = hex_chars[(scancode >> 4) & 0xF];
  hex[3] = hex_chars[scancode & 0xF];
  kprint(hex);

  send_eoi(1);
}
