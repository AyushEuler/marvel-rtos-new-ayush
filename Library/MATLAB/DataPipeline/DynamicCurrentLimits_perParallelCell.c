/*
 * Code generation for system system '<S1>/DynamicCurrentLimits_perParallelCell'
 *
 * Model                      : DataPipeline
 * Model version              : 4.1075
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Mon Dec 18 14:37:59 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "rtwtypes.h"
#include "DynamicCurrentLimits_perParallelCell.h"
#include "DataPipeline_private.h"
#include <math.h>

real_T rt_roundd_dp(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Output and update for atomic system: '<S1>/DynamicCurrentLimits_perParallelCell' */
void DynamicCurrentLimits_perParalle(int32_T rtu_SOC, real_T rtu_Temperature_C,
  B_DynamicCurrentLimits_perPar_T *localB)
{
  real_T tmp_0;
  int32_T tmp;

  /* :  DCL_ChargingCurrent_A = SOC*Temperature_C/20; */
  tmp_0 = rt_roundd_dp((real_T)rtu_SOC * rtu_Temperature_C);
  if (tmp_0 < 2.147483648E+9) {
    if (tmp_0 >= -2.147483648E+9) {
      tmp = (int32_T)tmp_0;
    } else {
      tmp = MIN_int32_T;
    }
  } else {
    tmp = MAX_int32_T;
  }

  localB->DCL_ChargingCurrent_A = (int32_T)rt_roundd_dp((real_T)tmp / 20.0);

  /* :  DCL_DisChargingCurrent_A = SOC*Temperature_C/20; */
  if (tmp_0 < 2.147483648E+9) {
    if (tmp_0 >= -2.147483648E+9) {
      tmp = (int32_T)tmp_0;
    } else {
      tmp = MIN_int32_T;
    }
  } else {
    tmp = MAX_int32_T;
  }

  localB->DCL_DisChargingCurrent_A = (int32_T)rt_roundd_dp((real_T)tmp / 20.0);
}
