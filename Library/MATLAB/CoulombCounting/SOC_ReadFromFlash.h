/*
 * Code generation for system system '<S1>/SOC_ReadFromFlash'
 * For more details, see corresponding source file SOC_ReadFromFlash.c
 *
 */

#ifndef RTW_HEADER_SOC_ReadFromFlash_h_
#define RTW_HEADER_SOC_ReadFromFlash_h_
#ifndef SOCEstimation_COMMON_INCLUDES_
#define SOCEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SOCEstimation_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/SOC_ReadFromFlash' */
typedef struct {
  int32_T CapacityRemains_mAh;         /* '<S1>/SOC_ReadFromFlash' */
} B_SOC_ReadFromFlash_SOCEstima_T;

extern void SOCEstimation_SOC_ReadFromFlash(B_SOC_ReadFromFlash_SOCEstima_T
  *localB);

#endif                                 /* RTW_HEADER_SOC_ReadFromFlash_h_ */
