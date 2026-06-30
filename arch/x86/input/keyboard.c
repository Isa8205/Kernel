#include <stdint.h>
#include "../display.h"

extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t val);

// static inline uint8_t inb(uint16_t port) {
//   uint8_t ret;
//   __asm__ __volatile__("in %1, %1"
//       : "=a"(ret)
//       : "Nd"(port));
//
//   return ret;
// }
//
// static inline void outb(uint16_t port, uint8_t val) {
//   __asm__ __volatile__("outb %0, %1"::"a"(val), "Nd"(port));
// }

void keyboard_callback(void) {
  uint8_t scancode = read_port(0x60);

  kprint((char *)scancode);

  write_port(0x20, 0x20);
}
