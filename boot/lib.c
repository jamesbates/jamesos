#ifndef __LIB32__
#include "boot.h"
#else
#include "tty32.h"
#include <stdarg.h>
#define putchar putchar32
#define printf printf32
#endif


#ifndef __LIB32__
REGPARAM3 void printf(char *f, uint16_t val1, uint16_t val2) {
#else
void printf32(char *f, ...) {
#endif

    static char digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#ifndef __LIB32__
    uint16_t vals[] = {val1, val2};
    uint8_t nextval = 0;
#else
    va_list va;
    va_start(va, f);
#endif

    while (*f) {

	if (*f != '%') {

	    putchar(*f);
        } else {
#ifndef __LIB32__
	    uint16_t val = vals[nextval++];
#else
	    int val = va_arg(va, int);
#endif
	    switch (*++f) {

	        case 'x': putchar(digits[(val >> 4) & 0xF]);
		          putchar(digits[val & 0xF]);
			  break;
		case 'X': putchar(digits[(val >> 12) & 0xF]);
		          putchar(digits[(val >> 8) & 0xF]);
			  putchar(digits[(val >> 4) &0xF]);
			  putchar(digits[val & 0xF]);
			  break;
		case 'd': putchar(digits[(val/100) % 10]);
		          putchar(digits[(val/10) % 10]);
			  putchar(digits[val % 10]);
			  break;
	    }
	}
	f++;
    }
}

