/*
 * Code generation for system system '<S1>/WriteMosfetsData'
 *
 * Model                      : CellBalancing
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:45:49 2024
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "rtwtypes.h"
#include "WriteMosfetsData.h"

/* Output and update for atomic system: '<S1>/WriteMosfetsData' */
void CellBalancing_WriteMosfetsData(const int8_T rtu_MosfetStates[20],
  B_WriteMosfetsData_CellBalanc_T *localB)
{
  int32_T i;

  /* :  A = NumberOfCellBalancingMosfets; */
  /* :  fetStates = MosfetStates; */
  for (i = 0; i < 20; i++) {
    localB->fetStates[i] = rtu_MosfetStates[i];
  }
}
