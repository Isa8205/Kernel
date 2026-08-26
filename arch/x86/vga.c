#include "vga.h"
#include <stdint.h>

uint16_t column = 0;
uint16_t line = 0;

volatile uint16_t *vga = (volatile uint16_t *const)0xB8000;
const uint16_t defaultColor = ((COLOR8_LIGHT_GRAY & 0x0F) << 8) | ((COLOR8_BLACK & 0x0F) << 12);

void clear_screen(void) {
  line = 0;
  column = 0;

  for (uint16_t y = 0; y < HEIGHT; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      vga[y * WIDTH + x] = ' ' | defaultColor;
    }
  }
}

void scrollUp(void) {
  for (uint16_t y = 1; y < HEIGHT; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      vga[(y - 1) * WIDTH + x] = vga[y * WIDTH + x];
    }
  }

  for (uint16_t x = 0; x < WIDTH; x++) {
    vga[(HEIGHT - 1) * WIDTH + x] = ' ' | defaultColor;
  }
}

void newLine(void) {
  if (line < HEIGHT - 1) {
    line++;
    column = 0;
  } else {
    scrollUp();
    column = 0;
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
        if (column >= WIDTH) {
          newLine();
        }
        uint16_t tabLen = 4 - (column % 4);
        while (tabLen != 0 && column < WIDTH) {
          vga[line * WIDTH + (column++)] = ' ' | defaultColor;
          tabLen--;
        }
        break;
      case '\b':
        column--;
        vga[line * WIDTH + column] = ' ' | defaultColor;
      default:
        if (column >= WIDTH) {
          newLine();
        }
        vga[line * WIDTH + (column++)] = (uint8_t)*str | defaultColor;
        break;
    }
    str++;
  }
}
