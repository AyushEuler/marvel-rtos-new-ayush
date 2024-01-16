/**
 * @file analogControls.h
 * @brief 
 * @version 0.1
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef WATCHDOGCONTROLS_H_
#define WATCHDOGCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include "main.h"
#include "iwdg.h"
#include "wwdg.h"
#include "bmsConfiguration.h"
/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
uint32_t calculateIWDGReloadValue(uint16_t resetTime, uint8_t preScaler);


#endif
/* End of File ---------------------------------------------------------------*/

