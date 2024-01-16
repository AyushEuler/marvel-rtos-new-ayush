/*
 * Code generation for system system '<S1>/Voltage_Vs_Capacity'
 * For more details, see corresponding source file VoltagemV_Vs_CapacitymAh.c
 *
 */

#ifndef RTW_HEADER_VoltagemV_Vs_CapacitymAh_h_
#define RTW_HEADER_VoltagemV_Vs_CapacitymAh_h_
#ifndef SOCEstimation_COMMON_INCLUDES_
#define SOCEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SOCEstimation_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/Voltage_Vs_Capacity' */
typedef struct {
  uint32_T Capacity_mAh;               /* '<S1>/Voltage_Vs_Capacity' */
} B_VoltagemV_Vs_CapacitymAh_T;

extern void VoltagemV_Vs_CapacitymAh(int32_T rtu_Voltage_mV,
  B_VoltagemV_Vs_CapacitymAh_T *localB);

#endif                              /* RTW_HEADER_VoltagemV_Vs_CapacitymAh_h_ */
