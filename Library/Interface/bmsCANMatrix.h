/**
 * @file bmsCANMatrix.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BMSCANMATRIX_H_
#define BMSCANMATRIX_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
//Backward Compatibility CAN ID
#ifdef CBMS_TEST
    #define CAN_TX_ID_001         0x710
    #define CAN_TX_ID_002         0x709
    #define CAN_TX_ID_003         0x712
    #define CAN_TX_ID_005         0x713
    #define CAN_TX_ID_006         0x714
    #define CAN_TX_ID_007         0x715
    #define CAN_TX_ID_008         0x716
    #define CAN_TX_ID_009         0x717
    #define CAN_TX_ID_010         0x718
    #define CAN_TX_ID_011         0x72A
    #define CAN_TX_ID_023         0x708
    #define CAN_TX_ID_012         0x71C
    #define CAN_TX_ID_013         0x758
    #define CAN_TX_ID_014         0x759
    #define CAN_TX_ID_CHARGER     0x1806E5F4
#else
    #define CAN_TX_ID_001         0x110
    #ifdef CAN_MATRIX_V_0_3
        #define CAN_TX_ID_002         0x109
    #else
        #define CAN_TX_ID_002         0x111
    #endif

    #ifdef CAN_CHARGER_EXTENDED_ID
        #define CAN_TX_ID_CHARGER     0x1806E5F4
    #else
        #define CAN_TX_ID_CHARGER     0x107
    #endif
    
    #define CAN_TX_ID_003         0x112
    #define CAN_TX_ID_005         0x113
    #define CAN_TX_ID_006         0x114
    #define CAN_TX_ID_007         0x115
    #define CAN_TX_ID_008         0x116
    #define CAN_TX_ID_009         0x117
    #define CAN_TX_ID_010         0x118
    #define CAN_TX_ID_011         0x12A
    #define CAN_TX_ID_023         0x108
    #define CAN_TX_ID_012         0x11C
    #define CAN_TX_ID_013         0x258
    #define CAN_TX_ID_014         0x259

#endif

#define CAN_TX_ID_004         0x11A
#define CAN_TX_ID_015         0x156
#define CAN_TX_ID_016         0x119
#define CAN_TX_ID_017         0x120
#define CAN_TX_ID_018         0x121
#define CAN_TX_ID_019         0x122
#define CAN_TX_ID_020         0x123
#define CAN_TX_ID_021         0x124
#define CAN_TX_ID_022         0x12B

#ifdef CAN_MATRIX_V_0_3
    #define CAN_TX_ID_023         0x108
#endif

#define CAN_TX_ID_024         0x12C

#ifdef MARVEL_CAN_ID
//Dedicated Marvel3 CAN ID
#define CANM3_P1A                   0x300
#define CANM3_P1B                   0x301
#define CANM3_P1C                   0x302
#define CANM3_P1D                   0x303
#define CANM3_P1E                   0x304
#define CANM3_P1F                   0x305
#define CANM3_P1G                   0x306
#define CANM3_P1H                   0x307

#define CANM3_C1A                   0x310
#define CANM3_C1B                   0x311
#define CANM3_C1C                   0x312
#define CANM3_C2A                   0x313
#define CANM3_C2B                   0x314
#define CANM3_C2C                   0x315
#define CANM3_C3A                   0x316
#define CANM3_C3B                   0x317
#define CANM3_C3C                   0x318
#define CANM3_C4A                   0x319
#define CANM3_C4B                   0x31A
#define CANM3_C4C                   0x31B
#define CANM3_C5A                   0x31C
#define CANM3_C5B                   0x31D
#define CANM3_C5C                   0x31E
#define CANM3_C6A                   0x31F
#define CANM3_C6B                   0x320
#define CANM3_C6C                   0x321
#define CANM3_C7A                   0x322
#define CANM3_C7B                   0x323
#define CANM3_C7C                   0x324
#define CANM3_C8A                   0x325
#define CANM3_C8B                   0x326
#define CANM3_C8C                   0x327
#define CANM3_C9A                   0x328
#define CANM3_C9B                   0x329
#define CANM3_C9C                   0x32A
#define CANM3_CAA                   0x32B
#define CANM3_CAB                   0x32C
#define CANM3_CAC                   0x32D
#define CANM3_CBA                   0x32E
#define CANM3_CBB                   0x32F
#define CANM3_CBC                   0x330
#define CANM3_CCA                   0x331
#define CANM3_CCB                   0x332
#define CANM3_CCC                   0x333

#define CANM3_TE1                   0x340
#define CANM3_TE2                   0x341
#define CANM3_TE3                   0x342
#define CANM3_TE4                   0x343
#define CANM3_TE5                   0x344
#define CANM3_TE6                   0x345
#define CANM3_TE7                   0x346
#define CANM3_TE8                   0x347
#define CANM3_TE9                   0x348
#define CANM3_TEA                   0x349
#define CANM3_TEB                   0x34A
#define CANM3_TEC                   0x34B

#define CANM3_TI1                   0x350
#define CANM3_TI2                   0x351
#define CANM3_TI3                   0x352
#define CANM3_TI4                   0x353
#define CANM3_TI5                   0x354
#define CANM3_TI6                   0x355
#define CANM3_TI7                   0x356
#define CANM3_TI8                   0x357
#define CANM3_TI9                   0x358
#define CANM3_TIA                   0x359
#define CANM3_TIB                   0x35A
#define CANM3_TIC                   0x35B

#define CANM3_E1A                   0x380
#define CANM3_E1B                   0x381
#define CANM3_E1C                   0x382
#define CANM3_E1D                   0x383
#define CANM3_E1E                   0x384
#define CANM3_E1F                   0x385
#define CANM3_E1G                   0x386
#define CANM3_E1H                   0x387

#endif

/* Reception CAN IDs*/
#define CAN_RX_ID_001                           0x103
#define CAN_RX_ID_002                           0x104
#define CAN_RX_ID_003                           0x105
#define CAN_RX_ID_004                           0x106

#define CAN_RX_ID_006                           0x6FA
#define CAN_RX_ID_007                           0x6FB
#define CAN_RX_ID_008                           0x900
#define CAN_RX_ID_009                           0x901
#define CAN_RX_ID_010                           0x1FE
#define CAN_RX_ID_011                           0x4FA
#define CAN_RX_ID_012                           0x400


#define FLASH_CONFIG_CAN_RX_ID                  CAN_RX_ID_001
#define VEHICLE_STATE_CAN_RX_ID                 CAN_RX_ID_002
#define EEPROM_RESET_CAN_RX_ID                  CAN_RX_ID_003
#define TEST_CAN_RX_ID                          CAN_RX_ID_004
#define STARK_VEHICLE_STATE                     CAN_RX_ID_012  

#define IS_CAN_RX_ID(ID)                        (((ID) == CAN_RX_ID_001) || ((ID) == CAN_RX_ID_002) ||\
                                                 ((ID) == CAN_RX_ID_006) || ((ID) == CAN_RX_ID_007) ||\
                                                 ((ID) == CAN_RX_ID_008) || ((ID) == CAN_RX_ID_009) ||\
                                                 ((ID) == CAN_RX_ID_010) || ((ID) == CAN_RX_ID_012))


/* Reception CAN Masks*/
#define CAN_RX_MASK                             0x7C0
#define IS_CAN_RX_MASK(MASK)                    (((MASK) == CAN_RX_MASK))

/* Scaling Factors*/
#define CMU_TEMP_MESSAGE_SCALING                1
#define CMU_CELL_VOLTAGE_MESSAGE_SCALING        0.1
#define PACK_CURRENT_MESSAGE_SCALING            0.00001

/* Private typedef -----------------------------------------------------------*/
typedef struct canDataTypeStd
{
    uint16_t messageID;
    uint8_t messageDLC;
    uint8_t messageStartByte;
    float messageScaling;
}CANdataStd;

typedef struct canDataTypeExt
{
    uint32_t messageID;
    uint8_t messageDLC;
    uint8_t messageStartByte;
    float messageScaling;
}CANdataExt;




/* External variables --------------------------------------------------------*/
//Backward Compatilble CAN
extern CANdataStd       CAN_flagsPreChargeActive;
extern CANdataStd       CAN_flagsBalancingActive;
extern CANdataStd       CAN_flagsFullCharge;

extern CANdataStd       CAN_balancingLimit;
extern CANdataStd       CAN_packCurrent;
extern CANdataStd       CAN_stateOfCharge;
extern CANdataStd       CAN_stateOfHealth;
extern CANdataStd       CAN_bmsState;
extern CANdataStd       CAN_packVoltage;
extern CANdataStd       CAN_packCapacity;
extern CANdataStd       CAN_dynamicLimitIN;
extern CANdataStd       CAN_dynamicLimitOUT;
extern CANdataStd       CAN_InstantaneousLimitIN;
extern CANdataStd       CAN_InstantaneousLimitOUT;
extern CANdataStd       CAN_FirmwareVersion;
extern CANdataStd       CAN_ConfigID;
extern CANdataStd       CAN_packCapacityDesign;
extern CANdataStd       CAN_packCapacityFull;
extern CANdataStd       CAN_balancingBitMask_CMU1;
extern CANdataStd       CAN_balancingBitMask_CMU2;
extern CANdataStd       CAN_timeBaseResetCounter;
extern CANdataStd       CAN_mcuResetCounter;
extern CANdataStd       CAN_2949CRCCounter;
extern CANdataStd       CAN_6815CRCCounter;

#ifdef CAN_MATRIX_V_0_3
extern CANdataStd       CAN_maxAllowedCellVoltage;
extern CANdataStd       CAN_maxAllowedPackVoltage;
extern CANdataStd       CAN_maxAllowedCellTemp;
extern CANdataStd       CAN_maxAllowedCurrent;
#endif

extern CANdataStd       CAN_cmu1Temp1;
extern CANdataStd       CAN_cmu1Temp2;
extern CANdataStd       CAN_cmu1Temp3;
extern CANdataStd       CAN_cmu1Temp4;
extern CANdataStd       CAN_cmu2Temp1;
extern CANdataStd       CAN_cmu2Temp2;
extern CANdataStd       CAN_cmu2Temp3;
extern CANdataStd       CAN_cmu2Temp4;
extern CANdataStd       CAN_cmu3Temp1;
extern CANdataStd       CAN_cmu3Temp2;
extern CANdataStd       CAN_cmu3Temp3;
extern CANdataStd       CAN_cmu3Temp4;
extern CANdataStd       CAN_cmu4Temp1;
extern CANdataStd       CAN_cmu4Temp2;
extern CANdataStd       CAN_cmu4Temp3;
extern CANdataStd       CAN_cmu4Temp4;

extern CANdataStd       CAN_cmu1Cell1;
extern CANdataStd       CAN_cmu1Cell2;
extern CANdataStd       CAN_cmu1Cell3;
extern CANdataStd       CAN_cmu1Cell4;
extern CANdataStd       CAN_cmu1Cell5;
extern CANdataStd       CAN_cmu1Cell6;
extern CANdataStd       CAN_cmu1Cell7;
extern CANdataStd       CAN_cmu1Cell8;
extern CANdataStd       CAN_cmu1Cell9;
extern CANdataStd       CAN_cmu1Cell10;
extern CANdataStd       CAN_cmu1Cell11;
extern CANdataStd       CAN_cmu1Cell12;
extern CANdataStd       CAN_cmu2Cell1;
extern CANdataStd       CAN_cmu2Cell2;
extern CANdataStd       CAN_cmu2Cell3;
extern CANdataStd       CAN_cmu2Cell4;
extern CANdataStd       CAN_cmu2Cell5;
extern CANdataStd       CAN_cmu2Cell6;
extern CANdataStd       CAN_cmu2Cell7;
extern CANdataStd       CAN_cmu2Cell8;
extern CANdataStd       CAN_cmu2Cell9;
extern CANdataStd       CAN_cmu2Cell10;
extern CANdataStd       CAN_cmu2Cell11;
extern CANdataStd       CAN_cmu2Cell12;
extern CANdataStd       CAN_cmu3Cell1;
extern CANdataStd       CAN_cmu3Cell2;
extern CANdataStd       CAN_cmu3Cell3;
extern CANdataStd       CAN_cmu3Cell4;
extern CANdataStd       CAN_cmu3Cell5;
extern CANdataStd       CAN_cmu3Cell6;
extern CANdataStd       CAN_cmu3Cell7;
extern CANdataStd       CAN_cmu3Cell8;
extern CANdataStd       CAN_cmu3Cell9;
extern CANdataStd       CAN_cmu3Cell10;
extern CANdataStd       CAN_cmu3Cell11;
extern CANdataStd       CAN_cmu3Cell12;
extern CANdataStd       CAN_cmu4Cell1;
extern CANdataStd       CAN_cmu4Cell2;
extern CANdataStd       CAN_cmu4Cell3;
extern CANdataStd       CAN_cmu4Cell4;
extern CANdataStd       CAN_cmu4Cell5;
extern CANdataStd       CAN_cmu4Cell6;
extern CANdataStd       CAN_cmu4Cell7;
extern CANdataStd       CAN_cmu4Cell8;
extern CANdataStd       CAN_cmu4Cell9;
extern CANdataStd       CAN_cmu4Cell10;
extern CANdataStd       CAN_cmu4Cell11;
extern CANdataStd       CAN_cmu4Cell12;

#ifdef DEBUG_CAN

//coulomb counting
#define CAN_DEBUG_ID_001         0x320
#define CAN_DEBUG_ID_002         0x321
#define CAN_DEBUG_ID_003         0x322

//contactor 
#define CAN_DEBUG_ID_004         0x323
#define CAN_DEBUG_ID_005         0x324
#define CAN_DEBUG_ID_006         0x325
#define CAN_DEBUG_ID_007         0x326

//cell balancing
#define CAN_DEBUG_ID_008         0x327
#define CAN_DEBUG_ID_009         0x328
#define CAN_DEBUG_ID_010         0x329
#define CAN_DEBUG_ID_011         0x32A

//protection in - voltages
#define CAN_DEBUG_ID_012         0x32B
#define CAN_DEBUG_ID_013         0x32C
#define CAN_DEBUG_ID_014         0x32D
#define CAN_DEBUG_ID_015         0x32E
#define CAN_DEBUG_ID_016         0x32F

//protection in - temp
#define CAN_DEBUG_ID_017         0x330
#define CAN_DEBUG_ID_018         0x331
#define CAN_DEBUG_ID_019         0x332
#define CAN_DEBUG_ID_020         0x333


//protection in - current
#define CAN_DEBUG_ID_021         0x334

#define CAN_DEBUG_ID_022         0x335
#define CAN_DEBUG_ID_023         0x336
#define CAN_DEBUG_ID_024         0x337
#define CAN_DEBUG_ID_025         0x338
#define CAN_DEBUG_ID_026         0x339
#define CAN_DEBUG_ID_027         0x33A
#define CAN_DEBUG_ID_028         0x33B
#define CAN_DEBUG_ID_029         0x33C
#define CAN_DEBUG_ID_030         0x33D
#define CAN_DEBUG_ID_031         0x33E
#define CAN_DEBUG_ID_032         0x33F

//
#define CAN_DEBUG_ID_033         0x7A1
#define CAN_DEBUG_ID_034         0x341
#define CAN_DEBUG_ID_035         0x103
#define CAN_DEBUG_ID_036         0x104
#define CAN_DEBUG_ID_037         0x6FA
#define CAN_DEBUG_ID_038         0x6FB
#define CAN_DEBUG_ID_039         0x900
#define CAN_DEBUG_ID_040         0x901
#define CAN_DEBUG_ID_042         0x7A2
#define CAN_DEBUG_ID_041         0x1FE
#define CAN_DEBUG_ID_043         0x1FF

//cc in
extern CANdataStd       CAN_ccIn_VehicleRest_Time;
extern CANdataStd       CAN_ccIn_Max_PackCapacity;
extern CANdataStd       CAN_ccIn_VehicleRest_DisCharge_Current;
extern CANdataStd       CAN_ccIn_VehicleRest_Charge_Current;

extern CANdataStd       CAN_ccIn_pOut_OVThreshold;
extern CANdataStd       CAN_ccIn_Voltage;
extern CANdataStd       CAN_ccIn_Current;

//cc out
extern CANdataStd       CAN_ccOut_Initial_Capacity_Ah; 
extern CANdataStd       CAN_ccOut_SOC_Pct; 
extern CANdataStd       CAN_ccOut_Total_Capacity_Discharged_Ah;
extern CANdataStd       CAN_ccOut_Total_Capacity_Remaining_Ah; 
extern CANdataStd       CAN_ccOut_Coulomb_Counting_State;

extern CANdataStd       CAN_ccOut_EKF_SOC_Pct; 
extern CANdataStd       CAN_ccOut_Capacity_EKF;
extern CANdataStd       CAN_ccOut_Voltage_EKF;
extern CANdataStd       CAN_ccOut_Terminal_Voltage;
//c in
extern CANdataStd      CAN_cIn_NegContactor_FB;
extern CANdataStd      CAN_cIn_PosContactor_FB;
extern CANdataStd      CAN_cIn_PreChargeContactor_FB;


extern CANdataStd      CAN_cIn_MaxImbalance;

extern CANdataStd      CAN_cIn_ThermalRunaway;
extern CANdataStd      CAN_cIn_OCC;
extern CANdataStd      CAN_cIn_OCD;
extern CANdataStd      CAN_cIn_OTC;
extern CANdataStd      CAN_cIn_OTD;
extern CANdataStd      CAN_cIn_UTC;
extern CANdataStd      CAN_cIn_UTD;
extern CANdataStd      CAN_cIn_OV;
extern CANdataStd      CAN_cIn_UV;

extern CANdataStd      CAN_cIn_contactorCutoffTime;
extern CANdataStd      CAN_cIn_preChargeContactorCurrent;
extern CANdataStd      CAN_cIn_contactorResponseTime_msec;

extern CANdataStd      CAN_cIn_contactorCommand;
extern CANdataStd      CAN_cIn_communicationFlag;
extern CANdataStd      CAN_cIn_shortCircuitFlag;
extern CANdataStd      CAN_cIn_permanentFailFlag;
extern CANdataStd      CAN_cIn_contactorErrorFlag;

//contactor out
extern CANdataStd      CAN_cOut_NegContactorState;
extern CANdataStd      CAN_cOut_PosContactorState;
extern CANdataStd      CAN_cOut_PreChargeContactorState;

//cb in 
extern CANdataStd      CAN_cbIn_balancingMinThreshold;
extern CANdataStd      CAN_cbIn_balancingMaxAllowedImbalance;
extern CANdataStd      CAN_cbIn_balancingOVProtectionThreshold;
extern CANdataStd      CAN_cbIn_balancingStateRestPeroid;
extern CANdataStd      CAN_cbIn_cellsInSeries;

//cb out
extern CANdataStd      CAN_cbOut_balancingState;

//protection input
extern CANdataStd      CAN_pIn_voltage[20];
extern CANdataStd      CAN_pIn_voltageMin;
extern CANdataStd      CAN_pIn_voltageMax;
extern CANdataStd      CAN_pIn_voltageDelta;

extern CANdataStd      CAN_pIn_temperatureMin;
extern CANdataStd      CAN_pIn_temperatureMax;
extern CANdataStd      CAN_pIn_temperatureDelta;

extern CANdataStd      CAN_pIn_current;
extern CANdataStd      CAN_pIn_OCC_WarningCurrent;
extern CANdataStd      CAN_pIn_OCC_ErrorCurrent;

// extern CANdataStd      CAN_pIn_dynamicCurrentCont;
// extern CANdataStd      CAN_pIn_dynamicCurrentInst;

extern CANdataStd      CAN_pIn_eFuseChargingLimit_A2s;
extern CANdataStd      CAN_pIn_eFuseDischargingLimit_A2s;



extern CANdataStd       CAN_ccOut_initialCapacity;
extern CANdataStd       CAN_ccOut_totalCapacityRemaining;
extern CANdataStd       CAN_ccOut_totalCapacityDischarge;
extern CANdataStd       CAN_ccOut_SOCPercentage;
extern CANdataStd       CAN_ccOut_coulombCountingState;
extern CANdataStd       CAN_ccOut_MaxUsableCapacity_mAh;
extern CANdataStd       CAN_ccOut_TotalCapacityExchange_mAh;
extern CANdataStd       CAN_ccOut_SOH_cpct;


void initializeDebugCANMatrix(void); 

    #ifdef CAN_MATRIX_V_0_3
        #define IS_CAN_TX_ID(ID)               (((ID) == CAN_TX_ID_001) || ((ID) == CAN_TX_ID_002)      ||\
                                                ((ID) == CAN_TX_ID_003) || ((ID) == CAN_TX_ID_004)      ||\
                                                ((ID) == CAN_TX_ID_005) || ((ID) == CAN_TX_ID_006)      ||\
                                                ((ID) == CAN_TX_ID_007) || ((ID) == CAN_TX_ID_008)      ||\
                                                ((ID) == CAN_TX_ID_009) || ((ID) == CAN_TX_ID_010)      ||\
                                                ((ID) == CAN_TX_ID_011) || ((ID) == CAN_TX_ID_012)      ||\
                                                ((ID) == CAN_TX_ID_013) || ((ID) == CAN_TX_ID_014)      ||\
                                                ((ID) == CAN_TX_ID_015) || ((ID) == CAN_TX_ID_016)      ||\
                                                ((ID) == CAN_TX_ID_017) || ((ID) == CAN_TX_ID_018)      ||\
                                                ((ID) == CAN_TX_ID_019) || ((ID) == CAN_TX_ID_020)      ||\
                                                ((ID) == CAN_TX_ID_021) || ((ID) == CAN_TX_ID_022)      ||\
                                                ((ID) == CAN_TX_ID_023) || ((ID) == CAN_TX_ID_CHARGER)  ||\
                                                ((ID) == CAN_DEBUG_ID_001) || ((ID) == CAN_DEBUG_ID_002)      ||\
                                                ((ID) == CAN_DEBUG_ID_003) || ((ID) == CAN_DEBUG_ID_004)      ||\
                                                ((ID) == CAN_DEBUG_ID_005) || ((ID) == CAN_DEBUG_ID_006)      ||\
                                                ((ID) == CAN_DEBUG_ID_007) || ((ID) == CAN_DEBUG_ID_008)      ||\
                                                ((ID) == CAN_DEBUG_ID_009) || ((ID) == CAN_DEBUG_ID_010)      ||\
                                                ((ID) == CAN_DEBUG_ID_011) || ((ID) == CAN_DEBUG_ID_012)      ||\
                                                ((ID) == CAN_DEBUG_ID_013) || ((ID) == CAN_DEBUG_ID_014)      ||\
                                                ((ID) == CAN_DEBUG_ID_015) || ((ID) == CAN_DEBUG_ID_016)      ||\
                                                ((ID) == CAN_DEBUG_ID_017) || ((ID) == CAN_DEBUG_ID_018)      ||\
                                                ((ID) == CAN_DEBUG_ID_019) || ((ID) == CAN_DEBUG_ID_020)      ||\
                                                ((ID) == CAN_DEBUG_ID_021) || ((ID) == CAN_DEBUG_ID_022)      ||\
                                                ((ID) == CAN_DEBUG_ID_023) || ((ID) == CAN_DEBUG_ID_024)      ||\
                                                ((ID) == CAN_DEBUG_ID_025) || ((ID) == CAN_DEBUG_ID_026)      ||\
                                                ((ID) == CAN_DEBUG_ID_027) || ((ID) == CAN_DEBUG_ID_028)      ||\
                                                ((ID) == CAN_DEBUG_ID_029) || ((ID) == CAN_DEBUG_ID_030)      ||\
                                                ((ID) == CAN_DEBUG_ID_031) || ((ID) == CAN_DEBUG_ID_032)      ||\
                                                ((ID) == CAN_DEBUG_ID_033) || ((ID) == CAN_DEBUG_ID_034)      ||\
                                                ((ID) == CAN_DEBUG_ID_042) || ((ID) == CAN_TX_ID_024)  \
                                                )
    #else

    #define IS_CAN_TX_ID(ID)                    (((ID) == CAN_TX_ID_001) || ((ID) == CAN_TX_ID_002)      ||\
                                                ((ID) == CAN_TX_ID_003) || ((ID) == CAN_TX_ID_004)      ||\
                                                ((ID) == CAN_TX_ID_005) || ((ID) == CAN_TX_ID_006)      ||\
                                                ((ID) == CAN_TX_ID_007) || ((ID) == CAN_TX_ID_008)      ||\
                                                ((ID) == CAN_TX_ID_009) || ((ID) == CAN_TX_ID_010)      ||\
                                                ((ID) == CAN_TX_ID_011) || ((ID) == CAN_TX_ID_012)      ||\
                                                ((ID) == CAN_TX_ID_013) || ((ID) == CAN_TX_ID_014)      ||\
                                                ((ID) == CAN_TX_ID_015) || ((ID) == CAN_TX_ID_016)      ||\
                                                ((ID) == CAN_TX_ID_017) || ((ID) == CAN_TX_ID_018)      ||\
                                                ((ID) == CAN_TX_ID_019) || ((ID) == CAN_TX_ID_020)      ||\
                                                ((ID) == CAN_TX_ID_021) || ((ID) == CAN_TX_ID_022)      ||\
                                                ((ID) == CAN_TX_ID_CHARGER)  ||\
                                                ((ID) == CAN_DEBUG_ID_001) || ((ID) == CAN_DEBUG_ID_002)      ||\
                                                ((ID) == CAN_DEBUG_ID_003) || ((ID) == CAN_DEBUG_ID_004)      ||\
                                                ((ID) == CAN_DEBUG_ID_005) || ((ID) == CAN_DEBUG_ID_006)      ||\
                                                ((ID) == CAN_DEBUG_ID_007) || ((ID) == CAN_DEBUG_ID_008)      ||\
                                                ((ID) == CAN_DEBUG_ID_009) || ((ID) == CAN_DEBUG_ID_010)      ||\
                                                ((ID) == CAN_DEBUG_ID_011) || ((ID) == CAN_DEBUG_ID_012)      ||\
                                                ((ID) == CAN_DEBUG_ID_013) || ((ID) == CAN_DEBUG_ID_014)      ||\
                                                ((ID) == CAN_DEBUG_ID_015) || ((ID) == CAN_DEBUG_ID_016)      ||\
                                                ((ID) == CAN_DEBUG_ID_017) || ((ID) == CAN_DEBUG_ID_018)      ||\
                                                ((ID) == CAN_DEBUG_ID_019) || ((ID) == CAN_DEBUG_ID_020)      ||\
                                                ((ID) == CAN_DEBUG_ID_021) || ((ID) == CAN_DEBUG_ID_022)      ||\
                                                ((ID) == CAN_DEBUG_ID_023) || ((ID) == CAN_DEBUG_ID_024)      ||\
                                                ((ID) == CAN_DEBUG_ID_025) || ((ID) == CAN_DEBUG_ID_026)      ||\
                                                ((ID) == CAN_DEBUG_ID_027) || ((ID) == CAN_DEBUG_ID_028)      ||\
                                                ((ID) == CAN_DEBUG_ID_029) || ((ID) == CAN_DEBUG_ID_030)      ||\
                                                ((ID) == CAN_DEBUG_ID_031) || ((ID) == CAN_DEBUG_ID_032)      ||\
                                                ((ID) == CAN_DEBUG_ID_033) || ((ID) == CAN_DEBUG_ID_034)      ||\
                                                ((ID) == CAN_DEBUG_ID_035) || ((ID) == CAN_DEBUG_ID_036)      ||\
                                                ((ID) == CAN_DEBUG_ID_037) || ((ID) == CAN_DEBUG_ID_038)      ||\
                                                ((ID) == CAN_DEBUG_ID_039) || ((ID) == CAN_DEBUG_ID_040)      ||\
                                                ((ID) == CAN_DEBUG_ID_041) || ((ID) == CAN_DEBUG_ID_041)      ||\
                                                ((ID) == CAN_DEBUG_ID_042) || ((ID) == CAN_DEBUG_ID_043)\
                                                )

    #endif
#else


    #ifdef CAN_MATRIX_V2
        #define IS_CAN_TX_ID(ID)               (((ID) == CAN_TX_ID_001) || ((ID) == CAN_TX_ID_002)      ||\
                                                ((ID) == CAN_TX_ID_003) || ((ID) == CAN_TX_ID_004)      ||\
                                                ((ID) == CAN_TX_ID_005) || ((ID) == CAN_TX_ID_006)      ||\
                                                ((ID) == CAN_TX_ID_007) || ((ID) == CAN_TX_ID_008)      ||\
                                                ((ID) == CAN_TX_ID_009) || ((ID) == CAN_TX_ID_010)      ||\
                                                ((ID) == CAN_TX_ID_011) || ((ID) == CAN_TX_ID_012)      ||\
                                                ((ID) == CAN_TX_ID_013) || ((ID) == CAN_TX_ID_014)      ||\
                                                ((ID) == CAN_TX_ID_015) || ((ID) == CAN_TX_ID_016)      ||\
                                                ((ID) == CAN_TX_ID_017) || ((ID) == CAN_TX_ID_018)      ||\
                                                ((ID) == CAN_TX_ID_019) || ((ID) == CAN_TX_ID_020)      ||\
                                                ((ID) == CAN_TX_ID_021) || ((ID) == CAN_TX_ID_022)      ||\
                                                ((ID) == CAN_TX_ID_023) || ((ID) == CAN_TX_ID_CHARGER)\
                                                )
    #else
        #define IS_CAN_TX_ID(ID)               (((ID) == CAN_TX_ID_001) || ((ID) == CAN_TX_ID_002)      ||\
                                                ((ID) == CAN_TX_ID_003) || ((ID) == CAN_TX_ID_004)      ||\
                                                ((ID) == CAN_TX_ID_005) || ((ID) == CAN_TX_ID_006)      ||\
                                                ((ID) == CAN_TX_ID_007) || ((ID) == CAN_TX_ID_008)      ||\
                                                ((ID) == CAN_TX_ID_009) || ((ID) == CAN_TX_ID_010)      ||\
                                                ((ID) == CAN_TX_ID_011) || ((ID) == CAN_TX_ID_012)      ||\
                                                ((ID) == CAN_TX_ID_013) || ((ID) == CAN_TX_ID_014)      ||\
                                                ((ID) == CAN_TX_ID_015) || ((ID) == CAN_TX_ID_016)      ||\
                                                ((ID) == CAN_TX_ID_017) || ((ID) == CAN_TX_ID_018)      ||\
                                                ((ID) == CAN_TX_ID_019) || ((ID) == CAN_TX_ID_020)      ||\
                                                ((ID) == CAN_TX_ID_021) || ((ID) == CAN_TX_ID_022)      ||\
                                                ((ID) == CAN_TX_ID_CHARGER)\
                                                )
    #endif
#endif

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t initializeCANMatrix(uint8_t canPort);

#endif
/* End of File ---------------------------------------------------------------*/

