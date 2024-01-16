/**
 * @file onBoardIOs.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDIOS_H_
#define ONBOARDIOS_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/



enum ledID
{
    errorLEDType = 0, 
    errorLEDCode
};

enum spiID
{
    cmuSpi = 0,
    ccmSpi 
    
};

enum canPort
{
    communicationCAN = 0, 

};

enum memoryType
{
    eeprom = 0,
    flash, 

};

enum eepromID
{
    eepromID = 0, 

};

enum i2cID
{
    eepromI2C = 0, 

};

enum switchID
{
    inLoad = 0,
    inCharge, 
    inBalance,
    inReserved
};

enum contactorID
{
    loadNegative = 0,
    chargeNegative, 
    commonPositive,
    prechargePositive
};

enum gpioID
{
    shortCircuit = 0,
};

#define IS_MEMORY_TYPE(VALUE)             (((VALUE) == eeprom) || ((VALUE) == flash))

#define IS_CAN_PORT(PORT)                 (((PORT) == communicationCAN))

#define IS_SPI_ID(ID)                     (((ID) == cmuSpi) || ((ID) == ccmSpi))

#define IS_I2C_ID(ID)                     (((ID) == eepromI2C))

#define IS_EEPROM_ID(ID)                  (((ID) == eepromID))


#define IS_LED_ID(ID)                     (((ID) == errorLEDType) || ((ID) == errorLEDCode))

#define IS_SWITCH_ID(ID)                  (((ID) == inLoad) || ((ID) == inCharge) ||\
                                                     ((ID) == inBalance) || ((ID) == inReserved))

#define IS_CONTACTOR_ID(ID)               (((ID) == loadNegative) || ((ID) == chargeNegative) ||\
                                                     ((ID) == commonPositive) || ((ID) == prechargePositive))

#define IS_GPIO_ID(ID)                     (((ID) == shortCircuit))


/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t flashErrorOnLEDs(uint8_t type, uint8_t code);
bool isLoadRequested(void);
bool isChargeRequested(void);
bool isBalancingRequested(void);
bmsStatus_t turnContactorON(uint8_t type);
bmsStatus_t turnContactorOFF(uint8_t type);
void readAmbientTemperature(void);
void readAmbientHumidity(void);

bmsStatus_t exitCriticalSection(void);
bmsStatus_t enterCriticalSection(void);
#endif
/* End of File ---------------------------------------------------------------*/

