#include "boot.h"
#include "tty.h"
#include "config.h"

void REGPARAM3 putchar(char c) {

    if (c == '\n') putchar('\r');
    __asm__ ("int $0x10\n" : : "a" (0x0E00 | c));
}

void REGPARAM3 puts(char *s) {

    while (*s) {

	putchar(*s++);
    }
}

void REGPARAM3 init_tty() {

#ifdef CONFIG_LINES_50
    /* Choose 8x8 font */
    __asm__("int $0x10" : : "a" (0x1112), "b" (0x0000));
#endif

    /* Set cursor size, scan lines 0 till 14 */
    __asm__("int $0x10" : : "a" (0x0100), "c" (0x000E));

    /* Set attribute of visible display memory */
    es(0xB800);
    __asm__("1: stosb\t\nincw %%di\n\tloop 1b" : : "a" (VGA_ATTRIBUTE), "c" (80 * VGA_LINES), "D" (0x0001));
    es_cs();
}
