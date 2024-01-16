/**
 * @file flashControls.c
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "flashControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint32_t GetPage(uint32_t address);
/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief Write to Flash memory
 * @param startAddress starting address of the Flash memory
 * @param writeData pointer to data to be written
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */

bmsStatus_t flashWrite(uint32_t startAddress, __IO uint64_t *data, uint16_t dataSize)
{
	bmsStatus_t status = BMS_ERROR;

	if( !(IS_FLASH_PROGRAM_ADDRESS(startAddress)) 	||\
		(NULL == data) 								||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	status = flashErase(startAddress);
	CHECK_STATUS(status);

	FLASH_UNLOCK();
	for(uint16_t index = 0; index < dataSize && BMS_OK == status; index++)
	{
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress, data[index]);
		startAddress += 8;
	}
	FLASH_LOCK();

	if(status != BMS_OK)
	{
		status = BMS_ERROR;			// HAL_FLASH_GetError();
	}

	return status;
}

bmsStatus_t flashWriteOTA(uint32_t startAddress, __IO uint64_t *data, uint16_t dataSize)
{
	bmsStatus_t status = BMS_OK;
	FLASH_UNLOCK();
	//HAL_Delay(3);
		for(uint16_t index = 0; index < dataSize; index++)
		{
			
			//HAL_Delay(5);
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress, data[index]);
			while(status != BMS_OK){
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress, data[index]);
			}
			startAddress += 8;
		}
	FLASH_LOCK();

	if(status != BMS_OK)
	{
		status = BMS_ERROR;			// HAL_FLASH_GetError();
	}

	return status;

}







/* Function definition -------------------------------------------------------*/
/**
 * @brief Read From Flash memory
 * @param startAddress starting address of the Flash memory
 * @param data pointer to data to be read
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */

bmsStatus_t flashRead(uint64_t startAddress, __IO uint64_t *data, uint16_t dataSize)
{
	bmsStatus_t status = BMS_ERROR;

	// if( !(IS_FLASH_PROGRAM_ADDRESS(startAddress)) )
	// {
	// 	status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	for(int i = 0; i < dataSize; i++)
	{
		*data = *(__IO uint64_t *)(uintptr_t)startAddress;
		startAddress += 8;
		data++;
	}

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Erase flash pages
 * @param pageNumber page number
 * @param numberOfPages number of pages
 * @param bank bank number
 * @retval BMS status
 */
bmsStatus_t flashErasePage(uint32_t pageNumber, uint32_t numberOfPages, uint8_t bank)
{
	bmsStatus_t status = BMS_ERROR;
	FLASH_EraseInitTypeDef EraseInitStruct = {0};

	if( (numberOfPages <= 0) 		 ||\
		!(IS_FLASH_BANK(bank)))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	FLASH_UNLOCK();
	uint32_t pageError			= 0;
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page 		= pageNumber;
	EraseInitStruct.NbPages 	= numberOfPages;
	EraseInitStruct.Banks 		= bank;
	FLASH->SR 					= 0xFFFFFFFF;

	status = HAL_FLASHEx_Erase(&EraseInitStruct, &pageError);
	FLASH_LOCK();

	if(status != BMS_OK)
	{
		status = BMS_ERROR;			// HAL_FLASH_GetError();
	}

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief helper function to earse the flash page based on the start address
 * @param startAddress starting address of the Flash memory
 * @retval BMS status
 */
bmsStatus_t flashErase(uint32_t startAddress)
{
	bmsStatus_t status = BMS_ERROR;

	if(!(IS_FLASH_PROGRAM_ADDRESS(startAddress)))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	uint32_t startPageIndex = GetPage(startAddress);

	switch(startAddress)
	{
		case FLASH_CONFIG_ADDRESS:
			status = flashErasePage(FLASH_CONFIG_PAGE_NUMBER, FLASH_CONFIG_NUMBER_OF_PAGES, FLASH_CONFIG_BANK);
			break;
	
		case FLASH_EXCEPTION_ADDRESS:
			status = flashErasePage(FLASH_EXCEPTION_PAGE_NUMBER, FLASH_EXCEPTION_NUMBER_OF_PAGES, FLASH_EXCEPTION_BANK);
			break;
		
		case FLASH_COMMON_DATA:
			status = flashErasePage(IMAGE_PAGE_NUMBER, IMAGE_DATA_PAGES, IMAGE_CONFIG_BANK);
			break;

		case PRIMARY_APP:
			status = flashErasePage(startPageIndex, PRIMARY_APP_PAGES, PRIMARY_APP_BANK);
			break;

		case SECONDARY_APP:
			status = flashErasePage(256, SECONDARY_APP_PAGES, SECONDARY_APP_BANK);
			break;

		case SOC_DATA_ADDRESS:
			status = flashErasePage(SOC_DATA_PAGE_NUMBER, SOC_DATA_PAGE_NUMBER_OF_PAGES, SOC_DATA_PAGE_BANK);
			break;

		default:
			status 	= BMS_ERROR;
			break;
	}
	// startPageIndex = GetPage(startAddress);
	return status;
}

/**
 * @brief Get the Page number from Flash
 *
 * @param address
 * @return uint32_t
 */
static uint32_t GetPage(uint32_t address)
{
	return ((address - FLASH_BASE) / FLASH_PAGE_SIZE) % KPAGE_PER_BANK;
}


/* End of File ---------------------------------------------------------------*/
