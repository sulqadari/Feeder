#include "hal_flash.h"

void
FLASH_Init(void)
{

}

uint32_t
FLASH_readWord(uint32_t address)
{
    uint32_t aligned = (address + 3) & 0xFFFFFFC;
    uint32_t word = *(uint32_t*)aligned;
    return word;
}

uint16_t
FLASH_readHalf(uint32_t address)
{
    uint32_t aligned = (address + 2) & 0xFFFFFFE;
    uint16_t half = *(uint16_t*)aligned;
    return half;
}

uint8_t
FLASH_readByte(uint32_t address)
{
    uint8_t byte = *(uint8_t*)address;
    return byte;
}

static void
unlockFlash(void)
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
}

static void
lockFlash(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

Flash_SW
FLASH_massErase(uint32_t address)
{
    uint32_t aligned = (address + 3) & 0xFFFFFFC;

    unlockFlash();

    while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.
    if (FLASH->SR & FLASH_SR_EOP) {
        FLASH->SR &= FLASH_SR_EOP;
    }

    FLASH->AR |= aligned;       // select a page to erase
    FLASH->CR |= FLASH_CR_MER;  // Mass erase chosen
    FLASH->CR |= FLASH_CR_STRT; // Trigger the start of erase operation

    while (FLASH->SR & FLASH_SR_EOP) { ; }  // Wait until current (if any) memory operation being commited.
    FLASH->SR &= FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PER;  // Reset the 'Page erase chosen' bit
    
    lockFlash();
    return fsw_OK;
}

Flash_SW
FLASH_erasePage(uint32_t address)
{
    uint32_t aligned = (address + 3) & 0xFFFFFFC;

    unlockFlash();

    while (FLASH->SR & FLASH_SR_BSY) { ; }  // Wait until current (if any) memory operation being commited.
    if (FLASH->SR & FLASH_SR_EOP) {
        FLASH->SR &= FLASH_SR_EOP;
    }

    FLASH->CR |= FLASH_CR_PER;  // Page erase chosen
    FLASH->AR |= aligned;       // select a page to erase
    FLASH->CR |= FLASH_CR_STRT; // Trigger the start of erase operation

    while (FLASH->SR & FLASH_SR_EOP) { ; }  // Wait until current (if any) memory operation being commited.
    FLASH->SR &= FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PER;  // Reset the 'Page erase chosen' bit
    
    lockFlash();
    return fsw_OK;
}

Flash_SW
FLASH_write(uint32_t address, uint16_t data)
{
    uint32_t aligned = (address + 2) & 0xFFFFFFE;

    unlockFlash();

    while (FLASH->SR & FLASH_SR_BSY) { ; }

    if (FLASH->SR & FLASH_SR_EOP) {
        FLASH->SR &= FLASH_SR_EOP;
    }

    FLASH->CR |= FLASH_CR_PG;           // Set the 'Programming' flag
    *(uint16_t*)aligned = data;         // Assign a half-word

    if (FLASH->SR & FLASH_SR_PGERR) {   // Does assignment led to error because of non-FF state of the memory?
        FLASH->SR &= FLASH_SR_PGERR;    // Reset the PGERR flag
        FLASH->CR &= ~FLASH_CR_PG;      // Reset the 'Programming' flag
        lockFlash();
        return fsw_ERROR;
    }
    
    while (!(FLASH->SR & FLASH_SR_EOP)) { ; }   // Wait until current (if any) memory operation being commited.

    FLASH->SR &= FLASH_SR_EOP;                  // Reset the 'End Of Programming' flag
    FLASH->CR &= ~FLASH_CR_PG;                  // Reset the 'Programming' flag

    lockFlash();

    return fsw_OK;
}