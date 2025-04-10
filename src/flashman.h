#ifndef FLASHMAN_H
#define FLASHMAN_H

#include <stdint.h>
#include "hal_flash.h"
#include "string.h"

#define MM_IS_STALE 0xDEAD
#define MM_IS_FREE  0xFFFF

typedef struct {
    uint16_t size;
    uint16_t stale;
} mmHeader;

void mmInit(void);

#endif /* FLASHMAN_H */