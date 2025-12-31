#include "boot.h"
#include "mbr.h"

extern part_entry_t boot_part;
extern volatile uint8_t drive;

void main() {

    init_tty();
    printf("System loaded. Size is 0x%x sectors.\n", (uint16_t )(&syssectors)+1,0);
    drive_info();
    boot_part_info();
    init_a20();
    init_mem();
    print_mem();
    init_pic();
    puts("Hold on for protected mode... ");
    init_gdt();
    pmjump();
}   

void boot_part_info() {

    if (drive & 0x80) {
        printf("boot partition status=0x%x;type=0x%x;", boot_part.boot, boot_part.type);
	printf("first sector (lba)=0x%X;len=0x%X.\n", boot_part.start_lba, boot_part.len);
    }
}
