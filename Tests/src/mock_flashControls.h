#ifndef MOCK_FLASHCONTROLS_H_
#define MOCK_FLASHCONTROLS_H_

#include "bmsConfiguration.h"

bmsStatus_t flashWrite(uint32_t startAddress, __IO uint64_t *data, uint16_t dataSize);
bmsStatus_t flashRead(uint64_t startAddress, __IO uint64_t *data, uint16_t dataSize);
bmsStatus_t flashErasePage(uint32_t pageNumber, uint32_t numberOfPages, uint8_t bank);
bmsStatus_t flashErase(uint32_t startAddress);

#endif