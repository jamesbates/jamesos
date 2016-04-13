#ifndef MBR_H
#define MBR_H

#include <stdint.h>

typedef struct __attribute__((__packed__)) chs_t {
    uint8_t head;
    uint8_t cyl1_sector;
    uint8_t cyl0;
} chs_t;

typedef struct __attribute__((__packed__)) part_entry_t {
    uint8_t boot;
    chs_t start_chs;
    uint8_t type;
    chs_t end_chs;
    uint32_t start_lba;
    uint32_t len;
} part_entry_t; 

#endif
