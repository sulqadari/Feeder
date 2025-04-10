#include "flashman.h"

extern uint32_t* _flashStartAddress;
static uint32_t* flashBase;
static uint32_t flashTotal;
static const uint32_t flashPagesCount = 64;
static const uint32_t flashPageSize = 1024;
static const uint32_t flashBound = (0x08000000 + (flashPagesCount * flashPageSize));

void
mmInit(void)
{
    flashBase = (uint32_t*)(*_flashStartAddress & 0x3FFF);  // align with page size
    flashTotal = flashBound - (uint32_t)flashBase;
}

#if (0)
static uint16_t
getShort(uint8_t* array)
{
    return ((array[0] << 8) | (array[1] & 0xFF));
}
#endif

static uint32_t
findCurrent(void)
{
    mmHeader* header = (mmHeader*)flashBase;
    mmHeader* next = header;

    do {
        if (next->stale != MM_IS_STALE) {
            break;
        }

        next = (mmHeader*)((uint8_t*)header + header->size + sizeof(mmHeader));
        if ((uint32_t)next >= flashBound) {
            return 0x00;
        }

    } while (1);
    return (uint32_t)header;
}

static uint32_t
findNext(void)
{
    mmHeader* header = (mmHeader*)flashBase;
    mmHeader* next = header;
    do {
        if (next->size == MM_IS_FREE) {
            break;
        }

        next = (mmHeader*)((uint8_t*)header + header->size + sizeof(mmHeader));
        if ((uint32_t)next >= flashBound) {
            return 0x00;
        }

    } while (1);
    return (uint32_t)header;
}

Flash_SW
mmRead(uint8_t* data, uint32_t length)
{
    uint32_t address = findCurrent();
    if (address == 0x00)
        return fsw_ERROR;

    address += sizeof(mmHeader);
    
    for (uint32_t i = address; i < length; ++i) {
        data[i] = FLASH_readByte(address + i);
    }
    return fsw_OK;
}

Flash_SW
mmWrite(uint8_t* data, uint32_t length)
{
    mmHeader* header = NULL;
    uint8_t* pData = data;
    uint32_t address = findCurrent();
    if (address == 0x00)
        return fsw_ERROR;
    
    header = (mmHeader*)address;
    
    if (FLASH_write((uint32_t)&header->stale, MM_IS_STALE) != fsw_OK) {
        return fsw_ERROR;
    }

    address = findNext();
    if (address == 0x00) {    // There is no more free memory. Erase all.
        for (uint32_t curr = (uint32_t)flashBase; curr < flashBound; curr += flashPageSize) {
            FLASH_erasePage(curr);
        }
        header = (mmHeader*)flashBase;  // start writing from the very first page.
    }

    if (FLASH_write((uint32_t)&header->size, length) != fsw_OK) {
        return fsw_ERROR;
    }

    // TODO: the data to be written must be alligned to two bytes.
    for (uint32_t curr = (uint32_t)((uint8_t*)header + sizeof(mmHeader)); curr < length; curr += 2) {
        FLASH_write(curr, ((pData[0] << 8) | (pData[1] & 0xFF)));
        pData += 2;
    }

    return fsw_OK;
}