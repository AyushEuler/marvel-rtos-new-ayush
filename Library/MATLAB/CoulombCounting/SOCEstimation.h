/*
 * SOCEstimation.h
 *
 * Code generation for model "SOCEstimation".
 *
 * Model version              : 4.1082
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Tue Dec 19 16:27:05 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (10), Warnings (3), Error (0)
 */

#ifndef RTW_HEADER_SOCEstimation_h_
#define RTW_HEADER_SOCEstimation_h_
#ifndef SOCEstimation_COMMON_INCLUDES_
#define SOCEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SOCEstimation_COMMON_INCLUDES_ */

#include "SOCEstimation_types.h"
#include "VoltagemV_Vs_CapacitymAh.h"
#include "UsableCapacity_mAh.h"
#include "SOC_WriteToFlash.h"
#include "SOC_ReadFromFlash.h"
#include "rt_zcfcn.h"
#include <stddef.h>
#include <string.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  real_T CalcEnable;                   /* '<S8>/SOH_Calibration' */
  uint32_T DataTypeConversion;         /* '<S17>/Data Type Conversion' */
  uint32_T Final_InstalledCapacity_mAh;/* '<S1>/Chart' */
  real32_T Final_IntegratedCurrent;    /* '<S17>/Chart' */
  real32_T IntegratedCurrent;          /* '<S7>/Chart' */
  real32_T IntegratedCurrent_h;        /* '<S3>/Chart1' */
  int32_T Initial_Capacity_mAh;        /* '<S1>/SOC_Calibration' */
  int32_T TotalCapacityRemains_mAh;    /* '<S3>/Chart' */
  B_VoltagemV_Vs_CapacitymAh_T sf_Voltage_Vs_Capacity;/* '<S1>/Voltage_Vs_Capacity' */
  B_UsableCapacity_mAh_SOCEst_l_T sf_UsableCapacity_mAh_m;/* '<S17>/UsableCapacity_mAh' */
  B_UsableCapacity_mAh_SOCEstim_T sf_UsableCapacity_mAh;/* '<S7>/UsableCapacity_mAh' */
  B_SOC_WriteToFlash_SOCEstimat_T sf_SOC_WriteToFlash;/* '<S1>/SOC_WriteToFlash' */
  B_SOC_ReadFromFlash_SOCEstima_T sf_SOC_ReadFromFlash;/* '<S1>/SOC_ReadFromFlash' */
} B_SOCEstimation_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real32_T DiscreteTimeIntegrator2_DSTATE;/* '<S3>/Discrete-Time Integrator2' */
  int32_T DiscreteTimeIntegrator_DSTATE;/* '<S7>/Discrete-Time Integrator' */
  real32_T IntegratedCurrent;          /* '<S17>/Chart' */
  int32_T SOC_pct;                     /* '<S1>/Data Store Memory1' */
  int32_T CapacityRemains_mAh;         /* '<S1>/Data Store Memory' */
  int32_T durationCounter_1;           /* '<S17>/Chart' */
  int32_T durationCounter_1_i;         /* '<S17>/Chart' */
  int32_T durationCounter_1_o;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2;           /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_j;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_k;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_m;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_d;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_p;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_f;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_1_dm;        /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_n;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_g;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_a;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_i;         /* '<S8>/SOH_Calibration' */
  int32_T durationCounter_2_d;         /* '<S8>/SOH_Calibration' */
  int32_T TransitionCapacity_mAh;      /* '<S1>/SOC_Calibration' */
  int32_T durationCounter_1_g;         /* '<S1>/SOC_Calibration' */
  int32_T durationCounter_2_e;         /* '<S1>/SOC_Calibration' */
  int32_T durationCounter_1_f4;        /* '<S1>/Chart' */
  uint32_T temporalCounter_i1;         /* '<S1>/SOC_Calibration' */
  int8_T DiscreteTimeIntegrator2_PrevRes;/* '<S3>/Discrete-Time Integrator2' */
  uint8_T is_c22_SOCEstimation;        /* '<S17>/Chart' */
  uint8_T is_active_c22_SOCEstimation; /* '<S17>/Chart' */
  uint8_T is_c21_SOCEstimation;        /* '<S8>/SOH_Calibration' */
  uint8_T is_active_c21_SOCEstimation; /* '<S8>/SOH_Calibration' */
  uint8_T is_active_c19_SOCEstimation; /* '<S7>/Chart' */
  uint8_T is_c15_SOCEstimation;        /* '<S1>/SOC_Calibration' */
  uint8_T is_active_c15_SOCEstimation; /* '<S1>/SOC_Calibration' */
  uint8_T is_c14_SOCEstimation;        /* '<S3>/Chart1' */
  uint8_T is_active_c14_SOCEstimation; /* '<S3>/Chart1' */
  uint8_T is_c27_SOCEstimation;        /* '<S3>/Chart' */
  uint8_T is_active_c27_SOCEstimation; /* '<S3>/Chart' */
  uint8_T is_c25_SOCEstimation;        /* '<S1>/Chart' */
  uint8_T is_active_c25_SOCEstimation; /* '<S1>/Chart' */
} DW_SOCEstimation_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Subsystem_Trig_ZCE;       /* '<S1>/Subsystem' */
} PrevZCX_SOCEstimation_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ProtectionOutput ProtectionOutput_e; /* '<Root>/ProtectionOutput' */
  int32_T Current_mA;                  /* '<Root>/Current_mA' */
  int32_T Voltage_mV;                  /* '<Root>/Voltage_mV' */
  CC_InputsBus CC_Inputs;              /* '<Root>/CC_Inputs' */
  CellBalancingOutputBus CellBalancingOutput;/* '<Root>/CellBalancingOutput' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
} ExtU_SOCEstimation_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  CC_OutputsBus CC_Outputs;            /* '<Root>/CC_Outputs' */
  int32_T SOC_FromFlash;               /* '<Root>/SOC_FromFlash' */
  real_T SOHCalibrationCycleCompletion;
                                    /* '<Root>/SOHCalibrationCycleCompletion' */
} ExtY_SOCEstimation_T;

/* Real-time Model Data Structure */
struct tag_RTM_SOCEstimation_T {
  const char_T *errorStatus;
};

/* Block signals (default storage) */
extern B_SOCEstimation_T SOCEstimation_B;

/* Block states (default storage) */
extern DW_SOCEstimation_T SOCEstimation_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_SOCEstimation_T SOCEstimation_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_SOCEstimation_T SOCEstimation_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SOCEstimation_T SOCEstimation_Y;

/* External data declarations for dependent source files */
extern const ProtectionOutput SOCEstimation_rtZProtectionOutp;/* ProtectionOutput ground */
extern const CellBalancingOutputBus SOCEstimation_rtZCellBalancingO;/* CellBalancingOutputBus ground */
extern const CC_OutputsBus SOCEstimation_rtZCC_OutputsBus;/* CC_OutputsBus ground */

/* Model entry point functions */
extern void SOCEstimation_initialize(void);
extern void SOCEstimation_step(void);
extern void SOCEstimation_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SOCEstimation_T *const SOCEstimation_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Display' : Unused code path elimination
 * Block '<S1>/Display1' : Unused code path elimination
 * Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion
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
 * hilite_system('BMS_Algo_Release_v10_7/SOCEstimation')    - opens subsystem BMS_Algo_Release_v10_7/SOCEstimation
 * hilite_system('BMS_Algo_Release_v10_7/SOCEstimation/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_7'
 * '<S1>'   : 'BMS_Algo_Release_v10_7/SOCEstimation'
 * '<S2>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/Chart'
 * '<S3>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/CoulombCounter'
 * '<S4>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/SOC_Calibration'
 * '<S5>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/SOC_ReadFromFlash'
 * '<S6>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/SOC_WriteToFlash'
 * '<S7>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calculation'
 * '<S8>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calibration'
 * '<S9>'   : 'BMS_Algo_Release_v10_7/SOCEstimation/Subsystem'
 * '<S10>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/Voltage_Vs_Capacity'
 * '<S11>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/powergui'
 * '<S12>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/CoulombCounter/Chart'
 * '<S13>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/CoulombCounter/Chart1'
 * '<S14>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calculation/Chart'
 * '<S15>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calculation/UsableCapacity_mAh'
 * '<S16>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calibration/SOH_Calibration'
 * '<S17>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calibration/Subsystem'
 * '<S18>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calibration/Subsystem/Chart'
 * '<S19>'  : 'BMS_Algo_Release_v10_7/SOCEstimation/SOH_Calibration/Subsystem/UsableCapacity_mAh'
 */
#endif                                 /* RTW_HEADER_SOCEstimation_h_ */
