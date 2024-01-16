/*
 * Code generation for system system '<S6>/SOH_Vs_Capacity_Parameter'
 *
 * Model                      : SOCEstimation
 * Model version              : 4.935
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Mon Nov  6 12:55:43 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "SOH_Vs_Capacity_Parameter.h"
#include "SOCEstimation_private.h"

/* Output and update for atomic system: '<S6>/SOH_Vs_Capacity_Parameter' */
void SOCEs_SOH_Vs_Capacity_Parameter(B_SOH_Vs_Capacity_Parameter_S_T *localB)
{
  /* :  SOH_Vs_Capacity_Gain = SOH_cpct * 0; */
  localB->SOH_Vs_Capacity_Gain = 0U;

  /* :  SOH_Vs_Capacity_Offset = SOH_cpct * 0; */
  localB->SOH_Vs_Capacity_Offset = 0U;
}
