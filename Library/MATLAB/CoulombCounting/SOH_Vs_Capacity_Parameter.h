/*
 * Code generation for system system '<S6>/SOH_Vs_Capacity_Parameter'
 * For more details, see corresponding source file SOH_Vs_Capacity_Parameter.c
 *
 */

#ifndef RTW_HEADER_SOH_Vs_Capacity_Parameter_h_
#define RTW_HEADER_SOH_Vs_Capacity_Parameter_h_
#ifndef SOCEstimation_COMMON_INCLUDES_
#define SOCEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SOCEstimation_COMMON_INCLUDES_ */

/* Block signals for system '<S6>/SOH_Vs_Capacity_Parameter' */
typedef struct {
  uint32_T SOH_Vs_Capacity_Gain;       /* '<S6>/SOH_Vs_Capacity_Parameter' */
  uint32_T SOH_Vs_Capacity_Offset;     /* '<S6>/SOH_Vs_Capacity_Parameter' */
} B_SOH_Vs_Capacity_Parameter_S_T;

extern void SOCEs_SOH_Vs_Capacity_Parameter(B_SOH_Vs_Capacity_Parameter_S_T
  *localB);

#endif                             /* RTW_HEADER_SOH_Vs_Capacity_Parameter_h_ */
