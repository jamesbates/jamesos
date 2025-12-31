#include "boot.h"

char a20_magic[] = {0x0F,0x1E,0x2D,0x3C};

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

REGPARAM3 void print_4bytes(uint16_t seg, uint16_t offset) {

    char *base;
    register int c;
    register char inc;

    printf("4 machine bytes at %X:%X are: ", seg, offset);
    
    base = (char *) offset;
    for (c=0; c < 4; c++) {

        ds(seg);
	__asm("movb (%1), %0\n" : "=a" (inc) : "S" (&base[c]));
	ds_cs();
	printf("0x%x ", inc, 0);
    }
    puts("\n");
}

void enable_a20_bios() {

    __asm__("int $0x15" : : "a" (0x2401));
}

void enable_a20_fast()
{
	char port_a;

	port_a = inb(0x92);
	port_a |=  0x02;	// Enable A20
	port_a &= ~0x01;	// Do not reset machine
	outb(port_a, 0x92);
        outb(port_a, 0x80);
}

void init_a20() {

/*    print_4bytes(0x0000, 0x7C00 + (uint16_t) &a20_magic[0]);
    print_4bytes(0xFFFF, 0x7C10 + (uint16_t) &a20_magic[0]);

    a20_magic[0] = 0xFF;
    a20_magic[1] = 0xEE;
    a20_magic[2] = 0xDD;
    a20_magic[3] = 0xCC;

    print_4bytes(0x0000, 0x7C00 + (uint16_t) &a20_magic[0]);
    print_4bytes(0xFFFF, 0x7C10 + (uint16_t) &a20_magic[0]);
*/
    enable_a20_bios();
    enable_a20_fast();

    a20_magic[0] = 0xF0;
    a20_magic[1] = 0xE1;
    a20_magic[2] = 0xD2;
    a20_magic[3] = 0xC3;

    print_4bytes(0x0000, 0x7C00 + (uint16_t) &a20_magic[0]);
    print_4bytes(0xFFFF, 0x7C10 + (uint16_t) &a20_magic[0]);
}
