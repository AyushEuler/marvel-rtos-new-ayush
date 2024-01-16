/*
 * Code generation for system system '<S1>/MaximumFunction'
 * For more details, see corresponding source file MaximumFunction.c
 *
 */

#ifndef RTW_HEADER_MaximumFunction_h_
#define RTW_HEADER_MaximumFunction_h_
#ifndef DataPipeline_COMMON_INCLUDES_
#define DataPipeline_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DataPipeline_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/MaximumFunction' */
typedef struct {
  int32_T Max;                         /* '<S1>/MaximumFunction' */
} B_MaximumFunction_DataPipelin_T;

extern void DataPipeline_MaximumFunction(real_T rtu_n, int32_T rtu_u1, int32_T
  rtu_u2, B_MaximumFunction_DataPipelin_T *localB);

#endif                                 /* RTW_HEADER_MaximumFunction_h_ */
