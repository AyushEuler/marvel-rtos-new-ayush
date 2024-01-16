#include "ota.h"
#include "memoryMap.h"
#include "main.h"
#include "flashControls.h"
#include "bmsConfiguration.h"


extern bmsStatus_t status;

/**
 * @brief Clear APP1 Partition
 *
 * @return uint8_t 0 = Failure, 1 = success
 */
uint8_t clearApp1()
{
	uint32_t address = (uint32_t)&__app1_start__;
	// uint8_t res;
	RETRY(RETRY_OTA, status, flashErase(address));
	return 1;
}

/**
 * @brief Clear APP2 Partition
 *
 * @return uint8_t 0 = Failure, 1 = success
 */
uint8_t clearApp2()
{
	// uint8_t res;
	uint32_t address = (uint32_t)&__app2_start__;
	RETRY(RETRY_OTA, status, flashErase(address));
	return 1;
}
