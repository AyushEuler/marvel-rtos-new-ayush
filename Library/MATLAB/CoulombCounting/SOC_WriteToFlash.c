/*
 * Code generation for system system '<S1>/SOC_WriteToFlash'
 *
 * Model                      : SOCEstimation
 * Model version              : 1.226
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Fri Sep 29 14:04:16 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "rtwtypes.h"
#include "multiword_types.h"
#include "SOC_WriteToFlash.h"
#include "SOCEstimation_private.h"

#include "threadxMain.h"

extern coulombCountingOutput_t ccOutputDataEEPROM;
extern coulombCountingOutput_t ccOutputDataFlash;
uint8_t WriteToFlashCounter = 0;

/* Output and update for atomic system: '<S1>/SOC_WriteToFlash' */
void SOCEstimation_SOC_WriteToFlash(uint32_T rtu_SOC_cpct,
  B_SOC_WriteToFlash_SOCEstimat_T *localB)
{
  if(WriteToFlashCounter == 0)
  {
    #ifdef SOC_IN_FLASH
    localB->SOC = ccOutputDataFlash.SOC_pct;
    #endif
    localB->SOC = ccOutputDataEEPROM.SOC_pct;
    WriteToFlashCounter = 1;
  }
}
