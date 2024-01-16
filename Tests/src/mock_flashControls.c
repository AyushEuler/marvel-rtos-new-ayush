

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
// #include "main.h"
// #include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "mock_flashControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

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

	// if( !(IS_FLASH_PROGRAM_ADDRESS(startAddress)) 	||\
	// 	(NULL == data) 								||\
	// 	(dataSize <= 0))
	// {
	// 	status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	// status = flashErase(startAddress);
	// CHECK_STATUS(status);

	// FLASH_UNLOCK();
	// for(uint16_t index = 0; index < dataSize && BMS_OK == status; index++)
	// {
	// 	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress, data[index]);
	// 	startAddress += 8;
	// }
	// FLASH_LOCK();

	// if(status != BMS_OK)
	// {
	// 	status = BMS_ERROR;			// HAL_FLASH_GetError();
	// }

	// return status;

	status == BMS_OK;

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

	// if( !(IS_FLASH_PROGRAM_ADDRESS(startAddress)) 	||\
	// 	(NULL == data) 								||\
	// 	(dataSize <= 0))
	// {
	// 	status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	// for(int i = 0; i < dataSize; i++)
	// {
	// 	*data = *(__IO uint64_t *)(uintptr_t)startAddress;
	// 	startAddress += 8;
	// 	data++;
	// }

	// status = BMS_OK;
	// return status;

	status == BMS_OK;

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
	// FLASH_EraseInitTypeDef EraseInitStruct = {0};

	// if( !(IS_FLASH_PAGE_NUMBER(pageNumber)) ||\
	// 	(numberOfPages <= 0) 		 ||\
	// 	!(IS_FLASH_BANK(bank)))
	// {
	// 	status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	// FLASH_UNLOCK();
	// uint32_t pageError			= 0;
	// EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_PAGES;
	// EraseInitStruct.Page 		= pageNumber;
	// EraseInitStruct.NbPages 	= numberOfPages;
	// EraseInitStruct.Banks 		= bank;
	// FLASH->SR 					= 0xFFFFFFFF;

	// status = HAL_FLASHEx_Erase(&EraseInitStruct, &pageError);
	// FLASH_LOCK();

	// if(status != BMS_OK)
	// {
	// 	status = BMS_ERROR;			// HAL_FLASH_GetError();
	// }

	// return status;

	status == BMS_OK;

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

	// if(!(IS_FLASH_PROGRAM_ADDRESS(startAddress)))
	// {
	// 	status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	// switch(startAddress)
	// {
	// 	case FLASH_CONFIG_ADDRESS:
	// 		status = flashErasePage(FLASH_CONFIG_PAGE_NUMBER, FLASH_CONFIG_NUMBER_OF_PAGES, FLASH_CONFIG_BANK);
	// 		break;
	
	// 	case FLASH_EXCEPTION_ADDRESS:
	// 		status = flashErasePage(FLASH_EXCEPTION_PAGE_NUMBER, FLASH_EXCEPTION_NUMBER_OF_PAGES, FLASH_EXCEPTION_BANK);
	// 		break;

	// 	default:
	// 		status 	= BMS_ERROR;
	// 		break;
	// }
	// return status;

	status == BMS_OK;

	return status;
}

/* End of File ---------------------------------------------------------------*/
