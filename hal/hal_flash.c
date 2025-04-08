#include "hal_flash.h"

void
FLASH_Init(void)
{

}

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

static Flash_SW
FLASH_pageErase(uint32_t pageAddr)
{
    uint32_t aligned = (pageAddr + 3) & 0xFFFFFFC;
    while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.

    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR |= aligned;   // select a page to erase
    FLASH->CR |= FLASH_CR_STRT;

    while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.
    FLASH->SR &= FLASH_SR_EOP;
}

static Flash_SW
FLASH_unlock(void)
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    return fsw_OK;
}

static Flash_SW
FLASH_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
    return fsw_OK;
}

Flash_SW
FLASH_write(uint32_t addr, uint16_t data)
{
    uint32_t aligned = (addr + 2) & 0xFFFFFFE;
    uint16_t temp = 0;
    FLASH_unlock();

    while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.
    FLASH->CR |= FLASH_CR_PG;

    while (1) {
        *(uint16_t*)aligned = data;

        if (FLASH->SR & FLASH_SR_PGERR) {
            FLASH_pageErase(addr);
            FLASH->SR &= FLASH_SR_PGERR;
            continue;
        }

        while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.
        break;
    }


    FLASH->SR &= FLASH_SR_EOP;
    FLASH_lock();
}