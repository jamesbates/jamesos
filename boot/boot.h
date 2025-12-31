#ifndef BOOT_H
#define BOOT_H
__asm__ (".code16gcc\n");

#define ds(val) __asm__ ("movw %0, %%ax\n\t" \
                         "movw %%ax, %%ds\n" :: "ir" (val) : "%eax")
#define ds_cs() __asm__ ("movw %%cs, %%ax\n\t" \
                         "movw %%ax, %%ds\n" ::: "%eax")
#define es(val) __asm__ ("movw %0, %%ax\n\t" \
                         "movw %%ax, %%es\n" :: "ir" (val) : "%eax")
#define es_cs() __asm__ ("movw %%cs, %%ax\n\t" \
                         "movw %%ax, %%es\n" ::: "%eax")
#define hlt() for(;;) __asm__ ("hlt")

#define FD0 0
#define NORETURN __attribute__((noreturn))
#define REGPARAM3 __attribute((regparm(3)))
#include <stdint.h>

#define outb(port,val) __asm__ ("outb %%al, %%dx\n" :: "a" (val), "d" (port))
static inline char inb(uint16_t port) {
    char v;

    __asm__("inb %%dx, %%al\n" : "=a" (v) : "d" (port));
    return v;
}

void init_tty() REGPARAM3;
void putchar(char c) REGPARAM3;
void puts(char *s) REGPARAM3;
void putint(uint8_t val) REGPARAM3;
void printf(char *f, ...);
void print_direct(char *s);
void read_sectors(uint16_t lba, void *buffer, uint8_t num) REGPARAM3;
void fd_init() REGPARAM3;
void main();
void drive_info();
void init_a20();
void boot_part_info();
void init_boot_part() REGPARAM3;
void init_idt_gdt();
void pmjump() NORETURN;
void init_mem();
void print_mem();
void init_pic();
void init_gdt();

extern uint8_t syssectors;
#endif
