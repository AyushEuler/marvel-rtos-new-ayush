/*
 * Code generation for system system '<S1>/AverageFunction'
 *
 * Model                      : DataPipeline
 * Model version              : 4.707
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Fri Sep 22 16:22:39 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "rtwtypes.h"
#include "AverageFunction.h"
#include "DataPipeline_private.h"

/* Output and update for atomic system: '<S1>/AverageFunction' */
void DataPipeline_AverageFunction(int16_T rtu_u1, int16_T rtu_u2, real_T rtu_n,
  B_AverageFunction_DataPipelin_T *localB)
{
  int32_T k;
  int32_T vlen;
  int32_T y;

  /* :  A = [u1,u2]; */
  /* :  Average = mean(A(1:n)); */
  if (rtu_n < 1.0) {
    vlen = 0;
  } else {
    vlen = (int32_T)rtu_n;
  }

  if (vlen == 0) {
    y = 0;
  } else {
    y = rtu_u1;
    for (k = 2; k <= vlen; k++) {
      y += rtu_u2;
    }
  }

  localB->Average = (real_T)y / (real_T)vlen;
}
