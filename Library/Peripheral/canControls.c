/**
 * @file canControls.c
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
#include "main.h"
#include "can.h"
#include "onBoardIOs.h"
#include "bmsCANMatrix.h"
// #include "arm_ghs.h"
#include "threadxMain.h"
#include "bmsConfiguration.h"
// #include "bootloaderFunctions.h"
#include "firmware_upgrade_app.h"
#include "can_tp_app.h"
/* Self include --------------------------------------------------------------*/
#include "canControls.h"
// #include "can_tp_app.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/

#define CAN_PORT 	&hcan1
extern TX_QUEUE 	canRxQueue;
extern IsoTpShims firmware_up_recv_shim;
extern IsoTpReceiveHandle firmware_up_recv_handle;
extern IsoTpMessage firmware_up_recv_message;
extern uint8_t firmware_up_flag ;
extern flash_write_progress_struct write_progress;
uint8_t RxMsgDataBuf[8] = {0}; 
int ota_ongoing_timer = -1000;


/* Function definition -------------------------------------------------------*/
/**
 * @brief Change the state of CAN to ready to start the transmission
 * @param canID CAN number
 * @retval BMS status
 */
bmsStatus_t beginCAN(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(canPort)
    {
        case communicationCAN:
            status = HAL_CAN_Start(CAN_PORT);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    return status;
}



/* Function definition -------------------------------------------------------*/
/**
 * @brief Send the data on the CAN
 * @param canID CAN number
 * @param CANIDtype CAN ID Type
 * @param CANID CAN ID Value
 * @param txCANData Date to be transmitted
 * @retval BMS status
 */
bmsStatus_t sendCAN(uint8_t canPort, uint8_t CANIDtype, uint32_t CANIDValue, uint8_t *txCANData, uint8_t U8_DLC)
{
    bmsStatus_t   status = BMS_ERROR;
    CAN_TxHeaderTypeDef pTxHeader;

	// if( !(IS_CAN_TX_ID(CANIDValue))   	||\
	//     !(IS_CAN_PORT(canPort)) 		||\
    //     !(IS_CAN_ID_TYPE(CANIDtype)) 	||\
    //     (NULL == txCANData))
	// {
	// 	status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

    pTxHeader.DLC = U8_DLC;
	pTxHeader.RTR = CAN_RTR_DATA;
    switch (CANIDtype)
    {
        case STANDARD_CAN_ID:
            pTxHeader.IDE = CAN_ID_STD;
	        pTxHeader.StdId = CANIDValue;
            break;
        case EXTENDED_CAN_ID:
            pTxHeader.IDE = CAN_ID_EXT;
	        pTxHeader.ExtId = CANIDValue;
            break;
        default:
            break;
    }

    switch(canPort)
    {
        case communicationCAN:
            status = HAL_CAN_AddTxMessage(CAN_PORT, &pTxHeader, (uint8_t *)txCANData, (uint32_t *)CAN_TX_MAILBOX0);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    
    return status;
}

/* Function Definition */
/**
 * @brief wrapper function for sendCAN

*/


void Transmit_on_CAN1(uint32_t U32_transmitCANid, TypeofCANID U8_idType,
                      uint8_t *U8_dataarr, uint8_t U8_DLC)
{

//wrapper function
	sendCAN(communicationCAN, U8_idType, U32_transmitCANid, U8_dataarr, U8_DLC);
}






/* Function definition -------------------------------------------------------*/
/**
 * @brief enable can filters
 * @param canID CAN number
 * @retval BMS status
 */
bmsStatus_t enableCANFilters(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(canPort)
    {
        case communicationCAN:
            status = canFilterIDList(CAN_RX_ID_001, CAN_RX_ID_002, STANDARD_CAN_ID, CAN_FILTER_BANK_0, CAN_FIFO_0, ENABLE);

			status = canFilterIDList(CAN_RX_ID_006, CAN_RX_ID_007, STANDARD_CAN_ID, CAN_FILTER_BANK_1, CAN_FIFO_0, ENABLE);

			status = canFilterIDList(CAN_RX_ID_010, CAN_RX_ID_011, STANDARD_CAN_ID, CAN_FILTER_BANK_2, CAN_FIFO_0, ENABLE);

			status = canFilterIDList(CAN_RX_ID_008, CAN_RX_ID_003, STANDARD_CAN_ID, CAN_FILTER_BANK_3, CAN_FIFO_0, ENABLE);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief enable can reception in interrupt mode
 * @param canID CAN number
 * @retval BMS status
 */
bmsStatus_t enableCANReceptionInInterruptMode(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(canPort)
    {
        case communicationCAN:
            status = HAL_CAN_ActivateNotification(CAN_PORT, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief Filter CAN IDs 
 * 
 * @param receivedCanId1 CAN ID1
 * @param receivedCanId2 CAN ID2
 * @param idType ID type
 * @param filterBank filter bank
 * @param fifo CAN fifo
 * @param state Enable or disable
 * @retval BMS status
 */
bmsStatus_t canFilterIDList(uint32_t receivedCanId1, uint32_t receivedCanId2,
                        canIDType_t idType, canFilterBank_t filterBank,
                        canFIFO_t fifo, FunctionalState state)
{
    bmsStatus_t status = BMS_ERROR;
	CAN_FilterTypeDef sFilterConfig;

	// if( !(IS_CAN_RX_ID(receivedCanId1)) 		||\
	// 	!(IS_CAN_RX_ID(receivedCanId2)) 		||\
	// 	!(IS_CAN_ID_TYPE(idType)) 				||\
	// 	!(IS_CAN_FILTER_BANK(filterBank)) 		||\
	// 	!(IS_CAN_FIFO(fifo)) 					||\
	// 	!(IS_FUNCTIONAL_STATE(state)))
	// {
	// 	status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	sFilterConfig.FilterActivation = state;

	sFilterConfig.FilterBank           = (uint32_t)(filterBank);  // 32-bit MASK Filter Bank (Reference Manual)
	sFilterConfig.SlaveStartFilterBank = (uint32_t)(filterBank);
	sFilterConfig.FilterMode           = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;

	if (fifo == CAN_FIFO_0)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	}
	else if (fifo == CAN_FIFO_1)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	}

	if (idType == EXTENDED_CAN_ID)
	{
		sFilterConfig.FilterIdHigh     = (receivedCanId1 & 0x1FFFE000) >> 13;
		sFilterConfig.FilterIdLow      = ((receivedCanId1 & 0x00001FFF) << 3) | 0x04;
		sFilterConfig.FilterMaskIdHigh = (receivedCanId2 & 0x1FFFE000) >> 13;
		sFilterConfig.FilterMaskIdLow  = ((receivedCanId2 & 0x00001FFF) << 3) | 0x04;
	}
	else if (idType == STANDARD_CAN_ID)
	{
		sFilterConfig.FilterIdHigh     = receivedCanId1 << 5;
		sFilterConfig.FilterIdLow      = 0;
		sFilterConfig.FilterMaskIdHigh = receivedCanId2 << 5;
		sFilterConfig.FilterMaskIdLow  = 0;
	}

	status = HAL_CAN_ConfigFilter(CAN_PORT, &sFilterConfig);
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Filter CAN IDs 
 * 
 * @param canID CAN ID
 * @param canMask CAN Mask
 * @param idType ID type
 * @param filterBank filter bank
 * @param fifo CAN fifo
 * @param state Enable or disable
 * @retval BMS status
 */
bmsStatus_t canFilterIDMask(uint32_t canID, uint32_t canMask,
                        canIDType_t idType, canFilterBank_t filterBank,
                        canFIFO_t fifo, FunctionalState state)
{
    bmsStatus_t status = BMS_ERROR;
	CAN_FilterTypeDef sFilterConfig;

	// if( !(IS_CAN_RX_MASK(canID)) 				||\
	// 	!(IS_CAN_RX_ID(canMask)) 				||\
	// 	!(IS_CAN_ID_TYPE(idType)) 				||\
	// 	!(IS_CAN_FILTER_BANK(filterBank)) 		||\
	// 	!(IS_CAN_FIFO(fifo)) 					||\
	// 	!(IS_FUNCTIONAL_STATE(state)))
	// {
	// 	status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
	// 	return status;
	// }

	sFilterConfig.FilterActivation = state;

	sFilterConfig.FilterBank  = (uint32_t)(filterBank);  // 32-bit MASK Filter Bank (Reference Manual)
	sFilterConfig.FilterMode  = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	if (fifo == CAN_FIFO_0)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	}
	else if (fifo == CAN_FIFO_1)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	}

	if (idType == EXTENDED_CAN_ID)
	{
		sFilterConfig.FilterIdHigh     = (uint16_t)(canID >> 13);
		sFilterConfig.FilterIdLow      = (uint16_t)(canID << 3);
		sFilterConfig.FilterMaskIdHigh = (uint16_t)(canMask >> 13);
		sFilterConfig.FilterMaskIdLow  = (uint16_t)(canMask << 3);
	}
	else if (idType == STANDARD_CAN_ID)
	{
		sFilterConfig.FilterIdHigh     = (uint16_t)(canID << 5);
		sFilterConfig.FilterIdLow      = 0;
		sFilterConfig.FilterMaskIdHigh = (uint16_t)(canMask << 5);
		sFilterConfig.FilterMaskIdLow  = 0;
	}

	status = HAL_CAN_ConfigFilter(CAN_PORT, &sFilterConfig);
    return status;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef pRxHeader = {0};
    canQueueData_t canRxQueueData = {0};

	if (hcan == CAN_PORT)
	{
		HAL_CAN_GetRxMessage(CAN_PORT, CAN_RX_FIFO0, &pRxHeader, canRxQueueData.data);
        canRxQueueData.identifier = pRxHeader.StdId;
        QUEUE_SEND(&canRxQueue, &canRxQueueData, NO_WAIT);

	if(pRxHeader.StdId == rx_CAN_TP_Stark)
	{
		CAN_TP_Receive_interrupt(rx_CAN_TP_Stark, canRxQueueData.data, pRxHeader.DLC, &firmware_up_recv_shim, &firmware_up_recv_message, &firmware_up_recv_handle, STARK_CAN_TP_RX, &system_event_flag);
	}

	if(pRxHeader.StdId == FIRMWARE_UPGRADE_COMM)
	{
		if(canRxQueueData.data[1] == MARVEL_ID){
			if ( (canRxQueueData.data[2] == FUPFG_REQUEST) && ( canRxQueueData.data[3] == RECEIVE_BIN)){
				tx_event_flags_set(&system_event_flag, START_OTA, TX_OR);
				uptype = FOTA;
			}
			if ( (canRxQueueData.data[2] == FUPFG_REQUEST) && ( canRxQueueData.data[3] == RECEIVE_CONF)){
				tx_event_flags_set(&system_event_flag, START_OTA, TX_OR);
				uptype = COTA;
			}
			else if ( (canRxQueueData.data[2] == FUPFG_REQUEST) && ( canRxQueueData.data[3] == PERFORM_UPGRADE)){
				tx_event_flags_set(&system_event_flag, COMPLETE_OTA, TX_OR);
			}
			else if ( (canRxQueueData.data[2] == FUPFG_REQUEST) && ( canRxQueueData.data[3] == PAUSE_UPGRADE)){
				tx_event_flags_set(&system_event_flag, PUASE_OTA, TX_OR);
			}
			else if ( (canRxQueueData.data[2] == FUPFG_REQUEST) && ( canRxQueueData.data[3] == RESUME_UPGRADE)){
				tx_event_flags_set(&system_event_flag, RESUME_OTA, TX_OR);
			}
		}

	}
	
	if(pRxHeader.StdId == tx_flash_write_progress)
	{
		memcpy(&write_progress, RxMsgDataBuf, 5);
		ota_ongoing_timer = tx_time_get();
	}
}
}

/* End of File ---------------------------------------------------------------*/

