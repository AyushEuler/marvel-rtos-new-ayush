/**
 * @file canControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef CANCONTROLS_H_
#define CANCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>
#include "main.h"
#include "bmsConfiguration.h"
/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    STANDARD_CAN_ID = 0,
    EXTENDED_CAN_ID,
}canIDType_t;

typedef enum
{
	CAN_FIFO_0 = 0,
	CAN_FIFO_1,
} canFIFO_t;

typedef enum
{
	E = 1,
	S = 0,
} TypeofCANID;


typedef enum
{
	CAN_FILTER_BANK_0  = 0,
	CAN_FILTER_BANK_1,
	CAN_FILTER_BANK_2,
	CAN_FILTER_BANK_3,
	CAN_FILTER_BANK_4,
	CAN_FILTER_BANK_5,
	CAN_FILTER_BANK_6,
	CAN_FILTER_BANK_7,
	CAN_FILTER_BANK_8,
	CAN_FILTER_BANK_9,
	CAN_FILTER_BANK_10,
	CAN_FILTER_BANK_11,
	CAN_FILTER_BANK_12,
	CAN_FILTER_BANK_13,
} canFilterBank_t;

#define IS_CAN_ID_TYPE(ID)            	   (((ID) == STANDARD_CAN_ID) || ((ID) == EXTENDED_CAN_ID))

#define IS_CAN_FIFO(FIFO)            	   (((FIFO) == CAN_FIFO_0)  || ((FIFO) == CAN_FIFO_1))

#define IS_CAN_FILTER_BANK(BANK)     	   (((BANK)  == CAN_FILTER_BANK_0)   || ((BANK) == CAN_FILTER_BANK_1) ||\
											((BANK)  == CAN_FILTER_BANK_2)   || ((BANK) == CAN_FILTER_BANK_3) ||\
											((BANK)  == CAN_FILTER_BANK_4)   || ((BANK) == CAN_FILTER_BANK_5) ||\
											((BANK)  == CAN_FILTER_BANK_6)   || ((BANK) == CAN_FILTER_BANK_7) ||\
											((BANK)  == CAN_FILTER_BANK_8)   || ((BANK) == CAN_FILTER_BANK_9) ||\
											((BANK)  == CAN_FILTER_BANK_10)  || ((BANK) == CAN_FILTER_BANK_11) ||\
											((BANK)  == CAN_FILTER_BANK_12)  || ((BANK) == CAN_FILTER_BANK_13) )

#define CAN_MESSAGE_DELAY               	29

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t beginCAN  (uint8_t canPort);
bmsStatus_t sendCAN   (uint8_t canPort, uint8_t CANIDtype, uint32_t CANIDValue, uint8_t *txCANData, uint8_t U8_DLC);
void Transmit_on_CAN1(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC);

bmsStatus_t enableCANReceptionInInterruptMode(uint8_t canPort);
bmsStatus_t enableCANFilters(uint8_t canPort);


bmsStatus_t canFilterIDList(uint32_t receivedCanId1, uint32_t receivedCanId2,
                        canIDType_t idType, canFilterBank_t filterBank,
                        canFIFO_t fifo, FunctionalState state);

bmsStatus_t canFilterIDMask(uint32_t canID, uint32_t canMask,
                        canIDType_t idType, canFilterBank_t filterBank,
                        canFIFO_t fifo, FunctionalState state);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
#endif
/* End of File ---------------------------------------------------------------*/

