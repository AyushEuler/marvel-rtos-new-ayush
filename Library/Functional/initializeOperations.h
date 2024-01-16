/**
 * @file initializeOperations.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef INITIALIZEOPERATIONS_H_
#define INITIALIZEOPERATIONS_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "onBoardCMU6815.h"
#include "onBoardCCM2949.h"
#include "onBoardIOs.h"
#include "dataProcessing.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t initializeCMU(bmsMainData_t *mdata);
bmsStatus_t initializeCCM(void);
bmsStatus_t initializeCAN(uint8_t canID);
bmsStatus_t initializeEEPROM(bmsMainData_t *mdata);
#endif
/* End of File ---------------------------------------------------------------*/

