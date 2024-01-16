/*
 * Contactors.h
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

#ifndef RTW_HEADER_Contactors_h_
#define RTW_HEADER_Contactors_h_
#ifndef Contactors_COMMON_INCLUDES_
#define Contactors_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Contactors_COMMON_INCLUDES_ */

#include "Contactors_types.h"
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
  boolean_T Flags_Detection;           /* '<S1>/AND' */
} B_Contactors_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int32_T durationCounter_1;           /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_2;           /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_e;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_i;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_h;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_p;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_2_d;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_d;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_2_e;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_1_m;         /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_2_ev;        /* '<S1>/Contactor_StateMachine' */
  int32_T durationCounter_2_en;        /* '<S1>/Contactor_StateMachine' */
  uint32_T temporalCounter_i1;         /* '<S1>/Contactor_StateMachine' */
  ContactorsFlags Memory_PreviousInput;/* '<S1>/Memory' */
  ContactorsFlags Memory1_PreviousInput;/* '<S1>/Memory1' */
  ContactorsFlags Memory2_PreviousInput;/* '<S1>/Memory2' */
  uint8_T is_ContactorOperations;      /* '<S1>/Contactor_StateMachine' */
  uint8_T ContactorWarningCounter;     /* '<S1>/Contactor_StateMachine' */
  uint8_T is_active_c1_Contactors;     /* '<S1>/Contactor_StateMachine' */
} DW_Contactors_T;

/* Invariant block signals (default storage) */
typedef struct {
  const ProtectionFlags DataTypeConversion9;/* '<S1>/Data Type Conversion9' */
} ConstB_Contactors_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  boolean_T PermanentFailRecoveryTrigger;
                                     /* '<Root>/PermanentFailRecoveryTrigger' */
  ProtectionOutput ProtectionOutput_e; /* '<Root>/ProtectionOutput' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
  uint8_T Communication_Flag;          /* '<Root>/Communication_Flag' */
  uint8_T ContactorCommand;            /* '<Root>/ContactorCommand' */
  uint8_T SC_Flag;                     /* '<Root>/SC_Flag' */
  ProtectionFlags PermanentFail_Flag;  /* '<Root>/PermanentFail_FB' */
  uint8_T ContactorError_Flag;         /* '<Root>/ContactorError_Flag' */
  ContactorsFlags PosContactors_FB;    /* '<Root>/PosContactors_FB' */
  ContactorsFlags PreChargeContactors_FB;/* '<Root>/PreChargeContactors_FB' */
  ContactorsFlags NegContactors_FB;    /* '<Root>/NegContactors_FB' */
} ExtU_Contactors_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  ContactorsFlags NegContactors;       /* '<Root>/NegContactors' */
  ContactorsFlags PreChargeContactors; /* '<Root>/PreChargeContactors' */
  ContactorsFlags PosContactors;       /* '<Root>/PosContactors' */
  ContactorsFlags ContactorsWeldWarning;/* '<Root>/ContactorsWeldWarning' */
  ProtectionFlags PermanentFaiil;      /* '<Root>/PermanentFaiil' */
  ContState ContactorsState;           /* '<Root>/ContactorsState' */
  uint8_T ExternalShortCheck;          /* '<Root>/ExternalShortCheck' */
  real_T Elapsed_TimeToOpen_msec;      /* '<Root>/Elapsed_TimeToOpen_msec' */
} ExtY_Contactors_T;

/* Real-time Model Data Structure */
struct tag_RTM_Contactors_T {
  const char_T *errorStatus;
};

/* Block signals (default storage) */
extern B_Contactors_T Contactors_B;

/* Block states (default storage) */
extern DW_Contactors_T Contactors_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Contactors_T Contactors_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Contactors_T Contactors_Y;

/* External data declarations for dependent source files */
extern const ProtectionOutput Contactors_rtZProtectionOutput;/* ProtectionOutput ground */
extern const ConstB_Contactors_T Contactors_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void Contactors_initialize(void);
extern void Contactors_step(void);
extern void Contactors_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Contactors_T *const Contactors_M;

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
 * hilite_system('BMS_Algo_Release_v10_10/Contactors')    - opens subsystem BMS_Algo_Release_v10_10/Contactors
 * hilite_system('BMS_Algo_Release_v10_10/Contactors/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_10'
 * '<S1>'   : 'BMS_Algo_Release_v10_10/Contactors'
 * '<S2>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant'
 * '<S3>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant1'
 * '<S4>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant10'
 * '<S5>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant11'
 * '<S6>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant12'
 * '<S7>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant13'
 * '<S8>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant14'
 * '<S9>'   : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant15'
 * '<S10>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant16'
 * '<S11>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant17'
 * '<S12>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant18'
 * '<S13>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant19'
 * '<S14>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant2'
 * '<S15>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant21'
 * '<S16>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant22'
 * '<S17>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant23'
 * '<S18>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant24'
 * '<S19>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant25'
 * '<S20>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant26'
 * '<S21>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant29'
 * '<S22>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant3'
 * '<S23>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant30'
 * '<S24>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant4'
 * '<S25>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant5'
 * '<S26>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant6'
 * '<S27>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant7'
 * '<S28>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant8'
 * '<S29>'  : 'BMS_Algo_Release_v10_10/Contactors/Compare To Constant9'
 * '<S30>'  : 'BMS_Algo_Release_v10_10/Contactors/Contactor_StateMachine'
 */
#endif                                 /* RTW_HEADER_Contactors_h_ */
