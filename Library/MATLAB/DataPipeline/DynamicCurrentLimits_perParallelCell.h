/*
 * Code generation for system system '<S1>/DynamicCurrentLimits_perParallelCell'
 * For more details, see corresponding source file DynamicCurrentLimits_perParallelCell.c
 *
 */

#ifndef RTW_HEADER_DynamicCurrentLimits_perParallelCell_h_
#define RTW_HEADER_DynamicCurrentLimits_perParallelCell_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/DynamicCurrentLimits_perParallelCell' */
typedef struct {
  int32_T DCL_ChargingCurrent_A;
                               /* '<S1>/DynamicCurrentLimits_perParallelCell' */
  int32_T DCL_DisChargingCurrent_A;
                               /* '<S1>/DynamicCurrentLimits_perParallelCell' */
} B_DynamicCurrentLimits_perPar_T;

extern void DynamicCurrentLimits_perParalle(int32_T rtu_SOC, real_T
  rtu_Temperature_C, B_DynamicCurrentLimits_perPar_T *localB);

#endif                  /* RTW_HEADER_DynamicCurrentLimits_perParallelCell_h_ */
