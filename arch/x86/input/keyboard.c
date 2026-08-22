#include <stdint.h>
#include "../vga.h"
#include "../idt.h"
#include "../util.h"

void keyboard_callback(void);

void keyboard_init(void) {
  irq_register_handler(1, (void *)keyboard_callback);
  kprint("[*] Keyboard Initialization done");
}

void keyboard_callback(void) {
  uint8_t scancode = read_port(0x60);

  // For now, just print the scancode
  char* hex = "0x00";
  char* hex_chars = "0123456789ABCDEF";
  hex[2] = hex_chars[(scancode >> 4) & 0xF];
  hex[3] = hex_chars[scancode & 0xF];
  kprint(hex);
}
