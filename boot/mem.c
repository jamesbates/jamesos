#include "boot.h"
#include <stdint.h>

uint16_t basemem;
uint16_t extmem;
uint16_t fullmem;

void init_mem() {

   __asm__("int $0x12" : "=a" (basemem));
   __asm__("int $0x15" : "=a" (extmem), "=b" (fullmem) : "a" (0xE881));
}

void print_mem() {

    printf("Base memory available: 0x%X 1K-blocks.\n", basemem);
    printf("Extended memory available (<16Mb): 0x%X 1K-blocks.\n", extmem);
    printf("Total memory available (>16Mb): 0x%X 64K-blocks.\n", fullmem);
}

