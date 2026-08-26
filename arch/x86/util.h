#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t val);
void memfill(void* dest, char val, uint32_t count);
void print_hex(char * val, char *label);

#endif // !UTIL_H
