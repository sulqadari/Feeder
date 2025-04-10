#ifndef FLASH_HAL_H
#define FLASH_HAL_H

#include "hal_gpio.h"
#include "hal_rcc.h"

typedef enum {
    fsw_OK,
    fsw_ERROR
} Flash_SW;

uint32_t FLASH_readWord(uint32_t addr);
uint16_t FLASH_readHalf(uint32_t addr);
uint8_t FLASH_readByte(uint32_t addr);
Flash_SW FLASH_erasePage(uint32_t address);
Flash_SW FLASH_massErase(uint32_t address);
Flash_SW FLASH_write(uint32_t addr, uint16_t data);

#endif /* FLASH_HAL_H */
