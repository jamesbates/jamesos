#include <stdint.h>
#include "io.h"
#include "tty.h"

#define CRTC_ADDRESS_PORT 0x3D4
#define CRTC_DATA_PORT 0x3D5

#define CRTC_CURPOS_LO 0x0F
#define CRTC_CURPOS_HI 0x0E
#define CRTC_START_ADDRESS_HI 0x0C
#define CRTC_START_ADDRESS_LO 0x0D

static inline unsigned char crtc_read(unsigned char index) {

    volatile unsigned char v;

    outb(CRTC_ADDRESS_PORT, index);
    io_delay();
    inb(CRTC_DATA_PORT, v);
    return v;
}

static inline void crtc_write(unsigned char index, unsigned char val) {

    outb(CRTC_ADDRESS_PORT, index);
    io_delay();
    outb(CRTC_DATA_PORT, val);
}

static uint16_t curpos32;
static uint16_t *video32 = (uint16_t *) (0xB8000 - 0x7C00);    

static void setcurpos32(uint16_t newpos) {

    curpos32 = newpos;
    crtc_write(CRTC_CURPOS_LO, curpos32 & 0xFF);
    crtc_write(CRTC_CURPOS_HI, curpos32 >> 8);
}

static void scroll(short int lines);

void putchar32(unsigned char c) {


    if (c == '\0') {

        return;
    } else if (c == '\n') {
 
        setcurpos32(curpos32 + 80);
	putchar32('\r');
    } else if (c == '\r') {
    
        setcurpos32(80 * (curpos32 / 80));
    } else if (c == '\b') {
    
        setcurpos32(curpos32-1);
    } else if (c == '\t') {
    
        setcurpos32(8 * (curpos32 / 8) + 8);
    } else {
 
        video32[curpos32] = (VGA_ATTRIBUTE<<8) | c;
	setcurpos32(curpos32+1);
    }

    if (curpos32 >= (80 * VGA_LINES)) {

        scroll(1);
    }
}

void puts32(unsigned char *s) {

    while (*s) {

        putchar32(*s++);
    }
}

void put_status(unsigned char *s) {

    int c = 0;
    while ((c < 80) && (*s)) {

	video32[VGA_LINES*80+c++] = (VGA_STATUS_ATTRIBUTE<<8) | (*s++);
    }
    while (c < 80) {

        video32[VGA_LINES*80+c++] = (VGA_STATUS_ATTRIBUTE<<8) | ' ';
    }
}

void showfont() {

    puts32("Text fontmap in use: ");
    uint16_t c = 0;
 
    while (c < 0x100) {
        video32[curpos32] = (VGA_ATTRIBUTE<<8) | c++;
	setcurpos32(curpos32+1);

        if (curpos32 >= (80 * VGA_LINES)) {
            scroll(1);
	}
    }
    putchar32('\n');
}

void init_tty32() {
    
    curpos32 = (((uint16_t) crtc_read(CRTC_CURPOS_HI)) << 8) | ((uint16_t) crtc_read(CRTC_CURPOS_LO));
    __asm__("1: incl %%edi; stosb; loop 1b" : : "a" (VGA_ATTRIBUTE), "c" (80 * VGA_LINES), "D" (video32));
    put_status("");
}

void cls() {

    __asm__("rep stosw" : : "a" (VGA_ATTRIBUTE << 8), "c" (80 * VGA_LINES), "D" (video32));
    setcurpos32(0);
}

static void scroll(short int lines) {

    __asm__("rep movsw" : : "S" (video32 + 80), "D" (video32), "c" ((VGA_LINES-1)*80));
    __asm__("rep stosw" : : "a" (VGA_ATTRIBUTE << 8), "c" (80), "D" (video32 + (VGA_LINES-1)*80));
    setcurpos32(curpos32-80);
    if (lines > 1) {

        scroll(lines-1);
    }
}

