/**
 * @file monitorOperations.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef MONITOROPERATIONS_H_
#define MONITOROPERATIONS_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "onBoardCMU6815.h"
#include "dataProcessing.h"
#include "threadxMain.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

enum errorFlags
{
    minCellVoltage_flag = 0,
	maxCellVoltage_flag,
	minCellTemperature_flag, 
	maxCellTemperature_flag, 
	packCurrent_maxLoad_flag,
	packCurrent_maxCharge_flag, 
	packCurrent_maxRegen_flag
};
/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

bmsStatus_t displayErrorState(uint8_t bmsError);
bmsStatus_t sendDebugString(char *dataToSend);

bmsStatus_t readCellVoltages(bmsMainData_t *mdata);
bmsStatus_t readCellTemperatures(bmsMainData_t *mdata);
bmsStatus_t readPackStatus(bmsMainData_t *mdata);

bmsStatus_t readPackVoltageCurrent(bmsMainData_t *mdata);
bmsStatus_t readChargeEnergyData(bmsMainData_t *mdata);
bmsStatus_t setOverCurrentConfiguration(int8_t threshold, uint8_t deglitchTime, uint8_t polarity);
bmsStatus_t calculateRemainingPackCapacity(bmsMainData_t *mdata);

bmsStatus_t checkPackCurrent(bmsMainData_t *mdata);
bmsStatus_t checkPackVoltages(bmsMainData_t *mdata);
bmsStatus_t checkPackTemperatures(bmsMainData_t *mdata);
bmsStatus_t sendDataOnCAN(bmsMainData_t *mdata, protectionOutput_t *pOutput, coulombCountingOutput_t *ccOutput);
bmsStatus_t readStatusFaultReg(bmsMainData_t *mdata);
#endif
/* End of File ---------------------------------------------------------------*/
