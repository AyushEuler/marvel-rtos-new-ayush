/*
 * Code generation for system system '<S1>/MaximumFunction'
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
#include "MaximumFunction.h"
#include "DataPipeline_private.h"

/* Output and update for atomic system: '<S1>/MaximumFunction' */
void DataPipeline_MaximumFunction(real_T rtu_n, int32_T rtu_u1, int32_T rtu_u2,
  B_MaximumFunction_DataPipelin_T *localB)
{
  int32_T istop;
  int32_T k;

  /* :  A = [u1,u2]; */
  /* :  Max = max(A(1:n)); */
  if (rtu_n < 1.0) {
    istop = 0;
  } else {
    istop = (int32_T)rtu_n;
  }

  localB->Max = rtu_u1;
  for (k = 2; k <= istop; k++) {
    if (localB->Max < rtu_u2) {
      localB->Max = rtu_u2;
    }
  }
}
