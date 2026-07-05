#include <stdint.h>

int current_loc = 0;
char *vidptr = (char*)0xb8000;

void printk(const char *str)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = 0x07;
	}
}

void kmain(void) {
  printk("Hello world\r\n");
  
  while(1);
}
