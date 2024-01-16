/**
 * @file gpioControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef GPIOCONTROLS_H_
#define GPIOCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t   toggleLED           (uint8_t gpioID);
bmsStatus_t   setLED              (uint8_t gpioID);
bmsStatus_t   resetLED            (uint8_t gpioID);
uint8_t       readSwitchState     (uint8_t switchID);

bmsStatus_t   setSPISlaveSelect   (uint8_t spiID);
bmsStatus_t   resetSPISlaveSelect (uint8_t spiID);

bmsStatus_t   setCANEnable        (uint8_t canPort);
bmsStatus_t   resetCANEnable      (uint8_t canPort);

bmsStatus_t   eepromEnableWriteProtection   (uint8_t eepromID);
bmsStatus_t   eepromDisableWriteProtection  (uint8_t eepromID);

bmsStatus_t   resetShortCircuit             (uint8_t gpioID);
#endif
/* End of File ---------------------------------------------------------------*/

