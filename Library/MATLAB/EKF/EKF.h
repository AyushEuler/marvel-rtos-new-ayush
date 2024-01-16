/*
 * EKF.h
 *
 * Code generation for model "EKF".
 *
 * Model version              : 1.103
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Fri May 19 14:35:15 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (10), Warnings (3), Error (0)
 */

#ifndef RTW_HEADER_EKF_h_
#define RTW_HEADER_EKF_h_
#ifndef EKF_COMMON_INCLUDES_
#define EKF_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* EKF_COMMON_INCLUDES_ */

#include "EKF_types.h"
#include <string.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T SOC_EKF;                      /* '<S1>/Gain' */
  real_T CapacityRemians_mAh;          /* '<S2>/Data Store Read' */
  real_T Source;                       /* '<S9>/Source' */
  real_T Memory;                       /* '<S7>/Memory' */
  real_T Memory1;                      /* '<S7>/Memory1' */
  real_T Switch;                       /* '<S7>/Switch' */
  real_T Total_CapacityRemains_mAh;    /* '<S2>/Saturation' */
  real_T Clock;                        /* '<S12>/Clock' */
  real_T V1;                           /* '<S19>/Unit Delay' */
  real_T Memory1_k;                    /* '<S19>/Memory1' */
  real_T V1_i;                         /* '<S19>/Add1' */
  real_T V1Vs;                         /* '<S3>/Add' */
  real_T V1Vs_b;                       /* '<S3>/Add1' */
  real_T Add2;                         /* '<S3>/Add2' */
  real_T Product;                      /* '<S3>/Product' */
  real_T Capacity_EKF_c;               /* '<S1>/EKF_Function' */
  boolean_T DataTypeConversion2;       /* '<S11>/Data Type Conversion2' */
  boolean_T Memory_m;                  /* '<S11>/Memory' */
  boolean_T LogicalOperator1;          /* '<S11>/Logical Operator1' */
  boolean_T RelationalOperator1;       /* '<S14>/Relational Operator1' */
  boolean_T RelationalOperator1_j;     /* '<S13>/Relational Operator1' */
} B_EKF_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
  real_T UnitDelay_DSTATE_l;           /* '<S19>/Unit Delay' */
  real_T Memory_PreviousInput;         /* '<S7>/Memory' */
  real_T Memory1_PreviousInput;        /* '<S7>/Memory1' */
  real_T CapacityRemains_mAh;          /* '<S2>/Data Store Memory' */
  real_T Memory1_PreviousInput_m;      /* '<S19>/Memory1' */
  real_T P_Old[4];                     /* '<S1>/EKF_Function' */
  int32_T durationCounter_1;           /* '<S2>/StateMachine' */
  int32_T durationCounter_1_f;         /* '<S2>/StateMachine' */
  int32_T durationCounter_1_fy;        /* '<S2>/StateMachine' */
  uint8_T is_c3_EKF;                   /* '<S2>/StateMachine' */
  uint8_T is_active_c3_EKF;            /* '<S2>/StateMachine' */
  boolean_T Memory_PreviousInput_a;    /* '<S11>/Memory' */
  boolean_T POSITIVEEdge_MODE;         /* '<S11>/POSITIVE Edge' */
  boolean_T NEGATIVEEdge_MODE;         /* '<S11>/NEGATIVE Edge' */
} DW_EKF_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState TriggeredSubsystem_Trig_ZCE;/* '<S12>/Triggered Subsystem' */
} PrevZCX_EKF_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T MultiportSwitch[2];     /* '<S11>/Multiport Switch' */
} ConstB_EKF_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: Capacity_mAh
   * Referenced by: '<S9>/Source'
   */
  real_T Source_tableData[43];

  /* Expression: Voltage_mV
   * Referenced by: '<S9>/Source'
   */
  real_T Source_bp01Data[43];

  /* Expression: [0.00333
     0.00333
     0.00111
     0.00076
     0.00056
     0.00542
     0.00061
     0.00078
     0.00112
     0.00042
     0.00041
     0.00046
     0.00051
     0.00050
     0.00068
     0.00034
     0.00040
     0.00052
     0.00060
     0.00065
     0.00068
     0.00084
     0.00112
     0.00116
     0.00196
     0.00106
     0.00106]
   * Referenced by: '<S23>/Source'
   */
  real_T Source_tableData_e[27];

  /* Pooled Parameter (Expression: [0
     2
     4
     6
     8
     10
     15
     20
     25
     30
     35
     40
     45
     50
     55
     60
     65
     70
     75
     80
     85
     90
     92
     94
     96
     98
     100])
   * Referenced by:
   *   '<S21>/Source'
   *   '<S22>/Source'
   *   '<S23>/Source'
   */
  real_T pooled3[27];

  /* Expression: [5.29E+05
     5.29E+05
     3.51E+04
     6.73E+04
     3.13E+05
     4.54E+05
     1.14E+05
     6.52E+04
     3.14E+04
     1.37E+05
     1.36E+05
     1.83E+05
     1.85E+05
     1.79E+05
     1.59E+05
     1.21E+05
     1.98E+05
     1.45E+05
     1.32E+05
     1.45E+05
     1.56E+05
     1.25E+05
     8.95E+04
     8.87E+04
     4.00E+04
     1.20E+05
     1.20E+05]
   * Referenced by: '<S21>/Source'
   */
  real_T Source_tableData_ez[27];

  /* Expression: [0.00083
     0.00083
     0.00036
     0.00055
     0.00107
     0.00115
     0.00071
     0.00053
     0.00031
     0.00100
     0.00096
     0.00117
     0.00121
     0.00109
     0.00106
     0.00107
     0.00105
     0.00104
     0.00126
     0.00122
     0.00135
     0.00163
     0.00158
     0.00160
     0.00170
     0.00471
     0.00471]
   * Referenced by: '<S22>/Source'
   */
  real_T Source_tableData_m[27];
} ConstP_EKF_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T ProtectionOutput_OV_Thresholds;
                                   /* '<Root>/ProtectionOutput_OV_Thresholds' */
  real_T ProtectionOutput_UV_Thresholds;
                                   /* '<Root>/ProtectionOutput_UV_Thresholds' */
  real_T Max_PackCapacity_mAh;         /* '<Root>/Max_PackCapacity_mAh' */
  real_T Min_PackCapacity_mAh;         /* '<Root>/Min_PackCapacity_mAh' */
  real_T VehicleRest_Time_msec;        /* '<Root>/VehicleRest_Time_msec' */
  real_T VehicleRest_Current_mA;       /* '<Root>/VehicleRest_Current_mA' */
  real_T Time_sec;                     /* '<Root>/Time_sec' */
  real_T Current_mA;                   /* '<Root>/Current_mA' */
  real_T AvgCellVoltage_mV;            /* '<Root>/AvgCellVoltage_mV' */
} ExtU_EKF_T;


/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Initial_Capacity_mAh;         /* '<Root>/<Initial_Capacity_mAh>' */
  real_T Total_Discharge_mAh;          /* '<Root>/<Total_Discharge_mAh>' */
  real_T Total_CapacityRemains_mAh;   /* '<Root>/<Total_CapacityRemains_mAh>' */
  real_T SOC_CC;                       /* '<Root>/<SOC_CC>' */
  real_T CC_State;                     /* '<Root>/<CC_State>' */
  real_T Capacity_EKF;                 /* '<Root>/Capacity_EKF' */
  real_T Voltage_EKF;                  /* '<Root>/Voltage_EKF' */
  real_T Terminal_Voltage;             /* '<Root>/Terminal_Voltage' */
  real_T SOC_EKF;                      /* '<Root>/SOC_EKF' */
} ExtY_EKF_T;

/* Real-time Model Data Structure */
struct tag_RTM_EKF_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    SimTimeStep simTimeStep;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_EKF_T EKF_B;

/* Block states (default storage) */
extern DW_EKF_T EKF_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_EKF_T EKF_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_EKF_T EKF_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_EKF_T EKF_Y;
extern const ConstB_EKF_T EKF_ConstB;  /* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_EKF_T EKF_ConstP;

/* Model entry point functions */
extern void EKF_initialize(void);
extern void EKF_step(void);
extern void EKF_terminate(void);

/* Real-time Model object */
extern RT_MODEL_EKF_T *const EKF_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Capacity_Compare' : Unused code path elimination
 * Block '<S12>/Constant' : Unused code path elimination
 * Block '<S12>/Relational Operator' : Unused code path elimination
 * Block '<S12>/Sum' : Unused code path elimination
 * Block '<S1>/Display' : Unused code path elimination
 * Block '<S19>/Display' : Unused code path elimination
 * Block '<S1>/Voltage_Compare' : Unused code path elimination
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
 * hilite_system('EKF_CodeGeneration/EKF')    - opens subsystem EKF_CodeGeneration/EKF
 * hilite_system('EKF_CodeGeneration/EKF/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'EKF_CodeGeneration'
 * '<S1>'   : 'EKF_CodeGeneration/EKF'
 * '<S2>'   : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased'
 * '<S3>'   : 'EKF_CodeGeneration/EKF/ECM_1RC'
 * '<S4>'   : 'EKF_CodeGeneration/EKF/EKF_Function'
 * '<S5>'   : 'EKF_CodeGeneration/EKF/powergui1'
 * '<S6>'   : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/1-D Lookup Table3'
 * '<S7>'   : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation'
 * '<S8>'   : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/StateMachine'
 * '<S9>'   : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/1-D Lookup Table3/Original'
 * '<S10>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1'
 * '<S11>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1/Model'
 * '<S12>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1/Model/Internal dirac generator'
 * '<S13>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1/Model/NEGATIVE Edge'
 * '<S14>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1/Model/POSITIVE Edge'
 * '<S15>'  : 'EKF_CodeGeneration/EKF/Coulomb_Counting_CapacityBased/Calculation/Edge Detector1/Model/Internal dirac generator/Triggered Subsystem'
 * '<S16>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/C1 lookup table'
 * '<S17>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/R0 lookup table'
 * '<S18>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/R1 lookup table'
 * '<S19>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/Subsystem'
 * '<S20>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/Subsystem1'
 * '<S21>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/C1 lookup table/Original'
 * '<S22>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/R0 lookup table/Original'
 * '<S23>'  : 'EKF_CodeGeneration/EKF/ECM_1RC/R1 lookup table/Original'
 */
#endif                                 /* RTW_HEADER_EKF_h_ */
