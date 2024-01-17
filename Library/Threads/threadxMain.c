/**
 * @file threadxMain.c
 * @brief 
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "math.h"

/* Private includes ----------------------------------------------------------*/
#include "bmsErrorCodes.h"
#include "customDelays.h"
#include "initializeOperations.h"
#include "monitorOperations.h"
#include "controlOperations.h"
#include "estimateParameters.h"
#include "conversion.h"
#include "usart.h"
#include "dataProcessing.h"
#include "dht.h"
#include "can.h"
#include "onBoardCANv2b.h"
#include "bmsCANMatrix.h"
#include "onBoardIOs.h"
#include "onBoardMemory.h"

#include "bmsConfiguration.h"
#include "gpioControls.h"
#include "onBoardUARTs.h"
#include "canControls.h"
#include "flashControls.h"
#include "onBoardFlash.h"
#include "wwdg.h"
#include "iwdg.h"
#include "timerControls.h"
#include "functionalTests.h"
#include "monitorOperations.h"
#include "lookupTable.h"
#include "firmware_upgrade_app.h"
#include "batteryCooling.h"
#include "HighLevelStatemachine_types.h"

#include "stm32l4xx_it.h"
/* Self include --------------------------------------------------------------*/
#include "threadxMain.h"



/* Type definiton ------------------------------------------------------------*/
TX_BYTE_POOL    mainBytePool;

TX_THREAD       errorStateHandler;
TX_THREAD       collectBatteryData;
TX_THREAD       protection;
TX_THREAD       contactorControl;
TX_THREAD       ccEKF;
TX_THREAD       coulombCounting;
TX_THREAD       stateMachine;
TX_THREAD       canTxHandler;
TX_THREAD       canRxHandler;
TX_THREAD       firmware_upgrade_thread;


TX_QUEUE        canTxQueue;
TX_QUEUE        canRxQueue;

TX_MUTEX        mutex_1;

TX_EVENT_FLAGS_GROUP system_event_flag;

/* External variables definiton ----------------------------------------------*/

uint8_t errorCode = 0;
char debugData[256] = {'\0'};

//MARVEL DATA
uint8_t MARVEL_DATA[8] = {0};
uint8_t HARDWARE_DATA[8] = {0};

//vehicle state from Matlab
HighLevelMode state;

volatile uint16_t timerCounter=0;
extern uint64_t __IO bmsDefaultConfig[25];
extern uint64_t Systemfrequency;
extern uint8_t waitCount;
extern struct bcsParameters_t bcsParameters;
extern uint8_t activeAPP;

uint8_t qstatus = 0;
uint8_t balanceActiveFlag = 0;
volatile uint32_t taskBreakpointCycleCount = 0;

//charging 
volatile float time = 0;
float k = 1.03;
int32_t targetCellVoltage_mV = 0;
uint8_t SlowChargingMaxCurrent_A = 40;
uint8_t findCase[2][2] = { {1 , 2}, {3, 4} };

//balancing
uint16_t voltageReadCounter = 0;
uint16_t cellsToBalance[NUMBER_OF_CMU] = {0};

//testing
uint8_t bmsInTestingFlag = 0;
uint8_t fullChargeLatch = 0;
uint8_t latchCheckFlag;

uint8_t ccReadBufferEEPROM[21] = {0};
uint64_t ccReadBufferFLASH[6] = {0};
uint8_t ccWriteBufferEEPROM[21] = {0};
uint64_t ccWriteBufferFLASH[6] = {0};

uint8_t eepromEraseFlag = 0;
uint16_t shortCircuitCounter = 0;

extern uint8_t externalShortCircuitDetectionFlag;

//soc
uint32_t eepromSOCCounter = 0;
bool startEEPROMWriteFlag;
bmsStatus_t statusSoC = BMS_ERROR;

//temp
uint8_t temp;
uint8_t humid;
uint8_t Temp_Humid[8];
/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

dataPipelineInput_t   dpInputData     = {0};
dataPipelineOutput_t  dpOutputData    = {0};

cellBalancingInput_t    cbInputData     = {0};
cellBalancingOutput_t   cbOutputData    = {0};

protectionInput_t       pInputData      = {0};
protectionOutput_t      pOutputData     = {0};

contactorInput_t        cInputData      = {0};
contactorOutput_t       cOutputData     = {0};

DynamicLimitsTable_t    dynamicLimitsTable = {0};
vcuData_t               vcuData            = {0};

#ifdef EKF
ccEKFInput_t        ccEKFInputData     = {0};
ccEKFOutput_t       ccEKFOutputData    = {0};
#else
coulombCountingInput_t  ccInputData     = {0};
coulombCountingOutput_t ccOutputData    = {0};
#endif

stateMachineInput_t        smInputData     = {0};
stateMachineOutput_t       smOutputData    = {0};

chargingOutputData_t       chargingOutputData = {0};

thresholdData_t     thresholdData = {0};
coulombCountingOutput_t ccOutputDataEEPROM = {0};
coulombCountingOutput_t ccOutputDataFlash = {0};


//GP100-2P
const int32_t continuousChargingTableData[132] = { 
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    20,		20,		20,		20,		20,		20,		20,		20,		20,		20,		10,
    40,		40,		40,		40,		40,		40,		40,		40,		40,		40,		15,
    40,		40,		40,		40,		40,		40,		40,		40,		40,		40,		30,
    80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		60,
    140,	140,	140,	140,	140,	140,	140,	140,	140,	100,	60,
    180,	180,	180,	180,	180,	180,	180,	180,	180,	100,	60,
    180,	180,	180,	180,	180,	180,	180,	180,	180,	100,	60,
    120,	120,	120,	120,	120,	120,	120,	120,	120,	100,	40,
    80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		20,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0 };

const int32_t continuousDischargingTableData[165] =  { 
    20,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
    40,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
    40,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
    40,		100,	100,		100,		100,		100,		100,		100,		100,		100,		100,
    40,		100,	120,		120,		120,		120,		120,		120,		120,		120,		120,
    40,		100,	120,		120,		120,		120,		120,		120,		120,		120,		120,
    60,		100,	160,		160,		160,		160,		160,		160,		160,		160,		160,
    90,		120,	160,		200,		200,		200,		200,		200,		200,		160,		160,
    150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
    150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
    150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
    150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
    40,		60,		120,		160,		160,		160,		160,		160,		160,		160,		160,
    20,		40,		60,			120,		120,		120,		120,		120,		120,		120,		120,
    20,		20,		20,			20,			20,			40,			40,			40,			40,			40,			40 };

    /*Instantaneous Dynamic current Limits Table*/

const int32_t instantaneousDischargingTableData[165] = {
    20,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
    40,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
    40,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
    40,         100,        100,         100,         100,         100,         100,         100,         100,         100,         100,
    40,         100,        120,         120,         120,         120,         120,         120,         120,         120,         120,
    40,         100,        120,         120,         120,         200,         200,         200,         200,         200,         200,
    60,         100,        200,         200,         200,         200,         200,         200,         200,         200,         200,
    90,         160,        300,         300,         300,         300,         300,         300,         300,         200,         200,
    100,        300,        300,         400,         400,         400,         400,         400,         400,         300,         200,
    100,        300,        300,         400,         400,         400,         400,         400,         400,         300,         200,
    100,        160,        300,         400,         400,         400,         400,         400,         400,         300,         200,
    140,        160,        300,         400,         400,         400,         400,         400,         400,         300,         200,
    80,         160,        200,         200,         200,         200,         200,         200,         200,         200,         160,
    40,         120,        120,         120,         120,         120,         120,         120,         120,         120,         120,
    20,         60,         60,          60,          60,          60,          60,          60,          60,          60,          60 };


const int32_t instantaneousChargingTableData[132] = {
    0,        0,         0,         0,         0,         0,         0,         0,         0,         0,         0,
    0,        30,        30,        30,        30,        30,        30,        30,        30,        20,        10,
    20,       30,        30,        30,        30,        30,        30,        30,        30,        20,        10,
    40,       60,        60,        60,        60,        60,        60,        60,        60,        60,        20,
    40,       80,        80,        80,        80,        80,        80,        80,        80,        60,        40,
    80,       120,       120,       120,       120,       120,       120,       120,       120,       100,       60,
    140,      160,       200,       240,       240,       240,       240,       240,       240,       200,       100,
    180,      180,       200,       300,       300,       300,       300,       300,       300,       200,       100,
    180,      180,       200,       300,       300,       300,       300,       300,       300,       200,       100,
    120,      160,       200,       300,       300,       300,       300,       300,       300,       200,       60,
    80,       100,       160,       200,       200,       200,       200,       200,       200,       160,       60,
    0,        40,        60,        100,       100,       100,       100,       100,       100,       100,       40 };


const int32_t eFuseChargingTableData[132] = {
    0,       0,        0,         0,        0,        0,        0,        0,        0,        0,       0,
    0,       9000,     9000,      9000,     9000,     9000,     9000,     9000,     9000,     4000,    1000,
    4000,    1000,     1000,      1000,     1000,     1000,     1000,     1000,     1000,     0,       0,
    16000,   4000,     4000,      4000,     4000,     4000,     4000,     4000,     4000,     4000,    250,
    16000,   16000,    16000,     16000,    16000,    16000,    16000,    16000,    16000,    4000,    1000,
    64000,   16000,    16000,     16000,    16000,    16000,    16000,    16000,    16000,    4000,    4000,
    196000,  4000,     36000,     100000,   100000,   100000,   100000,   100000,   100000,   100000,  36000,
    324000,  4000,     4000,      144000,   144000,   144000,   144000,   144000,   144000,   100000,  16000,
    324000,  4000,     4000,      144000,   144000,   144000,   144000,   144000,   144000,   100000,  16000,
    144000,  16000,    64000,     324000,   324000,   324000,   324000,   324000,   324000,   100000,  4000,
    64000,   4000,     64000,     144000,   144000,   144000,   144000,   144000,   144000,   64000,   16000,
    0,       16000,    36000,     100000,   100000,   100000,   100000,   100000,   100000,   100000,  16000 };  

const int32_t eFuseDischargingTableData[165] =  {   
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		2000,	2000,	2000,	32000,	32000,	32000,	32000,	32000,	32000,
    0,		0,		8000,	8000,	8000,	8000,	8000,	8000,	8000,	8000,	8000,
    0,		8000,	98000,	50000,	50000,	50000,	50000,	50000,	50000,	8000,	8000,
    0,		500,	50000,	200000,	200000,	200000,	200000,	200000,	200000,	50000,	50000,
    0,		500,	50000,	200000,	200000,	200000,	200000,	200000,	200000,	50000,	50000,
    0,		500,	50000,	200000,	200000,	200000,	200000,	200000,	200000,	50000,	50000,
    0,		500,	50000,	200000,	200000,	200000,	200000,	200000,	200000,	50000,	50000,
    0,		50000,	32000,	8000,	8000,	8000,	8000,	8000,	8000,	8000,	8000,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0 };




const int32_t socData[11] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
const int32_t chargingTemperatureData[12] =  { -0, 5, 10, 10, 15, 20, 25, 30, 40, 45, 50, 55 };
const int32_t dischargingTemperatureData[15] =  { -20, -15, -10, 0, 5, 10, 15, 20, 25, 30, 40, 45, 50, 55, 60 };

const int32_t chargingMaxElements[2] = { 12, 11 };    //[0] - temp, [1] - soc
const int32_t dischargingMaxElements[2] = { 15, 11 }; //[0] - temp, [1] - soc

uint8_t temperatureLimits[2][12] = 
{
    {
        OTC_ERROR_TEMPERATURE_GROUP_1, OTC_WARNING_TEMPERATURE_GROUP_1, OTC_RECOVERY_TEMPERATURE_GROUP_1, \
        OTD_ERROR_TEMPERATURE_GROUP_1, OTD_WARNING_TEMPERATURE_GROUP_1, OTD_RECOVERY_TEMPERATURE_GROUP_1, \
        UTC_ERROR_TEMPERATURE_GROUP_1, UTC_WARNING_TEMPERATURE_GROUP_1, UTC_RECOVERY_TEMPERATURE_GROUP_1, \
        UTD_ERROR_TEMPERATURE_GROUP_1, UTD_WARNING_TEMPERATURE_GROUP_1, UTD_RECOVERY_TEMPERATURE_GROUP_1  \
    },

    {
        OTC_ERROR_TEMPERATURE_GROUP_2, OTC_WARNING_TEMPERATURE_GROUP_2, OTC_RECOVERY_TEMPERATURE_GROUP_2, \
        OTD_ERROR_TEMPERATURE_GROUP_2, OTD_WARNING_TEMPERATURE_GROUP_2, OTD_RECOVERY_TEMPERATURE_GROUP_2, \
        UTC_ERROR_TEMPERATURE_GROUP_2, UTC_WARNING_TEMPERATURE_GROUP_2, UTC_RECOVERY_TEMPERATURE_GROUP_2, \
        UTD_ERROR_TEMPERATURE_GROUP_2, UTD_WARNING_TEMPERATURE_GROUP_2, UTD_RECOVERY_TEMPERATURE_GROUP_2  \
    },
};

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief ThreadX Kernel Entry - Initialize
 * 
 * @param first_unused_memory 
 */
void tx_application_define(void *first_unused_memory)
{
    CHAR *pointer;
    tx_byte_pool_create(&mainBytePool, "mainBytePool", first_unused_memory, sizeBytePool_mainBytePool);
    
    //collect battery data task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_collectBatteryData, TX_NO_WAIT);

    tx_thread_create(&collectBatteryData, "BatteryData", entry_collectBatteryData, 0, pointer, 
                     sizeStack_collectBatteryData, priority_P1, priority_P1, TX_NO_TIME_SLICE, TX_AUTO_START);

    // protection task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_protection, TX_NO_WAIT);

    tx_thread_create(&protection, "Protection", entry_protection, 0, pointer, 
                     sizeStack_protection, priority_P1, priority_P1, TX_NO_TIME_SLICE, TX_AUTO_START);

    // control main contactors task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_contactorControl, TX_NO_WAIT);


    tx_thread_create(&contactorControl, "contactorControl", entry_contactorControl, 0, pointer, 
                     sizeStack_contactorControl, priority_P2, priority_P2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&mainBytePool, (void*)&pointer, FIRMWARE_UPGRADE_STACK_SIZE, TX_NO_WAIT);

    tx_thread_create(&firmware_upgrade_thread, "firmware upgrade task", firmware_upgrade_task_entry, 0,
                    pointer, FIRMWARE_UPGRADE_STACK_SIZE, 0, 0,
                    TX_NO_TIME_SLICE, TX_AUTO_START);
                    //time slice or no activate
    
    tx_event_flags_create(&system_event_flag, "system event flag");


#ifdef EKF

    // task EKF
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_ccEKF, TX_NO_WAIT);

    tx_thread_create(&ccEKF, "ccEKF", entry_ccEKF, 0, pointer, 
                     sizeStack_ccEKF, priority_P3, priority_P3, TX_NO_TIME_SLICE, TX_AUTO_START);

#else
    // task coulomb counting
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_coulombCounting, TX_NO_WAIT);

    tx_thread_create(&coulombCounting, "coulombCounting", entry_coulombCounting, 0, pointer, 
                     sizeStack_coulombCounting, priority_P1, priority_P1, TX_NO_TIME_SLICE, TX_AUTO_START);

#endif

    // task state machine
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_stateMachine, TX_NO_WAIT);

    tx_thread_create(&stateMachine, "stateMachine", entry_stateMachine, 0, pointer, 
                     sizeStack_stateMachine, priority_P3, priority_P3, TX_NO_TIME_SLICE, TX_AUTO_START);

    //CAN Tx Data Task 
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_canTxHandler, TX_NO_WAIT);

    tx_thread_create(&canTxHandler, "canTxHandler", entry_canTxHandler, 0, pointer, 
                     sizeStack_canTxHandler, priority_P4, priority_P4, TX_NO_TIME_SLICE, TX_AUTO_START);

    //CAN Rx Data Task 
    qstatus = tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_canRxHandler, TX_NO_WAIT);

    qstatus = tx_thread_create(&canRxHandler, "canRxHandler", entry_canRxHandler, 0, pointer, 
                     sizeStack_canRxHandler, priority_P4, priority_P4, TX_NO_TIME_SLICE, TX_AUTO_START);

    /*************** Queue Init **********************************************************************************/
    
    //can Tx Queue
    tx_byte_allocate(&mainBytePool, (void*)&pointer, CAN_TX_QUEUE_LENGTH * CAN_TX_QUEUE_MESSAGE_SIZE, TX_NO_WAIT);

    tx_queue_create(&canTxQueue, "canTxQueue", CAN_TX_QUEUE_MESSAGE_SIZE, pointer, CAN_TX_QUEUE_LENGTH * CAN_TX_QUEUE_MESSAGE_SIZE);

    //can Rx Queue
    qstatus = tx_byte_allocate(&mainBytePool, (void*)&pointer, CAN_RX_QUEUE_LENGTH * CAN_RX_QUEUE_MESSAGE_SIZE, TX_NO_WAIT);

    qstatus = tx_queue_create(&canRxQueue, "canRxQueue", CAN_RX_QUEUE_MESSAGE_SIZE, pointer, CAN_RX_QUEUE_LENGTH * CAN_RX_QUEUE_MESSAGE_SIZE);
    
}

/**
 * @brief can transmit data thread
 * 
 * @param threadInput 
 */
void entry_canTxHandler(ULONG threadInput)
{
    bmsStatus_t     status          = BMS_ERROR;
    canQueueData_t  canTxQueueData  = {0};
    
	while(1)
	{
		if((QUEUE_RECEIVE(&canTxQueue, &canTxQueueData, WAIT_FOREVER)) == TX_SUCCESS)
		{
			// message received in queue, send it to CAN
            RETRY(RETRY_CAN_COMMUNICATION, status, sendBatteryDataOnCAN(STANDARD_CAN_ID, canTxQueueData.identifier, canTxQueueData.data));

            //if CAN Tx fails then send the same data to the queue again
            if(status != BMS_OK)
            {
                QUEUE_SEND(&canTxQueue, &canTxQueueData, NO_WAIT);
            }
            
            #ifdef WWDOG
                HAL_WWDG_Refresh(&hwwdg);
            #else  
                HAL_IWDG_Refresh(&hiwdg);
            #endif
        }
	}
}

/**
 * @brief can receive data thread
 * 
 * @param threadInput 
 */
void entry_canRxHandler(ULONG threadInput)
{
    canQueueData_t canRxQueueData = {0};

	while(1)
	{
		if((QUEUE_RECEIVE(&canRxQueue, &canRxQueueData, WAIT_FOREVER)) == TX_SUCCESS)
		{
			// message received in queue, now Process the message
            switch(canRxQueueData.identifier)
            {
                case FLASH_CONFIG_CAN_RX_ID:
                    writeDataToFlash(FLASH_CONFIG_SECTION, FLASH_CONFIG_ADDRESS, defaultFlashConfigData, FLASH_CONFIG_TOTAL_LENGTH, canRxQueueData.data);
                    break;

                // case STARK_VEHICLE_STATE:
                //     currentManipulation = (canRxQueueData.data[0] == 0) ? 1 : 0;                    
                //     break;

                case VEHICLE_STATE_CAN_RX_ID:
                    vcuData.FastCharge           = canRxQueueData.data[0];
                    vcuData.SlowCharge           = canRxQueueData.data[1];
                    vcuData.isChargerConnected   = canRxQueueData.data[2];
                    vcuData.ServiceMode          = canRxQueueData.data[4];
                    vcuData.TransmissionFail     = canRxQueueData.data[5];
                    vcuData.VCUPermanentFail     = canRxQueueData.data[6];

                    vcuData.BalanceEn_atRest     = canRxQueueData.data[7];
                    break;
                
                case STARK_VEHICLE_STATE:
                    canRxQueueData.data[3] == 1 ? (vcuData.isChargerConnected = 1):(vcuData.isChargerConnected = 0);
                    (canRxQueueData.data[1] & (1 << 5)) ? (vcuData.SlowCharge = 1):(vcuData.SlowCharge = 0);
                    (canRxQueueData.data[1] & (1 << 6)) ? (vcuData.FastCharge = 1):(vcuData.FastCharge = 0); 

                    // vcuData.ServiceMode          = canRxQueueData.data[4];
                    // vcuData.TransmissionFail     = canRxQueueData.data[5];
                    // vcuData.VCUPermanentFail     = canRxQueueData.data[6];
                    // vcuData.BalanceEn_atRest     = canRxQueueData.data[7]; 
                    break;

                

                case EEPROM_RESET_CAN_RX_ID:
                    //Byte 0 - EEPROM RESET
                    if(canRxQueueData.data[0] == 1 && eepromEraseFlag == 0)
                    {
                        eepromEraseFlag++;
                        memset((void*)&ccOutputDataEEPROM, 0, sizeof(coulombCountingOutput_t));
                        writeCCDataToBuffer(&ccOutputDataEEPROM, ccWriteBufferEEPROM, ccWriteBufferFLASH);
                        memoryWrite(eeprom, 0x00, ccWriteBufferEEPROM, sizeof(ccWriteBufferEEPROM));

#ifdef STORE_SOC_IN_FLASH
                        ccOutputFlash[0] = 0;
                        ccOutputFlash[1] = 0;
                        ccOutputFlash[2] = 0; 
                        ccOutputFlash[3] = 0;
                        ccOutputFlash[4] = 0;
    
                        flashWrite(SOC_DATA_ADDRESS, ccOutputFlash, sizeof(ccOutputFlash));
#endif
                        //clear the cc input and cc Output
                        memset((void*)&ccInputData, 0, sizeof(coulombCountingInput_t));
                        memset((void*)&ccOutputData, 0, sizeof(coulombCountingOutput_t));

                        systemReset();
                    }
                    
                    //Byte 1 - SHORT CIRCUIT 
                    if(canRxQueueData.data[1] == 1) //Reset the SC Pin
                    {
                        resetShortCircuit(shortCircuit);
                        externalShortCircuitDetectionFlag = 0;
                    }

                    //Byte 2 - BMS IN Testing
                    if(canRxQueueData.data[2] == 1) //Reset the SC Pin
                    {
                        bmsInTestingFlag = 1;
                    }
                    else
                    {
                        bmsInTestingFlag = 0;
                    }

                    //Byte 3 - BMS system reset
                    if(canRxQueueData.data[3] == 1)
                    {
                        systemReset();
                    }

                    break;
 
                default:
                    break;
            }
        
            #ifdef WWDOG
                HAL_WWDG_Refresh(&hwwdg);
            #else  
                HAL_IWDG_Refresh(&hiwdg);
            #endif
        }
	}
}

/**
 * @brief state machine thread
 * 
 * @param threadInput 
 */
void entry_stateMachine(ULONG threadInput)
{
    // bmsMainData_t mdata = {0};
    // bmsStatus_t status = BMS_ERROR;

    HighLevelStatemachine_initialize();
    ChargingAlgorithm_initialize();
    stateMachineInit(&thresholdData, &dpInputData, &HighLevelStatemachine_U);
    chargingInit(&thresholdData, &ChargingAlgorithm_U);

    while(1)
    {
        stateMachineInput(&dpOutputData, &pOutputData, &HighLevelStatemachine_U);
        HighLevelStatemachine_step();
        stateMachineOutput(&smOutputData, &HighLevelStatemachine_Y);

        chargingInput(&DataPipeline_U, &DataPipeline_Y, &ChargingAlgorithm_U);
        ChargingAlgorithm_step();
        chargingOutput(&chargingOutputData, &ChargingAlgorithm_Y);

        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif
        
        delayMilliSeconds(LOOP_DELAY_STATE_MACHINE);
    }
}

/**
 * @brief protection thread
 * 
 * @param threadInput 
 */
void entry_protection(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    // bmsStatus_t status = BMS_ERROR;
    
    pipelineInit();
    readFromPipeline(&mdata);

    Protection_initialize();
    protectionInit(&thresholdData, &dpInputData, &dpOutputData, &pInputData, &Protection_U);

    while(1)
    {
        readFromPipeline(&mdata);
        protectionInput(&mdata, &dpOutputData, &pInputData, &Protection_U);
        Protection_step();
        uint8_t data[8]={0};
        ProtectionFlags test = Protection_Y.ProtectionOutput_e.TemperatureGradient;
        if(test == Warning) data[0]=1;
        else if(test == Error) data[0]=2;
        else if(test == Recovery) data[0]=3;
        else if(test == PermanentFail) data[0]=4;
        else if(test == NoError) data[0]=0;
        sendCAN(0,CAN_ID_STD,0x100,data,8);
        protectionOutput(&mdata, &pOutputData, &Protection_Y);

        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif

        delayMilliSeconds(LOOP_DELAY_PROTECTION);
    }
}

/**
 * @brief upgrade firmware thread
 * 
 * @param threadInput 
 */

void firmware_upgrade_task_entry(ULONG thread_input){
    while(1){
        upgrade_firmware();
        delayMilliSeconds(LOOP_DELAY_CONTACTOR_CONTROL);

        }
	
}


/**
 * @brief contactor ocntrol thread
 * 
 * @param threadInput 
 */
void entry_contactorControl(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    pipelineInit();
    readFromPipeline(&mdata);
    Contactors_initialize();
    
    contactorInit(&thresholdData, &cInputData, &Contactors_U);
    delayMilliSeconds(1000);

    while(1)
    {
        readFromPipeline(&mdata);
        contactorInput(&mdata, &dpOutputData, &pOutputData, &cInputData, &cOutputData, &Contactors_U);
        Contactors_step();
        contactorOutput(&mdata, &cOutputData, &Contactors_Y);
        // Contactors_DW.temporalCounter_i1 += LOOP_DELAY_CONTACTOR_CONTROL;
        
        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif
        
        delayMilliSeconds(LOOP_DELAY_CONTACTOR_CONTROL);
    }
}


#ifdef EKF
/**
 * @brief ccEKF thread 
 * 
 * @param threadInput 
 */

void entry_ccEKF(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    // bmsStatus_t status = BMS_ERROR;

    // pipelineInit();
    delayMilliSeconds(1000); // delay before init
    EKF_initialize();
    coulombCountingEKFInit(&ccEKFInputData, &EKF_U);
    
    while(1)
    {
        readFromPipeline(&mdata);
        coulombCountingEKFInput(&mdata, &dpOutputData, &pOutputData, &ccEKFInputData, &EKF_U);
        EKF_step();
        coulombCountingEKFOutput(&mdata, &ccEKFOutputData, &EKF_Y);
        
        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif
        
        delayMilliSeconds(100);
    }
}

#else
/**
 * @brief coulomb counting thread 
 * 
 * @param threadInput 
 */

void entry_coulombCounting(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    // bmsStatus_t status = BMS_ERROR;

    // pipelineInit();
    delayMilliSeconds(1000); // delay before init
    SOCEstimation_initialize();
    coulombCountingInit(&ccInputData, &SOCEstimation_U, &ccOutputData);
    
    while(1)
    {
        readFromPipeline(&mdata);
        statusSoC = memoryRead(eeprom, 0x00, ccReadBufferEEPROM, sizeof(ccReadBufferEEPROM));
        if(0xFF != ccReadBufferEEPROM[0]) //When the device turns on for the very first time, EEPROM will be empty
        {
            readCCDataFromBuffer(&ccOutputDataEEPROM, ccReadBufferEEPROM, ccReadBufferFLASH);
        }
        coulombCountingInput(&mdata, &dpOutputData, &pOutputData, &ccInputData, &SOCEstimation_U);
        SOCEstimation_step();
        coulombCountingOutput(&mdata, &ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash, &SOCEstimation_Y);

        //Keep the function locked until ccOutput SoC == EEPROM SoC {indicating completion of first cycle}
        writeSoCToFlash(&ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash);

        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif

        delayMilliSeconds(100);
    }
}
#endif


/**
 * @brief collect battery data thread
 * 
 * @param threadInput 
 */
void entry_collectBatteryData(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    bmsStatus_t status = BMS_ERROR;
    // uint8_t shortCircuitBuf[2] = {0};

    pipelineInit();
    readFromPipeline(&mdata);
    initializeCMU(&mdata);
    

    

    CellBalancing_initialize();
    DataPipeline_initialize();

    #ifdef EKF
        thresholdInit(&thresholdData, &dpInputData, &cbInputData, &pInputData, &cInputData, &ccEKFInputData);
    #else
        thresholdInit(&thresholdData, &dpInputData, &cbInputData, &pInputData, &cInputData, &ccInputData);
    #endif
    dataPipelineInit(&dynamicLimitsTable, &dpInputData, &DataPipeline_U);
    cellBalancingInit(&thresholdData, &cbInputData, &CellBalancing_U, &dpInputData);

    #ifdef EEPROM_TEST
        ccOutputDataEEPROM.initialCapacity          = 0;
        ccOutputDataEEPROM.totalCapacityRemaining   = 9007;
        ccOutputDataEEPROM.totalCapacityDischarge    = 9007;
        ccOutputDataEEPROM.TotalCapacityExchange_mAh = 9008;
        ccOutputDataEEPROM.SOC_pct                   = 5;
            
        writeCCDataToBuffer(&ccOutputDataEEPROM, ccWriteBuffer);
        memoryWrite(eeprom, 0x00, ccWriteBuffer, sizeof(ccWriteBuffer));

        ccOutputDataEEPROM.initialCapacity          = 0;
        ccOutputDataEEPROM.totalCapacityRemaining   = 0;
        ccOutputDataEEPROM.totalCapacityDischarge    = 0;
        ccOutputDataEEPROM.TotalCapacityExchange_mAh = 0;
        ccOutputDataEEPROM.SOC_pct                   = 0;
    #endif

    //Read data from eeprom at start

    // #ifdef SOC_IN_FLASH



    // statusSoC = memoryRead(eeprom, 0x00, ccReadBufferEEPROM, sizeof(ccReadBufferEEPROM));
    // if(0xFF != ccReadBufferEEPROM[0]) //When the device turns on for the very first time, EEPROM will be empty
    // {
    //     readCCDataFromBuffer(&ccOutputDataEEPROM, ccReadBufferEEPROM, ccReadBufferFLASH);
    // }

    while(1)
    {
        uint32_t timerr = HAL_GetTick();
        if(timerr<0) break;
        // uint32_t TX_Mailbox;
        #ifdef DHT11_SENSOR
            dht11(&humid,&temp);
            Temp_Humid[0]=temp;
            Temp_Humid[1]=humid;
            //status = sendCANDataOnQueue(0x100, Temp_Humid, sizeof(Temp_Humid));
            //CHECK_STATUS(status);
            sendCAN(0,CAN_ID_STD,0x100,Temp_Humid,8);
            //HAL_CAN_AddTxMessage(&hcan1, CAN_ID_STD,Temp_Humid, &TX_Mailbox);
        #endif
        
        //don't take readings while balancing
        if((cbOutputData.balancingState == 0) || (cbOutputData.balancingState == 1))
        {   
            RETRY(RETRY_CMU_READ, status, readCellVoltages(&mdata));
            RETRY(RETRY_CMU_READ, status, readCellTemperatures(&mdata));
            RETRY(RETRY_CMU_READ, status, readPackStatus(&mdata));
            // RETRY(RETRY_CMU_READ, 1 , readCellVoltages(&mdata));
            // RETRY(RETRY_CMU_READ, BMS_ERROR, readCellTemperatures(&mdata));
            // RETRY(RETRY_CMU_READ, BMS_ERROR, readPackStatus(&mdata));
        }

#ifdef LTC2949
        #ifdef LTC2949_DEBUG
        RETRY(RETRY_CCM_READ, status, readStatusFaultReg(&mdata));
        #endif

        status = CheckDeviceReady();
        status = BMS_OK;
        if(status == BMS_OK)
        {  
            RETRY(RETRY_CCM_READ, status, readPackVoltageCurrent(&mdata));
            RETRY(RETRY_CCM_READ, status, readChargeEnergyData(&mdata));
            // RETRY(RETRY_CCM_READ, BMS_ERROR , readPackVoltageCurrent(&mdata));
            // RETRY(RETRY_CCM_READ, BMS_ERROR , readChargeEnergyData(&mdata));
            uint32_t dur = HAL_GetTick()- timerr;
            printf("%ld", dur);
        }
        else
        {
            mdata.ccmData.packCurrent           = 0.0;
            mdata.ccmData.packFilteredCurrent   = 0.0;
            mdata.ccmData.packVoltage           = 0.0;
            mdata.ccmData.packPower             = 0.0;
            mdata.ccmData.ccmInternalTemp       = 0.0;
            mdata.ccmData.packFastCurrent       = 0.0;
            mdata.ccmData.packCapacity 	        = 0.0;
            mdata.ccmData.packEnergy 		    = 0.0;
            mdata.ccmData.timeBase 		        = 0.0;
            mdata.ccmData.timeBaseResetCounter++;
		    mdata.ccmData.isCCMReset = 1;
        }
        
#elif ADS1120

#endif
        
        mdata.mcuResetCounter++;

        #ifdef EKF
            dataPipelineInput(&mdata, &dpInputData, &ccEKFOutputData, &DataPipeline_U);
        #else
            //faulty ntc
            mdata.cmuData.cmuTemperatures.externalNTC[0][1]=18;
            dataPipelineInput(&mdata, &dpInputData, &ccOutputData, &DataPipeline_U); //input to matlab
            //dpInputData.temperature1_C[1]=1;
        #endif
            
        DataPipeline_step(); //matlab functions
        dataPipelineOutput(&mdata, &dpOutputData, &DataPipeline_Y); //output from matlab
        
        calculateDynamicCurrentLimits(&mdata, &dpInputData, &dpOutputData, &dynamicLimitsTable, &DataPipeline_Y);

        cellBalancingInput(&mdata, &dpOutputData, &cbInputData, &CellBalancing_U);
        CellBalancing_step();//CellBalancing_step(&mdata);
        cellBalancingOutput(&mdata, &cbOutputData, &CellBalancing_Y);

        float requestedCurrent_A = 0;
        requestedCurrent_A = calculateChargingCurrent(&dpOutputData, &ccOutputData);
        // requestedCurrent_A = 10;
        // writeSoCToFlash(&ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash);

        SoCManipulation(&mdata, &SOCEstimation_Y);
        storeErrorCodes(&pOutputData);

        #ifndef CBMS_TEST
        //if charge enable, send charging data
        // if(dpOutputData.isChargeEn)
        if(1)
        {
            sendChargingDataOnCAN(62.0, requestedCurrent_A);
            delayMilliSeconds(CAN_MESSAGE_DELAY);
        }
        else
        {
            //sending CAN charger ID for sanity check
            //TODO: try sending this ID only once during boot up, because this for only sanity check
            sendChargingDataOnCAN(0, 0);
            delayMilliSeconds(CAN_MESSAGE_DELAY);
        }
        #endif
        
        #ifdef DEBUG_CAN
        	#ifdef EKF
                sendCoulombCountingEKFDataOnCAN(&ccEKFInputData, &ccEKFOutputData);
            #else
                sendCoulombCountingDataOnCAN(&ccInputData, &ccOutputData);
            #endif
        
            sendContactorDataOnCAN(&thresholdData, &cInputData, &cOutputData);
            sendCellBalancingDataOnCAN(&thresholdData, &cbInputData, &cbOutputData);
            sendProtectionDataOnCAN(&thresholdData, &pInputData, &pOutputData);
            sendBMSDataOnCAN(&mdata, &dpOutputData, &dpInputData);
        #endif

        writeCCMDataToPipeline(&mdata.ccmData);
        writeCMUDataToPipeline(&mdata.cmuData);

        status = sendDataOnCAN(&mdata, &pOutputData, &ccOutputData);

        //publishing MARVEL data on CAN 
        //TODO send this on queue
        //TODO change from debug IDs to MARVEL CAN IDs
        MARVEL_DATA[0] = 2;     
        MARVEL_DATA[1] = SW_MAJOR_VERSION;
        MARVEL_DATA[2] = SW_MINOR_VERSION;
        MARVEL_DATA[3] = SW_PATCH_VERSION;
        MARVEL_DATA[4] = HW_MAJOR_VERSION;
        MARVEL_DATA[5] = HW_MINOR_VERSION;
        MARVEL_DATA[6] = SW_CONFIG_VERSION;
        MARVEL_DATA[7] = activeAPP;
 
        status = sendCAN(communicationCAN,S, CAN_DEBUG_ID_033, MARVEL_DATA, 8);

       //publishing on 7A1
        HARDWARE_DATA[0] = 2;
        HARDWARE_DATA[1] = HW_MAJOR_VERSION;
        HARDWARE_DATA[2] = HW_MINOR_VERSION;
        HARDWARE_DATA[3] = 0;


        status = sendCAN(communicationCAN,S, CAN_DEBUG_ID_042, HARDWARE_DATA, 8);
        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif

        CellBalancing_DW.temporalCounter_i1 += LOOP_DELAY_CMU_DATA_COLLECTION;
        delayMilliSeconds(LOOP_DELAY_CMU_DATA_COLLECTION);
    }
}

bmsStatus_t writeCCDataToBuffer(coulombCountingOutput_t *ccOutput, uint8_t *writeBufferEEPROM, uint64_t *writeBufferFLASH)
{
    bmsStatus_t status = BMS_ERROR;
    
	if( (NULL == ccOutput)  
      )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    for(int i = 0; i < 4; i++) //i < 4, number of bytes - all data types are of 4 bytes
    {
        //if more elements are added then increase the size of ccWriteBuffer and ccReadBufferEEPROM array.
        writeBufferEEPROM[i]            = (int) ccOutput->initialCapacity           >> (i*8);
        writeBufferEEPROM[i+4]          = (int) ccOutput->totalCapacityRemaining    >> (i*8);
        writeBufferEEPROM[i+8]          = (int) ccOutput->totalCapacityDischarge    >> (i*8);
        writeBufferEEPROM[i+12]         = (int) ccOutput->TotalCapacityExchange_mAh >> (i*8);
        writeBufferEEPROM[i+16]         = (int) ccOutput->SOC_pct                   >> (i*8);
    }
        writeBufferEEPROM[20]         = (uint8_t) fullChargeLatch;

    #ifdef SOC_IN_FLASH

    for(int i = 0; i < 4; i++) //i < 4, number of bytes - all data types are of 4 bytes
    {
        writeBufferFLASH[0] = ccOutput->initialCapacity;
        writeBufferFLASH[1] = ccOutput->totalCapacityRemaining;
        writeBufferFLASH[2] = ccOutput->totalCapacityDischarge; 
        writeBufferFLASH[3] = ccOutput->TotalCapacityExchange_mAh;
        writeBufferFLASH[4] = ccOutput->SOC_pct;
    }
#endif


	return status;
}

bmsStatus_t readCCDataFromBuffer(coulombCountingOutput_t *ccOutput, uint8_t *readBufferEEPROM, uint64_t *readBufferFLASH)
{
    bmsStatus_t status = BMS_ERROR;
    
	if( (NULL == ccOutput)  
      )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    for(int i = 0; i < 4; i++)
    {
        ccOutput->initialCapacity           |= readBufferEEPROM[i]    << (i*8);
        ccOutput->totalCapacityRemaining    |= readBufferEEPROM[i+4]  << (i*8);
        ccOutput->totalCapacityDischarge    |= readBufferEEPROM[i+8]  << (i*8);
        ccOutput->TotalCapacityExchange_mAh |= readBufferEEPROM[i+12] << (i*8);
        ccOutput->SOC_pct                   |= readBufferEEPROM[i+16] << (i*8);
    }
        fullChargeLatch                     = readBufferEEPROM[20];

    #ifdef SOC_IN_FLASH
    
    status = flashRead(SOC_DATA_ADDRESS, readBufferFLASH, 5);

    if(0xFFFFFFFFFFFFFFFF != readBufferFLASH[0]) //When the device turns on for the very first time, FLASH will be empty
    // if(ccReadBuffer[3] > 0) //When the device turns on for the very first time, FLASH will be empty
    {
        ccOutputDataFlash.initialCapacity           = readBufferFLASH[0];
        ccOutputDataFlash.totalCapacityRemaining    = readBufferFLASH[1];
        ccOutputDataFlash.totalCapacityDischarge    = readBufferFLASH[2];
        ccOutputDataFlash.TotalCapacityExchange_mAh = readBufferFLASH[3];
        ccOutputDataFlash.SOC_pct                   = readBufferFLASH[4];
    }

    #endif

    status = BMS_OK;
	return status;
}

float calculateChargingCurrent(dataPipelineOutput_t *dpOutput, coulombCountingOutput_t *ccOutput)
{

    uint32_t dcli_A                 = 0;
    int32_t maxCellVoltage_mV       = 0;
    int isCurrent                   = 0;
    int isDCLI                      = 0;
    float requestedCurrent_A        = 0;

    maxCellVoltage_mV   = dpOutput->voltageSense.voltageMax;
    // maxCellVoltage_mV   = 3430; //target 3420
    targetCellVoltage_mV = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX] - 10;
    dcli_A              = (dpOutput->DCL_ChargingCurrent_A);
    isDCLI              = dcli_A >= SlowChargingMaxCurrent_A ? 1 : 0;
    requestedCurrent_A  = k * (targetCellVoltage_mV - maxCellVoltage_mV);
    isCurrent           = requestedCurrent_A >= SlowChargingMaxCurrent_A ? 1 : 0;
    requestedCurrent_A = (targetCellVoltage_mV - maxCellVoltage_mV) <= 0 ? 0 : requestedCurrent_A;
    switch(findCase[isCurrent][isDCLI])
    {
        case 1:
            if(requestedCurrent_A > dcli_A)
            {
                requestedCurrent_A = dcli_A;
            }
            else
            {
                requestedCurrent_A = requestedCurrent_A;
            }
            break;

        case 2:
            requestedCurrent_A = requestedCurrent_A;
            break;

        case 3:
            requestedCurrent_A = dcli_A;
            break;

        case 4:
            requestedCurrent_A = SlowChargingMaxCurrent_A;
            break;            
    }

    if(fullChargeLatch)
    {
        requestedCurrent_A = 0;
    }
    return requestedCurrent_A;
}

bmsStatus_t calculateDynamicCurrentLimits(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput, DynamicLimitsTable_t *ldata, ExtY_DataPipeline_T *dpOutputMBD)
{
    bmsStatus_t status                 = BMS_ERROR;

    if( (NULL == mdata)        ||\
        (NULL == dpInput)      ||\
        (NULL == dpOutput)     ||\
        (NULL == ldata)    )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    int32_t cellTemperature            = 0;
    int32_t contChargingLimit_A        = 0;
    int32_t contDischargingLimit_A     = 0;
    int32_t instantaneousChargingLimit_A    = 0;
    int32_t instantaneousDischargingLimit_A = 0; 
    int32_t eFuseChargingLimit_A2s     = 0;
    int32_t eFuseDischargingLimit_A2s  = 0;
    static uint8_t eFuseAllowedTime    = 10;

    if(dpInput->TemperatureSelectionforDynamicC) //1 is Avg temperature
    {
        for(uint8_t cellIndex = 0; cellIndex < 6; cellIndex++)
        {
            cellTemperature += dpInput->temperature1_C[cellIndex];
        }

        //taking avg of 6 temperatures
        cellTemperature /= 6;
    }
    else    //0 is for max temperature
    {
        cellTemperature = dpOutput->temperatureSense[0].temperatureMax;
    }
    
        //To Do: If SoC or Temperature is not changed then return and don't do the calculations

    //ContinuousCharging
    contChargingLimit_A = 0;
    lookup2d(&(ldata->ContinuousChargingTable), dpInput->soc, cellTemperature, &contChargingLimit_A);

    //ContinuousDischarging
    contDischargingLimit_A = 0;
    lookup2d(&(ldata->ContinuousDischargingTable), dpInput->soc, cellTemperature, &contDischargingLimit_A);

    //InstantaneousCharging
    instantaneousChargingLimit_A = 0;
    lookup2d(&(ldata->instantaneousChargingTableData), dpInput->soc, cellTemperature, &instantaneousChargingLimit_A);

    //InstantaneousDischarging
    instantaneousDischargingLimit_A = 0;
    lookup2d(&(ldata->instantaneousDischargingTableData), dpInput->soc, cellTemperature, &instantaneousDischargingLimit_A);

    //calculating eFuse values for charging and discharging
    // eFuseChargingLimit_A2s = ((instantaneousChargingLimit_A - contChargingLimit_A)^2) * eFuseAllowedTime;
    // eFuseDischargingLimit_A2s = ((instantaneousDischargingLimit_A - contDischargingLimit_A)^2) * eFuseAllowedTime;

    eFuseChargingLimit_A2s    = pow((instantaneousChargingLimit_A - contChargingLimit_A), 2) * eFuseAllowedTime;
    eFuseDischargingLimit_A2s = pow((instantaneousDischargingLimit_A - contDischargingLimit_A), 2) * eFuseAllowedTime;

    // //eFuseCharging
    // eFuseChargingLimit_A2s = 0;
    // lookup2d(&(ldata->eFuseChargingTable), dpInput->soc, cellTemperature, &eFuseChargingLimit_A2s);

    // //eFuseDischarging
    // eFuseDischargingLimit_A2s = 0;
    // lookup2d(&(ldata->eFuseDishargingTable), dpInput->soc, cellTemperature, &eFuseDischargingLimit_A2s);

    //copying dynamic current limits to dataPipeline Output and dataPipeline Output MBD
    dpOutput->DCL_ChargingCurrent_A     = contChargingLimit_A;
    dpOutput->DCL_DisChargingCurrent_A  = contDischargingLimit_A;

    dpOutputMBD->DataPipelineb.DCL_ChargingCurrent_A     = contChargingLimit_A;
    dpOutputMBD->DataPipelineb.DCL_DisChargingCurrent_A  = contDischargingLimit_A;

    //copying eFuse limits to dataPipeline Output and dataPipeline Output MBD
    dpOutput->i2t_ChargingCurrent_A2sec    = eFuseChargingLimit_A2s;
    dpOutput->i2t_DisChargingCurrent_A2sec = eFuseDischargingLimit_A2s;

    dpOutputMBD->DataPipelineb.i2t_ChargingCurrent_A2sec    = eFuseChargingLimit_A2s;
    dpOutputMBD->DataPipelineb.i2t_DisChargingCurrent_A2sec = eFuseDischargingLimit_A2s;

    //copying to main data
    //To Do: copy it using separate function
    mdata->dynamicLimitIn_A              = contChargingLimit_A;
    mdata->dynamicLimitOut_A             = contDischargingLimit_A;

    mdata->instaneousLimitIn_A           = instantaneousChargingLimit_A;
    mdata->instaneousLimitOut_A          = instantaneousDischargingLimit_A;

    status = BMS_OK;
	return status;
}


#ifdef EKF
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, protectionInput_t *pInput, contactorInput_t *cInput, ccEKFInput_t *ccEKFInput)
#else
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, protectionInput_t *pInput, contactorInput_t *cInput, coulombCountingInput_t *ccInput)
#endif
{
    bmsStatus_t status = BMS_ERROR;
#ifdef EKF
	if( (NULL == tData)        ||\
        (NULL == dpInput)      ||\
        (NULL == cbInput)      ||\
        (NULL == pInput)       ||\
        (NULL == cInput)       ||\
        (NULL == ccEKFInput))
#else
	if( (NULL == tData)        ||\
        (NULL == dpInput)      ||\
        (NULL == cbInput)      ||\
        (NULL == pInput)       ||\
        (NULL == cInput)       ||\
        (NULL == ccInput))
#endif
	{
		status = BMS_THRESHOLD_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    tData->MaxOperatingTemp_C           = 50;
    tData->MaxAllowedTempChange_C       = 50;
    tData->MaxAllowedTempGradientGroup1_C = 10;
    tData->MaxAllowedTempGradientGroup2_C = 100;

    //VCU
    tData->NormalModeTimeout_msec       = 100;
    tData->PowerSaverModeTimeout_msec   = 100;
    tData->FaultsModeTimeout_msec       = 100;
    tData->ServiceModeTimeout_msec      = 100;

    // tData->dynamicInstTimeout_msec      = 200;
    tData->i2tTimeout_msec              = 1000;
    tData->dynamicContTimeout_msec      = 5000;
    tData->dynamicCurrentThresholdsGain = 1;
    tData->TempGroupTimeout             = 100;
    tData->PermanentFailCounts_eFuse    = 4;
    

    //protections threshold init
    tData->OCC_ErrorCurrent 			= CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_ERROR_CURRENT_A_INDEX]);
    tData->OCC_WarningCurrent 		    = CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_WARNING_CURRENT_A_INDEX]);
    tData->OCD_ErrorCurrent 			= CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCD_ERROR_CURRENT_A_INDEX]	);
    tData->OCD_WarningCurrent 		    = CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_WARNING_CURRENT_A_INDEX]);

    tData->OCC_ErrorTimeout 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OCC_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX] ;
    tData->OCD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OCD_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX] ;

    tData->RecoveryProtections_OCC_Warning_mA       = 1000;
    tData->RecoveryProtections_OCC_Error_mA         = 1000;
    tData->RecoveryProtections_OCD_Warning_mA       = 1000;
    tData->RecoveryProtections_OCD_Error_mA         = 1000;
    tData->CurrentChangeTimeout_msec                = 1000;
    tData->eFuseMeltingTimeout_msec                 = 1000;

    //temperature external
    for(int i = 0; i < 2; i++)
    {
        tData->temperatureData[i].OTC_ErrorTemperature 		    = temperatureLimits[i][OTC_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTC_WarningTemperature 		= temperatureLimits[i][OTC_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTC_RecoveryTemperature 	    = temperatureLimits[i][OTC_RECOVERY_TEMPERATURE_INDEX];
        
        tData->temperatureData[i].OTD_ErrorTemperature 		    = temperatureLimits[i][OTD_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTD_WarningTemperature 		= temperatureLimits[i][OTD_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTD_RecoveryTemperature 	    = temperatureLimits[i][OTD_RECOVERY_TEMPERATURE_INDEX];
        
        tData->temperatureData[i].UTC_ErrorTemperature	 	    = temperatureLimits[i][UTC_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTC_WarningTemperature	 	= temperatureLimits[i][UTC_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTC_RecoveryTemperature	 	= temperatureLimits[i][UTC_RECOVERY_TEMPERATURE_INDEX];

        tData->temperatureData[i].UTD_ErrorTemperature	 	    = temperatureLimits[i][UTD_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTD_WarningTemperature	 	= temperatureLimits[i][UTD_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTD_RecoveryTemperature	 	= temperatureLimits[i][UTD_RECOVERY_TEMPERATURE_INDEX];

        /*------ Temperature Groups 1 to 5 Timeouts ------*/
        tData->temperatureData[i].OTC_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_RecoveryTimeout 	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

        tData->temperatureData[i].OTD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_WarningTimeout  	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_RecoveryTimeout 	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];


        tData->temperatureData[i].UTC_ErrorTimeout  	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_WarningTimeout	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_RecoveryTimeout  	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

        tData->temperatureData[i].UTD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_WarningTimeout	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    }
    
    //ov uv
    tData->slowCharging_OV_ErrorVoltage	 		    = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->slowCharging_OV_WarningVoltage	 		= defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX] - 20;
    tData->slowCharging_OV_RecoveryVoltage	 		= defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX] - 50;

    tData->fastCharging_OV_ErrorVoltage             = 3650;
    tData->fastCharging_OV_WarningVoltage           = 3650 - 20;          
    tData->fastCharging_OV_RecoveryVoltage          = 3650 - 50;


    tData->OV_ErrorTimeout	 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OV_WarningTimeout            = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
    tData->OV_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    tData->OV_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    
    tData->UV_ErrorVoltage	 		    = defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->UV_WarningVoltage	 		= defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX] + 20;
    tData->UV_RecoveryVoltage	 		= defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX] + 50;
    tData->UV_ErrorTimeout	 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->UV_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
    tData->UV_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX]; 
    tData->UV_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

    tData->HighImbalanceError_mV        = 100;
    tData->MinOpearatingVolt_mV         = 2900;

    tData->ShortCircuitCurrent_mA       = CONVERT_AMPERE_TO_MILLI_AMPERE(500);
    tData->SuddenVoltageDrop_mV_by_time = 1000;

    tData->ThermalRunawayWarningTimeout_msec    = 1000;
    tData->ThermalRunawayErrorTimeout_msec      = 1000;
    tData->TemperatureGradientTimeout_msec      = 1000;
    tData->HighImbalanceFlagTimeout_msec        = 1000;
    tData->ShortCircuitDetect_msec              = 500;
    tData->SuddenVoltageDropTimeout_msec        = 500;
    tData->AdditionalProtectionEn               = 0;     //0 - Disable, 1 - Enable
    tData->HighImbalanceFlagEn                  = 0;     //0 - Disable, 1 - Enable

    //cell balancing threshold init
    tData->balancingMinThreshold                  = defaultFlashConfigData[CELL_BALANCING_START_VOLTAGE_mV_INDEX];
    tData->balancingStateRestPeroid               = defaultFlashConfigData[BALANCING_MAX_OFF_TIME_ms_INDEX];          //off time
    tData->balancingMaxAllowedImbalance           = defaultFlashConfigData[CELL_IMBALANCE_THRESHOLD_mV_INDEX] ;
    // tData->balancingOVProtectionThreshold         = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->balancing_SlowCh_OVThr_mV              = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->balancing_FastCh_OVThr_mV              = 3610;  

    //   int32_T Balancing_SlowCh_OVThr_mV;
    //   int32_T Balancing_FastCh_OVThr_mV;

    // tData->balancingPeriod                        = 100;  //not used                 //on time  //100ms
    
    tData->balancing_DefaultBalancingTimeOn_msec  = defaultFlashConfigData[BALANCING_MAX_ON_TIME_ms_INDEX];
    tData->balancingCurrentDerateActivationTemp_C = 80;  //derating will start at 80 degree C
    tData->balancingCurrentDerateCheckTime_msec   = 100;

    tData->ExternalShortCheckTime_msec            = 1000;
    tData->ExternalShortLimit                     = 5;
    tData->ContWeldCheckLimit                     = 200;

    //contactor threshold init
    tData->contactorCutoffTime			          = 100;
    tData->preChargeContactorCurrent			  = 2000;
    tData->ContWeldCheckCurrent_mA                = CONVERT_AMPERE_TO_MILLI_AMPERE(500);
    tData->ContactorWarningCounterLimit           = 232;
    tData->ContactorWeldTimeout_msec              = 200;
    tData->OpenToPreChargeStateTime_msec          = 1000;
    tData->Min_PreChargeTime_msec                 = 3000;
    tData->Max_PreChargeTime_msec                 = 5000;
    tData->ContOpenCurrent_mA                     = 100000;
    tData->FlagDebounceTime_msec                  = 700;
    tData->Min_ClosedToOpeningTime_msec           = 500;
    tData->Max_ClosedToOpeningTime_msec           = 500;

    //VCU
    tData->BMS_Timeout_msec                     = 1000;
    tData->ContactorsCheck_Timeout_msec         = 1000;
    tData->VCU_Timeout_msec                     = 1000;
    tData->MosfetsPreChargeCurrentLimit_mA      = 1000;


    //charging
    // tData->ChargeAlgorithm.SOC_forCVCharge_cpct         = ;
    tData->ChargeAlgorithm.BatteryRevivalVoltage_mV     = 3100;
    tData->ChargeAlgorithm.NoCharge_K                   = 0;
    tData->ChargeAlgorithm.BatteryRevival_K             = 1000;
    tData->ChargeAlgorithm.CC_K                         = 1000;
    tData->ChargeAlgorithm.CC_Balancing_K               = 1000;
    tData->ChargeAlgorithm.CV_K                         = 1000;
    // tData->ChargeAlgorithm.FastCharging_K               = ;
    tData->ChargeAlgorithm.CVTransitionVoltage_mV       = 3380;
    tData->ChargeAlgorithm.TransitionTimeout_msec       = 500;


    //contactor data init
    cInput->communicationFlag 		              = 0;           // 0
    cInput->permanentFailFlag		              = 0;           // 0
    cInput->contactorErrorFlag		              = 0;           // 0

#ifdef EKF
    //ccEKF
    ccEKFInput->vehicleRestTime                   = CONVERT_HOUR_TO_MILLI_SECONDS(500);   //in msec
    ccEKFInput->vehicleRestCurrent                = CONVERT_AMPERE_TO_MILLI_AMPERE(5);
    ccEKFInput->maxPackCapacity                   = CONVERT_AMPERE_HOUR_TO_MILLI_AMPERE_HOUR(180);
    ccEKFInput->minPackCapacity                   = CONVERT_AMPERE_HOUR_TO_MILLI_AMPERE_HOUR(0);
#else
    //coulomb counting
    // ccInput->vehicleRest                          = CONVERT_HOUR_TO_MILLI_SECONDS(500);   //in msec
    // ccInput->vehicleRestCurrent                   = CONVERT_AMPERE_TO_MILLI_AMPERE(5);
    // ccInput->maxPackCapacity                      = CONVERT_AMPERE_HOUR_TO_MILLI_AMPERE_HOUR(180);
    // ccInput->minPackCapacity                      = CONVERT_AMPERE_HOUR_TO_MILLI_AMPERE_HOUR(0);
#endif

    //data pipeline
    dpInput->thresholdDataBus.chargeDetectionThreshold_mA        = CONVERT_AMPERE_TO_MILLI_AMPERE(5);
    dpInput->isDynamicCurrentEn                                  = 1;
    dpInput->nCells_series                                       = defaultFlashConfigData[CELL_IN_SERIES_INDEX];
    dpInput->nCells_parallel                                     = 2;
    dpInput->TemperatureSelectionforDynamicC                     = 0;    //0 - Max Temp as input, 1 - Avg Temp as input 
    dpInput->nCelltemperatureGroups                              = 1;    //group 1 temperatures are used for dynamic limits

    //cell balancing
    cbInput->BalancingTempGroup      = 2;
    cbInput->StLineEq_varA           = (defaultFlashConfigData[BALANCING_DERATING_START_TEMP_C_INDEX] - defaultFlashConfigData[BALANCING_DERATING_END_TEMP_C_INDEX])/(float)(defaultFlashConfigData[BALANCING_MAX_ON_TIME_ms_INDEX] - defaultFlashConfigData[BALANCING_MIN_ON_TIME_ms_INDEX]);
    cbInput->StLineEq_varB           = defaultFlashConfigData[BALANCING_DERATING_END_TEMP_C_INDEX] - (cbInput->StLineEq_varA * defaultFlashConfigData[BALANCING_MIN_ON_TIME_ms_INDEX]);

    status = BMS_OK;
    return status;
}

bmsStatus_t dataPipelineInit(DynamicLimitsTable_t *ldata, dataPipelineInput_t *dpInput, ExtU_DataPipeline_T *dpInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpInput)    ||\
        (NULL == dpInputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    dpInputMBD->Thresholds.ChargeDetectionThreshold_mA      =  dpInput->thresholdDataBus.chargeDetectionThreshold_mA;
    dpInputMBD->isDynamicCurrentEn                          =  dpInput->isDynamicCurrentEn;
    dpInputMBD->nCells_series                               =  dpInput->nCells_series;
    dpInputMBD->nCells_parallel                             =  dpInput->nCells_parallel;
    dpInputMBD->TemperatureSelectionforDynamicC             =  dpInput->TemperatureSelectionforDynamicC;
    dpInputMBD->nCelltemperatureGroups                      =  dpInput->nCelltemperatureGroups;
    
    // Create a Table2d structure for ContinuousCharging
    ldata->ContinuousChargingTable.numberOfColumns = (chargingMaxElements[1]);                               //number of elements in SoC table 
    ldata->ContinuousChargingTable.numberOfRows    = (chargingMaxElements[0]);                               //number of elements in temperature table
    ldata->ContinuousChargingTable.columnsData     = (const int32_t *) socData;                              //SoC data table
    ldata->ContinuousChargingTable.rowsData        = (const int32_t *) chargingTemperatureData;              //Temperaure data table  
    ldata->ContinuousChargingTable.tableData       = (const int32_t *) continuousChargingTableData;          //tableData

    // Create a Table2d structure for ContinuousDischarging  
    ldata->ContinuousDischargingTable.numberOfColumns = (dischargingMaxElements[1]);                         //number of elements in SoC table 
    ldata->ContinuousDischargingTable.numberOfRows    = (dischargingMaxElements[0]);                         //number of elements in temperature table
    ldata->ContinuousDischargingTable.columnsData     = (const int32_t *) socData;                           //SoC data table
    ldata->ContinuousDischargingTable.rowsData        = (const int32_t *) dischargingTemperatureData;        //Temperaure data table  
    ldata->ContinuousDischargingTable.tableData       = (const int32_t *) continuousDischargingTableData;    //tableData

    // Create a Table2d structure for InstantaneousCharging 
    ldata->instantaneousChargingTableData.numberOfColumns     = (chargingMaxElements[1]);                        //number of elements in SoC table 
    ldata->instantaneousChargingTableData.numberOfRows        = (chargingMaxElements[0]);                        //number of elements in temperature table
    ldata->instantaneousChargingTableData.columnsData         = (const int32_t *) socData;                          //SoC data table
    ldata->instantaneousChargingTableData.rowsData            = (const int32_t *) chargingTemperatureData;          //Temperaure data table 
    ldata->instantaneousChargingTableData.tableData           = (const int32_t *) instantaneousChargingTableData;   //tableData

    // Create a Table2d structure for InstantaneousCharging 
    ldata->instantaneousDischargingTableData.numberOfColumns     = (dischargingMaxElements[1]);                           //number of elements in SoC table 
    ldata->instantaneousDischargingTableData.numberOfRows        = (dischargingMaxElements[0]);                           //number of elements in temperature table
    ldata->instantaneousDischargingTableData.columnsData         = (const int32_t *) socData;                             //SoC data table
    ldata->instantaneousDischargingTableData.rowsData            = (const int32_t *) dischargingTemperatureData;             //Temperaure data table 
    ldata->instantaneousDischargingTableData.tableData           = (const int32_t *) instantaneousDischargingTableData;   //tableData

    // Create a Table2d structure for eFuseCharging  
    ldata->eFuseChargingTable.numberOfColumns = (chargingMaxElements[1]);                         //number of elements in SoC table 
    ldata->eFuseChargingTable.numberOfRows    = (chargingMaxElements[0]);                         //number of elements in temperature table
    ldata->eFuseChargingTable.columnsData     = (const int32_t *) socData;                        //SoC data table
    ldata->eFuseChargingTable.rowsData        = (const int32_t *) chargingTemperatureData;        //Temperaure data table  
    ldata->eFuseChargingTable.tableData       = (const int32_t *) eFuseChargingTableData;         //tableData

    // Create a Table2d structure for eFuseDischarging
    ldata->eFuseDishargingTable.numberOfColumns = (dischargingMaxElements[1]);                    //number of elements in SoC table
    ldata->eFuseDishargingTable.numberOfRows    = (dischargingMaxElements[0]);                    //number of elements in temperature table
    ldata->eFuseDishargingTable.columnsData     = (const int32_t *) socData;                      //SoC data table
    ldata->eFuseDishargingTable.rowsData        = (const int32_t *) dischargingTemperatureData;   //Temperaure data table 
    ldata->eFuseDishargingTable.tableData       = (const int32_t *) eFuseDischargingTableData;    //tableData

    status = BMS_OK;
	return status;
}

#ifdef EKF
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, ccEKFOutput_t *ccEKFOutput, ExtU_DataPipeline_T *dpInputMBD)
#else
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, coulombCountingOutput_t *ccOutput, ExtU_DataPipeline_T *dpInputMBD)
#endif
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == dpInput)      ||\
        (NULL == dpInputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    
    
    
    #ifdef FUNCTIONALTESTS
        //choose the test to be performed from the enum
    functionalTests(temperatureGradient , mdata);
#endif
            
    dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(mdata->ccmData.packCurrent);        //milli Ampere

    // dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(1);        //milli Ampere


#ifdef EKF
    dpInput->soc     = ccEKFOutput->socCC;
#else
    dpInput->soc     = ccOutput->SOC_pct;
#endif

    uint8_t index = 0;
    //cell voltages 
	for(uint8_t cmuIndex = 0; cmuIndex < defaultFlashConfigData[NUMBER_OF_CMU_INDEX]; cmuIndex++)
	{
		for(uint8_t cellIndex = 0; cellIndex < 10; cellIndex++)
		{
			dpInput->voltage[index++] = mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex];
		}
	}

    //To Do: Efficient way to select number of cells in series
    dpInput->voltage[9] = mdata->cmuData.cmuVolatges.cellVoltages[0][8];
    dpInput->voltage[19] = mdata->cmuData.cmuVolatges.cellVoltages[1][8];
    
    //temperature groups

    
    
    
    for(uint8_t cellIndex = 0; cellIndex < 6; cellIndex++)
    {
        if(cellIndex < 4)
        {
            //group 1 - 4 externalNTC- slave 1
            dpInput->temperature1_C[cellIndex] = mdata->cmuData.cmuTemperatures.externalNTC[0][cellIndex];
        }
        else
        {
            //group 1 - 4 externalNTC - slave 2
            dpInput->temperature1_C[cellIndex] = mdata->cmuData.cmuTemperatures.externalNTC[1][cellIndex - 4];
        }
    }
    
    dpInput->temperature1_C[6] = mdata->cmuData.cmuTemperatures.externalNTC[1][1];

    dpInput->shuntTemp = mdata->cmuData.cmuTemperatures.externalNTC[1][2];

    for(uint8_t cellIndex = 0; cellIndex < 6; cellIndex++)
    {
        if(cellIndex < 3)
        {
            //group 2 - 3 onBoardNTC - slave 1
            dpInput->temperature2_C[cellIndex] = mdata->cmuData.cmuTemperatures.onBoardNTC[0][cellIndex];
        }
        else
        {
            //group 2 - 3 onBoardNTC - slave 2
            dpInput->temperature2_C[cellIndex] = mdata->cmuData.cmuTemperatures.onBoardNTC[1][cellIndex - 3];
        }
    }
	
    // copy
    dpInput->temperature2_C[6] = mdata->cmuData.cmuTemperatures.onBoardNTC[1][2];

    //VCU
    dpInput->SlowCharge             = vcuData.SlowCharge;
    dpInput->FastCharge             = vcuData.FastCharge;
    dpInput->isChargerConnected     = vcuData.isChargerConnected;
    dpInput->TransmissionFail       = vcuData.TransmissionFail;
    dpInput->VCUPermanentFail       = vcuData.VCUPermanentFail;
    dpInput->ServiceMode            = vcuData.ServiceMode;
    dpInput->BalanceEn_atRest       = vcuData.BalanceEn_atRest;
    dpInput->EmergencyDisconnectEn  = vcuData.EmergencyDisconnectEn;
    dpInput->ChargersCurrentLimit_A = 40;   
    dpInput->LoopTimer_SOC_msec     = 100;


    //copying to main data
    //To Do: copy it using separate function
    // mdata->socData.estimatedSoC     = ccOutput->SOC_pct;

    switch(bmsInTestingFlag)
    // switch(1)
    {
        // case 0: //BMS not in Testing
        // break;

        case 1: //BMS in charge-discharge testing
            if(dpInput->current >= 500) //greater than +ve 1 Amps, mode: in charging
            {
                dpInput->SlowCharge             = 1;
                dpInput->isChargerConnected     = 1;
            }
            else //mode: in discharging
            {
                dpInput->SlowCharge             = 0;
                dpInput->isChargerConnected     = 0;
            }
        break;

        default:
        break;
    }


    /*************** Copy to MBD structures *********************************************************************************************************/
    
    dpInputMBD->CurrentSense                 = dpInput->current;
    dpInputMBD->SOC                          = dpInput->soc;

    //VCU
    dpInputMBD->SlowCharge                   = dpInput->SlowCharge;
    dpInputMBD->FastCharge                   = dpInput->FastCharge;
    dpInputMBD->isChargerConnected           = dpInput->isChargerConnected;
    dpInputMBD->TransmissionFail             = dpInput->TransmissionFail;
    dpInputMBD->VCUPermanentFail             = dpInput->VCUPermanentFail;
    dpInputMBD->ServiceMode                  = dpInput->ServiceMode;
    dpInputMBD->BalanceEn_atRest             = dpInput->BalanceEn_atRest;
    dpInputMBD->EmergencyDisconnectEn        = dpInput->EmergencyDisconnectEn;
    dpInputMBD->LoopTimer_SOC_msec           = dpInput->LoopTimer_SOC_msec;
    dpInputMBD->ChargersCurrentLimit_A       = dpInput->ChargersCurrentLimit_A;

    for(uint8_t i = 0; i < 20; i++)
    {
        dpInputMBD->VoltageSense[i]          = dpInput->voltage[i];
    }
    
    for(uint8_t i = 0; i < 7; i++)
    {
        dpInputMBD->Temperatures1_C[i]       = dpInput->temperature1_C[i];
        dpInputMBD->Temperatures2_C[i]       = dpInput->temperature2_C[i];
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t dataPipelineOutput(bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, ExtY_DataPipeline_T *dpOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == dpOutputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/

    dpOutput->current                   = dpOutputMBD->DataPipelineb.Current_mA;
    dpOutput->isChargeEn                = dpOutputMBD->DataPipelineb.isChargeEn; 
    
    dpOutput->temperatureSense[0].temperatureMax    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].T_max;
    dpOutput->temperatureSense[0].temperatureMin    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].T_min;
    dpOutput->temperatureSense[0].temperatureDelta  = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].TemperatureDelta_C;

    dpOutput->temperatureSense[1].temperatureMax    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].T_max;
    dpOutput->temperatureSense[1].temperatureMin    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].T_min;
    dpOutput->temperatureSense[1].temperatureDelta  = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].TemperatureDelta_C;

    dpOutput->maxTemperature_C          = dpOutputMBD->DataPipelineb.MaxTemperature_C;
    dpOutput->minTemperature_C          = dpOutputMBD->DataPipelineb.MinTemperature_C;
    dpOutput->deltaTemperature_C        = dpOutputMBD->DataPipelineb.DeltaTemperature_C;
    dpOutput->DeltaTemperatureGroup1_C  = dpOutputMBD->DataPipelineb.DeltaTemperatureGroup1_C;
    dpOutput->DeltaTemperatureGroup2_C  = dpOutputMBD->DataPipelineb.DeltaTemperatureGroup2_C;

    dpOutput->isDynamicCurrentEn        = dpOutputMBD->DataPipelineb.isDynamicCurrentEn;
    dpOutput->nCells_series             = dpOutputMBD->DataPipelineb.nCells_series;
    dpOutput->nCells_parallel           = dpOutputMBD->DataPipelineb.nCells_parallel;

    //VCU
    dpOutput->VCU.SlowCharge            = dpOutputMBD->DataPipelineb.VCU.SlowCharge;
    dpOutput->VCU.FastCharge            = dpOutputMBD->DataPipelineb.VCU.FastCharge;
    dpOutput->VCU.isChargerConnected    = dpOutputMBD->DataPipelineb.VCU.isChargerConnected;
    dpOutput->VCU.TransmissionFail      = dpOutputMBD->DataPipelineb.VCU.TransmissionFail;
    dpOutput->VCU.VCUPermanentFail      = dpOutputMBD->DataPipelineb.VCU.VCUPermanentFail;
    dpOutput->VCU.ServiceMode           = dpOutputMBD->DataPipelineb.VCU.ServiceMode;
    dpOutput->VCU.BalanceEn_atRest      = dpOutputMBD->DataPipelineb.VCU.BalanceEn_atRest;
    dpOutput->VCU.EmergencyDisconnectEn = dpOutputMBD->DataPipelineb.VCU.EmergencyDisconnectEn;
    dpOutput->LoopTimer_SOC_msec            = dpOutputMBD->DataPipelineb.LoopTimer_SOC_msec;
    dpOutput->VCU.ChargersCurrentLimit_A    = dpOutputMBD->DataPipelineb.VCU.ChargersCurrentLimit_A;


    for(uint8_t i = 0; i < 20; i++)
    {
        dpOutput->voltageSense.voltage[i] = dpOutputMBD->DataPipelineb.VoltageSenseBus.Voltages_mV[i];
    }

    dpOutput->voltageSense.voltageMin   = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_min;
    dpOutput->voltageSense.voltageMax   = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_max;
    dpOutput->voltageSense.voltageDelta = dpOutputMBD->DataPipelineb.VoltageSenseBus.VoltageDelta_mV;

    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        for(uint8_t i = 0; i < 7; i++)
        {
            dpOutput->temperatureSense[groupIndex].temperature[i] = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].Temperatures_C[i];
        }

        dpOutput->temperatureSense[groupIndex].temperatureMin   = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].T_min;
        dpOutput->temperatureSense[groupIndex].temperatureMax   = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].T_max;
        dpOutput->temperatureSense[groupIndex].temperatureDelta = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].TemperatureDelta_C;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingInit(thresholdData_t *tData, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD, dataPipelineInput_t *dpInput)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == cbInput)      ||\
        (NULL == cbInputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //cell balancing threshold
    cbInputMBD->Thresholds.Balancing_Min_Thr_mV               = tData->balancingMinThreshold;
    cbInputMBD->Thresholds.Balancing_StateRestPeriod_msec     = tData->balancingStateRestPeroid;
    cbInputMBD->Thresholds.Balancing_MaxAllowedImb_mV         = tData->balancingMaxAllowedImbalance;
    // cbInputMBD->Thresholds.Balancing_OVProtectThr_mV          = tData->balancingOVProtectionThreshold;

//       int32_T Balancing_SlowCh_OVThr_mV;
//   int32_T Balancing_FastCh_OVThr_mV;
    cbInputMBD->Thresholds.Balancing_SlowCh_OVThr_mV          = tData->balancing_SlowCh_OVThr_mV;
    cbInputMBD->Thresholds.Balancing_FastCh_OVThr_mV          = tData->balancing_FastCh_OVThr_mV;

    cbInputMBD->Thresholds.Balancing_DefaultBalancingTimeOn_msec           = tData->balancing_DefaultBalancingTimeOn_msec;
    cbInputMBD->Thresholds.BalancingCurrentDerateActivationTemp_C          = tData->balancingCurrentDerateActivationTemp_C;
    cbInputMBD->Thresholds.BalancingCurrentDerateCheckTime_msec            = tData->balancingCurrentDerateCheckTime_msec;

    cbInputMBD->BalancingTempGroup                            = cbInput->BalancingTempGroup;
    cbInputMBD->BalancingTempGroup_a                          = cbInput->BalancingTempGroup;
    cbInputMBD->StLineEq_varA                                 = cbInput->StLineEq_varA;
    cbInputMBD->StLineEq_varB                                 = cbInput->StLineEq_varB;

    cbInputMBD->DataPipeline.nCells_series                    = dpInput->nCells_series;

    for(int i = 0; i < 2; i++)
    {
        cbInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C   = tData->temperatureData[i].OTC_ErrorTemperature;
        cbInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C   = tData->temperatureData[i].OTD_ErrorTemperature;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == cbInput)      ||\
        (NULL == cbInputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    cbInput->dataPipelineBus.current                    = dpOutput->current;
    cbInput->dataPipelineBus.VCU.isChargerConnected     = dpOutput->VCU.isChargerConnected;
    cbInput->dataPipelineBus.VCU.BalanceEn_atRest       = dpOutput->VCU.BalanceEn_atRest;

    //Cell Voltages
    for(uint8_t i = 0; i < 20; i++)
    {
        cbInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    }

    cbInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    cbInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    cbInput->dataPipelineBus.voltageSense.voltageDelta = dpOutput->voltageSense.voltageDelta;
    
    //temperatures
    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        // for(uint8_t i = 0; i < 4; i++)
        // {
        //     cbInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i] = dpOutput->temperatureSense[groupIndex].temperature[i];
        // }

        // cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin     = dpOutput->temperatureSense[groupIndex].temperatureMin;
        cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax     = dpOutput->temperatureSense[groupIndex].temperatureMax;
        // cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta   = dpOutput->temperatureSense[groupIndex].temperatureDelta;
    }

    /*************** Copy to MBD structures *********************************************************************************************************/

    cbInputMBD->DataPipeline.Current_mA                 = cbInput->dataPipelineBus.current;
    cbInputMBD->DataPipeline.VCU.isChargerConnected     = cbInput->dataPipelineBus.VCU.isChargerConnected;
    cbInputMBD->DataPipeline.VCU.BalanceEn_atRest       = cbInput->dataPipelineBus.VCU.BalanceEn_atRest;

    //Cell Voltages
    for(uint8_t i = 0; i < 20; i++)
    {
        cbInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = cbInput->dataPipelineBus.voltageSense.voltage[i];
    }

    cbInputMBD->DataPipeline.VoltageSenseBus.mV_min             = cbInput->dataPipelineBus.voltageSense.voltageMin;
    cbInputMBD->DataPipeline.VoltageSenseBus.mV_max             = cbInput->dataPipelineBus.voltageSense.voltageMax;
    cbInputMBD->DataPipeline.VoltageSenseBus.VoltageDelta_mV    = cbInput->dataPipelineBus.voltageSense.voltageDelta;

    for (uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        // for (uint8_t i = 0; i < 4; i++)
        // {
        //     cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].Temperatures_C[i] = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i];
        // }

        // cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_min                  = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin;
        cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_max                  = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax;
        // cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].TemperatureDelta_C     = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingOutput(bmsMainData_t *mdata, cellBalancingOutput_t *cbOutput, ExtY_CellBalancing_T *cbOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == cbOutput)     ||\
        (NULL == cbOutputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    cbOutput->BalanceEn         = cbOutputMBD->CellBalancingOutput.BalanceEn;
    cbOutput->volatgeImbalance  = cbOutputMBD->CellBalancingOutput.MaxImbalance_mV;
    cbOutput->balancingState    = cbOutputMBD->CellBalancingOutput.CurrentBalancingState;
    balanceActiveFlag           = cbOutput->BalanceEn;

    switch(cbOutput->balancingState)
    {
        case 0: //Terminate
        case 1: //Lobby
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));
            adBms6815_clear_dcc_discharge(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;

        case 2: //Even Balancing
        case 3: //Odd Balancing
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));

            for(uint16_t i = 0; i < 20; i++)
            {
                cbOutput->balancingCommand[i]    = (int)cbOutputMBD->BalanceCmd[i];
                cellsToBalance[(i/10)]          |= (cbOutput->balancingCommand[i] << (i%10));  
            }
            
            adBms6815_set_dcc_discharge(cellsToBalance, mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;

        default:
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));
            adBms6815_clear_dcc_discharge(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t protectionInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == pInput)       ||\
        (NULL == pInputMBD) )
	{
		status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //
    pInputMBD->Thresholds.Protections_MaxOperatingTemp_C                     =   tData->MaxOperatingTemp_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempChange_C                 =   tData->MaxAllowedTempChange_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup1_C         =   tData->MaxAllowedTempGradientGroup1_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup2_C         =   tData->MaxAllowedTempGradientGroup2_C;

    //
    pInputMBD->Thresholds.i2tTimeout_msec                              = tData->i2tTimeout_msec;
    pInputMBD->Thresholds.DynContTimeout_msec                          = tData->dynamicContTimeout_msec;
    pInputMBD->Thresholds.DynamicCurrentThresholdsGain                 = tData->dynamicCurrentThresholdsGain;
    pInputMBD->Thresholds.TempGroupTimeout                             = tData->TempGroupTimeout;
    pInputMBD->Thresholds.PermanentFailCounts_eFuse                    = tData->PermanentFailCounts_eFuse;

    //
    pInputMBD->Thresholds.RecoveryProtections_OCC_Warning_mA           = tData->RecoveryProtections_OCC_Warning_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCC_Error_mA             = tData->RecoveryProtections_OCC_Error_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCD_Warning_mA           = tData->RecoveryProtections_OCD_Warning_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCD_Error_mA             = tData->RecoveryProtections_OCD_Error_mA;
    pInputMBD->Thresholds.CurrentChangeTimeout_msec                    = tData->CurrentChangeTimeout_msec;
    pInputMBD->Thresholds.eFuseMeltingTimeout_msec                     = tData->eFuseMeltingTimeout_msec;

    //Current
    pInputMBD->Thresholds.Protections_OCC_Warning_mA 	        = tData->OCC_WarningCurrent;
    pInputMBD->Thresholds.Protections_OCC_Error_mA 			    = tData->OCC_ErrorCurrent;
    pInputMBD->Thresholds.Protections_OCC_WarningTimeout_msec 	= tData->OCC_WarningTimeout;
    pInputMBD->Thresholds.Protections_OCC_ErrorTimeout_msec	    = tData->OCC_ErrorTimeout;
    
    pInputMBD->Thresholds.Protections_OCD_Warning_mA 		    = tData->OCD_WarningCurrent;
    pInputMBD->Thresholds.Protections_OCD_Error_mA 			    = tData->OCD_ErrorCurrent;
    pInputMBD->Thresholds.Protections_OCD_WarningTimeout_msec   = tData->OCD_WarningTimeout;
    pInputMBD->Thresholds.Protections_OCD_ErrorTimeout_msec     = tData->OCD_ErrorTimeout;
    
    //temperature groups
    for(int i = 0; i < 2; i++)
    {
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Warning_C      = tData->temperatureData[i].OTC_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C        = tData->temperatureData[i].OTC_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Recovery_C 	= tData->temperatureData[i].OTC_RecoveryTemperature;
        
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Warning_C 		= tData->temperatureData[i].OTD_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C 		= tData->temperatureData[i].OTD_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Recovery_C 	= tData->temperatureData[i].OTD_RecoveryTemperature;
        
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Warning_C	 	= tData->temperatureData[i].UTC_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Error_C	 	= tData->temperatureData[i].UTC_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Recovery_C	 	= tData->temperatureData[i].UTC_RecoveryTemperature;

        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Warning_C	 	= tData->temperatureData[i].UTD_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Error_C	 	= tData->temperatureData[i].UTD_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Recovery_C	    = tData->temperatureData[i].UTD_RecoveryTemperature;
    
        //timeouts
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_WarningTimeout_msec   = tData->temperatureData[i].OTC_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_ErrorTimeout_msec     = tData->temperatureData[i].OTC_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_RecoveryTimeout_msec  = tData->temperatureData[i].OTC_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_NoErrorTimeout_msec   = tData->temperatureData[i].OTC_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_OTD_WarningTimeout_msec   = tData->temperatureData[i].OTD_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_ErrorTimeout_msec	   = tData->temperatureData[i].OTD_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_RecoveryTimeout_msec  = tData->temperatureData[i].OTD_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_NoErrorTimeout_msec   = tData->temperatureData[i].OTD_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_UTC_WarningTimeout_msec   = tData->temperatureData[i].UTC_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_ErrorTimeout_msec     = tData->temperatureData[i].UTC_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_RecoveryTimeout_msec  = tData->temperatureData[i].UTC_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_NoErrorTimeout_msec   = tData->temperatureData[i].UTC_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_UTD_WarningTimeout_msec   = tData->temperatureData[i].UTD_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_ErrorTimeout_msec	   = tData->temperatureData[i].UTD_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_RecoveryTimeout_msec  = tData->temperatureData[i].UTD_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_NoErrorTimeout_msec   = tData->temperatureData[i].UTD_NoErrorTimeout_msec;

    }
    


    pInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    pInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    pInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    pInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;


    pInputMBD->Thresholds.Protections_OV_WarningTimeout_msec    = tData->OV_WarningTimeout;
    pInputMBD->Thresholds.Protections_OV_ErrorTimeout_msec	 	= tData->OV_ErrorTimeout;
    pInputMBD->Thresholds.Protections_OV_RecoveryTimeout_msec	= tData->OV_RecoveryTimeout;
    pInputMBD->Thresholds.Protections_OV_NoErrorTimeout_msec	= tData->OV_NoErrorTimeout_msec;
    
    pInputMBD->Thresholds.Protections_UV_Warning_mV	 		    = tData->UV_WarningVoltage;
    pInputMBD->Thresholds.Protections_UV_Error_mV	 		    = tData->UV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_UV_Recovery_mV	 		= tData->UV_RecoveryVoltage;
    pInputMBD->Thresholds.Protections_UV_WarningTimeout_msec 	= tData->UV_WarningTimeout;
    pInputMBD->Thresholds.Protections_UV_ErrorTimeout_msec	 	= tData->UV_ErrorTimeout;
    pInputMBD->Thresholds.Protections_UV_RecoveryTimeout_msec	= tData->UV_RecoveryTimeout;  
    pInputMBD->Thresholds.Protections_UV_NoErrorTimeout_msec	= tData->UV_NoErrorTimeout_msec;

    pInputMBD->Thresholds.Protections_HighImbalanceError_mV            = tData->HighImbalanceError_mV;
    pInputMBD->Thresholds.Protections_MinOpearatingVolt_mV             = tData->MinOpearatingVolt_mV;
    pInputMBD->Thresholds.Protections_ShortCircuitCurrent_mA           = tData->ShortCircuitCurrent_mA;
    pInputMBD->Thresholds.Protections_SuddenVoltageDrop_mV_by_time     = tData->SuddenVoltageDrop_mV_by_time;
    

    pInputMBD->Thresholds.ThermalRunawayWarningTimeout_msec            = tData->ThermalRunawayWarningTimeout_msec;
    pInputMBD->Thresholds.ThermalRunawayErrorTimeout_msec              = tData->ThermalRunawayErrorTimeout_msec;
    pInputMBD->Thresholds.TemperatureGradientTimeout_msec              = tData->TemperatureGradientTimeout_msec;
    pInputMBD->Thresholds.HighImbalanceFlagTimeout_msec                = tData->HighImbalanceFlagTimeout_msec;
    pInputMBD->Thresholds.ShortCircuitDetect_msec                      = tData->ShortCircuitDetect_msec;
    pInputMBD->Thresholds.SuddenVoltageDropTimeout_msec                = tData->SuddenVoltageDropTimeout_msec;
    pInputMBD->Thresholds.AdditionalProtectionEn                       = tData->AdditionalProtectionEn;
    pInputMBD->Thresholds.HighImbalanceFlagEn                          = tData->HighImbalanceFlagEn;

    pInputMBD->Thresholds.ExternalShortCheckTime_msec                  = tData->ExternalShortCheckTime_msec;
    pInputMBD->Thresholds.ExternalShortLimit                           = tData->ExternalShortLimit;
    pInputMBD->Thresholds.ContWeldCheckLimit                           = tData->ContWeldCheckLimit;

    //contactor
    pInputMBD->Thresholds.ContCutoffTime_msec                = tData->contactorCutoffTime;
    pInputMBD->Thresholds.ContCurrent_mA                     = tData->preChargeContactorCurrent;
    pInputMBD->Thresholds.ContWeldCheckCurrent_mA            = tData->ContWeldCheckCurrent_mA;
    pInputMBD->Thresholds.ContactorWarningCounterLimit       = tData->ContactorWarningCounterLimit;
    pInputMBD->Thresholds.ContactorWeldTimeout_msec          = tData->ContactorWeldTimeout_msec;
    pInputMBD->Thresholds.OpenToPreChargeStateTime_msec      = tData->OpenToPreChargeStateTime_msec;
    pInputMBD->Thresholds.Min_PreChargeTime_msec             = tData->Min_PreChargeTime_msec;
    pInputMBD->Thresholds.Max_PreChargeTime_msec             = tData->Max_PreChargeTime_msec;
    pInputMBD->Thresholds.ContOpenCurrent_mA                 = tData->ContOpenCurrent_mA;
    pInputMBD->Thresholds.FlagDebounceTime_msec              = tData->FlagDebounceTime_msec;
    pInputMBD->Thresholds.Min_ClosedToOpeningTime_msec       = tData->Min_ClosedToOpeningTime_msec;
    pInputMBD->Thresholds.Max_ClosedToOpeningTime_msec       = tData->Max_ClosedToOpeningTime_msec;

    //dcli enable
    pInputMBD->DataPipeline.isDynamicCurrentEn               = dpInput->isDynamicCurrentEn;

    status = BMS_OK;
	return status;
}

static uint32_t lasttick=0;
uint32_t currenttick=0;
uint32_t duration=0;

bmsStatus_t protectionInput(bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == pInput)       ||\
        (NULL == pInputMBD) )
	{
        status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    
    currenttick=HAL_GetTick();
    if(lasttick==0) duration=0;
    else duration=currenttick-lasttick;
    lasttick=currenttick;


    pInput->dataPipelineBus.current                      = dpOutput->current;
    pInput->dataPipelineBus.isChargeEn                   = dpOutput->isChargeEn;

    pInput->dataPipelineBus.maxTemperature_C             = dpOutput->maxTemperature_C;
    pInput->dataPipelineBus.deltaTemperature_C           = dpOutput->deltaTemperature_C;
    pInput->dataPipelineBus.DeltaTemperatureGroup1_C     = dpOutput->DeltaTemperatureGroup1_C;
    pInput->dataPipelineBus.DeltaTemperatureGroup2_C     = dpOutput->DeltaTemperatureGroup2_C;

    pInput->dataPipelineBus.DCL_ChargingCurrent_A        = dpOutput->DCL_ChargingCurrent_A;
    pInput->dataPipelineBus.DCL_DisChargingCurrent_A     = dpOutput->DCL_DisChargingCurrent_A;

    pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec    = dpOutput->i2t_ChargingCurrent_A2sec;
    pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec = dpOutput->i2t_DisChargingCurrent_A2sec;

    //Cell Voltages
    // for(uint8_t i = 0; i < 20; i++)
    // {
    //     pInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    // }

    pInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    pInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    pInput->dataPipelineBus.voltageSense.voltageDelta = dpOutput->voltageSense.voltageDelta;

    pInput->dataPipelineBus.VCU.FastCharge            = dpOutput->VCU.FastCharge;
    pInput->dataPipelineBus.VCU.isChargerConnected    = dpOutput->VCU.isChargerConnected;
    pInput->dataPipelineBus.VCU.ServiceMode           = dpOutput->VCU.ServiceMode;
    pInput->dataPipelineBus.VCU.SlowCharge            = dpOutput->VCU.SlowCharge;
    pInput->dataPipelineBus.VCU.TransmissionFail      = dpOutput->VCU.TransmissionFail;
    pInput->dataPipelineBus.VCU.VCUPermanentFail      = dpOutput->VCU.VCUPermanentFail;
    // pInput->dataPipelineBus.LoopTimer_SOC_msec        = dpOutput->LoopTimer_SOC_msec;
    pInput->dataPipelineBus.LoopTimer_SOC_msec        = duration;

    //temperatures
    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {

        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin     = dpOutput->temperatureSense[groupIndex].temperatureMin;
        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax     = dpOutput->temperatureSense[groupIndex].temperatureMax;
        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta   = dpOutput->temperatureSense[groupIndex].temperatureDelta;
    }

    /*************** Copy to MBD structures *********************************************************************************************************/

    pInputMBD->DataPipeline.Current_mA                   = pInput->dataPipelineBus.current;
    pInputMBD->DataPipeline.isChargeEn                   = pInput->dataPipelineBus.isChargeEn;

    pInputMBD->DataPipeline.MaxTemperature_C             = pInput->dataPipelineBus.maxTemperature_C;
    pInputMBD->DataPipeline.DeltaTemperature_C           = pInput->dataPipelineBus.deltaTemperature_C;
    pInputMBD->DataPipeline.DeltaTemperatureGroup1_C     = pInput->dataPipelineBus.DeltaTemperatureGroup1_C;
    pInputMBD->DataPipeline.DeltaTemperatureGroup2_C     = pInput->dataPipelineBus.DeltaTemperatureGroup2_C;

    pInputMBD->DataPipeline.DCL_ChargingCurrent_A        = pInput->dataPipelineBus.DCL_ChargingCurrent_A;
    pInputMBD->DataPipeline.DCL_DisChargingCurrent_A     = pInput->dataPipelineBus.DCL_DisChargingCurrent_A;

    pInputMBD->DataPipeline.i2t_ChargingCurrent_A2sec    = pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec;
    pInputMBD->DataPipeline.i2t_DisChargingCurrent_A2sec = pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec;

    // for (uint8_t i = 0; i < 20; i++)
    // {
    //     pInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = pInput->dataPipelineBus.voltageSense.voltage[i];
    // }

    pInputMBD->DataPipeline.VoltageSenseBus.mV_min             = pInput->dataPipelineBus.voltageSense.voltageMin;
    pInputMBD->DataPipeline.VoltageSenseBus.mV_max             = pInput->dataPipelineBus.voltageSense.voltageMax;
    pInputMBD->DataPipeline.VoltageSenseBus.VoltageDelta_mV    = pInput->dataPipelineBus.voltageSense.voltageDelta;
    
    //copying VCU parameters
    pInputMBD->DataPipeline.VCU.FastCharge                     = pInput->dataPipelineBus.VCU.FastCharge;
    pInputMBD->DataPipeline.VCU.isChargerConnected             = pInput->dataPipelineBus.VCU.isChargerConnected;
    pInputMBD->DataPipeline.VCU.ServiceMode                    = pInput->dataPipelineBus.VCU.ServiceMode;
    pInputMBD->DataPipeline.VCU.SlowCharge                     = pInput->dataPipelineBus.VCU.SlowCharge;
    pInputMBD->DataPipeline.VCU.TransmissionFail               = pInput->dataPipelineBus.VCU.TransmissionFail;
    pInputMBD->DataPipeline.VCU.VCUPermanentFail                        = pInput->dataPipelineBus.VCU.VCUPermanentFail;

    pInputMBD->DataPipeline.LoopTimer_SOC_msec                      = pInput->dataPipelineBus.LoopTimer_SOC_msec;

    for (uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_min                  = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin;
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_max                  = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax;
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].TemperatureDelta_C     = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t protectionOutput(bmsMainData_t *mdata, protectionOutput_t *pOutput, ExtY_Protection_T *pOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == pOutput)      ||\
        (NULL == pOutputMBD) )
	{
		status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
  
    pOutput->protectionOutputData.ThermalRunaway			= pOutputMBD->ProtectionOutput_e.ThermalRunaway;
    pOutput->protectionOutputData.TemperatureGradient		= pOutputMBD->ProtectionOutput_e.TemperatureGradient;
    pOutput->protectionOutputData.HighImbalanceFlag	        = pOutputMBD->ProtectionOutput_e.HighImbalanceFlag;
    pOutput->protectionOutputData.ShortCircuit		        = pOutputMBD->ProtectionOutput_e.ShortCircuitDetect;
    pOutput->protectionOutputData.SuddenVoltageDrop	        = pOutputMBD->ProtectionOutput_e.SuddenVoltageDrop;
    pOutput->protectionOutputData.OCC						= pOutputMBD->ProtectionOutput_e.OCC;
    pOutput->protectionOutputData.OCD						= pOutputMBD->ProtectionOutput_e.OCD;
    pOutput->protectionOutputData.OV						= pOutputMBD->ProtectionOutput_e.OV;
    pOutput->protectionOutputData.UV						= pOutputMBD->ProtectionOutput_e.UV;

    pOutput->protectionOutputData.TempOverallState				= pOutputMBD->ProtectionOutput_e.TempOverallState;
    pOutput->protectionOutputData.TempState1					= pOutputMBD->ProtectionOutput_e.TempState1;
    pOutput->protectionOutputData.TempState2					= pOutputMBD->ProtectionOutput_e.TempState2;

    pOutput->protectionOutputData.FlagGroup1				    = pOutputMBD->ProtectionOutput_e.FlagGroup1;
    pOutput->protectionOutputData.FlagGroup2				    = pOutputMBD->ProtectionOutput_e.FlagGroup2;

    pOutput->protectionOutputData.eFuseChargeFlag        		= pOutputMBD->ProtectionOutput_e.eFuseChargeFlag;
    pOutput->protectionOutputData.eFuseDischargeFlag            = pOutputMBD->ProtectionOutput_e.eFuseDischargeFlag;
    pOutput->protectionOutputData.i2t_Calculated_A2sec        	= pOutputMBD->ProtectionOutput_e.i2t_Calculated_A2sec;
    pOutput->protectionOutputData.ErrorDetect        			= pOutputMBD->ProtectionOutput_e.ErrorDetect;
    
    status = BMS_OK;
	return status;
}


bmsStatus_t contactorInit(thresholdData_t *tData, contactorInput_t *cInput, ExtU_Contactors_T *cInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)       ||\
        (NULL == cInput)      ||\
        (NULL == cInputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //
    cInputMBD->Thresholds.Protections_MaxOperatingTemp_C                     =   tData->MaxOperatingTemp_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempChange_C                 =   tData->MaxAllowedTempChange_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup1_C         =   tData->MaxAllowedTempGradientGroup1_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup2_C         =   tData->MaxAllowedTempGradientGroup2_C;

    //
    // cInputMBD->Thresholds.DynInstTimeout_msec                          = tData->dynamicInstTimeout_msec;
    cInputMBD->Thresholds.DynContTimeout_msec                          = tData->dynamicContTimeout_msec;
    cInputMBD->Thresholds.DynamicCurrentThresholdsGain                 = tData->dynamicCurrentThresholdsGain;
    cInputMBD->Thresholds.TempGroupTimeout                             = tData->TempGroupTimeout;
    
    //
    cInputMBD->Thresholds.RecoveryProtections_OCC_Warning_mA           = tData->RecoveryProtections_OCC_Warning_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCC_Error_mA             = tData->RecoveryProtections_OCC_Error_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCD_Warning_mA           = tData->RecoveryProtections_OCD_Warning_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCD_Error_mA             = tData->RecoveryProtections_OCD_Error_mA;
    cInputMBD->Thresholds.CurrentChangeTimeout_msec                    = tData->CurrentChangeTimeout_msec;

    //threshold
    cInputMBD->Thresholds.Protections_OCC_Warning_mA 	        = tData->OCC_WarningCurrent;
    cInputMBD->Thresholds.Protections_OCC_Error_mA 			    = tData->OCC_ErrorCurrent;
    cInputMBD->Thresholds.Protections_OCC_WarningTimeout_msec 	= tData->OCC_WarningTimeout;
    cInputMBD->Thresholds.Protections_OCC_ErrorTimeout_msec	    = tData->OCC_ErrorTimeout;
    
    cInputMBD->Thresholds.Protections_OCD_Warning_mA 		    = tData->OCD_WarningCurrent;
    cInputMBD->Thresholds.Protections_OCD_Error_mA 			    = tData->OCD_ErrorCurrent;
    cInputMBD->Thresholds.Protections_OCD_WarningTimeout_msec   = tData->OCD_WarningTimeout;
    cInputMBD->Thresholds.Protections_OCD_ErrorTimeout_msec     = tData->OCD_ErrorTimeout;
    
    //temperature groups
    for(int i = 0; i < 2; i++)
    {
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Warning_C      = tData->temperatureData[i].OTC_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C        = tData->temperatureData[i].OTC_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Recovery_C 	= tData->temperatureData[i].OTC_RecoveryTemperature;
        
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Warning_C 		= tData->temperatureData[i].OTD_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C 		= tData->temperatureData[i].OTD_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Recovery_C 	= tData->temperatureData[i].OTD_RecoveryTemperature;
        
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Warning_C	 	= tData->temperatureData[i].UTC_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Error_C	 	= tData->temperatureData[i].UTC_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Recovery_C	 	= tData->temperatureData[i].UTC_RecoveryTemperature;

        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Warning_C	 	= tData->temperatureData[i].UTD_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Error_C	 	= tData->temperatureData[i].UTD_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Recovery_C	    = tData->temperatureData[i].UTD_RecoveryTemperature;
        
        //timeouts
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_WarningTimeout_msec   = tData->temperatureData[i].OTC_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_ErrorTimeout_msec     = tData->temperatureData[i].OTC_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_RecoveryTimeout_msec  = tData->temperatureData[i].OTC_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_OTD_WarningTimeout_msec   = tData->temperatureData[i].OTD_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_ErrorTimeout_msec	   = tData->temperatureData[i].OTD_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_RecoveryTimeout_msec  = tData->temperatureData[i].OTD_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_UTC_WarningTimeout_msec   = tData->temperatureData[i].UTC_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_ErrorTimeout_msec     = tData->temperatureData[i].UTC_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_RecoveryTimeout_msec  = tData->temperatureData[i].UTC_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_UTD_WarningTimeout_msec   = tData->temperatureData[i].UTD_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_ErrorTimeout_msec	   = tData->temperatureData[i].UTD_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_RecoveryTimeout_msec  = tData->temperatureData[i].UTD_RecoveryTimeout;
    }
    
    // cInputMBD->Thresholds.Protections_OV_Warning_mV	 		    = tData->OV_WarningVoltage;
    // cInputMBD->Thresholds.Protections_OV_Error_mV	 		    = tData->OV_ErrorVoltage;
    // cInputMBD->Thresholds.Protections_OV_Recovery_mV	 		= tData->OV_RecoveryVoltage;
    cInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    cInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    cInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    cInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;

    cInputMBD->Thresholds.Protections_OV_WarningTimeout_msec    = tData->OV_WarningTimeout;
    cInputMBD->Thresholds.Protections_OV_ErrorTimeout_msec	 	= tData->OV_ErrorTimeout;
    cInputMBD->Thresholds.Protections_OV_RecoveryTimeout_msec	= tData->OV_RecoveryTimeout;
    
    cInputMBD->Thresholds.Protections_UV_Warning_mV	 		    = tData->UV_WarningVoltage;
    cInputMBD->Thresholds.Protections_UV_Error_mV	 		    = tData->UV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_UV_Recovery_mV	 		= tData->UV_RecoveryVoltage;
    cInputMBD->Thresholds.Protections_UV_WarningTimeout_msec 	= tData->UV_WarningTimeout;
    cInputMBD->Thresholds.Protections_UV_ErrorTimeout_msec	 	= tData->UV_ErrorTimeout;
    cInputMBD->Thresholds.Protections_UV_RecoveryTimeout_msec	= tData->UV_RecoveryTimeout;  
    
    //
    cInputMBD->Thresholds.Protections_HighImbalanceError_mV            = tData->HighImbalanceError_mV;
    cInputMBD->Thresholds.Protections_MinOpearatingVolt_mV             = tData->MinOpearatingVolt_mV;
    cInputMBD->Thresholds.Protections_ShortCircuitCurrent_mA           = tData->ShortCircuitCurrent_mA;
    cInputMBD->Thresholds.Protections_SuddenVoltageDrop_mV_by_time     = tData->SuddenVoltageDrop_mV_by_time;
    
    cInputMBD->Thresholds.ExternalShortCheckTime_msec                  = tData->ExternalShortCheckTime_msec;
    cInputMBD->Thresholds.ExternalShortLimit                           = tData->ExternalShortLimit;
    cInputMBD->Thresholds.ContWeldCheckLimit                           = tData->ContWeldCheckLimit;

    cInputMBD->Thresholds.AdditionalProtectionEn                       = tData->AdditionalProtectionEn;
    cInputMBD->Thresholds.HighImbalanceFlagEn                          = tData->HighImbalanceFlagEn;

    //contactor
    cInputMBD->Thresholds.ContCutoffTime_msec                = tData->contactorCutoffTime;
    cInputMBD->Thresholds.ContCurrent_mA                     = tData->preChargeContactorCurrent;
    cInputMBD->Thresholds.ContWeldCheckCurrent_mA            = tData->ContWeldCheckCurrent_mA;
    cInputMBD->Thresholds.ContactorWarningCounterLimit       = tData->ContactorWarningCounterLimit;
    cInputMBD->Thresholds.ContactorWeldTimeout_msec          = tData->ContactorWeldTimeout_msec;
    cInputMBD->Thresholds.OpenToPreChargeStateTime_msec      = tData->OpenToPreChargeStateTime_msec;
    cInputMBD->Thresholds.Min_PreChargeTime_msec             = tData->Min_PreChargeTime_msec;
    cInputMBD->Thresholds.Max_PreChargeTime_msec             = tData->Max_PreChargeTime_msec;
    cInputMBD->Thresholds.ContOpenCurrent_mA                 = tData->ContOpenCurrent_mA;
    cInputMBD->Thresholds.FlagDebounceTime_msec              = tData->FlagDebounceTime_msec;
    cInputMBD->Thresholds.Min_ClosedToOpeningTime_msec       = tData->Min_ClosedToOpeningTime_msec;
    cInputMBD->Thresholds.Max_ClosedToOpeningTime_msec       = tData->Max_ClosedToOpeningTime_msec;

    status = BMS_OK;
	return status;
}


bmsStatus_t contactorInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorInput_t *cInput, contactorOutput_t *cOutput, ExtU_Contactors_T *cInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == dpOutput)    ||\
        (NULL == pOutput)     ||\
        (NULL == cInput)      ||\
        (NULL == cOutput)     ||\
        (NULL == cInputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //protections output
    cInput->protectionOutputData.ThermalRunaway			    = pOutput->protectionOutputData.ThermalRunaway;
    cInput->protectionOutputData.TemperatureGradient		= pOutput->protectionOutputData.TemperatureGradient;
    cInput->protectionOutputData.HighImbalanceFlag	        = pOutput->protectionOutputData.HighImbalanceFlag;
    cInput->protectionOutputData.ShortCircuit		        = pOutput->protectionOutputData.ShortCircuit;
    cInput->protectionOutputData.SuddenVoltageDrop	        = pOutput->protectionOutputData.SuddenVoltageDrop;    
    cInput->protectionOutputData.OCC						= pOutput->protectionOutputData.OCC;
    cInput->protectionOutputData.OCD						= pOutput->protectionOutputData.OCD;
    cInput->protectionOutputData.OV						    = pOutput->protectionOutputData.OV;
    cInput->protectionOutputData.UV						    = pOutput->protectionOutputData.UV;
    cInput->protectionOutputData.TempOverallState           = pOutput->protectionOutputData.TempOverallState;

    cInput->PermanentFailRecoveryTrigger		            = 0;
    cInput->protectionOutputData.eFuseChargeFlag            = pOutput->protectionOutputData.eFuseChargeFlag;
    cInput->protectionOutputData.eFuseDischargeFlag         = pOutput->protectionOutputData.eFuseDischargeFlag;
    cInput->protectionOutputData.ErrorDetect                = pOutput->protectionOutputData.ErrorDetect;

    //contactor
    cInput->contactorCommand                            = isLoadRequested() || isChargeRequested();
    // cInput->contactorCommand                            = 1;
    cInput->positiveContactorFeedback                   = cOutput->positiveContactorState;
    cInput->preChargeContactorFeedback                  = cOutput->preChargeContactorState;    
    cInput->negativeContactorFeedback                   = cOutput->negativeContactorState; 

    cInput->dataPipelineBus.VCU.EmergencyDisconnectEn   = dpOutput->VCU.EmergencyDisconnectEn;
    cInput->shortCircuitFlag					        = externalShortCircuitDetectionFlag;

    //data pipeline
    cInput->dataPipelineBus.current                     = dpOutput->current;
    // cInput->dataPipelineBus.isChargeEn                  = dpOutput->isChargeEn;

    // cInput->dataPipelineBus.maxTemperature_C            = dpOutput->maxTemperature_C;
    // cInput->dataPipelineBus.deltaTemperature_C          = dpOutput->deltaTemperature_C;

    // //Cell Voltages
    // for(uint8_t i = 0; i < 20; i++)
    // {
    //     cInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    // }

    // cInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    // cInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    // cInput->dataPipelineBus.voltageSense.voltageDelta = dpOutput->voltageSense.voltageDelta;
    
    // //temperatures
    // for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    // {
    //     for(uint8_t i = 0; i < 4; i++)
    //     {
    //         cInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i] = dpOutput->temperatureSense[groupIndex].temperature[i];
    //     }

    //     cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin     = dpOutput->temperatureSense[groupIndex].temperatureMin;
    //     cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax     = dpOutput->temperatureSense[groupIndex].temperatureMax;
    //     cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta   = dpOutput->temperatureSense[groupIndex].temperatureDelta;
    // }

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    //protections
    cInputMBD->ProtectionOutput_e.ThermalRunaway			    = cInput->protectionOutputData.ThermalRunaway;
    cInputMBD->ProtectionOutput_e.TemperatureGradient		    = cInput->protectionOutputData.TemperatureGradient;
    cInputMBD->ProtectionOutput_e.HighImbalanceFlag			    = cInput->protectionOutputData.HighImbalanceFlag;
    cInputMBD->ProtectionOutput_e.OCC						    = cInput->protectionOutputData.OCC;
    cInputMBD->ProtectionOutput_e.OCD						    = cInput->protectionOutputData.OCD;
    cInputMBD->ProtectionOutput_e.OV						    = cInput->protectionOutputData.OV;
    cInputMBD->ProtectionOutput_e.UV						    = cInput->protectionOutputData.UV;
    cInputMBD->ProtectionOutput_e.TempOverallState			    = cInput->protectionOutputData.TempOverallState;
    cInputMBD->ProtectionOutput_e.ShortCircuitDetect            = cInput->protectionOutputData.ShortCircuit;

    cInputMBD->ProtectionOutput_e.eFuseChargeFlag               = cInput->protectionOutputData.eFuseChargeFlag;
    cInputMBD->ProtectionOutput_e.eFuseDischargeFlag            = cInput->protectionOutputData.eFuseDischargeFlag;
    cInputMBD->ProtectionOutput_e.ErrorDetect                   = cInput->protectionOutputData.ErrorDetect;

    cInputMBD->PermanentFailRecoveryTrigger                     = cInput->PermanentFailRecoveryTrigger;
    cInputMBD->PermanentFail_Flag                               = cInput->permanentFailFlag;
    cInputMBD->ContactorError_Flag                              = cInput->contactorErrorFlag;

    //contactor
    cInputMBD->ContactorCommand			   = cInput->contactorCommand;
    cInputMBD->PosContactors_FB            = cInput->positiveContactorFeedback; 
    cInputMBD->PreChargeContactors_FB      = cInput->preChargeContactorFeedback;    
    cInputMBD->NegContactors_FB            = cInput->negativeContactorFeedback;

    cInputMBD->DataPipeline.VCU.EmergencyDisconnectEn             = cInput->dataPipelineBus.VCU.EmergencyDisconnectEn;

    //data pipeline
    cInputMBD->DataPipeline.Current_mA             = cInput->dataPipelineBus.current;
    // cInputMBD->DataPipeline.isChargeEn             = cInput->dataPipelineBus.isChargeEn;

    // cInputMBD->DataPipeline.MaxTemperature_C       = cInput->dataPipelineBus.maxTemperature_C;
    // cInputMBD->DataPipeline.DeltaTemperature_C     = cInput->dataPipelineBus.deltaTemperature_C;

    // for (uint8_t i = 0; i < 20; i++)
    // {
    //     cInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = cInput->dataPipelineBus.voltageSense.voltage[i];
    // }

    // cInputMBD->DataPipeline.VoltageSenseBus.mV_min             = cInput->dataPipelineBus.voltageSense.voltageMin;
    // cInputMBD->DataPipeline.VoltageSenseBus.mV_max             = cInput->dataPipelineBus.voltageSense.voltageMax;
    // cInputMBD->DataPipeline.VoltageSenseBus.VoltageDelta_mV    = cInput->dataPipelineBus.voltageSense.voltageDelta;

    // for (uint8_t groupIndex = 0; groupIndex < 5; groupIndex++)
    // {
    //     for (uint8_t i = 0; i < 4; i++)
    //     {
    //         cInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].Temperatures_C[i] = cInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i];
    //     }

    //     cInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_min                  = cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin;
    //     cInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_max                  = cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax;
    //     cInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].TemperatureDelta_C     = cInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta;
    // }

    status = BMS_OK;
	return status;
}


bmsStatus_t contactorOutput(bmsMainData_t *mdata, contactorOutput_t *cOutput, ExtY_Contactors_T *cOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == cOutput)     ||\
        (NULL == cOutputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /******************************************************* Copy from MBD structures *****************************************************************************************/

    cOutput->negativeContactorState     = cOutputMBD->NegContactors;
    cOutput->preChargeContactorState    = cOutputMBD->PreChargeContactors;
    cOutput->positiveContactorState     = cOutputMBD->PosContactors;
    cOutput->contactorWeldWarning       = cOutputMBD->ContactorsWeldWarning;
    cOutput->permanentFaiil             = cOutputMBD->PermanentFaiil;
    cOutput->contactorsState            = cOutputMBD->ContactorsState;
    cOutput->ExternalShortCheck         = cOutputMBD->ExternalShortCheck;
    cOutput->Elapsed_TimeToOpen_msec    = cOutputMBD->Elapsed_TimeToOpen_msec;

    if(externalShortCircuitDetectionFlag == 1)
    {
        //Switch off contactor
        TIM3 -> CCR1 = 0;
        TIM3 -> CCR2 = 0;
        TIM3 -> CCR3 = 0;
        TIM3 -> CCR4 = 0;
    }
    else
    {
        if(cOutput->negativeContactorState)
                {
            turnContactorON(chargeNegative);
        }
        else
        {
            turnContactorOFF(chargeNegative);
        }

        // delayMilliSeconds(1000);

        if(cOutput->preChargeContactorState)
        {
            turnContactorON(prechargePositive);
        }
        else
        {
            turnContactorOFF(prechargePositive);
        }

        if (cOutput->positiveContactorState)
                {
            turnContactorON(commonPositive);
        }
        else
        {
            turnContactorOFF(commonPositive);
        }
    }

    status = BMS_OK;
	return status;
}


#ifdef EKF
bmsStatus_t coulombCountingEKFInit(ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == ccEKFInput)   ||\
        (NULL == ccEKFInputMBD) )
	{
		status = BMS_CC_EKF_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    ccEKFInputMBD->VehicleRest_Time_msec                  = ccEKFInput->vehicleRestTime;
    ccEKFInputMBD->VehicleRest_Current_mA                 = ccEKFInput->vehicleRestCurrent;
    ccEKFInputMBD->Max_PackCapacity_mAh                   = ccEKFInput->maxPackCapacity;
    ccEKFInputMBD->Min_PackCapacity_mAh                   = ccEKFInput->minPackCapacity; 
	
    status = BMS_OK;
	return status;
}


bmsStatus_t coulombCountingEKFInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == dpOutput)    ||\
        (NULL == pOutput)     ||\
        (NULL == ccEKFInput)  ||\
        (NULL == ccEKFInputMBD) )
	{
		status = BMS_CC_EKF_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    ccEKFInput->protectionOutputOVThreshold         = pOutput->protectionOutputData.OV;
    ccEKFInput->protectionOutputUVThreshold         = pOutput->protectionOutputData.UV;
    ccEKFInput->avgCellVoltage                      = dpOutput->voltageSense.voltage[0];
    ccEKFInput->current                             = dpOutput->current;
    ccEKFInput->time_s                             += 0.03; 
    
    /*************** Copy to MBD structures *********************************************************************************************************/

    ccEKFInputMBD->Current_mA                             = ccEKFInput->current;
    ccEKFInputMBD->ProtectionOutput_OV_Thresholds         = ccEKFInput->protectionOutputOVThreshold;
    ccEKFInputMBD->ProtectionOutput_UV_Thresholds         = ccEKFInput->protectionOutputUVThreshold;
    ccEKFInputMBD->AvgCellVoltage_mV                      = ccEKFInput->avgCellVoltage;
    ccEKFInputMBD->Time_sec                               = ccEKFInput->time_s;
	
    status = BMS_OK;
	return status;
}


bmsStatus_t coulombCountingEKFOutput(bmsMainData_t *mdata, ccEKFOutput_t *ccEKFOutput, ExtY_EKF_T *ccEKFOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

    if( (NULL == mdata)       ||\
        (NULL == ccEKFOutput)    ||\
        (NULL == ccEKFOutputMBD) )
	{
		status = BMS_CC_EKF_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    /*************** Copy from MBD structures *********************************************************************************************************/

    ccEKFOutput->initialCapacity                = ccEKFOutputMBD->Initial_Capacity_mAh;
    ccEKFOutput->totalCapacityRemaining         = ccEKFOutputMBD->Total_CapacityRemains_mAh;
    ccEKFOutput->totalCapacityDischarge         = ccEKFOutputMBD->Total_Discharge_mAh;
    ccEKFOutput->socCC                          = ccEKFOutputMBD->SOC_CC;
    ccEKFOutput->coulombCountingState           = ccEKFOutputMBD->CC_State;

    ccEKFOutput->capacityEKF                    = ccEKFOutputMBD->Capacity_EKF;
    ccEKFOutput->voltageEKF                     = ccEKFOutputMBD->Voltage_EKF;
    ccEKFOutput->terminalVoltage                = ccEKFOutputMBD->Terminal_Voltage;
    ccEKFOutput->socEKF                         = ccEKFOutputMBD->SOC_EKF;

    status = BMS_OK;
	return status;
}

#else

bmsStatus_t coulombCountingInit(coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD, coulombCountingOutput_t *ccOutputData)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == ccInput)   ||\
        (NULL == ccInputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    // status = flashRead(SOC_DATA_ADDRESS, ccReadBufferEEPROM, sizeof(ccReadBufferEEPROM));
    // // if(0xFFFFFFFFFFFFFFFF != ccReadBufferEEPROM[0]) //When the device turns on for the very first time, EEPROM will be empty
    // if(ccReadBufferEEPROM[3] > 0) //When the device turns on for the very first time, EEPROM will be empty
    // {
    //     readCCDataFromBuffer(&ccOutputDataEEPROM, ccReadBufferEEPROM);
    // }

    ccInput->cc_Inputs.EquivalentCycleCapacity_mAh  = EQUIVALENT_CYCLE_CAPACITY;  //TO DO: multiply by SoH, for degrading capacity
    ccInput->cc_Inputs.InstalledCapacity_mAh        = 180000;
    ccInput->cc_Inputs.SensorOffset_mA              = SENSOR_OFFSET_mA;
    ccInput->cc_Inputs.SensorAccuracy_pct           = SENSOR_ACCURACY;
    ccInput->cc_Inputs.BleedingCurrent_mA           = 0;                           //BLEEDING_CURRENT_mA;
    ccInput->cc_Inputs.CCTimeout_msec               = CC_TIMEOUT_mS;

    ccInput->cc_Inputs.DebouncingTimeout_msec       = DEBOUNCING_TIMEOUT_mS;
    ccInput->cc_Inputs.MaxCurrentLimit_mA           = 15000;
    ccInput->cc_Inputs.MinCurrentLimit_mA           = 5000;
    ccInput->cc_Inputs.MaxAllowedImb_mV             = 30;    //CELL_IMBALANCE_THRESHOLD_mV;
    ccInput->cc_Inputs.MinVoltageLimit_mV           = 3380;
    ccInput->cc_Inputs.MaxVoltageLimit_mV           = 3420;
    ccInput->cc_Inputs.CapacityDeltaCheck_mAh       = CAPACITY_DELTA_CHECK_mAh;
    ccInput->cc_Inputs.InitialCapacity_Guess        = 0;     // 0 - flash, 1 - lookup table
    ccInput->cc_Inputs.SOH_Vs_Capacity_Gain         = 0;
    ccInput->cc_Inputs.SOH_Vs_Capacity_Offset       = 0;
    ccInput->cc_Inputs.LoopTimeSelector             = 0;     //0 - Timer DWT, 1 - Integrated time
    ccInput->cc_Inputs.SOHCalibrationOn             = 0;     //0 - SoH calibration OFF, 1 - SoH calibration ON
    ccInput->cc_Inputs.SOHCalibrationTimeout_msec   = 500;

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    ccInputMBD->CC_Inputs.EquivalentCycleCapacity_mAh   = ccInput->cc_Inputs.EquivalentCycleCapacity_mAh;
    ccInputMBD->CC_Inputs.InstalledCapacity_mAh         = ccInput->cc_Inputs.InstalledCapacity_mAh;
    ccInputMBD->CC_Inputs.SensorOffset_mA               = ccInput->cc_Inputs.SensorOffset_mA;
    ccInputMBD->CC_Inputs.SensorAccuracy_pct            = ccInput->cc_Inputs.SensorAccuracy_pct;
    ccInputMBD->CC_Inputs.BleedingCurrent_mA            = ccInput->cc_Inputs.BleedingCurrent_mA;
    ccInputMBD->CC_Inputs.CCTimeout_msec                = ccInput->cc_Inputs.CCTimeout_msec;
    
    ccInputMBD->CC_Inputs.DebouncingTimeout_msec        = ccInput->cc_Inputs.DebouncingTimeout_msec;
    // ccInputMBD->CC_Inputs.MinVoltageLimit_mV            = ccInput->cc_Inputs.MinVoltageLimit_mV;
    ccInputMBD->CC_Inputs.MaxVoltageLimit_mV            = ccInput->cc_Inputs.MaxVoltageLimit_mV;
    ccInputMBD->CC_Inputs.MaxCurrentLimit_mA            = ccInput->cc_Inputs.MaxCurrentLimit_mA;
    ccInputMBD->CC_Inputs.MinCurrentLimit_mA            = ccInput->cc_Inputs.MinCurrentLimit_mA;
    ccInputMBD->CC_Inputs.MaxAllowedImb_mV              = ccInput->cc_Inputs.MaxAllowedImb_mV;
    ccInputMBD->CC_Inputs.CapacityDeltaCheck_mAh        = ccInput->cc_Inputs.CapacityDeltaCheck_mAh;
    ccInputMBD->CC_Inputs.InitialCapacity_Guess         = ccInput->cc_Inputs.InitialCapacity_Guess;
    ccInputMBD->CC_Inputs.SOHCalibrationOn              = ccInput->cc_Inputs.SOHCalibrationOn;
    ccInputMBD->CC_Inputs.SOH_Vs_Capacity_Gain          = ccInput->cc_Inputs.SOH_Vs_Capacity_Gain;
    ccInputMBD->CC_Inputs.SOH_Vs_Capacity_Offset        = ccInput->cc_Inputs.SOH_Vs_Capacity_Offset;
    ccInputMBD->CC_Inputs.LoopTimeSelector              = ccInput->cc_Inputs.LoopTimeSelector;
    
    status = BMS_OK;
	return status;
}

bmsStatus_t coulombCountingInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == dpOutput)    ||\
        (NULL == pOutput)     ||\
        (NULL == ccInput)     ||\
        (NULL == ccInputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    taskBreakpointCycleCount = DWT->CYCCNT;
    time = ((float)(taskBreakpointCycleCount)/Systemfrequency);
    DWT->CYCCNT = 0;

    ccInput->current_mA                                = dpOutput->current;
    ccInput->voltage_mV                                = dpOutput->voltageSense.voltageMax;
    ccInput->dataPipelineBus.VCU.isChargerConnected    = dpOutput->VCU.isChargerConnected;
    ccInput->CellBalancingOutput.volatgeImbalance      = dpOutput->voltageSense.voltageDelta;
    ccInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    ccInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    ccInput->LoopTimer_SOC_msec                        = time * 1000;
    // ccInput->EEPROM_FirstCycle                         = 1;

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    ccInputMBD->Current_mA                                 = ccInput->current_mA;
    ccInputMBD->Voltage_mV                                 = ccInput->voltage_mV;
    
    ccInputMBD->DataPipeline.VCU.isChargerConnected        = ccInput->dataPipelineBus.VCU.isChargerConnected;
    // ccInputMBD->DataPipeline.VCU.isChargerConnected        = 1;
    ccInputMBD->CellBalancingOutput.MaxImbalance_mV        = ccInput->CellBalancingOutput.volatgeImbalance;
    ccInputMBD->DataPipeline.VoltageSenseBus.mV_min        = ccInput->dataPipelineBus.voltageSense.voltageMin;
    ccInputMBD->DataPipeline.VoltageSenseBus.mV_max        = ccInput->dataPipelineBus.voltageSense.voltageMax;
    ccInputMBD->DataPipeline.LoopTimer_SOC_msec            = ccInput->LoopTimer_SOC_msec;
    // ccInputMBD->EEPROM_FirstCycle                          = ccInput->EEPROM_FirstCycle;

    status = BMS_OK;
	return status;  
}


bmsStatus_t coulombCountingOutput(bmsMainData_t *mdata, coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFLASH, ExtY_SOCEstimation_T *ccOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

    if( (NULL == mdata)       ||\
        (NULL == ccOutput)    ||\
        (NULL == ccOutputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/

    ccOutput->initialCapacity                = ccOutputMBD->CC_Outputs.Initial_Capacity_mAh;
    ccOutput->totalCapacityRemaining         = ccOutputMBD->CC_Outputs.Total_CapacityRemains_mAh;

    #ifdef SOC_IN_FLASH
    ccOutput->totalCapacityDischarge         = ccOutputMBD->CC_Outputs.Total_Discharge_mAh        + ccOutputFLASH->totalCapacityDischarge;    //add the previous stored value in eeprom
    ccOutput->TotalCapacityExchange_mAh      = ccOutputMBD->CC_Outputs.TotalCapacityExchange_mAh  + ccOutputFLASH->TotalCapacityExchange_mAh; 

    #else
    ccOutput->totalCapacityDischarge         = ccOutputMBD->CC_Outputs.Total_Discharge_mAh        + ccOutputEEPROM->totalCapacityDischarge;    //add the previous stored value in eeprom
    ccOutput->TotalCapacityExchange_mAh      = ccOutputMBD->CC_Outputs.TotalCapacityExchange_mAh  + ccOutputEEPROM->TotalCapacityExchange_mAh;

    #endif

    ccOutput->SOC_pct                        = (ccOutputMBD->CC_Outputs.SOC_cpct)/100;
    ccOutput->coulombCountingState           = ccOutputMBD->CC_Outputs.CC_State;
    ccOutput->MaxUsableCapacity_mAh          = ccOutputMBD->CC_Outputs.MaxUsableCapacity_mAh;
    ccOutput->SOH_pct                        = (ccOutputMBD->CC_Outputs.SOH_cpct)/100;
    ccOutput->CycleCount                     = ccOutputMBD->CC_Outputs.CycleCount;
    ccOutput->SOC_FromFlash                  = ccOutputMBD->SOC_FromFlash;

    mdata->ccmData.packCapacity = (ccOutput->totalCapacityRemaining)/10;
    // mdata->packCapacity = (uint16_t)(ccOutput->totalCapacityRemaining)/10;
    // mdata->packCapacity = 150; 
    // mdata->packCapacity /= 1000;

    #ifdef EEPROM_TEST
        ccOutput->initialCapacity           += 100;
        ccOutput->totalCapacityRemaining    += 100;
        ccOutput->totalCapacityDischarge    += 200;
        ccOutput->TotalCapacityExchange_mAh += 23;
        ccOutput->SOC_pct                   += 1;
    #endif

    // fullChargeLatch = 10;
    if(ccOutput->coulombCountingState == Recalibrate)
    {
        fullChargeLatch = 1;  //for how long should the latch true flag stay?
        latchCheckFlag = 1;
    }
    else if(ccOutput->totalCapacityRemaining <= 179000)
    {
        fullChargeLatch = 0;
    }

    status = BMS_OK;
	return status;
#endif
}

void writeSoCToFlash(coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFlash)
{
    startEEPROMWriteFlag = (ccOutput->SOC_FromFlash == ccOutput->SOC_pct) ? TRUE : FALSE;
    if(((abs(ccOutput->SOC_pct - ccOutputEEPROM->SOC_pct) >= 1) || latchCheckFlag) && startEEPROMWriteFlag)
    {
        ccOutputEEPROM->SOC_pct = ccOutput->SOC_pct;
        writeCCDataToBuffer(ccOutput, ccWriteBufferEEPROM, ccWriteBufferFLASH);
        statusSoC = memoryWrite(eeprom, 0x00, ccWriteBufferEEPROM, sizeof(ccWriteBufferEEPROM));

    #ifdef SOC_IN_FLASH
        ccOutputFlash->SOC_pct = ccOutput->SOC_pct;
        statusSoC = flashWrite(SOC_DATA_ADDRESS, ccWriteBufferFLASH, 5);

    #endif
        
        if(statusSoC != BMS_OK)
        {
            eepromSOCCounter++;
        }
        latchCheckFlag = 0;
    }
}

bmsStatus_t stateMachineInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, ExtU_HighLevelStatemachine_T *smInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)    ||\
        (NULL == smInputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    smInputMBD->Thresholds.ChargeDetectionThreshold_mA      = dpInput->thresholdDataBus.chargeDetectionThreshold_mA;
    smInputMBD->Thresholds.NormalModeTimeout_msec           = tData->NormalModeTimeout_msec;
    smInputMBD->Thresholds.PowerSaverModeTimeout_msec       = tData->PowerSaverModeTimeout_msec;
    smInputMBD->Thresholds.FaultsModeTimeout_msec           = tData->FaultsModeTimeout_msec;
    smInputMBD->Thresholds.ServiceModeTimeout_msec          = tData->ServiceModeTimeout_msec;

    status = BMS_OK;
	return status;
}

bmsStatus_t stateMachineInput(dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, ExtU_HighLevelStatemachine_T *smInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpOutput)        ||\
        (NULL == pOutput)         ||\
        (NULL == smInputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    smInputMBD->DataPipeline.Current_mA                 = dpOutput->current;
    smInputMBD->DataPipeline.isChargeEn                 = dpOutput->isChargeEn;

    smInputMBD->DataPipeline.VCU.SlowCharge             = dpOutput->VCU.SlowCharge;
    smInputMBD->DataPipeline.VCU.FastCharge             = dpOutput->VCU.FastCharge;
    smInputMBD->DataPipeline.VCU.isChargerConnected     = dpOutput->VCU.isChargerConnected;
    smInputMBD->DataPipeline.VCU.TransmissionFail       = dpOutput->VCU.TransmissionFail;
    smInputMBD->DataPipeline.VCU.VCUPermanentFail       = dpOutput->VCU.VCUPermanentFail;
    smInputMBD->DataPipeline.VCU.ServiceMode            = dpOutput->VCU.ServiceMode;

    smInputMBD->ProtectionOutput_a.ErrorDetect          = pOutput->protectionOutputData.ErrorDetect;
    
    status = BMS_OK;
	return status;
}

bmsStatus_t stateMachineOutput(stateMachineOutput_t *smOutput, ExtY_HighLevelStatemachine_T *smOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == smOutput)        ||\
        (NULL == smOutputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    smOutput->HighLevelStateMode = smOutputMBD->HighLevelStateMode;

    status = BMS_OK;
	return status;
}


bmsStatus_t chargingInit(thresholdData_t *tData, ExtU_ChargingAlgorithm_T *chargingInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == chargingInputMBD) )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    // chargingInputMBD->Thresholds.ChargeAlgorithm.SOC_forCVCharge_cpct         = tData->ChargeAlgorithm.SOC_forCVCharge_cpct;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV     = tData->ChargeAlgorithm.BatteryRevivalVoltage_mV;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.NoCharge_K                   = tData->ChargeAlgorithm.NoCharge_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.BatteryRevival_K             = tData->ChargeAlgorithm.BatteryRevival_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.CC_K                         = tData->ChargeAlgorithm.CC_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.CC_Balancing_K               = tData->ChargeAlgorithm.CC_Balancing_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.CV_K                         = tData->ChargeAlgorithm.CV_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.FastCharging_K               = tData->ChargeAlgorithm.FastCharging_K;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV       = tData->ChargeAlgorithm.CVTransitionVoltage_mV;
    // chargingInputMBD->Thresholds.ChargeAlgorithm.TransitionTimeout_msec       = tData->ChargeAlgorithm.TransitionTimeout_msec;


    chargingInputMBD->Thresholds.Balancing_Min_Thr_mV           = tData->balancingMinThreshold;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    chargingInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    chargingInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    chargingInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;
    status = BMS_OK;
	return status;
}


bmsStatus_t chargingInput(ExtU_DataPipeline_T *dpInputMBD, ExtY_DataPipeline_T *dpOutputMBD, ExtU_ChargingAlgorithm_T *chargingInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpInputMBD)        ||\
        (NULL == dpOutputMBD)       ||\
        (NULL == chargingInputMBD) )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    chargingInputMBD->DataPipeline.VCU.FastCharge               = dpOutputMBD->DataPipelineb.VCU.FastCharge;
    chargingInputMBD->DataPipeline.VCU.SlowCharge               = dpOutputMBD->DataPipelineb.VCU.SlowCharge;
    chargingInputMBD->DataPipeline.VCU.isChargerConnected       = dpOutputMBD->DataPipelineb.VCU.isChargerConnected;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_min       = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_min;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_max       = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_max;
    chargingInputMBD->DataPipeline.DCL_ChargingCurrent_A        = dpOutputMBD->DataPipelineb.DCL_ChargingCurrent_A;
    chargingInputMBD->DataPipeline.VCU.ChargersCurrentLimit_A   = dpOutputMBD->DataPipelineb.VCU.ChargersCurrentLimit_A;

    status = BMS_OK;
	return status;
}


bmsStatus_t chargingOutput(chargingOutputData_t *chargerOutput, ExtY_ChargingAlgorithm_T *chargingOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == chargingOutputMBD)
      )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    chargerOutput->ChargingState           = chargingOutputMBD->ChargingState;
    chargerOutput->ChargingCurrentReq_mA   = chargingOutputMBD->ChargingCurrentReq_mA;

    status = BMS_OK;
	return status;
}


float SoCManipulation(bmsMainData_t *mdata, ExtY_SOCEstimation_T *ccOutputMBD)
{
    mdata->socData.estimatedSoC = ccOutputMBD->CC_Outputs.SOC_cpct;   
    return mdata->socData.estimatedSoC;   
}

    /*Error Counts in BMS active mode*/
    uint8_t overVoltageWarningCount = 0;
    uint8_t underVoltageWarningCount = 0;
    uint8_t overVoltageErrorCount = 0;
    uint8_t underVoltageErrorCount = 0;

    uint8_t overCurrentDischargeWarningCount = 0;
    uint8_t overCurrentDischargeErrorCount = 0;
    uint8_t overCurrentChargeWarningCount = 0;
    uint8_t overCurrentChargeErrorCount = 0;

    uint8_t eFuseChargeFlagCount = 0;
    uint8_t eFuseDischargeFlagCount = 0;
    uint8_t highImbalanceFlagCount = 0;
    uint8_t suddenVoltageDropCount = 0;
    uint8_t temperatureGradientCount = 0;
    bool shortCircuitError = FALSE;
    bool thermalRunawayError = FALSE;

    uint8_t underTempChargeWarningCount = 0;
    uint8_t underTempChargeErrorCount = 0;
    uint8_t underTempDischargeWarningCount = 0;
    uint8_t underTempDischargeErrorCount = 0;

    uint8_t overTempChargeWarningCount = 0;
    uint8_t overTempChargeErrorCount = 0;
    uint8_t overTempDischargeWarningCount = 0;
    uint8_t overTempDischargeErrorCount = 0;

    uint8_t errorCodes1Dataframe[8] = {0};
    uint8_t errorCodes2Dataframe[8] = {0};

uint8_t storeErrorCodes(protectionOutput_t *pOutput)
{
    /*This function stores all the errors that has occured in active state and publishes them on 0x258 ID
      Temperature errors being published on 0x259 ID*/
    bmsStatus_t status = BMS_ERROR;

    if(pOutput->protectionOutputData.ErrorDetect)  //check whether any error has occcured and update the errorCodes with new values 
    {
        /*OVER VOLTAGE ERRORS*/


        // coolDown

        uint32_t coolDownTime = 3000;
        static uint32_t previousOVTime=0;
        uint32_t currentOVTime =HAL_GetTick();
        bool flag;
        if(previousOVTime!=0 && (currentOVTime-previousOVTime)<coolDownTime){
            flag=false;
        }
        else{
            flag=true;
            previousOVTime=currentOVTime;
        } 


        if(pOutput->protectionOutputData.OV && flag)
        {
            if(pOutput->protectionOutputData.OV == 1)
            {
                uint32_t time=HAL_GetTick();
                if(time){
                    overVoltageWarningCount++;
                    overVoltageWarningCount--;
                }
                overVoltageWarningCount ++;
                errorCodes1Dataframe[0] &= 0x0F;
                errorCodes1Dataframe[0] |= (overVoltageWarningCount & 0x0F) << 4;      
            }
            else if(pOutput->protectionOutputData.OV == 2)
            {
                overVoltageErrorCount++;                                
                errorCodes1Dataframe[0] &= 0xF0;
                errorCodes1Dataframe[0] |= overVoltageErrorCount;      
            }
        }

        /*UNDER VOLTAGE ERRORS*/
        if(pOutput->protectionOutputData.UV)
        {
            if(pOutput->protectionOutputData.UV == 1)
            {
                underVoltageWarningCount++;
                errorCodes1Dataframe[1] &= 0x0F;
                errorCodes1Dataframe[1] |= (underVoltageWarningCount & 0x0F) << 4;
            }
            else if(pOutput->protectionOutputData.UV == 2)
            {
                underVoltageErrorCount++;
                errorCodes1Dataframe[1] &= 0xF0;
                errorCodes1Dataframe[1] |= underVoltageErrorCount;
            }
        }

        /*OVER CURRENT DISCHARGE ERRORS*/
        if(pOutput->protectionOutputData.OCD)
        {
            if(pOutput->protectionOutputData.OCD == 1)
            {
                overCurrentDischargeWarningCount++;
                errorCodes1Dataframe[2] &= 0x0F;
                errorCodes1Dataframe[2] |= (overCurrentDischargeWarningCount & 0x0F) << 4;  
            }
            else if(pOutput->protectionOutputData.OCD == 2)
            {
                overCurrentDischargeErrorCount++;
                errorCodes1Dataframe[2] &= 0xF0;
                errorCodes1Dataframe[2] |= overCurrentDischargeErrorCount;
            }
        }

        /*OVER CURRENT CHARGE*/
        if(pOutput->protectionOutputData.OCC)
        {
            if(pOutput->protectionOutputData.OCC == 1)
            {
                overCurrentChargeWarningCount++;
                errorCodes1Dataframe[3] &= 0x0F;
                errorCodes1Dataframe[3] |= (overCurrentChargeWarningCount & 0x0F) << 4;  
            }
            else if(pOutput->protectionOutputData.OCC == 2)
            {
                overCurrentChargeErrorCount++;
                errorCodes1Dataframe[3] &= 0xF0;
                errorCodes1Dataframe[3] |= overCurrentChargeErrorCount;
            }
        }

        /*eFUSE FLAGS*/
        if(pOutput->protectionOutputData.eFuseChargeFlag || pOutput->protectionOutputData.eFuseDischargeFlag)
        {
            if(pOutput->protectionOutputData.eFuseChargeFlag)
            {
                eFuseChargeFlagCount++;
                errorCodes1Dataframe[4] &= 0x0F;
                errorCodes1Dataframe[4] |= (eFuseChargeFlagCount & 0x0F) << 4;
            }
            else if(pOutput->protectionOutputData.eFuseDischargeFlag)
            {
                eFuseDischargeFlagCount++;
                errorCodes1Dataframe[4] &= 0xF0;
                errorCodes1Dataframe[4] |= eFuseDischargeFlagCount;
            }

        }

        /*ADVANCED PROTECTIONS*/
        if(pOutput->protectionOutputData.HighImbalanceFlag || pOutput->protectionOutputData.SuddenVoltageDrop)
        {
            if(pOutput->protectionOutputData.HighImbalanceFlag)
            {
                highImbalanceFlagCount++;
                errorCodes1Dataframe[5] &= 0x0F;
                errorCodes1Dataframe[5] |= (highImbalanceFlagCount & 0x0F) << 4;
            }
            else if(pOutput->protectionOutputData.SuddenVoltageDrop)
            {
                suddenVoltageDropCount++;
                errorCodes1Dataframe[5] &= 0xF0;
                errorCodes1Dataframe[5] |= suddenVoltageDropCount;
            }
        }

        if(pOutput->protectionOutputData.TemperatureGradient || pOutput->protectionOutputData.ShortCircuit || pOutput->protectionOutputData.ThermalRunaway)
        {
            if(pOutput->protectionOutputData.TemperatureGradient)
            {
                temperatureGradientCount++;
                errorCodes1Dataframe[6] &= 0x0F;
                errorCodes1Dataframe[6] |= (temperatureGradientCount & 0x0F) << 4;
            }
            else if(pOutput->protectionOutputData.ShortCircuit)
            {
                shortCircuitError = TRUE;
                errorCodes1Dataframe[6] &= 0xF0;
                errorCodes1Dataframe[6] |= (shortCircuitError << 3);
            }
            else if(pOutput->protectionOutputData.ThermalRunaway)
            {
                thermalRunawayError = TRUE;
                errorCodes1Dataframe[6] &= 0xF0;
                errorCodes1Dataframe[6] |= (thermalRunaway << 2);
            }
        }

    /*-------------------------------------TEMPERATURE PROTECTION ERRORS-------------------------------------------*/
        uint8_t temp[2] = {0};
        temp[0] = pOutput->protectionOutputData.FlagGroup1;
        temp[1] = pOutput->protectionOutputData.FlagGroup2;

        for(uint8_t group = 0; group < 2; group++)
        {        
            if(temp[group])  
            {
                /*UNDER TEMPERATURE CHARGE*/
                if(temp[group] == 1 && pOutput->protectionOutputData.TempState1 == 1)
                {
                    underTempChargeWarningCount++;
                    errorCodes2Dataframe[0 + group*4] &= 0x0F;
                    errorCodes2Dataframe[0 + group*4] |= (underTempChargeWarningCount & 0x0F) << 4;
                }
                else if(temp[group] == 2 && pOutput->protectionOutputData.TempState1 == 1)
                {
                    underTempChargeErrorCount++;
                    errorCodes2Dataframe[0 + group*4] &= 0xF0;
                    errorCodes2Dataframe[0 + group*4] |= underTempChargeErrorCount;
                }

                /*UNDER TEMPERATURE DISCHARGE*/
                else if(temp[group] == 1 && pOutput->protectionOutputData.TempState1 == 2)
                {
                    underTempDischargeWarningCount++;
                    errorCodes2Dataframe[1 + group*4] &= 0x0F;
                    errorCodes2Dataframe[1 + group*4] |= (underTempDischargeWarningCount &= 0x0F) << 4;

                }
                else if(temp[group] == 2 && pOutput->protectionOutputData.TempState1 == 2)
                {
                    underTempDischargeErrorCount++;
                    errorCodes2Dataframe[1 + group*4] &= 0xF0;
                    errorCodes2Dataframe[1 + group*4] |= underTempDischargeErrorCount;
                }

                /*OVER TEMPERATURE CHARGE*/
                else if(temp[group] == 1 && pOutput->protectionOutputData.TempState1 == 3)
                {
                    overTempChargeWarningCount++;
                    errorCodes2Dataframe[2 + group*4] &= 0x0F;
                    errorCodes2Dataframe[2] |= (overTempChargeWarningCount &= 0x0F) << 4;
                }
                else if(temp[group] == 2 && pOutput->protectionOutputData.TempState1 == 3)
                {
                    overTempChargeErrorCount++;
                    errorCodes2Dataframe[2 + group*4] &= 0xF0;
                    errorCodes2Dataframe[2 + group*4] |= overTempChargeErrorCount;
                }

                /*OVER TEMPERATURE DISCHARGE*/
                else if(temp[group] == 1 && pOutput->protectionOutputData.TempState1 == 4)
                {
                    overTempDischargeWarningCount++;
                    errorCodes2Dataframe[3 + group*4] &= 0x0F;
                    errorCodes2Dataframe[3 + group*4] |= (overTempDischargeErrorCount &= 0x0F) << 4;
                }
                else if(temp[group] == 2 && pOutput->protectionOutputData.TempState1 == 4)
                {
                    overTempDischargeErrorCount++;
                    errorCodes2Dataframe[3 + group*4] &= 0xF0;
                    errorCodes2Dataframe[3 + group*4] |= overTempDischargeErrorCount;
                }
            }
        }
    }

    status = sendCANDataOnQueue(CAN_TX_ID_013, errorCodes1Dataframe, sizeof(errorCodes1Dataframe));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    status = sendCANDataOnQueue(CAN_TX_ID_014, errorCodes2Dataframe, sizeof(errorCodes2Dataframe));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    //TODO
    /*create a sanity function to check whether VCU transmission is normal
      Update VCU transmission fail using that*/

    return status;
}

#ifdef DEBUG_CAN
extern uint16_t pInVoltageMessageIDIndex[5];

#ifdef EKF
bmsStatus_t sendCoulombCountingEKFDataOnCAN(ccEKFInput_t *ccEKFInput, ccEKFOutput_t *ccEKFOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

		for(int8_t canDLC = CAN_ccIn_pOut_OVThreshold.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccIn_pOut_OVThreshold.messageStartByte + canDLC] = ((int) (ccEKFInput->protectionOutputOVThreshold/CAN_ccIn_pOut_OVThreshold.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_ccIn_Voltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccIn_Voltage.messageStartByte + canDLC] = ((int) (ccEKFInput->avgCellVoltage /CAN_ccIn_Voltage.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_ccIn_Current.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccIn_Current.messageStartByte + canDLC] = ((int) (ccEKFInput->current /CAN_ccIn_Current.messageScaling)  >> (8 * canDLC));
    	
        for(int8_t canDLC = CAN_ccIn_VehicleRest_Time.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccIn_VehicleRest_Time.messageStartByte + canDLC] = ((int) (ccEKFInput->vehicleRestTime /CAN_ccIn_VehicleRest_Time.messageScaling)  >> (8 * canDLC));
		
    status = sendCANDataOnQueue(CAN_ccIn_Current.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);
    
    //     //ID 321
		
	// 	for(int8_t canDLC = CAN_ccIn_Max_PackCapacity.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_ccIn_Max_PackCapacity.messageStartByte + canDLC] = ((int) (ccEKFInput->maxPackCapacity /CAN_ccIn_Max_PackCapacity.messageScaling)  >> (8 * canDLC));
		
	// 	for(int8_t canDLC = CAN_ccIn_VehicleRest_DisCharge_Current.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_ccIn_VehicleRest_DisCharge_Current.messageStartByte + canDLC] = ((int) (ccEKFInput->vehicleRestCurrent /CAN_ccIn_VehicleRest_DisCharge_Current.messageScaling)  >> (8 * canDLC));
		

    // status = sendCANDataOnQueue(CAN_ccIn_VehicleRest_DisCharge_Current.messageID, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    // delayMilliSeconds(CAN_MESSAGE_DELAY);

        for(int8_t canDLC = CAN_ccOut_Coulomb_Counting_State.messageDLC - 1; canDLC >= 0; canDLC--)
            canDataFrame[CAN_ccOut_Coulomb_Counting_State.messageStartByte + canDLC] = ((int) (ccEKFOutput->coulombCountingState/CAN_ccOut_Coulomb_Counting_State.messageScaling)  >> (8 * canDLC));
       
		for(int8_t canDLC = CAN_ccOut_SOC_Pct.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_SOC_Pct.messageStartByte + canDLC] = ((int) (ccEKFOutput->socCC/CAN_ccOut_SOC_Pct.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_ccOut_EKF_SOC_Pct.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_EKF_SOC_Pct.messageStartByte + canDLC] = ((int) (ccEKFOutput->socEKF/CAN_ccOut_EKF_SOC_Pct.messageScaling)  >> (8 * canDLC));

        for(int8_t canDLC = CAN_ccOut_Initial_Capacity_Ah.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Initial_Capacity_Ah.messageStartByte + canDLC] = ((int) (ccEKFOutput->initialCapacity /CAN_ccOut_Initial_Capacity_Ah.messageScaling)  >> (8 * canDLC));

    status = sendCANDataOnQueue(CAN_ccOut_Initial_Capacity_Ah.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_ccOut_Total_Capacity_Discharged_Ah.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Total_Capacity_Discharged_Ah.messageStartByte + canDLC] = ((int) (ccEKFOutput->totalCapacityDischarge/CAN_ccOut_Total_Capacity_Discharged_Ah.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_ccOut_Total_Capacity_Remaining_Ah.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Total_Capacity_Remaining_Ah.messageStartByte + canDLC] = ((int) (ccEKFOutput->totalCapacityRemaining/CAN_ccOut_Total_Capacity_Remaining_Ah.messageScaling)  >> (8 * canDLC));

    status = sendCANDataOnQueue(CAN_ccOut_Total_Capacity_Remaining_Ah.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    //ID 29 - 0x33C
		for(int8_t canDLC = CAN_ccOut_Capacity_EKF.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Capacity_EKF.messageStartByte + canDLC] = ((int) (ccEKFOutput->capacityEKF/CAN_ccOut_Capacity_EKF.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_ccOut_Voltage_EKF.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Voltage_EKF.messageStartByte + canDLC] = ((int) (ccEKFOutput->voltageEKF /CAN_ccOut_Voltage_EKF.messageScaling)  >> (8 * canDLC));
		
        for(int8_t canDLC = CAN_ccOut_Terminal_Voltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_ccOut_Terminal_Voltage.messageStartByte + canDLC] = ((int) (ccEKFOutput->terminalVoltage /CAN_ccOut_Terminal_Voltage.messageScaling)  >> (8 * canDLC));
    
    status = sendCANDataOnQueue(CAN_ccOut_Terminal_Voltage.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    return status;
}

#else

bmsStatus_t sendCoulombCountingDataOnCAN(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    //ID 28
    for(int8_t canDLC = CAN_ccOut_initialCapacity.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_initialCapacity.messageStartByte + canDLC] = ((int) (ccOutput->initialCapacity/CAN_ccOut_initialCapacity.messageScaling)  >> (8 * canDLC));
            
    for(int8_t canDLC = CAN_ccOut_totalCapacityRemaining.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_totalCapacityRemaining.messageStartByte + canDLC] = ((int) (ccOutput->totalCapacityRemaining /CAN_ccOut_totalCapacityRemaining.messageScaling)  >> (8 * canDLC));
    
    status = sendCANDataOnQueue(CAN_ccOut_totalCapacityRemaining.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);
    
	//ID 2
    for(int8_t canDLC = CAN_ccOut_MaxUsableCapacity_mAh.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_MaxUsableCapacity_mAh.messageStartByte + canDLC] = ((int) (ccOutput->MaxUsableCapacity_mAh /CAN_ccOut_MaxUsableCapacity_mAh.messageScaling)  >> (8 * canDLC));
    
    for(int8_t canDLC = CAN_ccOut_TotalCapacityExchange_mAh.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_TotalCapacityExchange_mAh.messageStartByte + canDLC] = ((int) (ccOutput->TotalCapacityExchange_mAh /CAN_ccOut_TotalCapacityExchange_mAh.messageScaling)  >> (8 * canDLC));
    

    status = sendCANDataOnQueue(CAN_ccOut_TotalCapacityExchange_mAh.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    //ID 3
    // for(int8_t canDLC = CAN_ccOut_totalCapacityDischarge.messageDLC - 1; canDLC >= 0; canDLC--)
    // 	canDataFrame[CAN_ccOut_totalCapacityDischarge.messageStartByte + canDLC] = ((int) (ccOutput->totalCapacityDischarge/CAN_ccOut_totalCapacityDischarge.messageScaling)  >> (8 * canDLC));

    for(int8_t canDLC = CAN_ccOut_SOCPercentage.messageDLC - 1; canDLC >= 0; canDLC--)
    	canDataFrame[CAN_ccOut_SOCPercentage.messageStartByte + canDLC] = ((int) (ccOutput->SOC_pct /CAN_ccOut_SOCPercentage.messageScaling)  >> (8 * canDLC));

    for(int8_t canDLC = CAN_ccOut_SOH_cpct.messageDLC - 1; canDLC >= 0; canDLC--)
            canDataFrame[CAN_ccOut_SOH_cpct.messageStartByte + canDLC] = ((int) (ccOutput->SOH_pct /CAN_ccOut_SOH_cpct.messageScaling )  >> (8 * canDLC));
    
    for(int8_t canDLC = CAN_ccOut_coulombCountingState.messageDLC - 1; canDLC >= 0; canDLC--)
            canDataFrame[CAN_ccOut_coulombCountingState.messageStartByte + canDLC] = ((int) (ccOutput->coulombCountingState /CAN_ccOut_coulombCountingState.messageScaling)  >> (8 * canDLC));
    
    
    status = sendCANDataOnQueue(CAN_ccOut_coulombCountingState.messageID , canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    return status;
}
#endif

bmsStatus_t sendContactorDataOnCAN(thresholdData_t *tData, contactorInput_t *cInput, contactorOutput_t *cOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

		for(int8_t canDLC = CAN_cIn_OCC.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OCC.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.OCC/CAN_cIn_OCC.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_OCD.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OCD.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.OCD/CAN_cIn_OCD.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_OTC.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OTC.messageStartByte + canDLC] = 0;//((int) (cInput->protectionOutputData.OTC/CAN_cIn_OTC.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cIn_OTD.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OTD.messageStartByte + canDLC] = 0;//((int) (cInput->protectionOutputData.OTD/CAN_cIn_OTD.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_UTC.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_UTC.messageStartByte + canDLC] = 0;//((int) (cInput->protectionOutputData.UTC/CAN_cIn_UTC.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cIn_UTD.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_UTD.messageStartByte + canDLC] = 0;//((int) (cInput->protectionOutputData.UTD/CAN_cIn_UTD.messageScaling)  >> (8 * canDLC));
		
        for(int8_t canDLC = CAN_cIn_OV.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OV.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.OV/CAN_cIn_OV.messageScaling)  >> (8 * canDLC));						
	
		for(int8_t canDLC = CAN_cIn_UV.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_UV.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.UV/CAN_cIn_UV.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_cIn_UV.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cIn_ThermalRunaway.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_ThermalRunaway.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.ThermalRunaway/CAN_cIn_ThermalRunaway.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_communicationFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_communicationFlag.messageStartByte + canDLC] = ((int) (cInput->communicationFlag/CAN_cIn_communicationFlag.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_shortCircuitFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_shortCircuitFlag.messageStartByte + canDLC] = ((int) (externalShortCircuitDetectionFlag/CAN_cIn_shortCircuitFlag.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cIn_permanentFailFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_permanentFailFlag.messageStartByte + canDLC] = ((int) (cInput->permanentFailFlag/CAN_cIn_permanentFailFlag.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_contactorErrorFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorErrorFlag.messageStartByte + canDLC] = ((int) (cInput->contactorErrorFlag/CAN_cIn_contactorErrorFlag.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cIn_preChargeContactorCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_preChargeContactorCurrent.messageStartByte + canDLC] = ((int) (tData->preChargeContactorCurrent/CAN_cIn_preChargeContactorCurrent.messageScaling)  >> (8 * canDLC));
		
        canDataFrame[6]     =     cInput->protectionOutputData.TemperatureGradient;
        canDataFrame[7]     =     cInput->protectionOutputData.HighImbalanceFlag;

    status = sendCANDataOnQueue(CAN_cIn_MaxImbalance.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cIn_NegContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_NegContactor_FB.messageStartByte + canDLC] = ((int) (cInput->negativeContactorFeedback/CAN_cIn_NegContactor_FB.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_PosContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_PosContactor_FB.messageStartByte + canDLC] = ((int) (cInput->positiveContactorFeedback/CAN_cIn_PosContactor_FB.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_PreChargeContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_PreChargeContactor_FB.messageStartByte + canDLC] = ((int) (cInput->preChargeContactorFeedback/CAN_cIn_PreChargeContactor_FB.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cIn_contactorCommand.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorCommand.messageStartByte + canDLC] = ((int) (cInput->contactorCommand/CAN_cIn_contactorCommand.messageScaling)  >> (8 * canDLC));
				
		// for(int8_t canDLC = CAN_cIn_contactorResponseTime_msec.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_cIn_contactorResponseTime_msec.messageStartByte + canDLC] = ((int) (tData->contactorResponseTime_msec/CAN_cIn_contactorResponseTime_msec.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cIn_contactorCutoffTime.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorCutoffTime.messageStartByte + canDLC] = ((int) (tData->contactorCutoffTime/CAN_cIn_contactorCutoffTime.messageScaling)  >> (8 * canDLC));
		
    
    status = sendCANDataOnQueue(CAN_cIn_contactorCutoffTime.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    	for(int8_t canDLC = CAN_cOut_NegContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_NegContactorState.messageStartByte + canDLC] = ((int) (cOutput->negativeContactorState/CAN_cOut_NegContactorState.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cOut_PosContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_PosContactorState.messageStartByte + canDLC] = ((int) (cOutput->positiveContactorState/CAN_cOut_PosContactorState.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cOut_PreChargeContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_PreChargeContactorState.messageStartByte + canDLC] = ((int) (cOutput->preChargeContactorState/CAN_cOut_PreChargeContactorState.messageScaling)  >> (8 * canDLC));
		
        canDataFrame[3] = cOutput->contactorWeldWarning;
        canDataFrame[4] = cOutput->permanentFaiil;

        canDataFrame[5] = cInput->protectionOutputData.ShortCircuit;
        canDataFrame[6] = cInput->protectionOutputData.SuddenVoltageDrop;
        canDataFrame[7] = cOutput->contactorsState;

    status = sendCANDataOnQueue(CAN_cOut_PreChargeContactorState.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);  
    return status;
}

bmsStatus_t sendCellBalancingDataOnCAN(thresholdData_t *tData, cellBalancingInput_t *cbInput, cellBalancingOutput_t *cbOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    	for(int8_t canDLC = CAN_cbIn_balancingMinThreshold.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbIn_balancingMinThreshold.messageStartByte + canDLC] = ((int) (tData->balancingMinThreshold/CAN_cbIn_balancingMinThreshold.messageScaling)  >> (8 * canDLC));
				
    	for(int8_t canDLC = CAN_cbIn_balancingMaxAllowedImbalance.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbIn_balancingMaxAllowedImbalance.messageStartByte + canDLC] = ((int) (tData->balancingMaxAllowedImbalance/CAN_cbIn_balancingMaxAllowedImbalance.messageScaling)  >> (8 * canDLC));
				   
    	// for(int8_t canDLC = CAN_cbIn_balancingOVProtectionThreshold.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_cbIn_balancingOVProtectionThreshold.messageStartByte + canDLC] = ((int) (tData->balancingOVProtectionThreshold/CAN_cbIn_balancingOVProtectionThreshold.messageScaling)  >> (8 * canDLC));
        
        for(int8_t canDLC = CAN_cbIn_balancingStateRestPeroid.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbIn_balancingStateRestPeroid.messageStartByte + canDLC] = ((int) (tData->balancingStateRestPeroid/CAN_cbIn_balancingStateRestPeroid.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_cbIn_balancingStateRestPeroid.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);   

        for(int8_t canDLC = CAN_cbIn_cellsInSeries.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbIn_cellsInSeries.messageStartByte + canDLC] = ((int) (20/CAN_cbIn_cellsInSeries.messageScaling)  >> (8 * canDLC));
        
        for(int8_t canDLC = CAN_cbOut_balancingState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbOut_balancingState.messageStartByte + canDLC] = ((int) (cbOutput->balancingState/CAN_cbOut_balancingState.messageScaling)  >> (8 * canDLC));
				
        for(int i = 0; i < 6; i++)
            canDataFrame[i+2] = (int) cbOutput->balancingCommand[i];


    status = sendCANDataOnQueue(CAN_cbOut_balancingState.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

        for(int i = 6; i < 14; i++)
            canDataFrame[i-6] = (int) cbOutput->balancingCommand[i];

    status = sendCANDataOnQueue(CAN_DEBUG_ID_010, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

        for(int i = 14; i < 20; i++)
            canDataFrame[i-14] = (int) cbOutput->balancingCommand[i];

    status = sendCANDataOnQueue(CAN_DEBUG_ID_011, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);
    return status;
}

bmsStatus_t sendBMSDataOnCAN(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, dataPipelineInput_t *dpInput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    //ID 27
    //2949 timebase
    canDataFrame[0] =  (int)(mdata->ccmData.timeBase/0.1);
    canDataFrame[1] = ((int)(mdata->ccmData.timeBase/0.1) >> 8);
    canDataFrame[2] = ((int)(mdata->ccmData.timeBase/0.1) >> 16);
    canDataFrame[3] = ((int)(mdata->ccmData.timeBase/0.1) >> 24);

    canDataFrame[4] =  (int)(mdata->ccmData.statusReg);
    canDataFrame[5] =  (int)(mdata->ccmData.opctrlReg);
    canDataFrame[6] =  (int)(mdata->ccmData.faultsReg);
    canDataFrame[7] =  (int)(mdata->ccmData.tbReg);

    status = sendCANDataOnQueue(CAN_DEBUG_ID_027, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    //p input temperatures 0 to 7
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[0].temperature[0]);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[0].temperature[1]);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[0].temperature[2]);
        canDataFrame[3] =  (int) floor(dpOutput->temperatureSense[0].temperature[3]);

        canDataFrame[4] =  (int) floor(dpOutput->temperatureSense[0].temperature[4]);
        canDataFrame[5] =  (int) floor(dpOutput->temperatureSense[0].temperature[5]);
        canDataFrame[6] =  (int) floor(dpOutput->temperatureSense[0].temperature[6]);
        canDataFrame[7] =  (int) floor(dpInput->shuntTemp);

    status = sendCANDataOnQueue(CAN_DEBUG_ID_018, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

    //p input temperatures 7 to 13
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[1].temperature[0]);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[1].temperature[1]);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[1].temperature[2]);
        canDataFrame[3] =  (int) floor(dpOutput->temperatureSense[1].temperature[3]);
        canDataFrame[4] =  (int) floor(dpOutput->temperatureSense[1].temperature[4]);
        canDataFrame[5] =  (int) floor(dpOutput->temperatureSense[1].temperature[5]);
        canDataFrame[6] =  (int) floor(dpOutput->temperatureSense[1].temperature[6]);


    status = sendCANDataOnQueue(CAN_DEBUG_ID_019, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //group1 max and min temperatures
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[0].temperatureMax);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[0].temperatureMin);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[0].temperatureDelta);
    
    //group2 max and min temperatures
        canDataFrame[3] = (int) floor(dpOutput->temperatureSense[1].temperatureMax);
        canDataFrame[4] = (int) floor(dpOutput->temperatureSense[1].temperatureMin);
        canDataFrame[5] = (int) floor(dpOutput->temperatureSense[1].temperatureDelta);
        canDataFrame[6] = dpOutput->isChargeEn;
    
    status = sendCANDataOnQueue(CAN_DEBUG_ID_020, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //dynamic current data on CAN

        // canDataFrame[0] =  (uint8_t)mdata.dynamicLimitIn & 0xFF;
        // canDataFrame[1] =  (uint8_t)mdata.dynamicLimitIn>>8 &0xFF;
        // canDataFrame[3] =  (uint8_t)mdata.dynamicLimitOut & 0xFF;
        // canDataFrame[4] =  (uint8_t)mdata.dynamicLimitOut >>8 &0XFF;
   

    // for(int8_t canDLC = CAN_dynamicLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_dynamicLimitIN.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitIn/CAN_dynamicLimitIN.messageScaling) >> (8 * canDLC));

    // for(int8_t canDLC = CAN_dynamicLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_dynamicLimitOUT.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitOut/CAN_dynamicLimitOUT.messageScaling) >> (8 * canDLC));

    // for(int8_t canDLC = CAN_FirmwareVersion.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_FirmwareVersion.messageStartByte + canDLC] = (MARVEL_FW_VER >> (8 * canDLC));

    // canDataFrame[6] = MARVEL_CONFIG;

    // status = sendCANDataOnQueue(CAN_TX_ID_011, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    // delayMilliSeconds(CAN_MESSAGE_DELAY); 

    return status;
}

bmsStatus_t sendProtectionDataOnCAN(thresholdData_t *tData, protectionInput_t *pInput, protectionOutput_t *pOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

        for(int8_t canDLC = CAN_pIn_voltageMax.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageMax.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMax/CAN_pIn_voltageMax.messageScaling)  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_voltageMin.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageMin.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMin/CAN_pIn_voltageMin.messageScaling)  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_voltageDelta.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageDelta.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageDelta/CAN_pIn_voltageDelta.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_pIn_voltageDelta.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_pIn_current.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_current.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.current/CAN_pIn_current.messageScaling)  >> (8 * canDLC));
		
        for(int8_t canDLC = CAN_pIn_OCC_WarningCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_OCC_WarningCurrent.messageStartByte + canDLC] = ((int) (tData->OCC_WarningCurrent/CAN_pIn_OCC_WarningCurrent.messageScaling)  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_OCC_ErrorCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_OCC_ErrorCurrent.messageStartByte + canDLC] = ((int) (tData->OCC_ErrorCurrent/CAN_pIn_OCC_ErrorCurrent.messageScaling)  >> (8 * canDLC));

    status = sendCANDataOnQueue(CAN_pIn_OCC_ErrorCurrent.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

        //ID 0x33D , i^2 fuse values
        for(int8_t canDLC = CAN_pIn_eFuseChargingLimit_A2s.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_eFuseChargingLimit_A2s.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec/(CAN_pIn_eFuseChargingLimit_A2s.messageScaling))  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_eFuseDischargingLimit_A2s.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_eFuseDischargingLimit_A2s.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec/(CAN_pIn_eFuseDischargingLimit_A2s.messageScaling))  >> (8 * canDLC));

        // canDataFrame[4] = contChargingLimit_A & 0xFF ;
        // canDataFrame[5] = (contChargingLimit_A >> 8) & 0xFF;

        // canDataFrame[6] = contDischargingLimit_A & 0xFF;
        // canDataFrame[7] = (contDischargingLimit_A >> 8) & 0xFF;

    status = sendCANDataOnQueue(CAN_pIn_eFuseChargingLimit_A2s.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

        //ID 0x33E, temp states
        canDataFrame[0] =  pOutput->protectionOutputData.TempOverallState;
        canDataFrame[1] =  pOutput->protectionOutputData.TempState1;
        canDataFrame[2] =  pOutput->protectionOutputData.TempState2;
        canDataFrame[3] =  pOutput->protectionOutputData.FlagGroup1;
        canDataFrame[4] =  pOutput->protectionOutputData.FlagGroup2;
        canDataFrame[5] =  pOutput->protectionOutputData.eFuseDischargeFlag; 
        canDataFrame[6] =  pOutput->protectionOutputData.ErrorDetect;
        canDataFrame[7] =  pOutput->protectionOutputData.eFuseChargeFlag;

    status = sendCANDataOnQueue(CAN_DEBUG_ID_031, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //     //ID 0x33F, temp groups error code
    //     canDataFrame[0] = pOutput->protectionOutputData.ErrorTypeGroup1;
    //     canDataFrame[1] = pOutput->protectionOutputData.ErrorTypeGroup2;
    //     canDataFrame[2] = pOutput->protectionOutputData.ErrorTypeGroup3;
    //     canDataFrame[3] = pOutput->protectionOutputData.ErrorTypeGroup4;
    //     canDataFrame[4] = pOutput->protectionOutputData.ErrorTypeGroup5;

    //     canDataFrame[5] =  0;
    //     canDataFrame[6] =  0;
    //     canDataFrame[7] =  0;

    // status = sendCANDataOnQueue(CAN_DEBUG_ID_032, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    // delayMilliSeconds(CAN_MESSAGE_DELAY); 


#ifdef CELL_VOLTAGES
    //id 22 
    canDataFrame[0] =  (int)(pInput->OCD_WarningCurrent);
    canDataFrame[1] = ((int)(pInput->OCD_WarningCurrent) >> 8);

    canDataFrame[2] =  (int)(pInput->OCD_ErrorCurrent);
    canDataFrame[3] = ((int)(pInput->OCD_ErrorCurrent) >> 8);

    canDataFrame[4] =  (int)(pInput->OTC_WarningTimeout);
    canDataFrame[5] = ((int)(pInput->OTC_WarningTimeout) >> 8);

    canDataFrame[6] =  (int)(pInput->OTC_ErrorTimeout);
    canDataFrame[7] = ((int)(pInput->OTC_ErrorTimeout) >> 8);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_022, canDataFrame);
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 
    
    //id 23 


    canDataFrame[0] =  (int)(pInput->OTC_RecoveryTimeout);
    canDataFrame[1] = ((int)(pInput->OTC_RecoveryTimeout) >> 8);

    canDataFrame[2] =  (int)(pInput->OTC_WarningTemperature);
    canDataFrame[3] =  (int)(pInput->OTC_ErrorTemperature);
    canDataFrame[4] =  (int)(pInput->OTC_RecoveryTemperature);
    
    canDataFrame[5] =  (int)(pInput->OTD_WarningTemperature);
    canDataFrame[6] =  (int)(pInput->OTD_ErrorTemperature);
    canDataFrame[7] =  (int)(pInput->OTD_RecoveryTemperature);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_023, canDataFrame);
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //id 24 

    canDataFrame[0] =  (int)(pInput->UTC_WarningTemperature);
    canDataFrame[1] =  (int)(pInput->UTC_ErrorTemperature);
    canDataFrame[2] =  (int)(pInput->UTC_RecoveryTemperature);

    canDataFrame[3] =  (int)(pInput->UTD_WarningTemperature);
    canDataFrame[4] =  (int)(pInput->UTD_ErrorTemperature);
    canDataFrame[5] =  (int)(pInput->UTD_RecoveryTemperature);

    canDataFrame[6] =  (int)(pInput->OV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[7] = ((int)(pInput->OV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_024, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    delayMilliSeconds(CAN_MESSAGE_DELAY); 


    //id 25 
    canDataFrame[0] =  (int)(pInput->OV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[1] = ((int)(pInput->OV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[2] =  (int)(pInput->OV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[3] = ((int)(pInput->OV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[4] =  (int)(pInput->UV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[5] = ((int)(pInput->UV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[6] =  (int)(pInput->UV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[7] = ((int)(pInput->UV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);


    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_025, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

    canDataFrame[0] =  (int)(pInput->UV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[1] = ((int)(pInput->UV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);


    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_026, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    delayMilliSeconds(CAN_MESSAGE_DELAY); 
#endif
    return status;
}

#endif

/* End of File ---------------------------------------------------------------*/