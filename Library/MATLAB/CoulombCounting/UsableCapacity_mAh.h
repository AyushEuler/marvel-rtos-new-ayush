/*
 * Code generation for system system '<S17>/UsableCapacity_mAh'
 * For more details, see corresponding source file UsableCapacity_mAh.c
 *
 */

#ifndef RTW_HEADER_UsableCapacity_mAh_h_
#define RTW_HEADER_UsableCapacity_mAh_h_
#ifndef SOCEstimation_COMMON_INCLUDES_
#define SOCEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SOCEstimation_COMMON_INCLUDES_ */

/* Block signals for system '<S7>/UsableCapacity_mAh' */
typedef struct {
  int32_T MaxUsableCapacity_mAh;       /* '<S7>/UsableCapacity_mAh' */
} B_UsableCapacity_mAh_SOCEstim_T;

/* Block signals for system '<S17>/UsableCapacity_mAh' */
typedef struct {
  uint32_T MaxUsableCapacity_mAh;      /* '<S17>/UsableCapacity_mAh' */
} B_UsableCapacity_mAh_SOCEst_l_T;

extern void SOCEstimatio_UsableCapacity_mAh(real32_T rtu_CycleCount, uint32_T
  rtu_SOH_Vs_Capacity_Gain, uint32_T rtu_SOH_Vs_Capacity_Offset, uint32_T
  rtu_InstalledCapacity_mAh, B_UsableCapacity_mAh_SOCEstim_T *localB);
extern void SOCEstimat_UsableCapacity_mAh_m(real32_T rtu_CycleCount, uint32_T
  rtu_InstalledCapacity_mAh, uint32_T rtu_SOH_Vs_Capacity_Gain, uint32_T
  rtu_SOH_Vs_Capacity_Offset, B_UsableCapacity_mAh_SOCEst_l_T *localB);

#endif                                 /* RTW_HEADER_UsableCapacity_mAh_h_ */
