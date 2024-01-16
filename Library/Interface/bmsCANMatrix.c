/**
 * @file bmsCANMatrix.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "onBoardIOs.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "bmsCANMatrix.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/
CANdataStd       CAN_flagsPreChargeActive;
CANdataStd       CAN_flagsBalancingActive;
CANdataStd       CAN_flagsFullCharge;

CANdataStd       CAN_balancingLimit;
CANdataStd       CAN_packCurrent;
CANdataStd       CAN_stateOfCharge;
CANdataStd       CAN_stateOfHealth;
CANdataStd       CAN_bmsState;
CANdataStd       CAN_packVoltage;
CANdataStd       CAN_packCapacity;
CANdataStd       CAN_dynamicLimitIN;
CANdataStd       CAN_dynamicLimitOUT;
CANdataStd       CAN_InstantaneousLimitIN;
CANdataStd       CAN_InstantaneousLimitOUT;
CANdataStd       CAN_FirmwareVersion;
CANdataStd       CAN_ConfigID;
CANdataStd       CAN_packCapacityDesign;
CANdataStd       CAN_packCapacityFull;
CANdataStd       CAN_balancingBitMask_CMU1;
CANdataStd       CAN_balancingBitMask_CMU2;
CANdataStd       CAN_timeBaseResetCounter;
CANdataStd       CAN_mcuResetCounter;

CANdataStd       CAN_2949CRCCounter;
CANdataStd       CAN_6815CRCCounter;
#ifdef CAN_MATRIX_V_0_3
CANdataStd       CAN_maxAllowedCellVoltage;
CANdataStd       CAN_maxAllowedPackVoltage;
CANdataStd       CAN_maxAllowedCellTemp;
CANdataStd       CAN_maxAllowedCurrent;
#endif

CANdataStd       CAN_cmu1Temp1;
CANdataStd       CAN_cmu1Temp2;
CANdataStd       CAN_cmu1Temp3;
CANdataStd       CAN_cmu1Temp4;
CANdataStd       CAN_cmu2Temp1;
CANdataStd       CAN_cmu2Temp2;
CANdataStd       CAN_cmu2Temp3;
CANdataStd       CAN_cmu2Temp4;
CANdataStd       CAN_cmu3Temp1;
CANdataStd       CAN_cmu3Temp2;
CANdataStd       CAN_cmu3Temp3;
CANdataStd       CAN_cmu3Temp4;
CANdataStd       CAN_cmu4Temp1;
CANdataStd       CAN_cmu4Temp2;
CANdataStd       CAN_cmu4Temp3;
CANdataStd       CAN_cmu4Temp4;

CANdataStd       CAN_cmu1Cell1;
CANdataStd       CAN_cmu1Cell2;
CANdataStd       CAN_cmu1Cell3;
CANdataStd       CAN_cmu1Cell4;
CANdataStd       CAN_cmu1Cell5;
CANdataStd       CAN_cmu1Cell6;
CANdataStd       CAN_cmu1Cell7;
CANdataStd       CAN_cmu1Cell8;
CANdataStd       CAN_cmu1Cell9;
CANdataStd       CAN_cmu1Cell10;
CANdataStd       CAN_cmu1Cell11;
CANdataStd       CAN_cmu1Cell12;
CANdataStd       CAN_cmu2Cell1;
CANdataStd       CAN_cmu2Cell2;
CANdataStd       CAN_cmu2Cell3;
CANdataStd       CAN_cmu2Cell4;
CANdataStd       CAN_cmu2Cell5;
CANdataStd       CAN_cmu2Cell6;
CANdataStd       CAN_cmu2Cell7;
CANdataStd       CAN_cmu2Cell8;
CANdataStd       CAN_cmu2Cell9;
CANdataStd       CAN_cmu2Cell10;
CANdataStd       CAN_cmu2Cell11;
CANdataStd       CAN_cmu2Cell12;
CANdataStd       CAN_cmu3Cell1;
CANdataStd       CAN_cmu3Cell2;
CANdataStd       CAN_cmu3Cell3;
CANdataStd       CAN_cmu3Cell4;
CANdataStd       CAN_cmu3Cell5;
CANdataStd       CAN_cmu3Cell6;
CANdataStd       CAN_cmu3Cell7;
CANdataStd       CAN_cmu3Cell8;
CANdataStd       CAN_cmu3Cell9;
CANdataStd       CAN_cmu3Cell10;
CANdataStd       CAN_cmu3Cell11;
CANdataStd       CAN_cmu3Cell12;
CANdataStd       CAN_cmu4Cell1;
CANdataStd       CAN_cmu4Cell2;
CANdataStd       CAN_cmu4Cell3;
CANdataStd       CAN_cmu4Cell4;
CANdataStd       CAN_cmu4Cell5;
CANdataStd       CAN_cmu4Cell6;
CANdataStd       CAN_cmu4Cell7;
CANdataStd       CAN_cmu4Cell8;
CANdataStd       CAN_cmu4Cell9;
CANdataStd       CAN_cmu4Cell10;
CANdataStd       CAN_cmu4Cell11;
CANdataStd       CAN_cmu4Cell12;



/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

#ifdef DEBUG_CAN

//c out
CANdataStd      CAN_cOut_NegContactor;
CANdataStd      CAN_cOut_PosContactor;
CANdataStd      CAN_cOut_PreChargeContactor;


// cc in
CANdataStd       CAN_ccIn_VehicleRest_Time;
CANdataStd       CAN_ccIn_Max_PackCapacity;
CANdataStd       CAN_ccIn_VehicleRest_DisCharge_Current;
CANdataStd       CAN_ccIn_VehicleRest_Charge_Current;


CANdataStd       CAN_ccIn_pOut_OVThreshold;
CANdataStd       CAN_ccIn_Voltage;
CANdataStd       CAN_ccIn_Current;

//cc out
// CANdataStd       CAN_ccOut_Coulomb_Counting_State;
// CANdataStd       CAN_ccOut_SOC_Pct; 
// CANdataStd       CAN_ccOut_Initial_Capacity_Ah; 
// CANdataStd       CAN_ccOut_Total_Capacity_Discharged_Ah;
// CANdataStd       CAN_ccOut_Total_Capacity_Remaining_Ah; 

CANdataStd       CAN_ccOut_EKF_SOC_Pct;
CANdataStd       CAN_ccOut_Capacity_EKF;
CANdataStd       CAN_ccOut_Voltage_EKF;
CANdataStd       CAN_ccOut_Terminal_Voltage;

CANdataStd       CAN_ccOut_initialCapacity;
CANdataStd       CAN_ccOut_totalCapacityRemaining;
CANdataStd       CAN_ccOut_totalCapacityDischarge;
CANdataStd       CAN_ccOut_SOCPercentage;
CANdataStd       CAN_ccOut_coulombCountingState;
CANdataStd       CAN_ccOut_MaxUsableCapacity_mAh;
CANdataStd       CAN_ccOut_TotalCapacityExchange_mAh;
CANdataStd       CAN_ccOut_SOH_cpct;

//c in
CANdataStd      CAN_cIn_NegContactor_FB;
CANdataStd      CAN_cIn_PosContactor_FB;
CANdataStd      CAN_cIn_PreChargeContactor_FB;


CANdataStd      CAN_cIn_MaxImbalance;

CANdataStd      CAN_cIn_ThermalRunaway;
CANdataStd      CAN_cIn_OCC;
CANdataStd      CAN_cIn_OCD;
CANdataStd      CAN_cIn_OTC;
CANdataStd      CAN_cIn_OTD;
CANdataStd      CAN_cIn_UTC;
CANdataStd      CAN_cIn_UTD;
CANdataStd      CAN_cIn_OV;
CANdataStd      CAN_cIn_UV;

CANdataStd      CAN_cIn_contactorCutoffTime;
CANdataStd      CAN_cIn_preChargeContactorCurrent;
CANdataStd      CAN_cIn_contactorResponseTime_msec;

CANdataStd      CAN_cIn_contactorCommand;
CANdataStd      CAN_cIn_communicationFlag;
CANdataStd      CAN_cIn_shortCircuitFlag;
CANdataStd      CAN_cIn_permanentFailFlag;
CANdataStd      CAN_cIn_contactorErrorFlag;

//contactor out
CANdataStd      CAN_cOut_NegContactorState;
CANdataStd      CAN_cOut_PosContactorState;
CANdataStd      CAN_cOut_PreChargeContactorState;

//cb in

CANdataStd      CAN_cbIn_balancingMinThreshold;
CANdataStd      CAN_cbIn_balancingMaxAllowedImbalance;
CANdataStd      CAN_cbIn_balancingOVProtectionThreshold;
CANdataStd      CAN_cbIn_balancingStateRestPeroid;
CANdataStd      CAN_cbIn_cellsInSeries;

//cb out
CANdataStd      CAN_cbOut_balancingState;
// balancingCommand

//protection input
CANdataStd      CAN_pIn_voltage[20];
CANdataStd      CAN_pIn_voltageMin;
CANdataStd      CAN_pIn_voltageMax;
CANdataStd      CAN_pIn_voltageDelta;

CANdataStd      CAN_pIn_temperatureMin;
CANdataStd      CAN_pIn_temperatureMax;
CANdataStd      CAN_pIn_temperatureDelta;


uint16_t pInVoltageMessageIDIndex[5] = {CAN_DEBUG_ID_012, CAN_DEBUG_ID_013,\
                                        CAN_DEBUG_ID_014, CAN_DEBUG_ID_015,\
                                        CAN_DEBUG_ID_016 };


uint8_t pInVoltageStartByteIndex[20] = { 0, 2, 4, 6, \
                                         0, 2, 4, 6, \
                                         0, 2, 4, 6, \
                                         0, 2, 4, 6, \
                                         0, 2, 4, 6  };


CANdataStd      CAN_pIn_current;
CANdataStd      CAN_pIn_OCC_WarningCurrent;
CANdataStd      CAN_pIn_OCC_ErrorCurrent;

// CANdataStd      CAN_pIn_dynamicCurrentCont;
// CANdataStd      CAN_pIn_dynamicCurrentInst;

CANdataStd      CAN_pIn_eFuseChargingLimit_A2s;
CANdataStd      CAN_pIn_eFuseDischargingLimit_A2s;

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize CAN
 * @param canID can ID
 * @retval BMS status
 */
void initializeDebugCANMatrix(void)
{

    //protection in ID 30
    CAN_pIn_eFuseChargingLimit_A2s.messageID                   = CAN_DEBUG_ID_030;
    CAN_pIn_eFuseChargingLimit_A2s.messageDLC                  = 4;
    CAN_pIn_eFuseChargingLimit_A2s.messageStartByte            = 0;
    CAN_pIn_eFuseChargingLimit_A2s.messageScaling              = 1;

    CAN_pIn_eFuseDischargingLimit_A2s.messageID                   = CAN_DEBUG_ID_030;
    CAN_pIn_eFuseDischargingLimit_A2s.messageDLC                  = 4;
    CAN_pIn_eFuseDischargingLimit_A2s.messageStartByte            = 4;
    CAN_pIn_eFuseDischargingLimit_A2s.messageScaling              = 1;

    //protection in ID 21
    CAN_pIn_current.messageID                   = CAN_DEBUG_ID_021;
    CAN_pIn_current.messageDLC                  = 4;
    CAN_pIn_current.messageStartByte            = 0;
    CAN_pIn_current.messageScaling              = 0.1;

    CAN_pIn_OCC_WarningCurrent.messageID                   = CAN_DEBUG_ID_021;
    CAN_pIn_OCC_WarningCurrent.messageDLC                  = 2;
    CAN_pIn_OCC_WarningCurrent.messageStartByte            = 4;
    CAN_pIn_OCC_WarningCurrent.messageScaling              = 1000;

    CAN_pIn_OCC_ErrorCurrent.messageID                   = CAN_DEBUG_ID_021;
    CAN_pIn_OCC_ErrorCurrent.messageDLC                  = 2;
    CAN_pIn_OCC_ErrorCurrent.messageStartByte            = 6;
    CAN_pIn_OCC_ErrorCurrent.messageScaling              = 1000;

    //protection In - temp ID 18 to ID 20
    //protection In - ID 17
    CAN_pIn_voltageMax.messageID                     = CAN_DEBUG_ID_017;
    CAN_pIn_voltageMax.messageDLC                    = 2;
    CAN_pIn_voltageMax.messageStartByte              = 0;
    CAN_pIn_voltageMax.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_pIn_voltageMin.messageID                     = CAN_DEBUG_ID_017;
    CAN_pIn_voltageMin.messageDLC                    = 2;
    CAN_pIn_voltageMin.messageStartByte              = 2;
    CAN_pIn_voltageMin.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_pIn_voltageDelta.messageID                     = CAN_DEBUG_ID_017;
    CAN_pIn_voltageDelta.messageDLC                    = 2;
    CAN_pIn_voltageDelta.messageStartByte              = 4;
    CAN_pIn_voltageDelta.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    //protection In ID 12 to 16
    for(int i = 0; i < 20; i++)
    {
        CAN_pIn_voltage[i].messageID                     = pInVoltageMessageIDIndex[i/4];
        CAN_pIn_voltage[i].messageDLC                    = 2;
        CAN_pIn_voltage[i].messageStartByte              = pInVoltageStartByteIndex[i];
        CAN_pIn_voltage[i].messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;
    }

    //ID 10 and 11 is used in cell balancing
    //cell balancing In and out ID 9
    CAN_cbIn_cellsInSeries.messageID                     = CAN_DEBUG_ID_009;
    CAN_cbIn_cellsInSeries.messageDLC                    = 1;
    CAN_cbIn_cellsInSeries.messageStartByte              = 0;
    CAN_cbIn_cellsInSeries.messageScaling                = 1;

    CAN_cbOut_balancingState.messageID                     = CAN_DEBUG_ID_009;
    CAN_cbOut_balancingState.messageDLC                    = 1;
    CAN_cbOut_balancingState.messageStartByte              = 1;
    CAN_cbOut_balancingState.messageScaling                = 1;

    //cell balancing In ID 8
    CAN_cbIn_balancingMinThreshold.messageID                = CAN_DEBUG_ID_008;
    CAN_cbIn_balancingMinThreshold.messageDLC               = 2;
    CAN_cbIn_balancingMinThreshold.messageStartByte         = 0;
    CAN_cbIn_balancingMinThreshold.messageScaling           = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_cbIn_balancingMaxAllowedImbalance.messageID                     = CAN_DEBUG_ID_008;
    CAN_cbIn_balancingMaxAllowedImbalance.messageDLC                    = 2;
    CAN_cbIn_balancingMaxAllowedImbalance.messageStartByte              = 2;
    CAN_cbIn_balancingMaxAllowedImbalance.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_cbIn_balancingOVProtectionThreshold.messageID                     = CAN_DEBUG_ID_008;
    CAN_cbIn_balancingOVProtectionThreshold.messageDLC                    = 2;
    CAN_cbIn_balancingOVProtectionThreshold.messageStartByte              = 4;
    CAN_cbIn_balancingOVProtectionThreshold.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_cbIn_balancingStateRestPeroid.messageID                     = CAN_DEBUG_ID_008;
    CAN_cbIn_balancingStateRestPeroid.messageDLC                    = 2;
    CAN_cbIn_balancingStateRestPeroid.messageStartByte              = 6;
    CAN_cbIn_balancingStateRestPeroid.messageScaling                = 1;

    //contactor out ID 7
    CAN_cOut_NegContactorState.messageID                = CAN_DEBUG_ID_007;
    CAN_cOut_NegContactorState.messageDLC               = 1;
    CAN_cOut_NegContactorState.messageStartByte         = 0;
    CAN_cOut_NegContactorState.messageScaling           = 1;

    CAN_cOut_PosContactorState.messageID                = CAN_DEBUG_ID_007;
    CAN_cOut_PosContactorState.messageDLC               = 1;
    CAN_cOut_PosContactorState.messageStartByte         = 1;
    CAN_cOut_PosContactorState.messageScaling           = 1;

    CAN_cOut_PreChargeContactorState.messageID                = CAN_DEBUG_ID_007;
    CAN_cOut_PreChargeContactorState.messageDLC               = 1;
    CAN_cOut_PreChargeContactorState.messageStartByte         = 2;
    CAN_cOut_PreChargeContactorState.messageScaling           = 1;  

    //contactor in ID 6
    CAN_cIn_NegContactor_FB.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_NegContactor_FB.messageDLC               = 1;
    CAN_cIn_NegContactor_FB.messageStartByte         = 0;
    CAN_cIn_NegContactor_FB.messageScaling           = 1;

    CAN_cIn_PosContactor_FB.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_PosContactor_FB.messageDLC               = 1;
    CAN_cIn_PosContactor_FB.messageStartByte         = 1;
    CAN_cIn_PosContactor_FB.messageScaling           = 1;

    CAN_cIn_PreChargeContactor_FB.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_PreChargeContactor_FB.messageDLC               = 1;
    CAN_cIn_PreChargeContactor_FB.messageStartByte         = 2;
    CAN_cIn_PreChargeContactor_FB.messageScaling           = 1;

    CAN_cIn_contactorCommand.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_contactorCommand.messageDLC               = 1;
    CAN_cIn_contactorCommand.messageStartByte         = 3;
    CAN_cIn_contactorCommand.messageScaling           = 1;

    CAN_cIn_contactorResponseTime_msec.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_contactorResponseTime_msec.messageDLC               = 2;
    CAN_cIn_contactorResponseTime_msec.messageStartByte         = 4;
    CAN_cIn_contactorResponseTime_msec.messageScaling           = 1;

    CAN_cIn_contactorCutoffTime.messageID                = CAN_DEBUG_ID_006;
    CAN_cIn_contactorCutoffTime.messageDLC               = 2;
    CAN_cIn_contactorCutoffTime.messageStartByte         = 6;
    CAN_cIn_contactorCutoffTime.messageScaling           = 1;

    //contactor in ID 5
    CAN_cIn_ThermalRunaway.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_ThermalRunaway.messageDLC               = 1;
    CAN_cIn_ThermalRunaway.messageStartByte         = 0;
    CAN_cIn_ThermalRunaway.messageScaling           = 1;

    CAN_cIn_communicationFlag.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_communicationFlag.messageDLC               = 1;
    CAN_cIn_communicationFlag.messageStartByte         = 1;
    CAN_cIn_communicationFlag.messageScaling           = 1;

    CAN_cIn_shortCircuitFlag.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_shortCircuitFlag.messageDLC               = 1;
    CAN_cIn_shortCircuitFlag.messageStartByte         = 2;
    CAN_cIn_shortCircuitFlag.messageScaling           = 1;

    CAN_cIn_permanentFailFlag.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_permanentFailFlag.messageDLC               = 1;
    CAN_cIn_permanentFailFlag.messageStartByte         = 3;
    CAN_cIn_permanentFailFlag.messageScaling           = 1;

    CAN_cIn_contactorErrorFlag.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_contactorErrorFlag.messageDLC               = 1;
    CAN_cIn_contactorErrorFlag.messageStartByte         = 4;
    CAN_cIn_contactorErrorFlag.messageScaling           = 1;

    CAN_cIn_preChargeContactorCurrent.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_preChargeContactorCurrent.messageDLC               = 1;
    CAN_cIn_preChargeContactorCurrent.messageStartByte         = 5;
    CAN_cIn_preChargeContactorCurrent.messageScaling           = 1;

    CAN_cIn_MaxImbalance.messageID                = CAN_DEBUG_ID_005;
    CAN_cIn_MaxImbalance.messageDLC               = 2;
    CAN_cIn_MaxImbalance.messageStartByte         = 6;
    CAN_cIn_MaxImbalance.messageScaling           = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    //contactor input ID 4
    CAN_cIn_OCC.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_OCC.messageDLC               = 1;
    CAN_cIn_OCC.messageStartByte         = 0;
    CAN_cIn_OCC.messageScaling           = 1;

    CAN_cIn_OCD.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_OCD.messageDLC               = 1;
    CAN_cIn_OCD.messageStartByte         = 1;
    CAN_cIn_OCD.messageScaling           = 1;

    CAN_cIn_OTC.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_OTC.messageDLC               = 1;
    CAN_cIn_OTC.messageStartByte         = 2;
    CAN_cIn_OTC.messageScaling           = 1;

    CAN_cIn_OTD.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_OTD.messageDLC               = 1;
    CAN_cIn_OTD.messageStartByte         = 3;
    CAN_cIn_OTD.messageScaling           = 1;

    CAN_cIn_UTC.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_UTC.messageDLC               = 1;
    CAN_cIn_UTC.messageStartByte         = 4;
    CAN_cIn_UTC.messageScaling           = 1;

    CAN_cIn_UTD.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_UTD.messageDLC               = 1;
    CAN_cIn_UTD.messageStartByte         = 5;
    CAN_cIn_UTD.messageScaling           = 1;

    CAN_cIn_OV.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_OV.messageDLC               = 1;
    CAN_cIn_OV.messageStartByte         = 6;
    CAN_cIn_OV.messageScaling           = 1;

    CAN_cIn_UV.messageID                = CAN_DEBUG_ID_004;
    CAN_cIn_UV.messageDLC               = 1;
    CAN_cIn_UV.messageStartByte         = 7;
    CAN_cIn_UV.messageScaling           = 1;

    // //coulomb counting output ID 3

    // CAN_ccOut_Coulomb_Counting_State.messageID          = CAN_DEBUG_ID_003;
    // CAN_ccOut_Coulomb_Counting_State.messageDLC         = 1;
    // CAN_ccOut_Coulomb_Counting_State.messageStartByte   = 0;
    // CAN_ccOut_Coulomb_Counting_State.messageScaling     = 1;

    // CAN_ccOut_SOC_Pct.messageID          = CAN_DEBUG_ID_003;
    // CAN_ccOut_SOC_Pct.messageDLC         = 1;
    // CAN_ccOut_SOC_Pct.messageStartByte   = 1;
    // CAN_ccOut_SOC_Pct.messageScaling     = 1;

    // CAN_ccOut_EKF_SOC_Pct.messageID          = CAN_DEBUG_ID_003;
    // CAN_ccOut_EKF_SOC_Pct.messageDLC         = 1;
    // CAN_ccOut_EKF_SOC_Pct.messageStartByte   = 2;
    // CAN_ccOut_EKF_SOC_Pct.messageScaling     = 1;

    // CAN_ccOut_Initial_Capacity_Ah.messageID          = CAN_DEBUG_ID_003;
    // CAN_ccOut_Initial_Capacity_Ah.messageDLC         = 4;
    // CAN_ccOut_Initial_Capacity_Ah.messageStartByte   = 4;
    // CAN_ccOut_Initial_Capacity_Ah.messageScaling     = 1;

    //coulomb counting output ID 3 moved to 28
    // CAN_ccOut_Total_Capacity_Discharged_Ah.messageID          = CAN_DEBUG_ID_028;
    // CAN_ccOut_Total_Capacity_Discharged_Ah.messageDLC         = 4;
    // CAN_ccOut_Total_Capacity_Discharged_Ah.messageStartByte   = 0;
    // CAN_ccOut_Total_Capacity_Discharged_Ah.messageScaling     = 1;

    // CAN_ccOut_Total_Capacity_Remaining_Ah.messageID          = CAN_DEBUG_ID_028;
    // CAN_ccOut_Total_Capacity_Remaining_Ah.messageDLC         = 4;
    // CAN_ccOut_Total_Capacity_Remaining_Ah.messageStartByte   = 4;
    // CAN_ccOut_Total_Capacity_Remaining_Ah.messageScaling     = 1;

    //cc EKF 29
    CAN_ccOut_Capacity_EKF.messageID          = CAN_DEBUG_ID_029;
    CAN_ccOut_Capacity_EKF.messageDLC         = 4;
    CAN_ccOut_Capacity_EKF.messageStartByte   = 0;
    CAN_ccOut_Capacity_EKF.messageScaling     = 1;

    CAN_ccOut_Voltage_EKF.messageID                     = CAN_DEBUG_ID_029;
    CAN_ccOut_Voltage_EKF.messageDLC                    = 2;
    CAN_ccOut_Voltage_EKF.messageStartByte              = 4;
    CAN_ccOut_Voltage_EKF.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_ccOut_Terminal_Voltage.messageID                     = CAN_DEBUG_ID_029;
    CAN_ccOut_Terminal_Voltage.messageDLC                    = 2;
    CAN_ccOut_Terminal_Voltage.messageStartByte              = 6;
    CAN_ccOut_Terminal_Voltage.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;



    // coulomb counting input hardcoded values ID 2
    CAN_ccIn_VehicleRest_Time.messageID                = CAN_DEBUG_ID_002;
    CAN_ccIn_VehicleRest_Time.messageDLC               = 2;
    CAN_ccIn_VehicleRest_Time.messageStartByte         = 0;
    CAN_ccIn_VehicleRest_Time.messageScaling           = 1000;

    CAN_ccIn_Max_PackCapacity.messageID                = CAN_DEBUG_ID_002;
    CAN_ccIn_Max_PackCapacity.messageDLC               = 2;
    CAN_ccIn_Max_PackCapacity.messageStartByte         = 2;
    CAN_ccIn_Max_PackCapacity.messageScaling           = 1000;



    CAN_ccIn_VehicleRest_Charge_Current.messageID                = CAN_DEBUG_ID_002;
    CAN_ccIn_VehicleRest_Charge_Current.messageDLC               = 1;
    CAN_ccIn_VehicleRest_Charge_Current.messageStartByte         = 5;
    CAN_ccIn_VehicleRest_Charge_Current.messageScaling           = 1000;

    //coulomb counting input ID 1
    CAN_ccIn_pOut_OVThreshold.messageID                = CAN_DEBUG_ID_001;
    CAN_ccIn_pOut_OVThreshold.messageDLC               = 1;
    CAN_ccIn_pOut_OVThreshold.messageStartByte         = 0;
    CAN_ccIn_pOut_OVThreshold.messageScaling           = 1;

    CAN_ccIn_Voltage.messageID                     = CAN_DEBUG_ID_001;
    CAN_ccIn_Voltage.messageDLC                    = 2;
    CAN_ccIn_Voltage.messageStartByte              = 1;
    CAN_ccIn_Voltage.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

    CAN_ccIn_Current.messageID                   = CAN_DEBUG_ID_001;
    CAN_ccIn_Current.messageDLC                  = 4;
    CAN_ccIn_Current.messageStartByte            = 3;
    CAN_ccIn_Current.messageScaling              = PACK_CURRENT_MESSAGE_SCALING;

    CAN_ccIn_VehicleRest_DisCharge_Current.messageID                = CAN_DEBUG_ID_001;
    CAN_ccIn_VehicleRest_DisCharge_Current.messageDLC               = 1;
    CAN_ccIn_VehicleRest_DisCharge_Current.messageStartByte         = 7;
    CAN_ccIn_VehicleRest_DisCharge_Current.messageScaling           = 1000;


    //coulomb counting output data
    CAN_ccOut_initialCapacity.messageID               =    CAN_DEBUG_ID_028;
    CAN_ccOut_initialCapacity.messageDLC              =    4;
    CAN_ccOut_initialCapacity.messageStartByte        =    0;
    CAN_ccOut_initialCapacity.messageScaling          =    1;

    CAN_ccOut_totalCapacityRemaining.messageID        =    CAN_DEBUG_ID_028;
    CAN_ccOut_totalCapacityRemaining.messageDLC       =    4;
    CAN_ccOut_totalCapacityRemaining.messageStartByte =    4;
    CAN_ccOut_totalCapacityRemaining.messageScaling   =    1;

    //ID 2
    CAN_ccOut_MaxUsableCapacity_mAh.messageID         =    CAN_DEBUG_ID_002;
    CAN_ccOut_MaxUsableCapacity_mAh.messageDLC        =    4;
    CAN_ccOut_MaxUsableCapacity_mAh.messageStartByte  =    0;
    CAN_ccOut_MaxUsableCapacity_mAh.messageScaling    =    1;

    CAN_ccOut_TotalCapacityExchange_mAh.messageID        =    CAN_DEBUG_ID_002;
    CAN_ccOut_TotalCapacityExchange_mAh.messageDLC       =    4;
    CAN_ccOut_TotalCapacityExchange_mAh.messageStartByte =    4;
    CAN_ccOut_TotalCapacityExchange_mAh.messageScaling   =    1;

    //ID 3
    CAN_ccOut_totalCapacityDischarge.messageID        =    CAN_DEBUG_ID_003;
    CAN_ccOut_totalCapacityDischarge.messageDLC       =    4;
    CAN_ccOut_totalCapacityDischarge.messageStartByte =    0;
    CAN_ccOut_totalCapacityDischarge.messageScaling   =    1;

    CAN_ccOut_SOCPercentage.messageID                 =    CAN_DEBUG_ID_003;
    CAN_ccOut_SOCPercentage.messageDLC                =    1;
    CAN_ccOut_SOCPercentage.messageStartByte          =    4;
    CAN_ccOut_SOCPercentage.messageScaling            =    1;

    CAN_ccOut_SOH_cpct.messageID                       =    CAN_DEBUG_ID_003;
    CAN_ccOut_SOH_cpct.messageDLC                      =    1;
    CAN_ccOut_SOH_cpct.messageStartByte                =    5;
    CAN_ccOut_SOH_cpct.messageScaling                  =    1;

    CAN_ccOut_coulombCountingState.messageID          =    CAN_DEBUG_ID_003;
    CAN_ccOut_coulombCountingState.messageDLC         =    1;
    CAN_ccOut_coulombCountingState.messageStartByte   =    6;
    CAN_ccOut_coulombCountingState.messageScaling     =    1;

}

#endif


/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize CAN
 * @param canID can ID
 * @retval BMS status
 */
bmsStatus_t initializeCANMatrix(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;
	
    if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    if(canPort == communicationCAN)
    {
        CAN_balancingLimit.messageID                = CAN_TX_ID_001;
        CAN_balancingLimit.messageDLC               = 2;
        CAN_balancingLimit.messageStartByte         = 0;
        CAN_balancingLimit.messageScaling           = 0.1;

        CAN_flagsPreChargeActive.messageID          = CAN_TX_ID_001;
        CAN_flagsPreChargeActive.messageDLC         = 1;
        CAN_flagsPreChargeActive.messageStartByte   = 2;
        CAN_flagsPreChargeActive.messageScaling     = 1;

        CAN_flagsBalancingActive.messageID          = CAN_TX_ID_001;
        CAN_flagsBalancingActive.messageDLC         = 1;
        CAN_flagsBalancingActive.messageStartByte   = 3;
        CAN_flagsBalancingActive.messageScaling     = 1;

        CAN_packCurrent.messageID                   = CAN_TX_ID_001;
        CAN_packCurrent.messageDLC                  = 4;
        CAN_packCurrent.messageStartByte            = 4;
        CAN_packCurrent.messageScaling              = PACK_CURRENT_MESSAGE_SCALING;

        CAN_stateOfCharge.messageID                 = CAN_TX_ID_002;
        CAN_stateOfCharge.messageDLC                = 2;
        CAN_stateOfCharge.messageStartByte          = 0;
        // CAN_stateOfCharge.messageScaling            = 0.01;
        CAN_stateOfCharge.messageScaling            = 1;

        CAN_stateOfHealth.messageID                 = CAN_TX_ID_002;
        CAN_stateOfHealth.messageDLC                = 2;
        CAN_stateOfHealth.messageStartByte          = 2;
        CAN_stateOfHealth.messageScaling            = 0.01;

        CAN_bmsState.messageID                      = CAN_TX_ID_002;
        CAN_bmsState.messageDLC                     = 1;
        CAN_bmsState.messageStartByte               = 4;
        CAN_bmsState.messageScaling                 = 1;

        CAN_flagsFullCharge.messageID               = CAN_TX_ID_002;
        CAN_flagsFullCharge.messageDLC              = 1;
        CAN_flagsFullCharge.messageStartByte        = 5;
        CAN_flagsFullCharge.messageScaling          = 1;

        CAN_packVoltage.messageID                   = CAN_TX_ID_002;
        CAN_packVoltage.messageDLC                  = 2;
        CAN_packVoltage.messageStartByte            = 6;
        CAN_packVoltage.messageScaling              = 0.1;

        CAN_cmu1Temp1.messageID                      = CAN_TX_ID_003;
        CAN_cmu1Temp1.messageDLC                     = 1;
        CAN_cmu1Temp1.messageStartByte               = 0;
        CAN_cmu1Temp1.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu1Temp2.messageID                      = CAN_TX_ID_003;
        CAN_cmu1Temp2.messageDLC                     = 1;
        CAN_cmu1Temp2.messageStartByte               = 1;
        CAN_cmu1Temp2.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu1Temp3.messageID                      = CAN_TX_ID_003;
        CAN_cmu1Temp3.messageDLC                     = 1;
        CAN_cmu1Temp3.messageStartByte               = 2;
        CAN_cmu1Temp3.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu1Temp4.messageID                      = CAN_TX_ID_003;
        CAN_cmu1Temp4.messageDLC                     = 1;
        CAN_cmu1Temp4.messageStartByte               = 3;
        CAN_cmu1Temp4.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu2Temp1.messageID                      = CAN_TX_ID_003;
        CAN_cmu2Temp1.messageDLC                     = 1;
        CAN_cmu2Temp1.messageStartByte               = 4;
        CAN_cmu2Temp1.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu2Temp2.messageID                      = CAN_TX_ID_003;
        CAN_cmu2Temp2.messageDLC                     = 1;
        CAN_cmu2Temp2.messageStartByte               = 5;
        CAN_cmu2Temp2.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_packCapacity.messageID                  = CAN_TX_ID_003;
        CAN_packCapacity.messageDLC                 = 2;
        CAN_packCapacity.messageStartByte           = 6;
        CAN_packCapacity.messageScaling             = 1;

        CAN_cmu2Temp3.messageID                      = CAN_TX_ID_004;
        CAN_cmu2Temp3.messageDLC                     = 1;
        CAN_cmu2Temp3.messageStartByte               = 0;
        CAN_cmu2Temp3.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu2Temp4.messageID                      = CAN_TX_ID_004;
        CAN_cmu2Temp4.messageDLC                     = 1;
        CAN_cmu2Temp4.messageStartByte               = 1;
        CAN_cmu2Temp4.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;

        CAN_cmu3Temp1.messageID                      = CAN_TX_ID_004;
        CAN_cmu3Temp1.messageDLC                     = 1;
        CAN_cmu3Temp1.messageStartByte               = 2;
        CAN_cmu3Temp1.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu3Temp2.messageID                      = CAN_TX_ID_004;
        CAN_cmu3Temp2.messageDLC                     = 1;
        CAN_cmu3Temp2.messageStartByte               = 3;
        CAN_cmu3Temp2.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu3Temp3.messageID                      = CAN_TX_ID_004;
        CAN_cmu3Temp3.messageDLC                     = 1;
        CAN_cmu3Temp3.messageStartByte               = 4;
        CAN_cmu3Temp3.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu3Temp4.messageID                      = CAN_TX_ID_004;
        CAN_cmu3Temp4.messageDLC                     = 1;
        CAN_cmu3Temp4.messageStartByte               = 5;
        CAN_cmu3Temp4.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu4Temp1.messageID                      = CAN_TX_ID_004;
        CAN_cmu4Temp1.messageDLC                     = 1;
        CAN_cmu4Temp1.messageStartByte               = 6;
        CAN_cmu4Temp1.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu4Temp2.messageID                      = CAN_TX_ID_004;
        CAN_cmu4Temp2.messageDLC                     = 1;
        CAN_cmu4Temp2.messageStartByte               = 7;
        CAN_cmu4Temp2.messageScaling                 = CMU_TEMP_MESSAGE_SCALING;
        
        CAN_cmu1Cell1.messageID                     = CAN_TX_ID_005;
        CAN_cmu1Cell1.messageDLC                    = 2;
        CAN_cmu1Cell1.messageStartByte              = 0;
        CAN_cmu1Cell1.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell2.messageID                     = CAN_TX_ID_005;
        CAN_cmu1Cell2.messageDLC                    = 2;
        CAN_cmu1Cell2.messageStartByte              = 2;
        CAN_cmu1Cell2.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell3.messageID                     = CAN_TX_ID_005;
        CAN_cmu1Cell3.messageDLC                    = 2;
        CAN_cmu1Cell3.messageStartByte              = 4;
        CAN_cmu1Cell3.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell4.messageID                     = CAN_TX_ID_005;
        CAN_cmu1Cell4.messageDLC                    = 2;
        CAN_cmu1Cell4.messageStartByte              = 6;
        CAN_cmu1Cell4.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell5.messageID                     = CAN_TX_ID_006;
        CAN_cmu1Cell5.messageDLC                    = 2;
        CAN_cmu1Cell5.messageStartByte              = 0;
        CAN_cmu1Cell5.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell6.messageID                     = CAN_TX_ID_006;
        CAN_cmu1Cell6.messageDLC                    = 2;
        CAN_cmu1Cell6.messageStartByte              = 2;
        CAN_cmu1Cell6.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell7.messageID                     = CAN_TX_ID_006;
        CAN_cmu1Cell7.messageDLC                    = 2;
        CAN_cmu1Cell7.messageStartByte              = 4;
        CAN_cmu1Cell7.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell8.messageID                     = CAN_TX_ID_006;
        CAN_cmu1Cell8.messageDLC                    = 2;
        CAN_cmu1Cell8.messageStartByte              = 6;
        CAN_cmu1Cell8.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell9.messageID                     = CAN_TX_ID_007;
        CAN_cmu1Cell9.messageDLC                    = 2;
        CAN_cmu1Cell9.messageStartByte              = 0;
        CAN_cmu1Cell9.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell10.messageID                     = CAN_TX_ID_007;
        CAN_cmu1Cell10.messageDLC                    = 2;
        CAN_cmu1Cell10.messageStartByte              = 2;
        CAN_cmu1Cell10.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell11.messageID                     = CAN_TX_ID_007;
        CAN_cmu1Cell11.messageDLC                    = 2;
        CAN_cmu1Cell11.messageStartByte              = 4;
        CAN_cmu1Cell11.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu1Cell12.messageID                     = CAN_TX_ID_007;
        CAN_cmu1Cell12.messageDLC                    = 2;
        CAN_cmu1Cell12.messageStartByte              = 6;
        CAN_cmu1Cell12.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell1.messageID                     = CAN_TX_ID_008;
        CAN_cmu2Cell1.messageDLC                    = 2;
        CAN_cmu2Cell1.messageStartByte              = 0;
        CAN_cmu2Cell1.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell2.messageID                     = CAN_TX_ID_008;
        CAN_cmu2Cell2.messageDLC                    = 2;
        CAN_cmu2Cell2.messageStartByte              = 2;
        CAN_cmu2Cell2.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell3.messageID                     = CAN_TX_ID_008;
        CAN_cmu2Cell3.messageDLC                    = 2;
        CAN_cmu2Cell3.messageStartByte              = 4;
        CAN_cmu2Cell3.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell4.messageID                     = CAN_TX_ID_008;
        CAN_cmu2Cell4.messageDLC                    = 2;
        CAN_cmu2Cell4.messageStartByte              = 6;
        CAN_cmu2Cell4.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell5.messageID                     = CAN_TX_ID_009;
        CAN_cmu2Cell5.messageDLC                    = 2;
        CAN_cmu2Cell5.messageStartByte              = 0;
        CAN_cmu2Cell5.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell6.messageID                     = CAN_TX_ID_009;
        CAN_cmu2Cell6.messageDLC                    = 2;
        CAN_cmu2Cell6.messageStartByte              = 2;
        CAN_cmu2Cell6.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell7.messageID                     = CAN_TX_ID_009;
        CAN_cmu2Cell7.messageDLC                    = 2;
        CAN_cmu2Cell7.messageStartByte              = 4;
        CAN_cmu2Cell7.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell8.messageID                     = CAN_TX_ID_009;
        CAN_cmu2Cell8.messageDLC                    = 2;
        CAN_cmu2Cell8.messageStartByte              = 6;
        CAN_cmu2Cell8.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell9.messageID                     = CAN_TX_ID_010;
        CAN_cmu2Cell9.messageDLC                    = 2;
        CAN_cmu2Cell9.messageStartByte              = 0;
        CAN_cmu2Cell9.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell10.messageID                     = CAN_TX_ID_010;
        CAN_cmu2Cell10.messageDLC                    = 2;
        CAN_cmu2Cell10.messageStartByte              = 2;
        CAN_cmu2Cell10.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell11.messageID                     = CAN_TX_ID_010;
        CAN_cmu2Cell11.messageDLC                    = 2;
        CAN_cmu2Cell11.messageStartByte              = 4;
        CAN_cmu2Cell11.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu2Cell12.messageID                     = CAN_TX_ID_010;
        CAN_cmu2Cell12.messageDLC                    = 2;
        CAN_cmu2Cell12.messageStartByte              = 6;
        CAN_cmu2Cell12.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_dynamicLimitIN.messageID                 = CAN_TX_ID_011;
        CAN_dynamicLimitIN.messageDLC                = 2;
        CAN_dynamicLimitIN.messageStartByte          = 0;
        CAN_dynamicLimitIN.messageScaling            = 0.1;

        CAN_dynamicLimitOUT.messageID                = CAN_TX_ID_011;
        CAN_dynamicLimitOUT.messageDLC               = 2;
        CAN_dynamicLimitOUT.messageStartByte         = 2;
        CAN_dynamicLimitOUT.messageScaling           = 0.1;

        CAN_FirmwareVersion.messageID                = CAN_TX_ID_011;
        CAN_FirmwareVersion.messageDLC               = 2;
        CAN_FirmwareVersion.messageStartByte         = 4;
        CAN_FirmwareVersion.messageScaling           = 1;

        CAN_ConfigID.messageID                       = CAN_TX_ID_011;
        CAN_ConfigID.messageDLC                      = 1;
        CAN_ConfigID.messageStartByte                = 6;
        CAN_ConfigID.messageScaling                  = 1;

        CAN_packCapacityDesign.messageID            = CAN_TX_ID_012;
        CAN_packCapacityDesign.messageDLC           = 2;
        CAN_packCapacityDesign.messageStartByte     = 0;
        CAN_packCapacityDesign.messageScaling       = 0.1;

        CAN_packCapacityFull.messageID              = CAN_TX_ID_012;
        CAN_packCapacityFull.messageDLC             = 2;
        CAN_packCapacityFull.messageStartByte       = 2;
        CAN_packCapacityFull.messageScaling         = 0.1;

        CAN_balancingBitMask_CMU1.messageID         = CAN_TX_ID_012;
        CAN_balancingBitMask_CMU1.messageDLC        = 2;
        CAN_balancingBitMask_CMU1.messageStartByte  = 4;
        CAN_balancingBitMask_CMU1.messageScaling    = 1;
        
        CAN_balancingBitMask_CMU2.messageID         = CAN_TX_ID_012;
        CAN_balancingBitMask_CMU2.messageDLC        = 2;
        CAN_balancingBitMask_CMU2.messageStartByte  = 6;
        CAN_balancingBitMask_CMU2.messageScaling    = 1;

        CAN_InstantaneousLimitIN.messageID          = CAN_TX_ID_015;
        CAN_InstantaneousLimitIN.messageDLC         = 2;
        CAN_InstantaneousLimitIN.messageStartByte   = 0;
        CAN_InstantaneousLimitIN.messageScaling     = 0.1;

        CAN_InstantaneousLimitOUT.messageID         = CAN_TX_ID_015;
        CAN_InstantaneousLimitOUT.messageDLC        = 2;
        CAN_InstantaneousLimitOUT.messageStartByte  = 2;
        CAN_InstantaneousLimitOUT.messageScaling    = 0.1;

        CAN_cmu3Cell1.messageID                     = CAN_TX_ID_016;
        CAN_cmu3Cell1.messageDLC                    = 2;
        CAN_cmu3Cell1.messageStartByte              = 0;
        CAN_cmu3Cell1.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell2.messageID                     = CAN_TX_ID_016;
        CAN_cmu3Cell2.messageDLC                    = 2;
        CAN_cmu3Cell2.messageStartByte              = 2;
        CAN_cmu3Cell2.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell3.messageID                     = CAN_TX_ID_016;
        CAN_cmu3Cell3.messageDLC                    = 2;
        CAN_cmu3Cell3.messageStartByte              = 4;
        CAN_cmu3Cell3.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell4.messageID                     = CAN_TX_ID_016;
        CAN_cmu3Cell4.messageDLC                    = 2;
        CAN_cmu3Cell4.messageStartByte              = 6;
        CAN_cmu3Cell4.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell5.messageID                     = CAN_TX_ID_017;
        CAN_cmu3Cell5.messageDLC                    = 2;
        CAN_cmu3Cell5.messageStartByte              = 0;
        CAN_cmu3Cell5.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell6.messageID                     = CAN_TX_ID_017;
        CAN_cmu3Cell6.messageDLC                    = 2;
        CAN_cmu3Cell6.messageStartByte              = 2;
        CAN_cmu3Cell6.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell7.messageID                     = CAN_TX_ID_017;
        CAN_cmu3Cell7.messageDLC                    = 2;
        CAN_cmu3Cell7.messageStartByte              = 4;
        CAN_cmu3Cell7.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell8.messageID                     = CAN_TX_ID_017;
        CAN_cmu3Cell8.messageDLC                    = 2;
        CAN_cmu3Cell8.messageStartByte              = 6;
        CAN_cmu3Cell8.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell9.messageID                     = CAN_TX_ID_018;
        CAN_cmu3Cell9.messageDLC                    = 2;
        CAN_cmu3Cell9.messageStartByte              = 0;
        CAN_cmu3Cell9.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell10.messageID                     = CAN_TX_ID_018;
        CAN_cmu3Cell10.messageDLC                    = 2;
        CAN_cmu3Cell10.messageStartByte              = 2;
        CAN_cmu3Cell10.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell11.messageID                     = CAN_TX_ID_018;
        CAN_cmu3Cell11.messageDLC                    = 2;
        CAN_cmu3Cell11.messageStartByte              = 4;
        CAN_cmu3Cell11.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu3Cell12.messageID                     = CAN_TX_ID_018;
        CAN_cmu3Cell12.messageDLC                    = 2;
        CAN_cmu3Cell12.messageStartByte              = 6;
        CAN_cmu3Cell12.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell1.messageID                     = CAN_TX_ID_019;
        CAN_cmu4Cell1.messageDLC                    = 2;
        CAN_cmu4Cell1.messageStartByte              = 0;
        CAN_cmu4Cell1.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell2.messageID                     = CAN_TX_ID_019;
        CAN_cmu4Cell2.messageDLC                    = 2;
        CAN_cmu4Cell2.messageStartByte              = 2;
        CAN_cmu4Cell2.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell3.messageID                     = CAN_TX_ID_019;
        CAN_cmu4Cell3.messageDLC                    = 2;
        CAN_cmu4Cell3.messageStartByte              = 4;
        CAN_cmu4Cell3.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell4.messageID                     = CAN_TX_ID_019;
        CAN_cmu4Cell4.messageDLC                    = 2;
        CAN_cmu4Cell4.messageStartByte              = 6;
        CAN_cmu4Cell4.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell5.messageID                     = CAN_TX_ID_020;
        CAN_cmu4Cell5.messageDLC                    = 2;
        CAN_cmu4Cell5.messageStartByte              = 0;
        CAN_cmu4Cell5.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell6.messageID                     = CAN_TX_ID_020;
        CAN_cmu4Cell6.messageDLC                    = 2;
        CAN_cmu4Cell6.messageStartByte              = 2;
        CAN_cmu4Cell6.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell7.messageID                     = CAN_TX_ID_020;
        CAN_cmu4Cell7.messageDLC                    = 2;
        CAN_cmu4Cell7.messageStartByte              = 4;
        CAN_cmu4Cell7.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell8.messageID                     = CAN_TX_ID_020;
        CAN_cmu4Cell8.messageDLC                    = 2;
        CAN_cmu4Cell8.messageStartByte              = 6;
        CAN_cmu4Cell8.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell9.messageID                     = CAN_TX_ID_021;
        CAN_cmu4Cell9.messageDLC                    = 2;
        CAN_cmu4Cell9.messageStartByte              = 0;
        CAN_cmu4Cell9.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell10.messageID                     = CAN_TX_ID_021;
        CAN_cmu4Cell10.messageDLC                    = 2;
        CAN_cmu4Cell10.messageStartByte              = 2;
        CAN_cmu4Cell10.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell11.messageID                     = CAN_TX_ID_021;
        CAN_cmu4Cell11.messageDLC                    = 2;
        CAN_cmu4Cell11.messageStartByte              = 4;
        CAN_cmu4Cell11.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_cmu4Cell12.messageID                     = CAN_TX_ID_021;
        CAN_cmu4Cell12.messageDLC                    = 2;
        CAN_cmu4Cell12.messageStartByte              = 6;
        CAN_cmu4Cell12.messageScaling                = CMU_CELL_VOLTAGE_MESSAGE_SCALING;

        CAN_timeBaseResetCounter.messageID           = CAN_TX_ID_022;
        CAN_timeBaseResetCounter.messageDLC          = 2;
        CAN_timeBaseResetCounter.messageStartByte    = 0;
        CAN_timeBaseResetCounter.messageScaling      = 1;

        CAN_mcuResetCounter.messageID                = CAN_TX_ID_022;
        CAN_mcuResetCounter.messageDLC               = 2;
        CAN_mcuResetCounter.messageStartByte         = 2;
        CAN_mcuResetCounter.messageScaling           = 1;

        CAN_2949CRCCounter.messageID                = CAN_TX_ID_022;
        CAN_2949CRCCounter.messageDLC               = 2;
        CAN_2949CRCCounter.messageStartByte         = 4;
        CAN_2949CRCCounter.messageScaling           = 1;

        CAN_6815CRCCounter.messageID                = CAN_TX_ID_022;
        CAN_6815CRCCounter.messageDLC               = 2;
        CAN_6815CRCCounter.messageStartByte         = 6;
        CAN_6815CRCCounter.messageScaling           = 1;

#ifdef CAN_MATRIX_V_0_3
        CAN_maxAllowedCellVoltage.messageID           = CAN_TX_ID_023;
        CAN_maxAllowedCellVoltage.messageDLC          = 2;
        CAN_maxAllowedCellVoltage.messageStartByte    = 0;
        CAN_maxAllowedCellVoltage.messageScaling      = 1;

        CAN_maxAllowedPackVoltage.messageID           = CAN_TX_ID_023;
        CAN_maxAllowedPackVoltage.messageDLC          = 2;
        CAN_maxAllowedPackVoltage.messageStartByte    = 2;
        CAN_maxAllowedPackVoltage.messageScaling      = 1;

        CAN_maxAllowedCellTemp.messageID              = CAN_TX_ID_023;
        CAN_maxAllowedCellTemp.messageDLC             = 2;
        CAN_maxAllowedCellTemp.messageStartByte       = 4;
        CAN_maxAllowedCellTemp.messageScaling         = 1;

        CAN_maxAllowedCurrent.messageID               = CAN_TX_ID_023;
        CAN_maxAllowedCurrent.messageDLC              = 2;
        CAN_maxAllowedCurrent.messageStartByte        = 6;
        CAN_maxAllowedCurrent.messageScaling          = 1;
#endif

        status = BMS_OK;
    }
    else
    {
        status = BMS_ERROR;
    }

    return status;
}


/* End of File ---------------------------------------------------------------*/

