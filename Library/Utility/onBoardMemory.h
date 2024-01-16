/**
 * @file onBoardMemory.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDMEMORY_H_
#define ONBOARDMEMORY_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#define REMAINING_CAPACITY_ADDRESS  0

bmsStatus_t memoryRead(uint8_t memoryType, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength);
bmsStatus_t memoryWrite(uint8_t memoryType, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength);

#endif
/* End of File ---------------------------------------------------------------*/

