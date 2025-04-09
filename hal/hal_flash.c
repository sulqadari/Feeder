#include "hal_flash.h"

void
FLASH_Init(void)
{

}

Flash_SW
FLASH_readWord(uint32_t addr, uint32_t* word)
{
    uint32_t aligned = (addr + 3) & 0xFFFFFFC;
    *word = *(uint32_t*)aligned;
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
erasePage(uint32_t pageAddr)
{
    uint32_t aligned = (pageAddr + 3) & 0xFFFFFFC;

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
    return fsw_OK;
}

static Flash_SW
unlockFlash(void)
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    return fsw_OK;
}

static Flash_SW
lockFlash(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
    return fsw_OK;
}

static Flash_SW
tryToWrite(uint32_t addr, uint32_t aligned, uint16_t data)
{
    FLASH->CR |= FLASH_CR_PG;               // Set the 'Programming' flag
    do {
        *(uint16_t*)aligned = data;         // Assign a half-word

        if (FLASH->SR & FLASH_SR_PGERR) {   // Does assignment led to error because of non-FF state of the memory?
            erasePage(addr);                // clear the page
            FLASH->SR &= FLASH_SR_PGERR;    // Reset the PGERR flag
            continue;                       // Try to write again
        }

        while (!(FLASH->SR & FLASH_SR_EOP)) { ; }   // Wait until current (if any) memory operation being commited.
        FLASH->SR &= FLASH_SR_EOP;                  // Reset the 'End Of Programming' flag
        FLASH->CR &= ~FLASH_CR_PG;                  // Reset the 'Programming' flag
    } while (0);
    return fsw_OK;
}

Flash_SW
FLASH_write(uint32_t addr, uint16_t data)
{
    uint32_t aligned = (addr + 2) & 0xFFFFFFE;

    unlockFlash();

    while (FLASH->SR & FLASH_SR_BSY) { ; }
    if (FLASH->SR & FLASH_SR_EOP) {
        FLASH->SR &= FLASH_SR_EOP;
    }

    tryToWrite(addr, aligned, data);

    lockFlash();
    return fsw_OK;
}