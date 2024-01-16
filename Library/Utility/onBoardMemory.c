/**
 * @file onBoardMemory.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "onBoardIOs.h"
#include "onBoardEEPROM.h"
#include "conversion.h"
#include "crc.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardMemory.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

#define EEPROM_CRC_SIZE                     4
#define EEPROM_CRC_START_ADDRESS            256
#define CALCULATE_CRC_ADDRESS(address)      ((address) + EEPROM_CRC_START_ADDRESS)

/* Function definition -------------------------------------------------------*/
/**
 * @brief read data from memory
 * @param memoryType memory Type
 * @param memoryAddress Memory address location of from data needs to be read
 * @param dataBuffer base address of data array that needs to be read from memory
 * @param dataLength Total number of bytes need to read from memory in bytes
 * @retval BMS status
 */
bmsStatus_t memoryRead(uint8_t memoryType, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength)
{
    bmsStatus_t status      = BMS_ERROR;
    uint32_t calculatedCRC  = 0;
    uint32_t storedCRC      = 0;
    uint32_t crcAddress     = 0;

	if( !(IS_MEMORY_TYPE(memoryType))       ||\
		(memoryAddress < 0)                 ||\
		(NULL == dataBuffer)                ||\
		(dataLength <= 0))
	{
		status = BMS_MEMORY_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    
    crcAddress              = CALCULATE_CRC_ADDRESS(memoryAddress);

    switch(memoryType)
    {
        case eeprom:
            //read data from eeprom
            status = eepromRead(EEPROM_DEVICE_ADDRESS, memoryAddress, dataBuffer, dataLength);
            CHECK_STATUS(status);
            //read the stored crc from eeprom
            status = eepromRead(EEPROM_DEVICE_ADDRESS, crcAddress, (uint8_t*)&storedCRC, EEPROM_CRC_SIZE);
            CHECK_STATUS(status);
            //calculate CRC
            calculatedCRC = calculateCRC(dataBuffer, dataLength);
            //compare the crc
            status = (storedCRC == calculatedCRC ? BMS_OK : BMS_EEPROM_READ_FAILED);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    
    return status;
}

bmsStatus_t statusMemoryWrite      = BMS_ERROR;

/* Function definition -------------------------------------------------------*/
/**
 * @brief write data to memory
 * @param memoryType memory Type
 * @param memoryAddress Memory address where data needs to be written
 * @param dataBuffer base address of data array that needs to be write from memory
 * @param dataLength Total number of bytes need to write from memory in bytes
 * @retval BMS status
 */
bmsStatus_t memoryWrite(uint8_t memoryType,  uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength)
{
    // bmsStatus_t statusMemoryWrite      = BMS_ERROR;
    uint32_t calculatedCRC  = 0;
    uint32_t storedDataCRC  = 0;
    uint32_t crcAddress     = 0;
    uint8_t readData[100]   = {0};

	if( !(IS_MEMORY_TYPE(memoryType))       ||\
		(memoryAddress < 0)                 ||\
		(NULL == dataBuffer)                ||\
		(dataLength <= 0))
	{
		statusMemoryWrite = BMS_MEMORY_FUNCTION_PARAMETER_ERROR;
		return statusMemoryWrite;
	}

    calculatedCRC           = calculateCRC(dataBuffer, dataLength);
    crcAddress              = CALCULATE_CRC_ADDRESS(memoryAddress);

    switch(memoryType)
    {
        case eeprom:
            //write data to eeprom
            statusMemoryWrite = eepromWrite(EEPROM_DEVICE_ADDRESS, memoryAddress, dataBuffer, dataLength);
            CHECK_STATUS(statusMemoryWrite);
            //write crc to eeprom
            statusMemoryWrite = eepromWrite(EEPROM_DEVICE_ADDRESS, crcAddress, (uint8_t*)&calculatedCRC, EEPROM_CRC_SIZE);
            CHECK_STATUS(statusMemoryWrite);
            //read back the data
            statusMemoryWrite = eepromRead(EEPROM_DEVICE_ADDRESS, memoryAddress, readData, dataLength);
            CHECK_STATUS(statusMemoryWrite);
            //calculate CRC
            storedDataCRC = calculateCRC(readData, dataLength);
            //compare the crc
            statusMemoryWrite = (calculatedCRC == storedDataCRC ? BMS_OK : BMS_EEPROM_WRITE_FAILED);
            break;
        default:
            statusMemoryWrite = BMS_ERROR;
            break;
    }
    
    return statusMemoryWrite;
}

/* End of File ---------------------------------------------------------------*/

