/*
 * Code generation for system system '<S1>/MinimumFunction'
 * For more details, see corresponding source file MinimumFunction.c
 *
 */

#ifndef RTW_HEADER_MinimumFunction_h_
#define RTW_HEADER_MinimumFunction_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/MinimumFunction' */
typedef struct {
  int32_T Min;                         /* '<S1>/MinimumFunction' */
} B_MinimumFunction_DataPipelin_T;

extern void DataPipeline_MinimumFunction(real_T rtu_n, int32_T rtu_u1, int32_T
  rtu_u2, B_MinimumFunction_DataPipelin_T *localB);

#endif                                 /* RTW_HEADER_MinimumFunction_h_ */
