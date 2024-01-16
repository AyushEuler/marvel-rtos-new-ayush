/**
 * @file i2cControls.c
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
#include "main.h"
#include "i2c.h"

#include "onBoardIOs.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "i2cControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/


/* Function definition -------------------------------------------------------*/
/**
 * @brief Used to write data array to specified i2c device
 * @param deviceAddress i2c device address
 * @param dataBuffer base address of data array that needs to be write to i2c device
 * @param dataLength Total number of bytes need to write to i2c device in bytes
 * @retval bms Status
 */
bmsStatus_t i2cMasterWrite(uint8_t i2cID, uint16_t deviceAddress, uint8_t *data, uint16_t size)
{
    bmsStatus_t status = BMS_ERROR;

	if( !(IS_I2C_ID(i2cID)) 		            ||\
		!(IS_I2C_ADDRESS(deviceAddress)) 	    ||\
		(NULL == data) 						    ||\
		(size <= 0))
	{
		status = BMS_I2C_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(i2cID)
    {
        case eepromI2C:
            status = HAL_I2C_Master_Transmit(&hi2c1, deviceAddress, data, size, 100);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    status = BMS_OK;
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Used to read data array from specified i2c device
 * @param deviceAddress i2c device address
 * @param dataBuffer base address of data array that needs to be read from i2c device
 * @param dataLength Total number of bytes need to read from i2c device in bytes
 * @retval bms Status
 */

bmsStatus_t i2cMasterRead(uint8_t i2cID, uint16_t deviceAddress, uint8_t *data, uint16_t size)
{
    bmsStatus_t status = BMS_ERROR;

	if( !(IS_I2C_ID(i2cID)) 		            ||\
		!(IS_I2C_ADDRESS(deviceAddress)) 	    ||\
		(NULL == data) 						    ||\
		(size <= 0))
	{
		status = BMS_I2C_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(i2cID)
    {
        case eepromI2C:
            status = HAL_I2C_Master_Receive(&hi2c1, deviceAddress, data, size, 100);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    status = BMS_OK;
    return status;
}

/* End of File ---------------------------------------------------------------*/

