#include <stdint.h>
#include "io.h"
#include "tty32.h"
#include "pic.h"

#define PIT0_DATA_PORT	0x40
#define PIT1_DATA_PORT	0x41
#define PIT2_DATA_PORT	0x42

#define PIT_CMD_PORT	0x43

#define PIT_MODE3	0x36

#define PIT_100HZ_LO	(11931 & 0xFF)
#define PIT_100HZ_HI	(11931 >> 8)


void init_pit() {

    outb(PIT_CMD_PORT, PIT_MODE3);
    io_delay();
    outb(PIT0_DATA_PORT, PIT_100HZ_LO);
    io_delay();
    outb(PIT0_DATA_PORT, PIT_100HZ_HI);
    io_delay();
}

void irq0() {

    static int counter = 0;
    static int fraction = 99;

    char status[] = "JamesOS 0.1                                            uptime: 000 seconds";

    if (!--fraction) {

	fraction = 99;
	counter++;
	status[63] = (counter / 100) % 10 + '0';
	status[64] = (counter / 10) % 10 + '0';
	status[65] = (counter % 10) + '0';

	put_status(status);

    }
    
    pic_eoi(0);
}

