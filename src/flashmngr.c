#include "flashmngr.h"

uint32_t* flashBase;
uint32_t flashTotal;
static const uint32_t flashPagesCount = 64;
static const uint32_t flashPageSize = 1024;
const uint32_t flashBound = (0x08000000 + (flashPagesCount * flashPageSize));

void
mmInit(void)
{
	uint32_t aligned = (uint32_t)&_flashStartAddress;
    flashBase = (uint32_t*)((aligned + 0x3FF) & 0xFFFFFC00);
    flashTotal = flashBound - (uint32_t)flashBase;
}

static uint32_t
getCurrentBlock(void)
{
    mmHeader* header = (mmHeader*)flashBase;

    do {
        if (header->state != MM_STALED) {
            break;
        }

        header = (mmHeader*)((uint8_t*)header + sizeof(mmHeader) + header->size);
        if ((uint32_t)header >= flashBound) {
            return 0x00;
        }

    } while (1);
    return (uint32_t)header;
}

static uint32_t
getNextBlock(void)
{
    mmHeader* header = (mmHeader*)flashBase;

    do {
        if (header->size == MM_FREE) {
            break;
        }

        header = (mmHeader*)((uint8_t*)header + sizeof(mmHeader) + header->size);
        if ((uint32_t)header >= flashBound) {
            return 0x00;
        }

    } while (1);

    return (uint32_t)header;
}

static void
markPreviousAsStaled(mmHeader* curr)
{
    mmHeader* prev;
    
    // Corner case: we are at the very beginning of the flash.
    if ((uint32_t*)curr == flashBase) {
        return;
    }

    prev = (mmHeader*)flashBase;

    do {
        if (prev->state != MM_STALED) {
            FLASH_write((uint32_t)&prev->state, MM_STALED);
        }

        prev = (mmHeader*)((uint8_t*)prev + sizeof(mmHeader) + prev->size);
    } while (prev < curr);

}

Flash_SW
mmRead(uint8_t* data, uint32_t length)
{
    uint8_t* pData = data;
    uint32_t address = getCurrentBlock();
    if (address == 0x00)
        return fsw_ERROR;

    address += sizeof(mmHeader);
    
    for (uint32_t offset = address; offset < length; ++offset) {
        *pData = FLASH_readByte(offset);
        pData++;
    }
    return fsw_OK;
}

Flash_SW
mmWrite(uint16_t* data, uint16_t length)
{
    uint32_t address;
    mmHeader* header = NULL;
    uint16_t* pData = data;
    
    address = getNextBlock();

    if (address == 0x00) {    // There is no more free memory. Erase entire avilable memory.
        for (uint32_t curr = (uint32_t)flashBase; curr < flashBound; curr += flashPageSize) {
            FLASH_erasePage(curr);
        }
        header = (mmHeader*)flashBase;  // start writing from the very first page.
    } else {
        header = (mmHeader*)address;
        markPreviousAsStaled(header);
    }

    // Align the length before writing its value into the block.
    // This is crucial to get both 'getCurrectBlock()' and 'getNextBlock()' functions work properly.
    length = ((length + 2) & 0xFFFE);

    if (FLASH_write((uint32_t)&header->size, length) != fsw_OK) {
        return fsw_ERROR;
    }

    uint32_t body = (uint32_t)((uint8_t*)header + sizeof(mmHeader));

    for (uint32_t off = 0; off < length; off += 2) {
        if (FLASH_write(body + off, *pData) != fsw_OK) {
            return fsw_ERROR;
        }
        pData++;
    }

    return fsw_OK;
}