/**
 * @file onBoardEEPROM.c
 * @author Lalit Ahuja
 * @brief 
 * @version 0.1
 * @date 2023-01-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "i2c.h"
#include "gpioControls.h"
#include "onBoardIOs.h"
#include "i2cControls.h"
#include "customDelays.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardEEPROM.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

static bmsStatus_t calculateMemoryAddress  (uint32_t *memoryAddress, uint8_t *calculatedMemoryAddress);
static bmsStatus_t calculateDeviceAddress  (uint16_t *deviceAddress, uint32_t *memoryAddress);
static uint16_t calculateRemainingDataLength (uint16_t dataLength, uint16_t offset);


/* Function definition -------------------------------------------------------*/
/**
 * @brief Used to read data array from specified EEPROM memory location
 * @param deviceAddress EEPROM device address
 * @param memoryAddress Memory address location of EEPROM that data needs to be read from.
 * @param dataBuffer base address of data array that needs to be read from EEPROM
 * @param dataLength Total number of bytes need to read from EEPROM in bytes
 * @retval Status
 */
bmsStatus_t eepromRead(uint16_t deviceAddress, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength)
{
	bmsStatus_t status					=  BMS_ERROR;
	uint8_t  calculatedMemoryAddress[2] = {0};
	uint16_t startPage 					=  0 ;
	uint16_t endPage 					=  0 ;
	uint16_t numberOfPages 				=  0 ;
	uint16_t position 					=  0 ;
	uint16_t offset 					=  0 ;
	uint16_t remainingDataLength		=  0 ;

	if( !(IS_EEPROM_ADDRESS(deviceAddress)) 		||\
		(memoryAddress < 0)						 	||\
		(NULL == dataBuffer) 						||\
		(dataLength <= 0))
	{
		status = BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	startPage 			=  (memoryAddress / EEPROM_PAGE_SIZE) ;
	endPage 			=  ((memoryAddress + (dataLength - 1))/ EEPROM_PAGE_SIZE) ;
	numberOfPages 		=  ((endPage-startPage) + 1);
	offset 				=  (memoryAddress - (EEPROM_PAGE_SIZE * startPage));

	for (int i = 0; i < numberOfPages; i++)
	{
        /* We compute the MSB and LSB parts of the device address and memory address */
        calculateDeviceAddress(&deviceAddress, &memoryAddress);
        calculateMemoryAddress(&memoryAddress, &calculatedMemoryAddress[0]);

        remainingDataLength = calculateRemainingDataLength(dataLength, offset);

        /* First we send the memory location address where start reading data */
        status = i2cMasterWrite(eepromI2C, deviceAddress, calculatedMemoryAddress, EEPROM_MEMORY_ADDRESS_SIZE);
		CHECK_STATUS(status);

        /* Next we can retrieve the data from EEPROM */
        status = i2cMasterRead(eepromI2C, deviceAddress, &dataBuffer[position], remainingDataLength);
		CHECK_STATUS(status);

		offset         = 0;
		dataLength    -= remainingDataLength;
		position      += remainingDataLength;
		memoryAddress += remainingDataLength;
		delayMilliSeconds(EEPROM_DELAY);
	}
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Used to write data array into specified EEPROM memory location
 * @param deviceAddress EEPROM device address
 * @param memoryAddress Memory address location of EEPROM that data needs to be written to.
 * @param dataBuffer base address of data array that needs to be written in EEPROM
 * @param dataLength Total number of bytes need to write into EEPROM in bytes
 * @retval Status
 */
bmsStatus_t eepromWrite(uint16_t deviceAddress, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength)
{
	bmsStatus_t status					=  BMS_ERROR;
	uint16_t startPage 					=  0 ;
	uint16_t endPage 					=  0 ;
	uint16_t numberOfPages 				=  0 ;
	uint16_t position 					=  0 ;
	uint16_t offset 					=  0 ;
	uint16_t remainingDataLength		=  0 ;
	uint8_t data[20]					= {0};	//16 - page size + 4 max eeprom address size

	if( !(IS_EEPROM_ADDRESS(deviceAddress)) 		||\
		(memoryAddress < 0)						 	||\
		(NULL == dataBuffer) 						||\
		(dataLength <= 0))
	{
		status = BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	startPage 			=  (memoryAddress / EEPROM_PAGE_SIZE) ;
	endPage 			=  ((memoryAddress + (dataLength - 1))/ EEPROM_PAGE_SIZE) ;
	numberOfPages 		=  ((endPage-startPage) + 1);
	offset 				=  (memoryAddress - (EEPROM_PAGE_SIZE * startPage));

	// eepromDisableWriteProtection(eepromID);
	for (int i = 0; i < numberOfPages; i++)
	{
        remainingDataLength = calculateRemainingDataLength(dataLength, offset);

        /* We compute the MSB and LSB parts of the memory address */
        calculateDeviceAddress(&deviceAddress, &memoryAddress);
        calculateMemoryAddress(&memoryAddress, &data[0]);

		/* And copy the content of the dataBuffer array in the temporary buffer */
		memcpy(data + EEPROM_MEMORY_ADDRESS_SIZE, &dataBuffer[position], remainingDataLength);

		/* write the data */
		status = i2cMasterWrite(eepromI2C, deviceAddress, data, (remainingDataLength + EEPROM_MEMORY_ADDRESS_SIZE));
		CHECK_STATUS(status);

		offset         = 0;
		dataLength    -= remainingDataLength;
		position      += remainingDataLength;
		memoryAddress += remainingDataLength;
		memset(data, 0, sizeof(data));
		delayMilliSeconds(EEPROM_DELAY);
	}
	// eepromEnableWriteProtection(eepromID);

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief erases all the eeprom by writing 0xFF at every memory location
 * @param none
 * @retval EEPROM Status
 */
bmsStatus_t eepromEraseAllPage(void)
{
	bmsStatus_t status 						= BMS_ERROR;
	uint16_t dataLength 					= CONVERT_KBIT_TO_BYTE(EEPROM_SIZE);
	uint8_t *data 							= (uint8_t *)calloc(dataLength, sizeof(uint8_t));

    if(data == NULL)
    {
        status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
        return status;
    }

	memset(data, 0xFF, dataLength);
	status = eepromWrite(EEPROM_DEVICE_ADDRESS, EEPROM_START_ADDRESS, data, dataLength);
	free(data);
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief calculates the MSB and LSB of memory address
 * @param memoryAddress memory address
 * @param calculatedMemoryAddress array to store the MSB and LSB of the calculated address
 * @retval Status
 */
static bmsStatus_t calculateMemoryAddress(uint32_t *memoryAddress, uint8_t *calculatedMemoryAddress)
{
	if( (NULL == memoryAddress)				||\
		(NULL == calculatedMemoryAddress) )
	{
		return BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
	}

	#if(EEPROM_MEMORY_ADDRESS_SIZE == EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE)
	  	calculatedMemoryAddress[0] = (uint8_t) (*memoryAddress & 0xFF);

	#elif (EEPROM_MEMORY_ADDRESS_SIZE == EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE)
		  *calculatedMemoryAddress[0] = (uint8_t) (*memoryAddress & 0xFF);
		  *calculatedMemoryAddress[1] = (uint8_t) ((*memoryAddress & 0xFF00) >> 8);
	#endif

	return BMS_OK;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief calculates the device address based upon the memory address 
 * @param deviceAddress device address
 * @param memoryAddress memory address
 * @retval Status
 */
static bmsStatus_t calculateDeviceAddress  (uint16_t *deviceAddress, uint32_t *memoryAddress)
{
	if( (NULL == deviceAddress)	  ||\
		(NULL == memoryAddress) )
	{
		return BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
	}

	#if (EEPROM_SIZE == EEPROM_SIZE_1_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_2_KBIT)
		*deviceAddress = *deviceAddress & 0xFF;

	#elif (EEPROM_SIZE == EEPROM_SIZE_4_KBIT)
		  *deviceAddress = (*deviceAddress) | (((*memoryAddress) & EEPROM_SIZE_4_KBIT_MEMORY_ADDRESS_MASK) >> EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE_SHIFT_POSITION) ;

	#elif (EEPROM_SIZE == EEPROM_SIZE_8_KBIT)
		  *deviceAddress = (*deviceAddress) | (((*memoryAddress) & EEPROM_SIZE_8_KBIT_MEMORY_ADDRESS_MASK) >> EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE_SHIFT_POSITION) ;

	#elif (EEPROM_SIZE == EEPROM_SIZE_16_KBIT)
		  *deviceAddress = (*deviceAddress) | (((*memoryAddress) & EEPROM_SIZE_16_KBIT_MEMORY_ADDRESS_MASK) >> EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE_SHIFT_POSITION) ;


	#elif (EEPROM_SIZE == EEPROM_SIZE_32_KBIT)  || (EEPROM_SIZE == EEPROM_SIZE_64_KBIT)  ||\
		  (EEPROM_SIZE == EEPROM_SIZE_128_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_256_KBIT) ||\
		  (EEPROM_SIZE == EEPROM_SIZE_512_KBIT)
		  *deviceAddress = *deviceAddress & 0xFFFF;

	#elif (EEPROM_SIZE == EEPROM_SIZE_1_MBIT)
		  *deviceAddress = (*deviceAddress) | (((*memoryAddress) & EEPROM_SIZE_1_MBIT_MEMORY_ADDRESS_MASK) >> EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE_SHIFT_POSITION) ;
	#elif (EEPROM_SIZE == EEPROM_SIZE_2_MBIT)
		  *deviceAddress = (*deviceAddress) | (((*memoryAddress) & EEPROM_SIZE_2_MBIT_MEMORY_ADDRESS_MASK) >> EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE_SHIFT_POSITION) ;

	#endif

	return BMS_OK;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief send battery data on the CAN
 * @param dataLength length of the data
 * @param offset offset
 * @retval length
 */
static uint16_t calculateRemainingDataLength (uint16_t dataLength, uint16_t offset)
{
	uint16_t length = 0;

	if( (dataLength < 0)  ||\
		(offset < 0) )
	{
		return BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
	}

	if ( (dataLength + offset) < EEPROM_PAGE_SIZE )
	{
		length = dataLength;
	}
	else
	{
		length = EEPROM_PAGE_SIZE - offset;
	}

	return length;
}


/* End of File ---------------------------------------------------------------*/

