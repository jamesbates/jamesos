#include "boot.h"

#include "mbr.h"

volatile uint16_t s_boot_part;
volatile uint16_t o_boot_part;
extern volatile uint8_t drive;

part_entry_t boot_part;

void REGPARAM3 init_boot_part() {

    int c;
    char *p = (char *) &boot_part;
    es(s_boot_part);
    if (drive & 0x80) {
       for (c=0; c < sizeof(boot_part); c++) {

           __asm__("movb %%es:(%1), %%al\n\t"
                   "movb %%al, (%0)\n" : : "d" (p + c), "b" (o_boot_part + c) : "%eax");
        }
    }
    es_cs();
}
/*
void boot_part_info() {

    printf("boot: 0x%x. type: 0x%x. first sector (lba): 0x%X. len: 0x%X.\n", boot_part.boot, boot_part.type, boot_part.start_lba, boot_part.len);
}
*/
