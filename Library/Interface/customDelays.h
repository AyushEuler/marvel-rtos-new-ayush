/**
 * @file customDelays.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef CUSTOMDELAYS_H_
#define CUSTOMDELAYS_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "tx_api.h"
#include "stm32l4xx_hal.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/

// #ifdef THREADX 
//     #define delayInMilliSeconds(timeToWait)     timeToWait>=10 ? tx_thread_sleep(timeToWait/10) : HAL_Delay(timeToWait)
//     #define delayMilliSeconds(timeToWait)       delayInMilliSeconds(timeToWait)
// #else
//     #define delayInMilliSeconds(timeToWait)     HAL_Delay(timeToWait)
//     #define delayMilliSeconds(timeToWait)       delayInMilliSeconds(timeToWait)
// #endif

#define delayInMilliSeconds(timeToWait)     timeToWait>=10 ? tx_thread_sleep(timeToWait/10) : HAL_Delay(timeToWait)
#define delayMilliSeconds(timeToWait)       delayInMilliSeconds(timeToWait)

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


#endif
/* End of File ---------------------------------------------------------------*/

