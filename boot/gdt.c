#include "boot.h"
#include "gdt.h"

typedef struct __attribute__((__packed__)) gdt_entry_t {
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_hi0;
    uint8_t flags0;
    uint8_t limit_hi_flags1;
    uint8_t base_hi1;
} gdt_entry_t;


gdt_entry_t gdt[] __attribute__((aligned(16))) = {
    [GDT_KERN_CODE] = {.limit_lo = 0xFFFF, .base_lo = 0x7C00, .base_hi0 = 0x00, .flags0 = FLAGS0_KERN_CODE, .limit_hi_flags1 = 0xF | FLAGS1_KERN, .base_hi1 = 0x00},
    [GDT_KERN_DATA] = {.limit_lo = 0xFFFF, .base_lo = 0x7C00, .base_hi0 = 0x00, .flags0 = FLAGS0_KERN_DATA, .limit_hi_flags1 = 0xF | FLAGS1_KERN, .base_hi1 = 0x00}
};

typedef struct __attribute__((__packed__)) dtr_t {
    uint16_t size;
    uint32_t address;
} dtr_t;

void init_gdt() {

    dtr_t idtr __attribute__((aligned(16))) = {
        .size = 0x0000,
	.address = 0x0000000
    };

    static dtr_t gdtr __attribute__((aligned(16))) = {
    	.size = sizeof(gdt) - 1,
	.address = (uint32_t) gdt + 0x7C00
    };

    __asm__("lidt %0" : : "m" (idtr));
    __asm__("lgdt %0" : : "m" (gdtr));
}
