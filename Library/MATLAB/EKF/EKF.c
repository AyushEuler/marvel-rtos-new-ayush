/*
 * EKF.c
 *
 * Code generation for model "EKF".
 *
 * Model version              : 1.103
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Fri May 19 14:35:15 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (10), Warnings (3), Error (0)
 */

#include "EKF.h"
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include "EKF_private.h"
#include "zero_crossing_types.h"

/* Named constants for Chart: '<S2>/StateMachine' */
#define EKF_IN_Lobby                   ((uint8_T)1U)
#define EKF_IN_NO_ACTIVE_CHILD         ((uint8_T)0U)
#define EKF_IN_OV_Thresholds           ((uint8_T)2U)
#define EKF_IN_UV_Thresholds           ((uint8_T)3U)
#define EKF_IN_Vehicle_at_Rest         ((uint8_T)4U)

/* Block signals (default storage) */
B_EKF_T EKF_B;

/* Block states (default storage) */
DW_EKF_T EKF_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_EKF_T EKF_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_EKF_T EKF_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_EKF_T EKF_Y;

/* Real-time model */
static RT_MODEL_EKF_T EKF_M_;
RT_MODEL_EKF_T *const EKF_M = &EKF_M_;
real_T look1_binlx_ekf(real_T u0, const real_T bp0[], const real_T table[], uint32_T
                   maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

/* Model step function */
void EKF_step(void)
{
  real_T A[4];
  real_T A_0[4];
  real_T A_1[4];
  real_T H_idx_0;
  real_T K_idx_1;
  real_T P_Predicted_idx_3;
  real_T rtb_SOC_Pct;
  real_T rtb_Source;
  real_T rtb_Source_e;
  int32_T i;

  /* Outport: '<Root>/Capacity_EKF' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   */
  EKF_Y.Capacity_EKF = EKF_DW.UnitDelay_DSTATE;

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/Max_PackCapacity_mAh'
   *  Outport: '<Root>/Capacity_EKF'
   *  Product: '<S1>/Divide'
   */
  EKF_B.SOC_EKF = EKF_Y.Capacity_EKF / EKF_U.Max_PackCapacity_mAh * 100.0;

  /* DataStoreRead: '<S2>/Data Store Read' */
  EKF_B.CapacityRemians_mAh = EKF_DW.CapacityRemains_mAh;

  /* Lookup_n-D: '<S9>/Source' incorporates:
   *  Inport: '<Root>/AvgCellVoltage_mV'
   */
  EKF_B.Source = look1_binlx_ekf(EKF_U.AvgCellVoltage_mV, EKF_ConstP.Source_bp01Data,
    EKF_ConstP.Source_tableData, 42U);

  /* Chart: '<S2>/StateMachine' incorporates:
   *  Inport: '<Root>/Current_mA'
   *  Inport: '<Root>/ProtectionOutput_OV_Thresholds'
   *  Inport: '<Root>/ProtectionOutput_UV_Thresholds'
   *  Inport: '<Root>/VehicleRest_Current_mA'
   *  Inport: '<Root>/VehicleRest_Time_msec'
   */
  if (EKF_DW.is_active_c3_EKF == 0U) {
    EKF_DW.is_active_c3_EKF = 1U;
    EKF_DW.is_c3_EKF = EKF_IN_Vehicle_at_Rest;

    /* Outport: '<Root>/<CC_State>' */
    /* :  CC_State = 2; */
    EKF_Y.CC_State = 2.0;

    /* Outport: '<Root>/<Initial_Capacity_mAh>' */
    /* :  Initial_Capacity_mAh = Initial_Guess_mAh; */
    EKF_Y.Initial_Capacity_mAh = EKF_B.Source;
  } else {
    switch (EKF_DW.is_c3_EKF) {
     case EKF_IN_Lobby:
      /* Outport: '<Root>/<CC_State>' */
      EKF_Y.CC_State = 0.0;

      /* :  sf_internal_predicateOutput = OV_Thresholds == 2; */
      if (EKF_U.ProtectionOutput_OV_Thresholds == 2.0) {
        EKF_DW.durationCounter_1_f = 0;
        EKF_DW.is_c3_EKF = EKF_IN_OV_Thresholds;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 1; */
        EKF_Y.CC_State = 1.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' incorporates:
         *  Inport: '<Root>/Max_PackCapacity_mAh'
         */
        /* :  Initial_Capacity_mAh = Max_PackCapacity_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_U.Max_PackCapacity_mAh;
      } else {
        /* :  sf_internal_predicateOutput = duration((abs(Current_mA) < VehicleRest_Current_mA ),msec) >= VehicleRest_Time_msec; */
        if (fabs(EKF_U.Current_mA) >= EKF_U.VehicleRest_Current_mA) {
          EKF_DW.durationCounter_1 = 0;
        }

        if ((uint32_T)EKF_DW.durationCounter_1 >= (uint32_T)ceil
            (EKF_U.VehicleRest_Time_msec / 960000.0 - 9.6E-6)) {
          EKF_DW.is_c3_EKF = EKF_IN_Vehicle_at_Rest;

          /* Outport: '<Root>/<CC_State>' */
          /* :  CC_State = 2; */
          EKF_Y.CC_State = 2.0;

          /* Outport: '<Root>/<Initial_Capacity_mAh>' */
          /* :  Initial_Capacity_mAh = Initial_Guess_mAh; */
          EKF_Y.Initial_Capacity_mAh = EKF_B.Source;

          /* :  sf_internal_predicateOutput = UV_Thresholds == 2; */
        } else if (EKF_U.ProtectionOutput_UV_Thresholds == 2.0) {
          EKF_DW.durationCounter_1_fy = 0;
          EKF_DW.is_c3_EKF = EKF_IN_UV_Thresholds;

          /* Outport: '<Root>/<CC_State>' */
          /* :  CC_State = 1; */
          EKF_Y.CC_State = 1.0;

          /* Outport: '<Root>/<Initial_Capacity_mAh>' incorporates:
           *  Inport: '<Root>/Min_PackCapacity_mAh'
           */
          /* :  Initial_Capacity_mAh = Min_PackCapacity_mAh; */
          EKF_Y.Initial_Capacity_mAh = EKF_U.Min_PackCapacity_mAh;
        }
      }
      break;

     case EKF_IN_OV_Thresholds:
      /* Outport: '<Root>/<CC_State>' */
      EKF_Y.CC_State = 1.0;

      /* :  sf_internal_predicateOutput = (duration((abs(Current_mA) <VehicleRest_Current_mA),msec) >= VehicleRest_Time_msec); */
      if (fabs(EKF_U.Current_mA) >= EKF_U.VehicleRest_Current_mA) {
        EKF_DW.durationCounter_1_f = 0;
      }

      if ((uint32_T)EKF_DW.durationCounter_1_f >= (uint32_T)ceil
          (EKF_U.VehicleRest_Time_msec / 960000.0 - 9.6E-6)) {
        EKF_DW.is_c3_EKF = EKF_IN_Vehicle_at_Rest;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 2; */
        EKF_Y.CC_State = 2.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' */
        /* :  Initial_Capacity_mAh = Initial_Guess_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_B.Source;

        /* :  sf_internal_predicateOutput = OV_Thresholds == 0 || OV_Thresholds == 1; */
      } else if ((EKF_U.ProtectionOutput_OV_Thresholds == 0.0) ||
                 (EKF_U.ProtectionOutput_OV_Thresholds == 1.0)) {
        EKF_DW.durationCounter_1 = 0;
        EKF_DW.is_c3_EKF = EKF_IN_Lobby;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 0; */
        EKF_Y.CC_State = 0.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' */
        /* :  Initial_Capacity_mAh = CapacityRemains_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_B.CapacityRemians_mAh;
      }
      break;

     case EKF_IN_UV_Thresholds:
      /* Outport: '<Root>/<CC_State>' */
      EKF_Y.CC_State = 1.0;

      /* :  sf_internal_predicateOutput = UV_Thresholds == 0 || UV_Thresholds == 1; */
      if ((EKF_U.ProtectionOutput_UV_Thresholds == 0.0) ||
          (EKF_U.ProtectionOutput_UV_Thresholds == 1.0)) {
        EKF_DW.durationCounter_1 = 0;
        EKF_DW.is_c3_EKF = EKF_IN_Lobby;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 0; */
        EKF_Y.CC_State = 0.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' */
        /* :  Initial_Capacity_mAh = CapacityRemains_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_B.CapacityRemians_mAh;
      } else {
        /* :  sf_internal_predicateOutput = (duration((abs(Current_mA) < VehicleRest_Current_mA),sec) >= VehicleRest_Time_msec); */
        if (fabs(EKF_U.Current_mA) >= EKF_U.VehicleRest_Current_mA) {
          EKF_DW.durationCounter_1_fy = 0;
        }

        if ((uint32_T)(EKF_DW.durationCounter_1_fy * 960) >= (uint32_T)ceil
            (EKF_U.VehicleRest_Time_msec)) {
          EKF_DW.is_c3_EKF = EKF_IN_Vehicle_at_Rest;

          /* Outport: '<Root>/<CC_State>' */
          /* :  CC_State = 2; */
          EKF_Y.CC_State = 2.0;

          /* Outport: '<Root>/<Initial_Capacity_mAh>' */
          /* :  Initial_Capacity_mAh = Initial_Guess_mAh; */
          EKF_Y.Initial_Capacity_mAh = EKF_B.Source;
        }
      }
      break;

     default:
      /* Outport: '<Root>/<CC_State>' */
      /* case IN_Vehicle_at_Rest: */
      EKF_Y.CC_State = 2.0;

      /* :  sf_internal_predicateOutput = OV_Thresholds == 2; */
      if (EKF_U.ProtectionOutput_OV_Thresholds == 2.0) {
        EKF_DW.durationCounter_1_f = 0;
        EKF_DW.is_c3_EKF = EKF_IN_OV_Thresholds;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 1; */
        EKF_Y.CC_State = 1.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' incorporates:
         *  Inport: '<Root>/Max_PackCapacity_mAh'
         */
        /* :  Initial_Capacity_mAh = Max_PackCapacity_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_U.Max_PackCapacity_mAh;

        /* :  sf_internal_predicateOutput = (abs(Current_mA) > VehicleRest_Current_mA); */
      } else if (fabs(EKF_U.Current_mA) > EKF_U.VehicleRest_Current_mA) {
        EKF_DW.durationCounter_1 = 0;
        EKF_DW.is_c3_EKF = EKF_IN_Lobby;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 0; */
        EKF_Y.CC_State = 0.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' */
        /* :  Initial_Capacity_mAh = CapacityRemains_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_B.CapacityRemians_mAh;

        /* :  sf_internal_predicateOutput = UV_Thresholds == 2; */
      } else if (EKF_U.ProtectionOutput_UV_Thresholds == 2.0) {
        EKF_DW.durationCounter_1_fy = 0;
        EKF_DW.is_c3_EKF = EKF_IN_UV_Thresholds;

        /* Outport: '<Root>/<CC_State>' */
        /* :  CC_State = 1; */
        EKF_Y.CC_State = 1.0;

        /* Outport: '<Root>/<Initial_Capacity_mAh>' incorporates:
         *  Inport: '<Root>/Min_PackCapacity_mAh'
         */
        /* :  Initial_Capacity_mAh = Min_PackCapacity_mAh; */
        EKF_Y.Initial_Capacity_mAh = EKF_U.Min_PackCapacity_mAh;
      }
      break;
    }
  }

  if (fabs(EKF_U.Current_mA) < EKF_U.VehicleRest_Current_mA) {
    EKF_DW.durationCounter_1++;
    EKF_DW.durationCounter_1_f++;
    EKF_DW.durationCounter_1_fy++;
  } else {
    EKF_DW.durationCounter_1 = 0;
    EKF_DW.durationCounter_1_f = 0;
    EKF_DW.durationCounter_1_fy = 0;
  }

  /* End of Chart: '<S2>/StateMachine' */

  /* DataTypeConversion: '<S11>/Data Type Conversion2' incorporates:
   *  Outport: '<Root>/<CC_State>'
   */
  EKF_B.DataTypeConversion2 = (EKF_Y.CC_State != 0.0);

  /* Memory: '<S11>/Memory' */
  EKF_B.Memory_m = EKF_DW.Memory_PreviousInput_a;

  /* Outputs for Enabled SubSystem: '<S11>/POSITIVE Edge' incorporates:
   *  EnablePort: '<S14>/Enable'
   */
  EKF_DW.POSITIVEEdge_MODE = (EKF_ConstB.MultiportSwitch[0] > 0.0);
  if (EKF_DW.POSITIVEEdge_MODE) {
    /* RelationalOperator: '<S14>/Relational Operator1' */
    EKF_B.RelationalOperator1 = ((int32_T)EKF_B.Memory_m < (int32_T)
      EKF_B.DataTypeConversion2);
  }

  /* End of Outputs for SubSystem: '<S11>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S11>/NEGATIVE Edge' incorporates:
   *  EnablePort: '<S13>/Enable'
   */
  EKF_DW.NEGATIVEEdge_MODE = (EKF_ConstB.MultiportSwitch[1] > 0.0);
  if (EKF_DW.NEGATIVEEdge_MODE) {
    /* RelationalOperator: '<S13>/Relational Operator1' */
    EKF_B.RelationalOperator1_j = ((int32_T)EKF_B.Memory_m > (int32_T)
      EKF_B.DataTypeConversion2);
  }

  /* End of Outputs for SubSystem: '<S11>/NEGATIVE Edge' */

  /* Logic: '<S11>/Logical Operator1' */
  EKF_B.LogicalOperator1 = (EKF_B.RelationalOperator1 ||
    EKF_B.RelationalOperator1_j);

  /* Memory: '<S7>/Memory' */
  EKF_B.Memory = EKF_DW.Memory_PreviousInput;

  /* Memory: '<S7>/Memory1' */
  EKF_B.Memory1 = EKF_DW.Memory1_PreviousInput;

  /* Switch: '<S7>/Switch' */
  if (EKF_B.LogicalOperator1) {
    /* Switch: '<S7>/Switch' incorporates:
     *  Constant: '<S7>/Constant'
     */
    EKF_B.Switch = 0.0;
  } else {
    /* Switch: '<S7>/Switch' incorporates:
     *  Gain: '<S7>/Multiply'
     *  Inport: '<Root>/Current_mA'
     *  Inport: '<Root>/Time_sec'
     *  Product: '<S7>/Product'
     *  Sum: '<S7>/Add2'
     *  Sum: '<S7>/Add3'
     */
    EKF_B.Switch = 0.00027777777777777778 * EKF_U.Current_mA * (EKF_U.Time_sec -
      EKF_B.Memory) + EKF_B.Memory1;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Sum: '<S7>/Add' incorporates:
   *  Outport: '<Root>/<Initial_Capacity_mAh>'
   */
  rtb_SOC_Pct = EKF_Y.Initial_Capacity_mAh + EKF_B.Switch;

  /* DataStoreWrite: '<S2>/Data Store Write' */
  EKF_DW.CapacityRemains_mAh = rtb_SOC_Pct;

  /* Saturate: '<S2>/Saturation' */
  if (rtb_SOC_Pct > 206400.0) {
    /* Saturate: '<S2>/Saturation' */
    EKF_B.Total_CapacityRemains_mAh = 206400.0;
  } else if (rtb_SOC_Pct < 0.0) {
    /* Saturate: '<S2>/Saturation' */
    EKF_B.Total_CapacityRemains_mAh = 0.0;
  } else {
    /* Saturate: '<S2>/Saturation' */
    EKF_B.Total_CapacityRemains_mAh = rtb_SOC_Pct;
  }

  /* End of Saturate: '<S2>/Saturation' */

  /* Clock: '<S12>/Clock' */
  EKF_B.Clock = EKF_M->Timing.t[0];

  /* Outputs for Triggered SubSystem: '<S12>/Triggered Subsystem' incorporates:
   *  TriggerPort: '<S15>/Trigger'
   */
  EKF_PrevZCX.TriggeredSubsystem_Trig_ZCE = EKF_B.LogicalOperator1;

  /* End of Outputs for SubSystem: '<S12>/Triggered Subsystem' */

  /* UnitDelay: '<S19>/Unit Delay' */
  EKF_B.V1 = EKF_DW.UnitDelay_DSTATE_l;

  /* Lookup_n-D: '<S21>/Source' incorporates:
   *  Gain: '<S1>/Gain'
   */
  rtb_Source_e = look1_binlx_ekf(EKF_B.SOC_EKF, EKF_ConstP.pooled3,
    EKF_ConstP.Source_tableData_ez, 26U);

  /* Memory: '<S19>/Memory1' */
  EKF_B.Memory1_k = EKF_DW.Memory1_PreviousInput_m;

  /* Product: '<S19>/Product' incorporates:
   *  Gain: '<S1>/Gain'
   *  Lookup_n-D: '<S23>/Source'
   *  Product: '<S3>/Product'
   */
  P_Predicted_idx_3 = look1_binlx_ekf(EKF_B.SOC_EKF, EKF_ConstP.pooled3,
    EKF_ConstP.Source_tableData_e, 26U) * rtb_Source_e;

  /* Sum: '<S19>/Add1' incorporates:
   *  Inport: '<Root>/Current_mA'
   *  Product: '<S19>/Divide'
   *  Product: '<S19>/Divide1'
   *  Product: '<S19>/Product'
   *  Sum: '<S19>/Add'
   */
  EKF_B.V1_i = (1.0 / rtb_Source_e * EKF_U.Current_mA - EKF_B.V1 /
                P_Predicted_idx_3) + EKF_B.Memory1_k;

  /* Sum: '<S3>/Add' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/Current_mA'
   *  Lookup_n-D: '<S22>/Source'
   *  Product: '<S20>/Product'
   */
  EKF_B.V1Vs = EKF_U.Current_mA * look1_binlx_ekf(EKF_B.SOC_EKF, EKF_ConstP.pooled3,
    EKF_ConstP.Source_tableData_m, 26U) + EKF_B.V1_i;

  /* Sum: '<S3>/Add1' incorporates:
   *  Inport: '<Root>/AvgCellVoltage_mV'
   */
  EKF_B.V1Vs_b = EKF_U.AvgCellVoltage_mV + EKF_B.V1Vs;

  /* Sum: '<S3>/Add2' incorporates:
   *  Gain: '<S3>/Gain'
   *  Inport: '<Root>/Current_mA'
   */
  EKF_B.Add2 = EKF_U.Current_mA - 0.01 * EKF_U.Current_mA;

  /* Product: '<S3>/Product' */
  EKF_B.Product = P_Predicted_idx_3;

  /* MATLAB Function: '<S1>/EKF_Function' */
  /* :  Q = Error_Current; */
  /* :  R = Error_Voltage; */
  /* :  A1 = 1; */
  /* :  A2 = exp(-1/Tau); */
  /* :  A = [A1 0; 0 A2]; */
  A[0] = 1.0;
  A[2] = 0.0;
  A[1] = 0.0;
  A[3] = exp(-1.0 / EKF_B.Product);

  /* :  if isempty(P_Old) */
  /* :  P_New = [P_Old(1) P_Old(2); P_Old(3) P_Old(4)]; */
  /* :  P_Predicted = A * P_New * A' + [Q 0; 0 Q]; */
  rtb_Source_e = EKF_DW.P_Old[0];
  P_Predicted_idx_3 = EKF_DW.P_Old[1];
  K_idx_1 = EKF_DW.P_Old[2];
  rtb_Source = EKF_DW.P_Old[3];
  for (i = 0; i < 2; i++) {
    A_0[i] = 0.0;
    A_0[i] += A[i] * rtb_Source_e;
    H_idx_0 = A[i + 2];
    A_0[i] += H_idx_0 * K_idx_1;
    A_0[i + 2] = 0.0;
    A_0[i + 2] += A[i] * P_Predicted_idx_3;
    A_0[i + 2] += H_idx_0 * rtb_Source;
    A_1[i] = 0.0;
    A_1[i] += A_0[i];
    A_1[i + 2] = 0.0;
    A_1[i + 2] += A_0[i + 2] * A[3];
  }

  real_T K_idx_0;
  rtb_Source_e = A_1[0] + EKF_B.Add2;
  P_Predicted_idx_3 = A_1[3] + EKF_B.Add2;

  /* :  H1 = 12.4 - 88.6 * 2 * Capacity_CC^1 + 347 * 3 * Capacity_CC^2 - 766.8 * 4 * Capacity_CC^3 + 955.3 * 5 * Capacity_CC^4 - 622.1 * 6 *Capacity_CC^5 + 164.2 * 7 *Capacity_CC^6; */
  /* :  H = [H1 -1]; */
  H_idx_0 = (((((12.4 - 177.2 * EKF_B.Total_CapacityRemains_mAh) +
                EKF_B.Total_CapacityRemains_mAh *
                EKF_B.Total_CapacityRemains_mAh * 1041.0) - 3067.2 * pow
               (EKF_B.Total_CapacityRemains_mAh, 3.0)) + 4776.5 * pow
              (EKF_B.Total_CapacityRemains_mAh, 4.0)) - 3732.6000000000004 * pow
             (EKF_B.Total_CapacityRemains_mAh, 5.0)) + 1149.3999999999999 * pow
    (EKF_B.Total_CapacityRemains_mAh, 6.0);

  /* :  S = H * P_Predicted * H' + R; */
  K_idx_0 = H_idx_0 * rtb_Source_e - A_1[1];
  K_idx_1 = H_idx_0 * A_1[2] - P_Predicted_idx_3;
  rtb_Source = (K_idx_0 * H_idx_0 - K_idx_1) + EKF_B.V1Vs;

  /* :  B = H * P_Predicted; */
  /* :  K = B/S; */
  /* :  X_Initial = [Capacity_CC ; Terminal_Voltage_ECM]; */
  /* :  X_Update = X_Initial+ (K * R); */
  /* :  P_Update = P_Predicted * 1 - (K .* H); */
  K_idx_0 /= rtb_Source;
  K_idx_1 /= rtb_Source;

  /* :  Capacity_EKF = X_Update(1); */
  rtb_Source = K_idx_0 * EKF_B.V1Vs;
  EKF_B.Capacity_EKF_c = rtb_Source + EKF_B.Total_CapacityRemains_mAh;

  /* :  Voltage_EKF = X_Update(2); */
  /* :  P_Old = [P_Update(1,1) P_Update(1,2); P_Update(2,1) P_Update(2,2)]; */
  H_idx_0 *= K_idx_0;
  EKF_DW.P_Old[0] = rtb_Source_e - H_idx_0;
  EKF_DW.P_Old[2] = A_1[2] - (-K_idx_1);
  EKF_DW.P_Old[1] = A_1[1] - H_idx_0;
  EKF_DW.P_Old[3] = P_Predicted_idx_3 - (-K_idx_1);

  /* Outport: '<Root>/<Total_Discharge_mAh>' */
  EKF_Y.Total_Discharge_mAh = EKF_B.Total_CapacityRemains_mAh;

  /* Outport: '<Root>/<Total_CapacityRemains_mAh>' */
  EKF_Y.Total_CapacityRemains_mAh = EKF_B.Switch;

  /* Gain: '<S7>/Gain' incorporates:
   *  Inport: '<Root>/Max_PackCapacity_mAh'
   *  Product: '<S7>/Divide'
   */
  rtb_SOC_Pct = rtb_SOC_Pct / EKF_U.Max_PackCapacity_mAh * 100.0;

  /* Saturate: '<S2>/Saturation1' */
  if (rtb_SOC_Pct > 100.0) {
    /* Outport: '<Root>/<SOC_CC>' */
    EKF_Y.SOC_CC = 100.0;
  } else if (rtb_SOC_Pct < 0.0) {
    /* Outport: '<Root>/<SOC_CC>' */
    EKF_Y.SOC_CC = 0.0;
  } else {
    /* Outport: '<Root>/<SOC_CC>' */
    EKF_Y.SOC_CC = rtb_SOC_Pct;
  }

  /* End of Saturate: '<S2>/Saturation1' */

  /* Outport: '<Root>/Voltage_EKF' incorporates:
   *  MATLAB Function: '<S1>/EKF_Function'
   */
  EKF_Y.Voltage_EKF = rtb_Source + EKF_B.V1Vs_b;

  /* Outport: '<Root>/Terminal_Voltage' */
  EKF_Y.Terminal_Voltage = EKF_B.V1Vs_b;

  /* Outport: '<Root>/SOC_EKF' */
  EKF_Y.SOC_EKF = EKF_B.SOC_EKF;

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  EKF_DW.UnitDelay_DSTATE = EKF_B.Capacity_EKF_c;

  /* Update for Memory: '<S11>/Memory' */
  EKF_DW.Memory_PreviousInput_a = EKF_B.DataTypeConversion2;

  /* Update for Memory: '<S7>/Memory' incorporates:
   *  Inport: '<Root>/Time_sec'
   */
  EKF_DW.Memory_PreviousInput = EKF_U.Time_sec;

  /* Update for Memory: '<S7>/Memory1' */
  EKF_DW.Memory1_PreviousInput = EKF_B.Switch;

  /* Update for UnitDelay: '<S19>/Unit Delay' */
  EKF_DW.UnitDelay_DSTATE_l = EKF_B.V1_i;

  /* Update for Memory: '<S19>/Memory1' */
  EKF_DW.Memory1_PreviousInput_m = EKF_B.V1_i;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++EKF_M->Timing.clockTick0)) {
    ++EKF_M->Timing.clockTickH0;
  }

  EKF_M->Timing.t[0] = EKF_M->Timing.clockTick0 * EKF_M->Timing.stepSize0 +
    EKF_M->Timing.clockTickH0 * EKF_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [960.0s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 960.0, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    EKF_M->Timing.clockTick1++;
    if (!EKF_M->Timing.clockTick1) {
      EKF_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void EKF_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)EKF_M, 0,
                sizeof(RT_MODEL_EKF_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&EKF_M->solverInfo, &EKF_M->Timing.simTimeStep);
    rtsiSetTPtr(&EKF_M->solverInfo, &rtmGetTPtr(EKF_M));
    rtsiSetStepSizePtr(&EKF_M->solverInfo, &EKF_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&EKF_M->solverInfo, (&rtmGetErrorStatus(EKF_M)));
    rtsiSetRTModelPtr(&EKF_M->solverInfo, EKF_M);
  }

  rtsiSetSimTimeStep(&EKF_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&EKF_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(EKF_M, &EKF_M->Timing.tArray[0]);
  EKF_M->Timing.stepSize0 = 960.0;

  /* block I/O */
  (void) memset(((void *) &EKF_B), 0,
                sizeof(B_EKF_T));

  /* states (dwork) */
  (void) memset((void *)&EKF_DW, 0,
                sizeof(DW_EKF_T));

  /* external inputs */
  (void)memset(&EKF_U, 0, sizeof(ExtU_EKF_T));

  /* external outputs */
  (void)memset(&EKF_Y, 0, sizeof(ExtY_EKF_T));

  /* Start for DataStoreMemory: '<S2>/Data Store Memory' */
  EKF_DW.CapacityRemains_mAh = 0.0;
  EKF_PrevZCX.TriggeredSubsystem_Trig_ZCE = POS_ZCSIG;

  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay' */
  EKF_DW.UnitDelay_DSTATE = 0.0;

  /* InitializeConditions for Memory: '<S11>/Memory' */
  EKF_DW.Memory_PreviousInput_a = false;

  /* InitializeConditions for Memory: '<S7>/Memory' */
  EKF_DW.Memory_PreviousInput = 0.0;

  /* InitializeConditions for Memory: '<S7>/Memory1' */
  EKF_DW.Memory1_PreviousInput = 0.0;

  /* InitializeConditions for UnitDelay: '<S19>/Unit Delay' */
  EKF_DW.UnitDelay_DSTATE_l = 0.0;

  /* InitializeConditions for Memory: '<S19>/Memory1' */
  EKF_DW.Memory1_PreviousInput_m = 0.0;

  /* SystemInitialize for Chart: '<S2>/StateMachine' */
  EKF_DW.is_active_c3_EKF = 0U;
  EKF_DW.is_c3_EKF = EKF_IN_NO_ACTIVE_CHILD;

  /* SystemInitialize for Outport: '<Root>/<CC_State>' incorporates:
   *  Chart: '<S2>/StateMachine'
   */
  EKF_Y.CC_State = 0.0;

  /* SystemInitialize for Outport: '<Root>/<Initial_Capacity_mAh>' incorporates:
   *  Chart: '<S2>/StateMachine'
   */
  EKF_Y.Initial_Capacity_mAh = 0.0;

  /* SystemInitialize for Enabled SubSystem: '<S11>/POSITIVE Edge' */
  /* SystemInitialize for RelationalOperator: '<S14>/Relational Operator1' incorporates:
   *  Outport: '<S14>/OUT'
   */
  EKF_B.RelationalOperator1 = false;

  /* End of SystemInitialize for SubSystem: '<S11>/POSITIVE Edge' */

  /* SystemInitialize for Enabled SubSystem: '<S11>/NEGATIVE Edge' */
  /* SystemInitialize for RelationalOperator: '<S13>/Relational Operator1' incorporates:
   *  Outport: '<S13>/OUT'
   */
  EKF_B.RelationalOperator1_j = false;

  /* End of SystemInitialize for SubSystem: '<S11>/NEGATIVE Edge' */

  /* SystemInitialize for MATLAB Function: '<S1>/EKF_Function' */
  /* :  P_Old = [0 0;0 0]; */
  EKF_DW.P_Old[0] = 0.0;
  EKF_DW.P_Old[1] = 0.0;
  EKF_DW.P_Old[2] = 0.0;
  EKF_DW.P_Old[3] = 0.0;
}

/* Model terminate function */
void EKF_terminate(void)
{
  /* (no terminate code required) */
}
