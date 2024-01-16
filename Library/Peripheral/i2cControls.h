/**
 * @file i2cControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef I2CCONTROLS_H_
#define I2CCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "onBoardIOs.h"
#include "onBoardEEPROM.h"
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#define IS_I2C_ADDRESS(ADDRESS)	    (((ADDRESS) == EEPROM_DEVICE_ADDRESS)  || ((ADDRESS) == EEPROM_DEVICE_ADDRESS_1) ||\
                                    ((ADDRESS) == EEPROM_DEVICE_ADDRESS_2) || ((ADDRESS) == EEPROM_DEVICE_ADDRESS_3)  )

bmsStatus_t i2cMasterRead(uint8_t i2cID, uint16_t deviceAddress, uint8_t *data, uint16_t size);
bmsStatus_t i2cMasterWrite(uint8_t i2cID, uint16_t deviceAddress, uint8_t *data, uint16_t size);
#endif
/* End of File ---------------------------------------------------------------*/

