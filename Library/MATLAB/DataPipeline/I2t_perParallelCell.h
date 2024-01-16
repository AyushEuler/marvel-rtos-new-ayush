/*
 * Code generation for system system '<S1>/I2t_perParallelCell'
 * For more details, see corresponding source file I2t_perParallelCell.c
 *
 */

#ifndef RTW_HEADER_I2t_perParallelCell_h_
#define RTW_HEADER_I2t_perParallelCell_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/I2t_perParallelCell' */
typedef struct {
  int32_T i2t_Charge_A2sec;            /* '<S1>/I2t_perParallelCell' */
  int32_T i2t_Discharge_A2sec;         /* '<S1>/I2t_perParallelCell' */
} B_I2t_perParallelCell_DataPip_T;

extern void DataPipelin_I2t_perParallelCell(int32_T rtu_SOC, real_T
  rtu_Temperature_C, B_I2t_perParallelCell_DataPip_T *localB);

#endif                                 /* RTW_HEADER_I2t_perParallelCell_h_ */
