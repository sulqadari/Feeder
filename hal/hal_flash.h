#ifndef FLASH_HAL_H
#define FLASH_HAL_H

#include "hal_gpio.h"
#include "hal_rcc.h"

typedef enum {
    fsw_OK,
    fsw_ERROR
} Flash_SW;

Flash_SW FLASH_readWord(uint32_t addr, uint32_t* word);
Flash_SW FLASH_readHalf(uint32_t addr, uint16_t* half);
Flash_SW FLASH_readByte(uint32_t addr, uint8_t* byte);

Flash_SW FLASH_write(uint32_t addr, uint16_t data);

#endif /* FLASH_HAL_H */
