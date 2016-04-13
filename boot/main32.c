#include "tty32.h"
#include "lib32.h"
#include "config.h"
#include "pic.h"
#include "pit.h"

void __attribute__((noreturn)) main32() {
    
    init_tty32();
    printf32("done!\n32-bit C-code startup.\n");

    put_status("JamesOS 0.1 ");    
#ifdef CONFIG_TEST_TTY    
    int n;
    for (n=0; n < 100; n++) {

        printf32("Useless line no. %d\n", n);
    }
    cls();
#endif
#ifdef CONFIG_TEST_FONT
    showfont();
#endif
    init_interrupt();
    __asm__("sti\n");
    printf32("Interrupts enabled.\n");
#ifdef CONFIG_TEST_INT
    __asm__("int $0x50");
#endif
    init_pit();
    pic_enable(1);
    printf32("Keyboard IRQ enabled.\n");
    pic_enable(0);
    printf32("Timer IRQ enabled.\n");
#ifdef CONFIG_TEST_DIV
    int i, i2;

    i = 0;
    i2 = 4;
    i2 /= i;
    printf32("Result after 0-divide: %d\n", i2);
#endif

    /* And here is our poor man's init "process" ;) */
    printf32("System ready.\n");
    while (1) {
    
        __asm__("hlt");
    }
}
