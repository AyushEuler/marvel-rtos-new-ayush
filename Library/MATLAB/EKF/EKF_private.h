/*
 * EKF_private.h
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

#ifndef RTW_HEADER_EKF_private_h_
#define RTW_HEADER_EKF_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "EKF_types.h"
#include "EKF.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern real_T look1_binlx_ekf(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T maxIndex);

#endif                                 /* RTW_HEADER_EKF_private_h_ */
