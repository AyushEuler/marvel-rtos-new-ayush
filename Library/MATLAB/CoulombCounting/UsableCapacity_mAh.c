/*
 * Code generation for system system '<S17>/UsableCapacity_mAh'
 *
 * Model                      : SOCEstimation
 * Model version              : 4.1082
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Tue Dec 19 16:27:05 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "rtwtypes.h"
#include "UsableCapacity_mAh.h"
#include <math.h>
#include "SOCEstimation_private.h"

/* Output and update for atomic system: '<S7>/UsableCapacity_mAh' */
void SOCEstimatio_UsableCapacity_mAh(real32_T rtu_CycleCount, uint32_T
  rtu_SOH_Vs_Capacity_Gain, uint32_T rtu_SOH_Vs_Capacity_Offset, uint32_T
  rtu_InstalledCapacity_mAh, B_UsableCapacity_mAh_SOCEstim_T *localB)
{
  real32_T tmp;

  /* :  MaxUsableCapacity_mAh = int32(single(InstalledCapacity_mAh) - (single(SOH_Vs_Capacity_Gain) * (single(CycleCount) + single(SOH_Vs_Capacity_Offset)))); */
  tmp = roundf((real32_T)rtu_InstalledCapacity_mAh - (rtu_CycleCount + (real32_T)
    rtu_SOH_Vs_Capacity_Offset) * (real32_T)rtu_SOH_Vs_Capacity_Gain);
  if (tmp < 2.14748365E+9F) {
    if (tmp >= -2.14748365E+9F) {
      localB->MaxUsableCapacity_mAh = (int32_T)tmp;
    } else {
      localB->MaxUsableCapacity_mAh = MIN_int32_T;
    }
  } else {
    localB->MaxUsableCapacity_mAh = MAX_int32_T;
  }
}

/* Output and update for atomic system: '<S17>/UsableCapacity_mAh' */
void SOCEstimat_UsableCapacity_mAh_m(real32_T rtu_CycleCount, uint32_T
  rtu_InstalledCapacity_mAh, uint32_T rtu_SOH_Vs_Capacity_Gain, uint32_T
  rtu_SOH_Vs_Capacity_Offset, B_UsableCapacity_mAh_SOCEst_l_T *localB)
{
  real32_T tmp;

  /* :  MaxUsableCapacity_mAh = uint32(single(InstalledCapacity_mAh) - (single(SOH_Vs_Capacity_Gain) * (single(CycleCount) + single(SOH_Vs_Capacity_Offset)))); */
  tmp = roundf((real32_T)rtu_InstalledCapacity_mAh - (rtu_CycleCount + (real32_T)
    rtu_SOH_Vs_Capacity_Offset) * (real32_T)rtu_SOH_Vs_Capacity_Gain);
  if (tmp < 4.2949673E+9F) {
    if (tmp >= 0.0F) {
      localB->MaxUsableCapacity_mAh = (uint32_T)tmp;
    } else {
      localB->MaxUsableCapacity_mAh = 0U;
    }
  } else {
    localB->MaxUsableCapacity_mAh = MAX_uint32_T;
  }
}
