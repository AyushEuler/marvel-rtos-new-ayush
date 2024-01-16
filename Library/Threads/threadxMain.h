/**
 * @file threadxMain.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef THREADXMAIN_H_
#define THREADXMAIN_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "tx_api.h"
#include "bmsConfiguration.h"
#include "dataProcessing.h"

#include "CellBalancing.h"
#include "Contactors.h"
#include "SOCEstimation.h"
#include "DataPipeline.h"
#include "EKF.h"
#include "Protection.h"
#include "HighLevelStatemachine.h"
#include "ChargingAlgorithm.h"
#include "lookupTable.h"
#include "batteryCooling.h"


/* Private define ------------------------------------------------------------*/

//dcli, dclo
#define DOD_FACTOR      0.9F

#define MIN(x,y) (((x) < (y)) ? (x) : (y) )
#define MAX(x,y) (((x) > (y)) ? (x) : (y) )

/* ------------------------------- Tasks size and Priority  -------------------------*/
#define sizeBytePool_mainBytePool           65536
#define sizeStack_collectBatteryData        8192
#define sizeStack_protection                8192
#define sizeStack_contactorControl          4096
#define sizeStack_coulombCounting           4096
#define sizeStack_ccEKF                     4096
#define sizeStack_stateMachine              4096
#define sizeStack_canHandler                2048
#define sizeStack_canTxHandler              sizeStack_canHandler
#define sizeStack_canRxHandler              sizeStack_canHandler

#define priority_P0                     0
#define priority_P1                     2
#define priority_P2                     4
#define priority_P3                     8
#define priority_P4                     16
#define priority_P5                     32

#define FIRMWARE_UPGRADE_STACK_SIZE (uint32_t)8196
#define FIRMWARE_UPGRADE_PRIORITY   (uint32_t)priority_P1
#define TIME_SLICE_DEFAULT          (uint32_t)100 


#define QUEUE_SEND(queue, data, wait)       tx_queue_send(queue, data, wait)
#define QUEUE_RECEIVE(queue, data, wait)    tx_queue_receive(queue, data, wait)
// #define SUCCESS                             TX_SUCCESS
#define WAIT_FOREVER                        TX_WAIT_FOREVER
#define NO_WAIT                             TX_NO_WAIT
#define TRUE            1
#define FALSE           0
/* ------------------------------- Loop delays and variables  -------------------------*/

#define LOOP_DELAY_100MS                     100
#define LOOP_DELAY_250MS                     250
#define LOOP_DELAY_1000MS                    1000
#define LOOP_DELAY_CMU_DATA_COLLECTION       LOOP_DELAY_100MS
#define LOOP_DELAY_CONTACTOR_CONTROL         LOOP_DELAY_100MS
#define LOOP_DELAY_PROTECTION                LOOP_DELAY_100MS
#define LOOP_DELAY_STATE_MACHINE             LOOP_DELAY_100MS

// Define the maximum number of CAN messages that the queue can hold
#define CAN_QUEUE_LENGTH               20
#define CAN_TX_QUEUE_LENGTH            CAN_QUEUE_LENGTH
#define CAN_RX_QUEUE_LENGTH            CAN_QUEUE_LENGTH

// Define the size of each CAN message in the queue
#define CAN_QUEUE_MESSAGE_SIZE        sizeof(canQueueData_t)
#define CAN_TX_QUEUE_MESSAGE_SIZE     CAN_QUEUE_MESSAGE_SIZE
#define CAN_RX_QUEUE_MESSAGE_SIZE     CAN_QUEUE_MESSAGE_SIZE

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern TX_QUEUE canTxQueue;
extern TX_QUEUE canRxQueue;
extern TX_EVENT_FLAGS_GROUP system_event_flag;


// enum vehicleState
// {
//     fastCharge = 0, 
//     slowCharge, 
//     isChargerConnected,
//     ignition, 
//     idle,
//     permanentFail, 
//     serviceMode,
//     transmissionFail,
//     VCUPermanentFail,
//     balanceEn_atRest
// };
// Define a struct to hold the CAN message information
typedef struct canQueueData
{
    uint8_t data[8];
    uint32_t identifier;
} canQueueData_t;

typedef struct {
  uint32_T SOC_forCVCharge_cpct;
  uint32_T BatteryRevivalVoltage_mV;
  uint32_T NoCharge_K;
  uint32_T BatteryRevival_K;
  uint32_T CC_K;
  uint32_T CC_Balancing_K;
  uint32_T CV_K;
  uint32_T FastCharging_K;
  uint32_T CVTransitionVoltage_mV;
  uint32_T TransitionTimeout_msec;
  uint32_T ChargersCurrentLimit_A;
} ChargeAlgorithm_t;

//temperature
typedef struct temperature
{
    int32_T OTC_WarningTemperature;
    int32_T OTC_ErrorTemperature;
    int32_T OTC_RecoveryTemperature;
    int32_T OTC_NoErrorTimeout_msec;
    int32_T OTC_WarningTimeout;
    int32_T OTC_ErrorTimeout;
    int32_T OTC_RecoveryTimeout;
    
    int32_T OTD_WarningTemperature;
    int32_T OTD_ErrorTemperature;
    int32_T OTD_RecoveryTemperature;
    int32_T OTD_NoErrorTimeout_msec;
    int32_T OTD_WarningTimeout;
    int32_T OTD_ErrorTimeout;
    int32_T OTD_RecoveryTimeout;

    int32_T UTC_WarningTemperature;
    int32_T UTC_ErrorTemperature;
    int32_T UTC_RecoveryTemperature;
    int32_T UTC_NoErrorTimeout_msec;
    int32_T UTC_WarningTimeout;
    int32_T UTC_ErrorTimeout;
    int32_T UTC_RecoveryTimeout;

    int32_T UTD_WarningTemperature;
    int32_T UTD_ErrorTemperature;
    int32_T UTD_RecoveryTemperature;
    int32_T UTD_NoErrorTimeout_msec;
    int32_T UTD_WarningTimeout;
    int32_T UTD_ErrorTimeout;
    int32_T UTD_RecoveryTimeout;
}temperatureData_t;

//threshold
typedef struct thresholdData
{
    //protection
    int32_T MaxOperatingTemp_C;
    int32_T MaxAllowedTempChange_C;
    int32_T MaxAllowedTempGradientGroup1_C;
    int32_T MaxAllowedTempGradientGroup2_C;
 
    int32_T chargeDetectionThreshold_mA;
    int32_T NormalModeTimeout_msec;
    int32_T PowerSaverModeTimeout_msec;
    int32_T FaultsModeTimeout_msec;
    int32_T ServiceModeTimeout_msec;

    int32_T i2tTimeout_msec;
    int32_T dynamicContTimeout_msec;
    int32_T dynamicCurrentThresholdsGain;
    int32_T TempGroupTimeout;
    uint8_T PermanentFailCounts_eFuse;
    
    int32_T i2t_CalculationTimeout_msec;
    int32_T CurrentChangeTimeout_msec;
    int32_T eFuseMeltingTimeout_msec;
    
    int32_T OCC_WarningCurrent;
    int32_T OCC_ErrorCurrent;
    int32_T RecoveryProtections_OCC_Warning_mA;
    int32_T RecoveryProtections_OCC_Error_mA;
    int32_T OCC_NoErrorTimeout_msec; //
    int32_T OCC_WarningTimeout;
    int32_T OCC_ErrorTimeout;
    int32_T OCC_RecoveryTimeout_msec; //

    int32_T OCD_WarningCurrent;
    int32_T OCD_ErrorCurrent;
    int32_T RecoveryProtections_OCD_Warning_mA;
    int32_T RecoveryProtections_OCD_Error_mA;
    int32_T OCD_NoErrorTimeout_msec; //
    int32_T OCD_WarningTimeout;
    int32_T OCD_ErrorTimeout;
    int32_T OCD_RecoveryTimeout_msec;

    temperatureData_t temperatureData[2];

    int32_T slowCharging_OV_WarningVoltage;
    int32_T slowCharging_OV_ErrorVoltage;
    int32_T slowCharging_OV_RecoveryVoltage;

    int32_T fastCharging_OV_WarningVoltage;
    int32_T fastCharging_OV_ErrorVoltage;
    int32_T fastCharging_OV_RecoveryVoltage;


    int32_T OV_NoErrorTimeout_msec;
    int32_T OV_WarningTimeout;
    int32_T OV_ErrorTimeout;
    int32_T OV_RecoveryTimeout;

    int32_T HighImbalanceError_mV;

    int32_T UV_WarningVoltage;
    int32_T UV_ErrorVoltage;
    int32_T UV_RecoveryVoltage;
    int32_T UV_NoErrorTimeout_msec;
    int32_T UV_WarningTimeout;
    int32_T UV_ErrorTimeout;
    int32_T UV_RecoveryTimeout;

    int32_T MinOpearatingVolt_mV;
    int32_T ShortCircuitCurrent_mA;
    int32_T SuddenVoltageDrop_mV_by_time;

    uint16_T ThermalRunawayWarningTimeout_msec;
    uint16_T ThermalRunawayErrorTimeout_msec;
    uint16_T TemperatureGradientTimeout_msec;
    uint16_T HighImbalanceFlagTimeout_msec;
    uint16_T ShortCircuitDetect_msec;
    uint16_T SuddenVoltageDropTimeout_msec;

    //balancing
    int32_T balancingMinThreshold;  //voltage at which balancing should start
    int32_T balancingStateRestPeroid;
    int32_T balancingMaxAllowedImbalance;
    // int32_T balancingOVProtectionThreshold;
    int32_T balancing_SlowCh_OVThr_mV;
    int32_T balancing_FastCh_OVThr_mV;

    int32_T balancingPeriod;

    int32_T balancing_DefaultBalancingTimeOn_msec;
    int8_T  balancingCurrentDerateActivationTemp_C;
    int32_T balancingCurrentDerateCheckTime_msec;

    int32_T ExternalShortCheckTime_msec;
    int8_T  ExternalShortLimit;
    int8_T  ContWeldCheckLimit;

    //contactor
    int32_T contactorCutoffTime;
    int32_T preChargeContactorCurrent; //ContCurrent_mA
    int32_T ContWeldCheckCurrent_mA;
    uint8_T ContactorWarningCounterLimit;
    int32_T ContactorWeldTimeout_msec;
    int32_T OpenToPreChargeStateTime_msec;
    int32_T Min_PreChargeTime_msec;
    int32_T Max_PreChargeTime_msec;
    int32_T ContOpenCurrent_mA;
    int32_T FlagDebounceTime_msec;
    int32_T Min_ClosedToOpeningTime_msec;
    int32_T Max_ClosedToOpeningTime_msec;

    uint8_T  AdditionalProtectionEn;
    uint8_T  HighImbalanceFlagEn;

    int32_T BMS_Timeout_msec;
    int32_T ContactorsCheck_Timeout_msec;
    int32_T VCU_Timeout_msec;
    int32_T MosfetsPreChargeCurrentLimit_mA;

    ChargeAlgorithm_t ChargeAlgorithm;
}thresholdData_t;


//temperature sense
typedef struct {
    int32_T temperature[7];
    int32_T temperatureMin;
    int32_T temperatureMax;
    int32_T temperatureDelta;
} temperatureSense_t;

//Voltage Sense
typedef struct {
    int32_T voltage[CELL_IN_SERIES];
    int32_T voltageMin;
    int32_T voltageMax;
    int32_T voltageDelta;
} voltageSense_t;

//temperature state
typedef struct {
    uint8_T TempOverallState;
    uint8_T TempGroup1;
    uint8_T TempGroup2;

    uint8_T ErrorTypeGroup1;
    uint8_T ErrorTypeGroup2;

} temperatureState_t;

typedef struct {
    int8_T SlowCharge;
    int8_T FastCharge;
    int8_T isChargerConnected;
    int8_T TransmissionFail;
    int8_T VCUPermanentFail;
    int8_T ServiceMode;
    int8_T BalanceEn_atRest;
    int8_T EmergencyDisconnectEn;
    uint32_T ChargersCurrentLimit_A;
} vcuData_t;

typedef struct DynamicLimitsTable {
    Table2d ContinuousChargingTable;
    Table2d ContinuousDischargingTable;
    Table2d instantaneousChargingTableData;
    Table2d instantaneousDischargingTableData;
    Table2d eFuseChargingTable;
    Table2d eFuseDishargingTable;
}DynamicLimitsTable_t;

//data pipeline
typedef struct dataPipelineInput_t
{
    int16_T temperature1_C[7];
    int16_T temperature2_C[7];
    int32_T voltage[CELL_IN_SERIES];
    int32_T current;
    thresholdData_t thresholdDataBus;
    int32_T soc;
    uint8_T isDynamicCurrentEn;
    uint8_T nCells_series;
    uint8_T nCells_parallel;
    uint8_T TemperatureSelectionforDynamicC;
    uint8_T nCelltemperatureGroups;
    int8_T SlowCharge;
    int8_T FastCharge;
    int8_T isChargerConnected;
    int8_T TransmissionFail;
    int8_T VCUPermanentFail;
    int8_T ServiceMode;
    int8_T BalanceEn_atRest;
    uint32_T LoopTimer_SOC_msec;
    int8_T EmergencyDisconnectEn;
    uint32_T ChargersCurrentLimit_A;
    int32_T shuntTemp;
}dataPipelineInput_t;

typedef struct dataPipelineOutput
{
    int32_T current;
    //voltage sense bus
    voltageSense_t voltageSense;
    int32_T maxTemperature_C;
    int32_T deltaTemperature_C;
    int32_T minTemperature_C;
    //TemperatureSense
    int32_T DeltaTemperatureGroup1_C;
    int32_T DeltaTemperatureGroup2_C;
    temperatureSense_t temperatureSense[2];
    uint8_T isChargeEn;
    uint8_T isDynamicCurrentEn;
    uint8_T nCells_series;
    uint8_T nCells_parallel;

    int32_T DCL_ChargingCurrent_A;
    int32_T DCL_DisChargingCurrent_A;
    int32_T i2t_ChargingCurrent_A2sec;
    int32_T i2t_DisChargingCurrent_A2sec;
    uint32_T LoopTimer_SOC_msec;

    vcuData_t VCU;
}dataPipelineOutput_t;


//cell balancing
typedef struct cellBalancingInput
{
    int32_T BalancingTempGroup;
    float StLineEq_varA;
    float StLineEq_varB;

    thresholdData_t         thresholdDataBus;
    dataPipelineOutput_t    dataPipelineBus;
}cellBalancingInput_t;

typedef struct cellBalancingOutput
{
    uint8_T balancingCommand[CELL_IN_SERIES];
    int32_T volatgeImbalance;
    uint8_T BalanceEn;
    uint8_T balancingState;
}cellBalancingOutput_t;

//protection
typedef struct protectionOutputData
{
    uint8_T ThermalRunaway;
    uint8_T TemperatureGradient;
    uint8_T HighImbalanceFlag;
    uint8_T ShortCircuit;
    uint8_T SuddenVoltageDrop;
    uint8_T OCC;         //over current during charge
    uint8_T OCD;         //over current during discharge
    uint8_T OV;          //over voltage
    uint8_T UV;          //under voltage
    float   i2t_Calculated_A2sec;
    uint8_T eFuseChargeFlag;
    uint8_T eFuseDischargeFlag;
    uint8_T TempState1;
    uint8_T TempState2;
    uint8_T FlagGroup1;
    uint8_T FlagGroup2;
    uint8_T TempOverallState;
    uint8_T ErrorDetect;
}protectionOutputData_t;

typedef struct protectionInput
{
    thresholdData_t         thresholdDataBus;
    dataPipelineOutput_t    dataPipelineBus;
}protectionInput_t;

typedef struct protectionOutput
{
    protectionOutputData_t protectionOutputData;
}protectionOutput_t;

//contactor
typedef struct contactorInput
{
    //data pipeline
    thresholdData_t         thresholdDataBus;
    uint8_T                 PermanentFailRecoveryTrigger;
    protectionOutputData_t  protectionOutputData;
    dataPipelineOutput_t    dataPipelineBus;

    uint8_T communicationFlag;
    uint8_T contactorCommand;
    uint8_T shortCircuitFlag;
    uint8_T permanentFailFlag;
    uint8_T contactorErrorFlag;

    uint8_T positiveContactorFeedback;
    uint8_T preChargeContactorFeedback;
    uint8_T negativeContactorFeedback;
}contactorInput_t;

typedef struct contactorOutput
{
    uint8_T negativeContactorState;
    uint8_T preChargeContactorState;
    uint8_T positiveContactorState;
    uint8_T contactorWeldWarning;
    uint8_T permanentFaiil;
    uint8_T contactorsState;
    uint8_T ExternalShortCheck;
    uint32_T Elapsed_TimeToOpen_msec;
}contactorOutput_t;

#ifdef EKF
//ccEKF
typedef struct ccEKFInput
{
    float current;
    float time_s;
    float vehicleRestTime;
    float vehicleRestCurrent;
    float protectionOutputOVThreshold;
    float protectionOutputUVThreshold;
    float maxPackCapacity;   //Max_CellCapacity_Ah
    float minPackCapacity;
    float avgCellVoltage;
}ccEKFInput_t;

typedef struct ccEKFOutput
{
    float initialCapacity;
    float totalCapacityRemaining;
    float totalCapacityDischarge;
    float socCC;
    float coulombCountingState;

    float capacityEKF;
    float voltageEKF;
    float terminalVoltage;
    float socEKF;
}ccEKFOutput_t;

#else

//coulomb counting

typedef struct coulombCountingInputBus {
    int32_t InstalledCapacity_mAh;
    int32_t EquivalentCycleCapacity_mAh;
    int32_t SensorOffset_mA;
    int8_t SensorAccuracy_pct;
    int32_t BleedingCurrent_mA;
    int32_t CCTimeout_msec;
    int32_t DebouncingTimeout_msec;
    int32_t MinVoltageLimit_mV;
    int32_t MaxVoltageLimit_mV;
    int32_t MaxCurrentLimit_mA;
    int32_t MinCurrentLimit_mA;
    int32_t MaxAllowedImb_mV;
    int32_t CapacityDeltaCheck_mAh;
    int8_t InitialCapacity_Guess;
    uint32_T SOH_Vs_Capacity_Gain;
    uint32_T SOH_Vs_Capacity_Offset;
    uint8_t IntegratorLoopTime;
    uint8_t LoopTimeSelector;
    uint8_t SOHCalibrationOn;
    uint16_t SOHCalibrationTimeout_msec;
} coulombCountingInputBus;

typedef struct coulombCountingInput
{
    int32_t current_mA;
    int32_t voltage_mV;
    coulombCountingInputBus cc_Inputs;
    dataPipelineOutput_t dataPipelineBus;
    cellBalancingOutput_t CellBalancingOutput;
    float LoopTimer_SOC_msec;
    uint8_T EEPROM_FirstCycle;
}coulombCountingInput_t;

typedef struct coulombCountingOutput
{
    int32_T initialCapacity;
    int32_T totalCapacityRemaining;
    int32_T totalCapacityDischarge;
    uint32_T SOC_pct;
    uint32_T coulombCountingState;
    uint32_T MaxUsableCapacity_mAh;
    uint32_T TotalCapacityExchange_mAh;
    uint32_T SOH_pct;
    uint32_T CycleCount;
    uint32_T SOC_FromFlash;
}coulombCountingOutput_t;

#endif


typedef struct stateMachineInput
{
    uint8_T HighLevelStateMode;
}stateMachineInput_t;

typedef struct stateMachineOutput
{
    uint8_T HighLevelStateMode;
}stateMachineOutput_t;

typedef struct chargingOutput
{
    int32_T ChargingCurrentReq_mA;       /* '<Root>/ChargingCurrentReq_mA' */
    uint8_t ChargingState; 
}chargingOutputData_t;

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

void entry_canTxHandler(ULONG threadInput);
void entry_canRxHandler(ULONG threadInput);

void entry_collectBatteryData(ULONG threadInput);
void entry_protection(ULONG threadInput);
void entry_contactorControl(ULONG threadInput);
void entry_coulombCounting(ULONG threadInput);
void entry_stateMachine(ULONG threadInput);
void entry_ccEKF(ULONG threadInput);
void firmware_upgrade_task_entry(ULONG thread_input);

#ifdef EKF
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, \
            protectionInput_t *pInput, contactorInput_t *cInput, ccEKFInput_t *ccEKFInput);

#else
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, \
            protectionInput_t *pInput, contactorInput_t *cInput, coulombCountingInput_t *ccInput);

#endif
bmsStatus_t dataPipelineInit(DynamicLimitsTable_t *ldata, dataPipelineInput_t *dpInput, ExtU_DataPipeline_T *dpInputMBD);
bmsStatus_t cellBalancingInit(thresholdData_t *tData, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD, dataPipelineInput_t *dpInput);
bmsStatus_t contactorInit(thresholdData_t *tData, contactorInput_t *cInput, ExtU_Contactors_T *cInputMBD);
bmsStatus_t protectionInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD);

#ifdef EKF
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, ccEKFOutput_t *ccEKFOutput, ExtU_DataPipeline_T *dpInputMBD);
#else
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, coulombCountingOutput_t *ccOutput, ExtU_DataPipeline_T *dpInputMBD);
#endif
bmsStatus_t dataPipelineOutput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, ExtY_DataPipeline_T *dpOutputMBD);

bmsStatus_t cellBalancingInput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD);
bmsStatus_t cellBalancingOutput(bmsMainData_t *mdata, cellBalancingOutput_t *cbOutput, ExtY_CellBalancing_T *cbOutputMBD);

bmsStatus_t protectionInput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD);
bmsStatus_t protectionOutput (bmsMainData_t *mdata, protectionOutput_t *pOutput, ExtY_Protection_T *pOutputMBD);

bmsStatus_t contactorInput (bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorInput_t *cInput, contactorOutput_t *cOutput, ExtU_Contactors_T *cInputMBD);
bmsStatus_t contactorOutput (bmsMainData_t *mdata, contactorOutput_t *cOutput, ExtY_Contactors_T *cOutputMBD);

#ifdef EKF
bmsStatus_t coulombCountingEKFInit(ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD);
bmsStatus_t coulombCountingEKFInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD);
bmsStatus_t coulombCountingEKFOutput(bmsMainData_t *mdata, ccEKFOutput_t *ccEKFOutput, ExtY_EKF_T *ccEKFOutputMBD);
#else
bmsStatus_t coulombCountingInit(coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD, coulombCountingOutput_t *ccOutputData);
bmsStatus_t coulombCountingInput (bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD);
bmsStatus_t coulombCountingOutput(bmsMainData_t *mdata, coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFLASH, ExtY_SOCEstimation_T *ccOutputMBD);
#endif


bmsStatus_t stateMachineInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, ExtU_HighLevelStatemachine_T *smInputMBD);
bmsStatus_t stateMachineInput(dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, ExtU_HighLevelStatemachine_T *smInputMBD);
bmsStatus_t stateMachineOutput(stateMachineOutput_t *smOutput, ExtY_HighLevelStatemachine_T *smOutputMBD);

bmsStatus_t chargingInit(thresholdData_t *tData, ExtU_ChargingAlgorithm_T *chargingInputMBD);
bmsStatus_t chargingInput(ExtU_DataPipeline_T *dpInputMBD, ExtY_DataPipeline_T *dpOutputMBD, ExtU_ChargingAlgorithm_T *chargingInputMBD);
bmsStatus_t chargingOutput(chargingOutputData_t *chargerOutput, ExtY_ChargingAlgorithm_T *chargingOutputMBD);

void writeSoCToFlash(coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFlash);
bmsStatus_t writeCCDataToBuffer(coulombCountingOutput_t *ccOutput, uint8_t *writeBufferEEPROM, uint64_t *writeBufferFLASH);
bmsStatus_t readCCDataFromBuffer(coulombCountingOutput_t *ccOutput, uint8_t *readBufferEEPROM, uint64_t *readBufferFLASH);
float calculateChargingCurrent(dataPipelineOutput_t *dpOutput, coulombCountingOutput_t *ccOutput);
bmsStatus_t calculateDynamicCurrentLimits(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput, DynamicLimitsTable_t *ldata, ExtY_DataPipeline_T *dpOutputMBD);

#ifdef DEBUG_CAN
    #ifdef EKF
        bmsStatus_t sendCoulombCountingEKFDataOnCAN(ccEKFInput_t *ccEKFInput, ccEKFOutput_t *ccEKFOutput);
    #else
        bmsStatus_t sendCoulombCountingDataOnCAN(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput);
    #endif
bmsStatus_t sendContactorDataOnCAN(thresholdData_t *tData, contactorInput_t *cInput, contactorOutput_t *cOutput);
bmsStatus_t sendCellBalancingDataOnCAN(thresholdData_t *tData, cellBalancingInput_t *cbInput, cellBalancingOutput_t *cbOutput);
bmsStatus_t sendProtectionDataOnCAN(thresholdData_t *tData, protectionInput_t *pInput, protectionOutput_t *pOutput);
bmsStatus_t sendBMSDataOnCAN(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, dataPipelineInput_t *dpInput);
float SoCManipulation(bmsMainData_t *mdata, ExtY_SOCEstimation_T *ccOutputMBD);
uint8_t storeErrorCodes(protectionOutput_t *pOutput);


#endif


#endif
/* End of File ---------------------------------------------------------------*/

