/*
 * Code generation for system system '<S1>/I2t_perParallelCell'
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
#include "I2t_perParallelCell.h"
#include "DataPipeline_private.h"

/* Output and update for atomic system: '<S1>/I2t_perParallelCell' */
void DataPipelin_I2t_perParallelCell(int32_T rtu_SOC, real_T rtu_Temperature_C,
  B_I2t_perParallelCell_DataPip_T *localB)
{
  real_T tmp_0;
  int32_T tmp;

  /* :  i2t_Charge_A2sec = SOC*Temperature_C*12; */
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

  localB->i2t_Charge_A2sec = mul_s32_sat_dp(tmp, 12);

  /* :  i2t_Discharge_A2sec = SOC*Temperature_C*12; */
  if (tmp_0 < 2.147483648E+9) {
    if (tmp_0 >= -2.147483648E+9) {
      tmp = (int32_T)tmp_0;
    } else {
      tmp = MIN_int32_T;
    }
  } else {
    tmp = MAX_int32_T;
  }

  localB->i2t_Discharge_A2sec = mul_s32_sat_dp(tmp, 12);
}
