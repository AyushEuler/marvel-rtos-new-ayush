/*
 * Contactors_types.h
 *
 * Code generation for model "Contactors".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:51:18 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: All passed
 */

#ifndef RTW_HEADER_Contactors_types_h_
#define RTW_HEADER_Contactors_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_Temperature_
#define DEFINED_TYPEDEF_FOR_Temperature_

typedef struct {
  int32_T Protections_OTC_Warning_C;
  int32_T Protections_OTC_Error_C;
  int32_T Protections_OTC_Recovery_C;
  int32_T Protections_OTC_NoErrorTimeout_msec;
  int32_T Protections_OTC_WarningTimeout_msec;
  int32_T Protections_OTC_ErrorTimeout_msec;
  int32_T Protections_OTC_RecoveryTimeout_msec;
  int32_T Protections_OTD_Warning_C;
  int32_T Protections_OTD_Error_C;
  int32_T Protections_OTD_Recovery_C;
  int32_T Protections_OTD_NoErrorTimeout_msec;
  int32_T Protections_OTD_WarningTimeout_msec;
  int32_T Protections_OTD_ErrorTimeout_msec;
  int32_T Protections_OTD_RecoveryTimeout_msec;
  int32_T Protections_UTC_Warning_C;
  int32_T Protections_UTC_Error_C;
  int32_T Protections_UTC_Recovery_C;
  int32_T Protections_UTC_NoErrorTimeout_msec;
  int32_T Protections_UTC_WarningTimeout_msec;
  int32_T Protections_UTC_ErrorTimeout_msec;
  int32_T Protections_UTC_RecoveryTimeout_msec;
  int32_T Protections_UTD_Warning_C;
  int32_T Protections_UTD_Error_C;
  int32_T Protections_UTD_Recovery_C;
  int32_T Protections_UTD_NoErrorTimeout_msec;
  int32_T Protections_UTD_WarningTimeout_msec;
  int32_T Protections_UTD_ErrorTimeout_msec;
  int32_T Protections_UTD_RecoveryTimeout_msec;
} Temperature;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ChargeAlgorithm_
#define DEFINED_TYPEDEF_FOR_ChargeAlgorithm_

typedef struct {
  uint32_T SOC_forCVCharge_cpct;
  uint32_T BatteryRevivalVoltage_mV;
  uint32_T CVTransitionVoltage_mV;
  uint32_T TransitionTimeout_msec;
  uint32_T ChargersCurrentLimit_A;
} ChargeAlgorithm;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SlowChargeAlgorithm_
#define DEFINED_TYPEDEF_FOR_SlowChargeAlgorithm_

typedef struct {
  real32_T NoCharge_K;
  real32_T BatteryRevival_K;
  real32_T CC_K;
  real32_T CC_Balancing_K;
  real32_T CV_K;
} SlowChargeAlgorithm;

#endif

#ifndef DEFINED_TYPEDEF_FOR_FastChargeAlgorithm_
#define DEFINED_TYPEDEF_FOR_FastChargeAlgorithm_

typedef struct {
  real32_T BatteryRevival_K;
  real32_T CC_K;
  real32_T CC_Balancing_K;
  real32_T CV_K;
} FastChargeAlgorithm;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ThresholdsBus_
#define DEFINED_TYPEDEF_FOR_ThresholdsBus_

typedef struct {
  int32_T Protections_MaxOperatingTemp_C;
  int32_T Protections_MaxAllowedTempChange_C;
  int32_T Protections_MaxAllowedTempGradientGroup1_C;
  int32_T Protections_MaxAllowedTempGradientGroup2_C;
  int32_T ChargeDetectionThreshold_mA;
  int32_T NormalModeTimeout_msec;
  int32_T PowerSaverModeTimeout_msec;
  int32_T FaultsModeTimeout_msec;
  int32_T ServiceModeTimeout_msec;
  int32_T i2tTimeout_msec;
  int32_T DynContTimeout_msec;
  int32_T DynamicCurrentThresholdsGain;
  int32_T TempGroupTimeout;
  uint8_T PermanentFailCounts_eFuse;
  int32_T i2t_CalculationTimeout_msec;
  int32_T CurrentChangeTimeout_msec;
  int32_T eFuseMeltingTimeout_msec;
  int32_T Protections_OCC_Warning_mA;
  int32_T Protections_OCC_Error_mA;
  int32_T RecoveryProtections_OCC_Warning_mA;
  int32_T RecoveryProtections_OCC_Error_mA;
  int32_T Protections_OCC_NoErrorTimeout_msec;
  int32_T Protections_OCC_WarningTimeout_msec;
  int32_T Protections_OCC_ErrorTimeout_msec;
  int32_T Protections_OCC_RecoveryTimeout_msec;
  int32_T Protections_OCD_Warning_mA;
  int32_T Protections_OCD_Error_mA;
  int32_T RecoveryProtections_OCD_Warning_mA;
  int32_T RecoveryProtections_OCD_Error_mA;
  int32_T Protections_OCD_NoErrorTimeout_msec;
  int32_T Protections_OCD_WarningTimeout_msec;
  int32_T Protections_OCD_ErrorTimeout_msec;
  int32_T Protections_OCD_RecoveryTimeout_msec;
  Temperature Temperature[5];
  int32_T TempGroupTransition_msec[5];
  int32_T TimerCounter_msec[5];
  int32_T Protections_SlowCh_OV_Warning_mV;
  int32_T Protections_SlowCh_OV_Error_mV;
  int32_T Protections_SlowCh_OV_Recovery_mV;
  int32_T Protections_FastCh_OV_Warning_mV;
  int32_T Protections_FastCh_OV_Error_mV;
  int32_T Protections_FastCh_OV_Recovery_mV;
  int32_T Protections_OV_NoErrorTimeout_msec;
  int32_T Protections_OV_WarningTimeout_msec;
  int32_T Protections_OV_ErrorTimeout_msec;
  int32_T Protections_OV_RecoveryTimeout_msec;
  int32_T Protections_HighImbalanceError_mV;
  int32_T Protections_UV_Warning_mV;
  int32_T Protections_UV_Error_mV;
  int32_T Protections_UV_Recovery_mV;
  int32_T Protections_UV_NoErrorTimeout_msec;
  int32_T Protections_UV_WarningTimeout_msec;
  int32_T Protections_UV_ErrorTimeout_msec;
  int32_T Protections_UV_RecoveryTimeout_msec;
  int32_T Protections_MinOpearatingVolt_mV;
  int32_T Protections_ShortCircuitCurrent_mA;
  int32_T Protections_SuddenVoltageDrop_mV_by_time;
  uint16_T ThermalRunawayWarningTimeout_msec;
  uint16_T ThermalRunawayErrorTimeout_msec;
  uint16_T TemperatureGradientTimeout_msec;
  uint16_T HighImbalanceFlagTimeout_msec;
  uint16_T ShortCircuitDetect_msec;
  uint16_T SuddenVoltageDropTimeout_msec;
  int32_T Balancing_Min_Thr_mV;
  int32_T Balancing_StateRestPeriod_msec;
  int32_T Balancing_MaxAllowedImb_mV;
  int32_T Balancing_DefaultBalancingTimeOn_msec;
  int8_T BalancingCurrentDerateActivationTemp_C;
  int32_T BalancingCurrentDerateCheckTime_msec;
  int32_T Balancing_SlowCh_OVThr_mV;
  int32_T Balancing_FastCh_OVThr_mV;
  int32_T Balancing_BalancingPeriod_msec;
  int32_T ExternalShortCheckTime_msec;
  int8_T ExternalShortLimit;
  int8_T ContWeldCheckLimit;
  int32_T ContCutoffTime_msec;
  int32_T ContCurrent_mA;
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
  boolean_T AdditionalProtectionEn;
  boolean_T HighImbalanceFlagEn;
  int32_T BMS_Timeout_msec;
  int32_T ContactorsCheck_Timeout_msec;
  int32_T VCU_Timeout_msec;
  int32_T MosfetsPreChargeCurrentLimit_mA;
  ChargeAlgorithm ChargeAlgorithm;
  SlowChargeAlgorithm SlowChargeAlgorithm;
  FastChargeAlgorithm FastChargeAlgorithm;
  int32_T targetVoltageSlowCharging;
  int32_T targetVoltagesFastCharging;
  uint32_T HardwareError_Timeout_msec;
} ThresholdsBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ProtectionFlags_
#define DEFINED_TYPEDEF_FOR_ProtectionFlags_

typedef enum {
  NoError = 0,                         /* Default value */
  Warning,
  Error,
  Recovery,
  PermanentFail
} ProtectionFlags;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TempState_
#define DEFINED_TYPEDEF_FOR_TempState_

typedef enum {
  NoErr = 0,                           /* Default value */
  UTC,
  UTD,
  OTC,
  OTD
} TempState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ProtectionOutput_
#define DEFINED_TYPEDEF_FOR_ProtectionOutput_

typedef struct {
  ProtectionFlags ThermalRunaway;
  ProtectionFlags TemperatureGradient;
  ProtectionFlags HighImbalanceFlag;
  ProtectionFlags ShortCircuitDetect;
  ProtectionFlags SuddenVoltageDrop;
  ProtectionFlags OV;
  ProtectionFlags UV;
  ProtectionFlags OCC;
  ProtectionFlags OCD;
  real32_T i2t_Calculated_A2sec;
  ProtectionFlags eFuseChargeFlag;
  ProtectionFlags eFuseDischargeFlag;
  TempState TempState1;
  TempState TempState2;
  ProtectionFlags FlagGroup1;
  ProtectionFlags FlagGroup2;
  ProtectionFlags TempOverallState;
  ProtectionFlags ErrorDetect;
} ProtectionOutput;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VoltageSenseBus_
#define DEFINED_TYPEDEF_FOR_VoltageSenseBus_

typedef struct {
  int32_T Voltages_mV[20];
  int32_T mV_min;
  int32_T mV_max;
  int32_T VoltageDelta_mV;
} VoltageSenseBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TemperatureSenseBus_
#define DEFINED_TYPEDEF_FOR_TemperatureSenseBus_

typedef struct {
  int16_T Temperatures_C[7];
  int16_T T_min;
  int16_T T_max;
  int16_T TemperatureDelta_C;
} TemperatureSenseBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VCU_
#define DEFINED_TYPEDEF_FOR_VCU_

typedef struct {
  boolean_T SlowCharge;
  boolean_T FastCharge;
  boolean_T isChargerConnected;
  boolean_T TransmissionFail;
  boolean_T VCUPermanentFail;
  boolean_T ServiceMode;
  boolean_T BalanceEn_atRest;
  boolean_T EmergencyDisconnectEn;
  uint32_T ChargersCurrentLimit_A;
} VCU;

#endif

#ifndef DEFINED_TYPEDEF_FOR_DataPipelineBus_
#define DEFINED_TYPEDEF_FOR_DataPipelineBus_

typedef struct {
  int32_T Current_mA;
  VoltageSenseBus VoltageSenseBus;
  int16_T MaxTemperature_C;
  int16_T DeltaTemperature_C;
  int16_T MinTemperature_C;
  int16_T DeltaTemperatureGroup1_C;
  int16_T DeltaTemperatureGroup2_C;
  TemperatureSenseBus TemperatureSenseBus[2];
  uint8_T isChargeEn;
  boolean_T isDynamicCurrentEn;
  uint8_T nCells_series;
  int8_T nCells_parallel;
  int32_T DCL_ChargingCurrent_A;
  int32_T DCL_DisChargingCurrent_A;
  int32_T i2t_ChargingCurrent_A2sec;
  int32_T i2t_DisChargingCurrent_A2sec;
  real32_T LoopTimer_SOC_msec;
  VCU VCU;
} DataPipelineBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ContactorsFlags_
#define DEFINED_TYPEDEF_FOR_ContactorsFlags_

typedef enum {
  Open = 0,                            /* Default value */
  Closed,
  None
} ContactorsFlags;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ContState_
#define DEFINED_TYPEDEF_FOR_ContState_

typedef enum {
  OpenSubstate = 0,                    /* Default value */
  PreCharging_NegContactorClosed,
  PreCharging_PreChargeClosed,
  PreCharging_PosContactorClosed,
  PreCharging_PreChargeOpen,
  ClosedSubstate,
  OpeningSubstate,
  ExternalShortCircuit,
  PermanentFailure
} ContState;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Contactors_T RT_MODEL_Contactors_T;

#endif                                 /* RTW_HEADER_Contactors_types_h_ */
