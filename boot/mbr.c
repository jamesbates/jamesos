#include "boot.h"

#include "mbr.h"

volatile uint16_t s_boot_part;
volatile uint16_t o_boot_part;
extern volatile uint8_t drive;

volatile part_entry_t boot_part;

void REGPARAM3 init_boot_part() {

    if (drive & 0x80) {

	__asm__("pushw %%ds\n\t"
		"movw %%ax, %%ds\n\t"
		"rep movsb\n\t"
		"popw %%ds\n" : : "a" (s_boot_part), "c" (sizeof(boot_part)), "S" (o_boot_part), "D" (&boot_part));
    }
}
