/*
 * Protection.h
 *
 * Code generation for model "Protection".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:48:22 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (12), Warning (1), Error (0)
 */

#ifndef RTW_HEADER_Protection_h_
#define RTW_HEADER_Protection_h_
#ifndef Protection_COMMON_INCLUDES_
#define Protection_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Protection_COMMON_INCLUDES_ */

#include "Protection_types.h"
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
  int32_T Diff;                        /* '<S3>/Diff' */
  int32_T Diff_n;                      /* '<S4>/Diff' */
  TempState TempState2;      /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  TempState TempState1;      /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags OCCFlag;   /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags OCDFlag;   /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags OVFlag;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags UVFlag;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags FlagGroup1;/* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags FlagGroup2;/* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags ThermalRunaway;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags TemperatureGradient;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags HighImbalanceFlag;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags ShortCircuitDetect;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags SuddenVoltageDrop;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags ErrorDetect;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags TempOverallState;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags eFuseDischargeFlag;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  ProtectionFlags eFuseChargeFlag;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
} B_Protection_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int32_T UD_DSTATE;                   /* '<S3>/UD' */
  int32_T UD_DSTATE_p;                 /* '<S4>/UD' */
  real32_T DCL_Current_A;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T OCC_Error_mA;      /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T OCC_Warning_mA;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T OCD_Error_mA;      /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T OCD_Warning_mA;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T Protections_OV_Recovery_mV;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T Protections_OV_Warning_mV;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T Protections_OV_Error_mV;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1; /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_b;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_c;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2; /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_k;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_k;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_d;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_n;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_a;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_g;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_c;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_f;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_l;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_3; /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_e;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ag;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_o;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_o2;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_d4;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_n;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_h;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_k0;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_od;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_nq;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_kq;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_b;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_j;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_oz;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_cg;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_i;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_e1;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_f;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ga;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ec;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_h;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_p;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ht;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_fo;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_bu;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_i0;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_pi;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_l;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_cp;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_e;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_p;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_bq;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_nw;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_lr;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_j;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_3_c;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_m;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_cc;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_4; /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_cpz;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_a1;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_fy;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_l2;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_i;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_la;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_lc;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_kt;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_cm;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_oa;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_o0;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_j5;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_k3;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ev;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_ck;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_jo;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_ie;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_jr;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_nn;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_ke;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_3_a;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ok;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_hd;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_4_l;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_lm;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_bw;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_a;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_az;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_i5;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_1_ab;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_hg;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  int32_T durationCounter_2_m;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint32_T temporalCounter_i1;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_Current_Protection;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_Voltage_Protection;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_ThermalRunaway; /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_SuddenVoltageDrop;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_TemperatureGradient;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_OverallState;   /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_HighImbalanceFlag;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_ErrorDetect;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_ShortCircuitDetect;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_OVThresholds_Selection;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_Temperature_Protection_Group;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_Temperature_Protection_Gro_n;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T eFuseMeltCount;    /* '<S1>/CurrentVoltageTemperature_Statemachine' */
  uint8_T is_active_c2_Protection;
                             /* '<S1>/CurrentVoltageTemperature_Statemachine' */
} DW_Protection_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
} ExtU_Protection_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  ProtectionOutput ProtectionOutput_e; /* '<Root>/ProtectionOutput' */
} ExtY_Protection_T;

/* Real-time Model Data Structure */
struct tag_RTM_Protection_T {
  const char_T *errorStatus;
};

/* Block signals (default storage) */
extern B_Protection_T Protection_B;

/* Block states (default storage) */
extern DW_Protection_T Protection_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Protection_T Protection_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Protection_T Protection_Y;

/* External data declarations for dependent source files */
extern const ProtectionOutput Protection_rtZProtectionOutput;/* ProtectionOutput ground */

/* Model entry point functions */
extern void Protection_initialize(void);
extern void Protection_step(void);
extern void Protection_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Protection_T *const Protection_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S3>/Data Type Duplicate' : Unused code path elimination
 * Block '<S4>/Data Type Duplicate' : Unused code path elimination
 */

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
 * hilite_system('BMS_Algo_Release_v10_10/Protection')    - opens subsystem BMS_Algo_Release_v10_10/Protection
 * hilite_system('BMS_Algo_Release_v10_10/Protection/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_10'
 * '<S1>'   : 'BMS_Algo_Release_v10_10/Protection'
 * '<S2>'   : 'BMS_Algo_Release_v10_10/Protection/CurrentVoltageTemperature_Statemachine'
 * '<S3>'   : 'BMS_Algo_Release_v10_10/Protection/Discrete Derivative'
 * '<S4>'   : 'BMS_Algo_Release_v10_10/Protection/Discrete Derivative1'
 */
#endif                                 /* RTW_HEADER_Protection_h_ */
