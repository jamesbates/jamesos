#define outb(port,val) __asm__ ("outb %%al, %%dx\n" :: "a" (val), "d" (port))
#define inb(port,val) __asm__ ("inb %%dx, %%al\n" : "=a" (val) : "d" (port))
#define io_delay() outb(0x80, 0x00); outb(0x80, 0x00); outb(0x80, 0x00)
