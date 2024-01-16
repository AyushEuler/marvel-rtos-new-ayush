/*
 * Code generation for system system '<S1>/WriteMosfetsData'
 * For more details, see corresponding source file WriteMosfetsData.c
 *
 */

#ifndef RTW_HEADER_WriteMosfetsData_h_
#define RTW_HEADER_WriteMosfetsData_h_
#ifndef CellBalancing_COMMON_INCLUDES_
#define CellBalancing_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* CellBalancing_COMMON_INCLUDES_ */

/* Block signals for system '<S1>/WriteMosfetsData' */
typedef struct {
  int8_T fetStates[20];                /* '<S1>/WriteMosfetsData' */
} B_WriteMosfetsData_CellBalanc_T;

extern void CellBalancing_WriteMosfetsData(const int8_T rtu_MosfetStates[20],
  B_WriteMosfetsData_CellBalanc_T *localB);

#endif                                 /* RTW_HEADER_WriteMosfetsData_h_ */
