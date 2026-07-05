#include "vga.h"
#include <stdint.h>

uint16_t column = 0;
uint16_t line = 0;

uint16_t *vga = (uint16_t *const)0xB800;
const uint16_t defaultColor = (COLOR8_BLACK << 8) | (COLOR8_LIGHT_GRAY << 12);

void clear_screen(void) {
  line = 0;
  column = 0;

  for (uint16_t y=0; y < HEIGHT; y++) {
    for (uint16_t x=0; x < WIDTH; x++) {
      vga[y * WIDTH + x] = ' ' | defaultColor;
    }
  }

}

void newLine() {
  if (line < HEIGHT -1) {
    line++;
    column=0;
  } else {
    scrollUp();
    column = 0;
  }
}

void scrollUp() {
  for (uint16_t y=0; y < HEIGHT; y++) {
    for (uint16_t x=0; x < WIDTH; x++) {
      vga[(y-1) * WIDTH + x] = vga[y * WIDTH + x];
    }
  }

  for (uint16_t x=0; x < WIDTH; x++) {
    vga[(HEIGHT-1) * WIDTH + x] = ' ' | defaultColor;
  }
}

void kprint(const char *str) {
  while (*str != '\0') {
    switch (*str) {
      case '\n':
        newLine();
        break;
      case '\r':
        column = 0;
        break;
      case '\t':
        if (column == WIDTH) {
          newLine();
        }
        uint16_t tabLen = 4 - (column % 4);
        while (tabLen != 0) {
          vga[line * WIDTH + (column++)] = ' ' | defaultColor;
          tabLen--;
        }
        break;
      default:
        if (column == WIDTH) {
          newLine();
        }
        vga[line * WIDTH + (column++)] = *str;
        break;
    }
    str++;
  }
}
