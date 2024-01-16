/**
 * @file bmsErrorCodes.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BMSERRORCODES_H_
#define BMSERRORCODES_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define bmsError_NORMAL_OPERATION                0x00

#define bmsError_PERIPHERAL_UART                 0x01
#define bmsError_PERIPHERAL_SPI                  0x02
#define bmsError_PERIPHERAL_CAN                  0x03
#define bmsError_PERIPHERAL_I2C                  0x04

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t errorCode;
extern char debugData[256];
/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


#endif
/* End of File ---------------------------------------------------------------*/

