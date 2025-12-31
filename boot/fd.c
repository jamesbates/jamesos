#include "boot.h"

volatile uint8_t drive;
uint16_t sectors;
uint16_t cyls;
uint16_t heads;

/*typedef struct dap_t {

    uint16_t size;
    uint16_t sectors;
    uint16_t o_databuf;
    uint16_t s_databuf;
    uint32_t lba_lo;
    uint32_t lba_hi;
} dap_t;
*/

void REGPARAM3 fd_init() {
    
    __asm__("xorw %%bx, %%bx\n\t"
            "movw %%bx, %%es\n\t"
	    "int $0x13\n\t"
	    "movw %%cs, %%bx\n\t"
	    "movw %%bx, %%es" : "=d" (heads), "=c" (sectors) : "d" (drive), "a" (0x0800) : "%di","%bx");
    heads = (heads >> 8) + 1;
    cyls = sectors;
    sectors &= 0x3F;
    cyls = (cyls >> 8) | ((cyls << 2) & 0x0300);
    cyls++;
}

void drive_info() {
    printf("boot drive=0x%x;cyls=%d;", drive, cyls);
    printf("heads=%d;sectors=%d\n", heads, sectors);
}

void REGPARAM3 read_sectors(uint16_t lba, void *buffer, uint8_t num) {
/*    static dap_t dap;

    dap.size = 0x10;
    dap.sectors = num;
    dap.o_databuf = (uint16_t) buffer;
    __asm__ ("movw %%ds, %0" : "=a" (dap.s_databuf));
    dap.lba_lo = lba;
    __asm__ ("int $0x13" : : "a" (0x4200), "S" (&dap), "d" (drive));
*/


    uint16_t cyl = lba / sectors / heads;
    uint16_t head = (lba / sectors) % heads;
    uint16_t sector = (lba % sectors) + 1;

    __asm__ ("int $0x13" : : "a" (0x0200 | num), "c" ((cyl << 8) | sector), "d" ((head << 8) | drive), "b" (buffer));





}
