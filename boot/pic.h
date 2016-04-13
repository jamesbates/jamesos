#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_eoi(uint8_t irq);
void pic_enable(uint8_t irq);
void pic_disable(uint8_t irq);

#endif
