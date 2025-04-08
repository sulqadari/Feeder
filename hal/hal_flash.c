#include "hal_flash.h"

Flash_SW
FLASH_readWord(uint32_t addr, uint32_t* word)
{
    uint32_t aligned = (addr + 3) & 0xFFFFFFC;
    *word = (uint32_t*)aligned;
    return fsw_OK;
}

Flash_SW
FLASH_readHalf(uint32_t addr, uint16_t* half)
{
    uint32_t aligned = (addr + 2) & 0xFFFFFFE;
    *half = *(uint16_t*)aligned;
    return fsw_OK;
}

Flash_SW
FLASH_readByte(uint32_t addr, uint8_t* byte)
{
    *byte = *(uint8_t*)addr;
    return fsw_OK;
}