/*
 * ChargingAlgorithm.h
 *
 * Code generation for model "ChargingAlgorithm".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:58:57 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (12), Warning (1), Error (0)
 */

#ifndef RTW_HEADER_ChargingAlgorithm_h_
#define RTW_HEADER_ChargingAlgorithm_h_
#ifndef ChargingAlgorithm_COMMON_INCLUDES_
#define ChargingAlgorithm_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* ChargingAlgorithm_COMMON_INCLUDES_ */

#include "ChargingAlgorithm_types.h"
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
  real32_T Constant_K;                 /* '<S1>/ChargingAlgorithm' */
} B_ChargingAlgorithm_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int32_T durationCounter_1;           /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_g;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_k;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_gu;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_d;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_e;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_l;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_o;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2;           /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_f;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_b;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_f;         /* '<S1>/ChargingAlgorithm' */
  uint8_T is_c5_ChargingAlgorithm;     /* '<S1>/ChargingAlgorithm' */
  uint8_T is_SlowCharging;             /* '<S1>/ChargingAlgorithm' */
  uint8_T is_FastCharging;             /* '<S1>/ChargingAlgorithm' */
  uint8_T is_active_c5_ChargingAlgorithm;/* '<S1>/ChargingAlgorithm' */
} DW_ChargingAlgorithm_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
} ExtU_ChargingAlgorithm_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int32_T ChargingCurrentReq_mA;       /* '<Root>/ChargingCurrentReq_mA' */
  ChargeState ChargingState;           /* '<Root>/ChargingState' */
} ExtY_ChargingAlgorithm_T;

/* Real-time Model Data Structure */
struct tag_RTM_ChargingAlgorithm_T {
  const char_T *errorStatus;
};

/* Block signals (default storage) */
extern B_ChargingAlgorithm_T ChargingAlgorithm_B;

/* Block states (default storage) */
extern DW_ChargingAlgorithm_T ChargingAlgorithm_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_ChargingAlgorithm_T ChargingAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_ChargingAlgorithm_T ChargingAlgorithm_Y;

/* Model entry point functions */
extern void ChargingAlgorithm_initialize(void);
extern void ChargingAlgorithm_step(void);
extern void ChargingAlgorithm_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ChargingAlgorithm_T *const ChargingAlgorithm_M;

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
 * hilite_system('BMS_Algo_Release_v10_10/ChargingAlgorithm')    - opens subsystem BMS_Algo_Release_v10_10/ChargingAlgorithm
 * hilite_system('BMS_Algo_Release_v10_10/ChargingAlgorithm/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_10'
 * '<S1>'   : 'BMS_Algo_Release_v10_10/ChargingAlgorithm'
 * '<S2>'   : 'BMS_Algo_Release_v10_10/ChargingAlgorithm/ChargingAlgorithm'
 */
#endif                                 /* RTW_HEADER_ChargingAlgorithm_h_ */
