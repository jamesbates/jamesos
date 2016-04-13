#include "boot.h"
#include "mbr.h"

extern volatile uint8_t drive;
extern part_entry_t boot_part;

/* Needed since we cast pointers to 16-bit real-mode offsets, although gcc thinks pointers are 32-bit */
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

/**
 * BIOS will have loaded first sector at 07C0:0000. setup() loads the remaining sectors behind the
 * first one, and calls main() in main.c, which is the main 16-bit boot code.
 */
void NORETURN setup() {

    fd_init();
    init_boot_part();
    read_sectors(boot_part.start_lba + 1, (void *) 0x0200, (uint8_t) &syssectors);
    main();
    hlt();
}   

