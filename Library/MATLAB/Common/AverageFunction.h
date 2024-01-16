/*
 * Code generation for system system '<S1>/AverageFunction'
 * For more details, see corresponding source file AverageFunction.c
 *
 */

#ifndef RTW_HEADER_AverageFunction_h_
#define RTW_HEADER_AverageFunction_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/AverageFunction' */
typedef struct {
  real_T Average;                      /* '<S1>/AverageFunction' */
} B_AverageFunction_DataPipelin_T;

extern void DataPipeline_AverageFunction(int16_T rtu_u1, int16_T rtu_u2, real_T
  rtu_n, B_AverageFunction_DataPipelin_T *localB);

#endif                                 /* RTW_HEADER_AverageFunction_h_ */
