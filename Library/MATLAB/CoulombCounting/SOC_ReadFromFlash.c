/*
 * Code generation for system system '<S1>/SOC_ReadFromFlash'
 *
 * Model                      : SOCEstimation
 * Model version              : 1.226
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Fri Sep 29 14:04:16 2023
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "SOC_ReadFromFlash.h"
#include "SOCEstimation_private.h"

#include "threadxMain.h"
#include "bmsConfiguration.h"

extern coulombCountingOutput_t ccOutputDataEEPROM;
extern coulombCountingOutput_t ccOutputDataFlash;
uint8_t ReadFromFlashCounter = 0;

/* Output and update for atomic system: '<S1>/SOC_ReadFromFlash' */
void SOCEstimation_SOC_ReadFromFlash(B_SOC_ReadFromFlash_SOCEstima_T *localB)
{
    if(ReadFromFlashCounter == 0)
    {
        //totalCapacityRemaining from previous cycle will be the initial capacity for next cycle
        #ifdef SOC_IN_FLASH
        localB->CapacityRemains_mAh = ccOutputDataFlash.totalCapacityRemaining;
        #else
        localB->CapacityRemains_mAh = ccOutputDataEEPROM.totalCapacityRemaining;
        // localB->CapacityRemains_mAh = 180000;
        #endif

        ReadFromFlashCounter = 1;
    }
}
