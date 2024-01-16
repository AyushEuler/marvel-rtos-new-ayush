/**
 * @file initializeOperations.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsCANMatrix.h"
#include "bmsErrorCodes.h"
#include "bmsConfiguration.h"

#include "onBoardCMU6815.h"
#include "onBoardCCM2949.h"
#include "onBoardCANv2b.h"
#include "onBoardMemory.h"
#include "gpioControls.h"
#include "conversion.h"
#include "monitorOperations.h"
/* Self include --------------------------------------------------------------*/
#include "initializeOperations.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize cell monitoring unit
 * @param 
 * @retval BMS status
 */
bmsStatus_t initializeCMU(bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;
	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}	
    status = adBms6815_init_configA_B(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData, CONVERT_MILLI_VOLT_TO_VOLT(CELL_MAX_VOLTAGE_THRESHOLD_mV), CONVERT_MILLI_VOLT_TO_VOLT(CELL_MIN_VOLTAGE_THRESHOLD_mV));
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize cell control module
 * @param 
 * @retval BMS status
 */
bmsStatus_t initializeCCM(void)
{
    bmsStatus_t status = BMS_ERROR;
    status = LTC2949Initialize();
#ifdef ENABLE_LTC2949_SHORT_CIRCUIT
	CHECK_STATUS(status);
	status = setOverCurrentConfiguration(OCC_DAC_THRESHOLD_52_mV, OCC_DEGLITCH_TIME_20_us, OCC_POLARITY_BOTH_POLARITY);
#endif    
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize CAN
 * @param canID can ID
 * @retval BMS status
 */
bmsStatus_t initializeCAN(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    status = initializeCANMatrix(canPort);
#ifdef DEBUG_CAN
    initializeDebugCANMatrix();
#endif
    CHECK_STATUS(status);
    status = initializeCANCommunication(canPort);
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize EEPROM
 * @param mdata pointer to bmsData structure
 * @retval BMS status
 */
bmsStatus_t initializeEEPROM(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	
	if((NULL == mdata))
	{
		status = BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
		return status;
	}	

	// status = eepromEnableWriteProtection(eepromID);
	eepromDisableWriteProtection(eepromID);

	return status;
}

/* End of File ---------------------------------------------------------------*/
