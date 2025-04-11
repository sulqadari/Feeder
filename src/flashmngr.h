#ifndef FLASHMAN_H
#define FLASHMAN_H

#include <stdint.h>
#include "hal_flash.h"
#include "string.h"

#define MM_STALED 0xDEAD
#define MM_FREE  0xFFFF

typedef struct {
    uint16_t size;
    uint16_t state;
} mmHeader;

extern uint32_t _flashStartAddress;
extern uint32_t* flashBase;
extern const uint32_t flashBound;

void mmInit(void);

#endif /* FLASHMAN_H */