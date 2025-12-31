#include <stdint.h>
#include "tty32.h"
#include "pic.h"
#include "io.h"
#include "config.h"
#include "lib32.h"

#define MOD_PLAIN 0
#define MOD_SHIFT 1
#define MOD_ALTGR 2

static uint8_t keyb_mod = MOD_PLAIN;

const unsigned char us_layout[4][0x80] = {
    [MOD_PLAIN] = "\0\0" \
    		  "1234567890-=\b" \
    		  "\tqwertyuiop[]\n" \
    		  "\0asdfghjkl;'`\0\\" \
    		  "zxcvbnm,./\0" \
    		  "\0\0 ",
    [MOD_SHIFT] = "\0\0" \
    		  "!@#$%^&*()_+\b" \
    		  "\tQWERTYUIOP{}\n" \
    		  "\0ASDFGHJKL:\"~\0|" \
    		  "ZXCVBNM<>?\0" \
    		  "\0\0 ",
    [MOD_ALTGR] = "\0\0" \
    		  "1234567890-=\b" \
    		  "\tqwerty\x81i\x94p[]\n" \
    		  "\0\x84\xE1""dfghjkl;'`\0\\" \
    		  "zxcvbnm,./\0" \
    		  "\0\0 ",
    [MOD_SHIFT | MOD_ALTGR] = "\0\0" \
    		  "!@#$%^&*()_+\b" \
    		  "\tQWERTY\x9AI\x99P{}\n" \
    		  "\0\x8E\xE1""DFGHJKL:\"~\0|" \
    		  "ZXCVBNM<>?\0" \
    		  "\0\0 "
    };


void irq1() {

    uint8_t keyb_data;
    inb(0x060, keyb_data);
    if ((keyb_data == 0x2A) || (keyb_data == 0x36)) {
    
        keyb_mod |= MOD_SHIFT;
    } else if((keyb_data ==0xAA) || (keyb_data == 0xB6)) {

        keyb_mod &= ~MOD_SHIFT;
    } else if (keyb_data == 0xE0) {
        if (keyb_mod & MOD_ALTGR) {
	    keyb_mod &= ~MOD_ALTGR;
	} else {
	    keyb_mod |= MOD_ALTGR;
	}
    } else if (keyb_data < 0x80) {
	putchar32(us_layout[keyb_mod][keyb_data]);
    }
#ifdef CONFIG_TEST_SCANCODES
    printf32("0x%x ", keyb_data);
#endif
    pic_eoi(1);
}

