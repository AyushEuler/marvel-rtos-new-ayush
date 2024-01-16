/**

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "threadxMain.h"
/* Self include --------------------------------------------------------------*/
#include "dataProcessing.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

TX_MUTEX        dataReadWriteMutex;

ccmData_t ccmData = {0};
static bmsMainData_t bmsMainDataPipeline = {0};

bmsStatus_t pipelineInit(void)
{
    bmsStatus_t status                                      = BMS_ERROR;
    bmsMainDataPipeline.cmuData.cmuCount                    = NUMBER_OF_CMU;
	// bmsMainDataPipeline.protectionData.maxChargeCurrent     = CURRENT_LIMIT_CHARGE;
	// bmsMainDataPipeline.protectionData.maxRegenCurrent      = CURRENT_LIMIT_REGEN;
	// bmsMainDataPipeline.protectionData.maxLoadCurrent       = -1*CURRENT_LIMIT_DISCHARGE;
    status = BMS_OK;
    return status;
}

bmsStatus_t readFromPipeline(bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == mdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(mdata, (void*)&bmsMainDataPipeline, sizeof(bmsMainData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

bmsStatus_t writeToPipeline(bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == mdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline, mdata, sizeof(bmsMainData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

bmsStatus_t writeCCMDataToPipeline(ccmData_t *cdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == cdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.ccmData, cdata, sizeof(ccmData_t));
	// release mutex

    status = BMS_OK;
    return status;

}

bmsStatus_t writeCMUDataToPipeline(cmuData_t *cdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == cdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.cmuData, cdata, sizeof(cmuData_t));
	// release mutex

    status = BMS_OK;
    return status;

}

bmsStatus_t writePreChargeDataToPipeline(preCharge_t *pdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == pdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.preCharge, pdata, sizeof(preCharge_t));
	// release mutex

    status = BMS_OK;
    return status; 
}

bmsStatus_t writeChargeDataToPipeline(chargeData_t *cdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == cdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.chargeData, cdata, sizeof(chargeData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

bmsStatus_t writeFaultDataToPipeline(faultData_t *fdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == fdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.faultData, fdata, sizeof(faultData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

bmsStatus_t writeSocDataToPipeline(socData_t *sdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == sdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.socData, sdata, sizeof(socData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

bmsStatus_t writeProtectionDataToPipeline(protectionData_t *pdata)
{
    bmsStatus_t status = BMS_ERROR;

	if((NULL == pdata))
	{
		status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// acquire mutex
	memcpy(&bmsMainDataPipeline.protectionData, pdata, sizeof(protectionData_t));
	// release mutex

    status = BMS_OK;
    return status;
}

/* Function definition -------------------------------------------------------*/
/**

/* Function definition -------------------------------------------------------*/
/**

/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/