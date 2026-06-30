#include "display.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

unsigned int current_loc = 0;

char *vidptr = (char *)0xb8000;

void clear_screen(void) {
  for (unsigned int i=0; i<SCREENSIZE; i++) {
    vidptr[i] = ' ';
    vidptr[i+1] = 0x07;
    i += BYTES_FOR_EACH_ELEMENT;
  }
}

void kprint(const char *str) {
  unsigned int j = 0;

  while (str[j++] != '\0') {
    vidptr[current_loc++] = str[j];
    vidptr[current_loc++] = 0x07;
  }
}
