/**
 * @file onBoardCANv2b.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDCANV2B_H_
#define ONBOARDCANV2B_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "onBoardIOs.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t initializeCANCommunication    (uint8_t canPort);
bmsStatus_t sendChargingDataOnCAN         (float chargeV, float chargeI);
bmsStatus_t sendBatteryDataOnCAN          (uint8_t canIDType, uint32_t canID, uint8_t *canData);
bmsStatus_t sendCANDataOnQueue            (uint32_t canID, uint8_t *canData, uint8_t canDataSize);
#endif
/* End of File ---------------------------------------------------------------*/

