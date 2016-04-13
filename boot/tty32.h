#ifndef TTY32_H
#define TTY32_H
#include <stdint.h>

void putchar32(char c);
void puts32(char *s);
void put_status(char *s);
void init_tty32();
void cls();
void showfont();

#endif

