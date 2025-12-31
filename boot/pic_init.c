#include "boot.h"
#include "io.h"

#define PIC0_CMD_PORT	0x20
#define PIC0_DATA_PORT	0x21

#define PIC1_CMD_PORT	0xA0
#define PIC1_DATA_PORT	0xA1

#define PICCMD_EOI	0x20
#define PICCMD_INIT	0x11

#define PIC0_INT_OFFSET 0x20
#define PIC1_INT_OFFSET 0x28

#define PIC0_CHAIN_ID	0x04
#define PIC1_CHAIN_ID	0x02

#define PIC_8086	0x01
void init_pic() {
    // Initialize both PICs
    outb(PIC0_CMD_PORT, PICCMD_INIT);
    io_delay();
    outb(PIC1_CMD_PORT, PICCMD_INIT);
    io_delay();
    outb(PIC0_DATA_PORT, PIC0_INT_OFFSET);
    io_delay();
    outb(PIC1_DATA_PORT, PIC1_INT_OFFSET);
    io_delay();
    outb(PIC0_DATA_PORT, PIC0_CHAIN_ID);
    io_delay();
    outb(PIC1_DATA_PORT, PIC1_CHAIN_ID);
    io_delay();
    outb(PIC0_DATA_PORT, PIC_8086);
    io_delay();
    outb(PIC1_DATA_PORT, PIC_8086);
    io_delay();

    // Full mask for both
    outb(PIC0_DATA_PORT, 0xFF);
    io_delay();
    outb(PIC1_DATA_PORT, 0xFF);
    io_delay();
}

