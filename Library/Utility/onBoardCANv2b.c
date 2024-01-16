/**
 * @file onBoardCANv2b.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "gpioControls.h"
#include "canControls.h"
#include "onBoardIOs.h"
#include "bmsConfiguration.h"
#include "bmsCANMatrix.h"
#include "dataProcessing.h"
#include "threadxMain.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardCANv2b.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief initializes CAN Communication
 * @param canID CAN peripheral number
 * @retval BMS status
 */
bmsStatus_t initializeCANCommunication(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    
    status = resetCANEnable(canPort);
    CHECK_STATUS(status);
    status = beginCAN(canPort);
    CHECK_STATUS(status);
    status = enableCANFilters(canPort);
    CHECK_STATUS(status);
    status = enableCANReceptionInInterruptMode(canPort);
    return status;
}
/* Function definition -------------------------------------------------------*/
/**
 * @brief Sending charging information on the CAN
 * @param chargeV charging voltage
 * @param chargeI chargeing current 
 * @retval BMS status
 */
bmsStatus_t sendChargingDataOnCAN(float chargeV, float chargeI)
{
    bmsStatus_t status = BMS_ERROR;
    
#ifdef CAN_CHARGER_EXTENDED_ID
    uint8_t messageType = EXTENDED_CAN_ID;
    uint32_t messageID = 0x1806E5F4;
#else 
    uint8_t messageType = STANDARD_CAN_ID;
    uint32_t messageID = 0x107;
#endif

    uint8_t messageData[8] = {'\0'};
    
    //Big Endian
    for(uint8_t canDLC = 0; canDLC < 2; canDLC++)
    {
        messageData[0 + canDLC] = ((int) (chargeV/0.1)  >> (8 * (2 - canDLC - 1)));
    }
	 	
    for(uint8_t canDLC = 0; canDLC < 2; canDLC++)
    {
        messageData[2 + canDLC] = ((int) (chargeI/0.1)  >> (8 * (2 - canDLC - 1)));
    }
	 	
    messageData[5] = (int) 0;   //Control Bit = 0

    status = sendCAN(communicationCAN, messageType, messageID, messageData, 8);
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief send battery data on the CAN
 * @param canIDType type of the CAN ID. standard or Extended
 * @param canID id of the message
 * @param canData pointer to data to be sent
 * @retval BMS status
 */
bmsStatus_t sendBatteryDataOnCAN(uint8_t canIDType, uint32_t canID, uint8_t *canData)
{
    bmsStatus_t status = BMS_ERROR;
   
    if( !(IS_CAN_ID_TYPE(canIDType))  ||\
        !(IS_CAN_TX_ID(canID))        ||\
		(NULL == canData) )
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    status = sendCAN(communicationCAN, canIDType, canID, canData, 8);
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief send CAN data on the Queue
 * @param canID id of the message
 * @param canData pointer to data to be sent
 * @param canDataSize size of the data to be sent
 * @retval BMS status
 */
bmsStatus_t sendCANDataOnQueue(uint32_t canID, uint8_t *canData, uint8_t canDataSize)
{
	bmsStatus_t     status          = BMS_ERROR;
    canQueueData_t  canTxQueueData  = {0};

	if( !(IS_CAN_TX_ID(canID))    ||\
		(NULL == canData)         ||\
		(canDataSize <= 0))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	canTxQueueData.identifier = canID;
	memcpy(&canTxQueueData.data, canData, canDataSize);

	status = QUEUE_SEND(&canTxQueue, &canTxQueueData, NO_WAIT);
	memset(canData, 0, canDataSize);
	return status;
}
/* End of File ---------------------------------------------------------------*/

