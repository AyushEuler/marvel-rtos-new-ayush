/*
 * DataPipeline.h
 *
 * Code generation for model "DataPipeline".
 *
 * Model version              : 4.1075
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Mon Dec 18 14:37:59 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_DataPipeline_h_
#define RTW_HEADER_DataPipeline_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

#include "DataPipeline_types.h"
#include "MinimumFunction.h"
#include "MaximumFunction.h"
#include "I2t_perParallelCell.h"
#include "DynamicCurrentLimits_perParallelCell.h"
#include "AverageFunction.h"
#include "multiword_types.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  B_MinimumFunction_DataPipelin_T sf_MinimumFunction;/* '<S1>/MinimumFunction' */
  B_MaximumFunction_DataPipelin_T sf_MaximumFunction;/* '<S1>/MaximumFunction' */
  B_I2t_perParallelCell_DataPip_T sf_I2t_perParallelCell;/* '<S1>/I2t_perParallelCell' */
  B_DynamicCurrentLimits_perPar_T sf_DynamicCurrentLimits_perPara;
                               /* '<S1>/DynamicCurrentLimits_perParallelCell' */
  B_AverageFunction_DataPipelin_T sf_AverageFunction;/* '<S1>/AverageFunction' */
} B_DataPipeline_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int16_T Mean_AccVal;                 /* '<S1>/Mean' */
  int16_T Mean1_AccVal;                /* '<S1>/Mean1' */
} DW_DataPipeline_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: CapacityFactor)
   * Referenced by:
   *   '<S1>/Gain6'
   *   '<S1>/Gain7'
   */
  int64m_T pooled1;
} ConstP_DataPipeline_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  int16_T Temperatures2_C[7];          /* '<Root>/Temperatures2_C' */
  int16_T Temperatures1_C[7];          /* '<Root>/Temperatures1_C' */
  int32_T VoltageSense[20];            /* '<Root>/VoltageSense_mV' */
  int32_T CurrentSense;                /* '<Root>/CurrentSense_mA' */
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  int32_T SOC;                         /* '<Root>/SOC' */
  boolean_T isDynamicCurrentEn;        /* '<Root>/isDynamicCurrentEn' */
  uint8_T nCells_series;               /* '<Root>/nCells_Series' */
  int8_T nCells_parallel;              /* '<Root>/nCells_Parallel' */
  boolean_T TemperatureSelectionforDynamicC;
                            /* '<Root>/TemperatureSelectionforDynamicCurrent' */
  real_T nCelltemperatureGroups;       /* '<Root>/nCelltemperatureGroups' */
  boolean_T SlowCharge;                /* '<Root>/SlowCharge' */
  boolean_T FastCharge;                /* '<Root>/FastCharge' */
  boolean_T isChargerConnected;        /* '<Root>/isChargerConnected' */
  boolean_T TransmissionFail;          /* '<Root>/TransmissionFail' */
  boolean_T VCUPermanentFail;          /* '<Root>/VCUPermanentFail' */
  boolean_T ServiceMode;               /* '<Root>/ServiceMode' */
  boolean_T BalanceEn_atRest;          /* '<Root>/BalanceEn_atRest' */
  real32_T LoopTimer_SOC_msec;         /* '<Root>/LoopTimer_SOC_msec' */
  boolean_T EmergencyDisconnectEn;     /* '<Root>/EmergencyDisconnectEn' */
  uint32_T ChargersCurrentLimit_A;     /* '<Root>/ChargersCurrentLimit_A' */
} ExtU_DataPipeline_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  DataPipelineBus DataPipelineb;       /* '<Root>/DataPipelineb' */
} ExtY_DataPipeline_T;

/* Parameters (default storage) */
struct P_DataPipeline_T_ {
  int8_T nCells_parallel;              /* Variable: nCells_parallel
                                        * Referenced by:
                                        *   '<S1>/Gain1'
                                        *   '<S1>/Gain2'
                                        *   '<S1>/Gain3'
                                        *   '<S1>/Gain5'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_DataPipeline_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
extern P_DataPipeline_T DataPipeline_P;

/* Block signals (default storage) */
extern B_DataPipeline_T DataPipeline_B;

/* Block states (default storage) */
extern DW_DataPipeline_T DataPipeline_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_DataPipeline_T DataPipeline_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_DataPipeline_T DataPipeline_Y;

/* External data declarations for dependent source files */
extern const DataPipelineBus DataPipeline_rtZDataPipelineBus;/* DataPipelineBus ground */

/* Constant parameters (default storage) */
extern const ConstP_DataPipeline_T DataPipeline_ConstP;

/* Model entry point functions */
extern void DataPipeline_initialize(void);
extern void DataPipeline_step(void);
extern void DataPipeline_terminate(void);

/* Real-time Model object */
extern RT_MODEL_DataPipeline_T *const DataPipeline_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('BMS_Algo_Release_v10_6/DataPipeline')    - opens subsystem BMS_Algo_Release_v10_6/DataPipeline
 * hilite_system('BMS_Algo_Release_v10_6/DataPipeline/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_6'
 * '<S1>'   : 'BMS_Algo_Release_v10_6/DataPipeline'
 * '<S2>'   : 'BMS_Algo_Release_v10_6/DataPipeline/AverageFunction'
 * '<S3>'   : 'BMS_Algo_Release_v10_6/DataPipeline/DynamicCurrentLimits_perParallelCell'
 * '<S4>'   : 'BMS_Algo_Release_v10_6/DataPipeline/I2t_perParallelCell'
 * '<S5>'   : 'BMS_Algo_Release_v10_6/DataPipeline/MaximumFunction'
 * '<S6>'   : 'BMS_Algo_Release_v10_6/DataPipeline/MinimumFunction'
 */
#endif                                 /* RTW_HEADER_DataPipeline_h_ */
