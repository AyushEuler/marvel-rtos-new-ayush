/**
 * @file onBoardUARTs.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDUARTS_H_
#define ONBOARDUARTS_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
enum uartID
{
    debugPort = 0, 
    vcuPort
};

#define IS_UART_ID(ID)                     (((ID) == debugPort) || ((ID) == vcuPort))

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t sendDataOnDebugPort(char *outString);

#endif
/* End of File ---------------------------------------------------------------*/

