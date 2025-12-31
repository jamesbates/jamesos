#include <stdint.h>
#include "gdt.h"
#include "tty32.h"
#include "pic.h"
#include "io.h"

extern void printf32(char *fmt, ...);

extern void int_divide_zero();
extern void int_default_isr();
extern void int_irq1();
extern void int_irq0();

typedef struct __attribute__((__packed__)) idt_entry_t {

    uint16_t addr0;
    uint16_t cs;
    uint8_t zero;
    uint8_t type_flags;
    uint16_t addr1;
} idt_entry_t;

typedef struct __attribute__((__packed__)) dtr_t {

    uint16_t size;
    uint32_t address;
} dtr_t;

void divide_zero() {

    puts32("Divide by zero!\n");
    while(1) __asm__("hlt");
}

void default_isr() {

    puts32("Default ISR\n");
}

idt_entry_t idt[256] __attribute__((aligned(16)));

static void load_idt_entry(idt_entry_t *idt_entry, void *isr) {

    uint32_t isr_addr = (uint32_t) isr;

    (*idt_entry).addr0 = (uint16_t) (isr_addr & 0xFFFF);
    (*idt_entry).addr1 = (uint16_t) (isr_addr >> 16);
    (*idt_entry).cs = __KERN_CS;
    (*idt_entry).zero = 0x00;
    (*idt_entry).type_flags = 0x8F;
}

void init_interrupt() {

    uint16_t intno;

    static dtr_t idtr __attribute__((aligned(16))) = {
	.size = sizeof(idt) - 1,
	.address = 0x7C00 + (uint32_t) idt
    };

    load_idt_entry(&idt[0x00], &int_divide_zero);
    for (intno=1; intno < 256 ; intno++) {

	load_idt_entry(&idt[intno], &int_default_isr);
    }
    load_idt_entry(&idt[0x20], &int_irq0);
    load_idt_entry(&idt[0x21], &int_irq1);    
    __asm__("lidt %0\n" : : "m" (idtr));
}

