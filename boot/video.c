#include "boot.h"
#include <stdint.h>

static uint16_t c_pos = 320;

void print_direct(char *s) {
    
    es(0xB800);
    uint16_t *video_memory = (uint16_t *) 0x0;
    
    while (*s) { 
	
	if (*s == '\n') {
	
	    c_pos = ((c_pos / 80) + 1) * 80;
	    s++;
	} else {
	    __asm__ ("movw %0, %%es:(%1)" :: "r" (0x0C00 | *s++), "D" (video_memory+(c_pos++)));
	}
    }
    
    es_cs();

    outb(0x3D4, 14);  //ster 14 first
    outb(0x3D5, (c_pos>>8) & 0xFF); // output high byte
    outb(0x3D4, 15);                    // again to register 15
    outb(0x3D5, c_pos & 0xFF);
}
