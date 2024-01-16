/*
 * ChargingAlgorithm.c
 *
 * Code generation for model "ChargingAlgorithm".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:58:57 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (12), Warning (1), Error (0)
 */

#include "ChargingAlgorithm.h"
#include "rtwtypes.h"
#include <math.h>
#include "multiword_types.h"
#include "ChargingAlgorithm_types.h"
#include <string.h>
#include "ChargingAlgorithm_private.h"

/* Named constants for Chart: '<S1>/ChargingAlgorithm' */
#define ChargingAlgo_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define ChargingAlgor_IN_BatteryRevival ((uint8_T)1U)
#define ChargingAlgori_IN_ChargingLobby ((uint8_T)1U)
#define ChargingAlgorit_IN_CC_Balancing ((uint8_T)3U)
#define ChargingAlgorit_IN_FastCharging ((uint8_T)2U)
#define ChargingAlgorit_IN_SlowCharging ((uint8_T)3U)
#define ChargingAlgorithm_IN_CC        ((uint8_T)2U)
#define ChargingAlgorithm_IN_CV        ((uint8_T)4U)

/* Block signals (default storage) */
B_ChargingAlgorithm_T ChargingAlgorithm_B;

/* Block states (default storage) */
DW_ChargingAlgorithm_T ChargingAlgorithm_DW;

/* External inputs (root inport signals with default storage) */
ExtU_ChargingAlgorithm_T ChargingAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_ChargingAlgorithm_T ChargingAlgorithm_Y;

/* Real-time model */
static RT_MODEL_ChargingAlgorithm_T ChargingAlgorithm_M_;
RT_MODEL_ChargingAlgorithm_T *const ChargingAlgorithm_M = &ChargingAlgorithm_M_;
real_T sMultiWord2Double(const uint32_T u1[], int32_T n1, int32_T e1)
{
  real_T y;
  int32_T exp_0;
  int32_T i;
  y = 0.0;
  exp_0 = e1;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    uint32_T cb;
    cb = 1U;
    for (i = 0; i < n1; i++) {
      uint32_T u1i;
      u1i = ~u1[i];
      cb += u1i;
      y -= ldexp(cb, exp_0);
      cb = (uint32_T)(cb < u1i);
      exp_0 += 32;
    }
  } else {
    for (i = 0; i < n1; i++) {
      y += ldexp(u1[i], exp_0);
      exp_0 += 32;
    }
  }

  return y;
}

void MultiWordSignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[])
{
  int32_T i;
  int32_T n1m1;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  uint32_T mask;
  uint32_T ys;
  mask = 1U << (31U - n2);
  ys = (u1[n1 - 1] & mask) != 0U ? MAX_uint32_T : 0U;
  mask = (mask << 1U) - 1U;
  y[n1 - 1] = (u1[n1 - 1] & mask) | (~mask & ys);
}

void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
                   n2, uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T j;
  int32_T k;
  uint32_T cb;
  uint32_T cb1;
  uint32_T yk;
  boolean_T isNegative1;
  boolean_T isNegative2;
  isNegative1 = ((u1[n1 - 1] & 2147483648U) != 0U);
  isNegative2 = ((u2[n2 - 1] & 2147483648U) != 0U);
  cb1 = 1U;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (i = 0; i < n1; i++) {
    int32_T ni;
    uint32_T a0;
    uint32_T a1;
    uint32_T cb2;
    uint32_T u1i;
    cb = 0U;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (uint32_T)(u1i < cb1);
    }

    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    cb2 = 1U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      uint32_T b1;
      uint32_T w01;
      uint32_T w10;
      u1i = u2[j];
      if (isNegative2) {
        u1i = ~u1i + cb2;
        cb2 = (uint32_T)(u1i < cb2);
      }

      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (uint32_T)(yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1 != isNegative2) {
    cb = 1U;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (uint32_T)(yk < cb);
    }
  }
}

real_T uMultiWord2Double(const uint32_T u1[], int32_T n1, int32_T e1)
{
  real_T y;
  int32_T exp_1;
  int32_T i;
  y = 0.0;
  exp_1 = e1;
  for (i = 0; i < n1; i++) {
    y += ldexp(u1[i], exp_1);
    exp_1 += 32;
  }

  return y;
}

void MultiWordUnsignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[])
{
  int32_T i;
  int32_T n1m1;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  y[n1 - 1] = ((1U << (32U - n2)) - 1U) & u1[n1 - 1];
}

void uMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
                   n2, uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T j;
  int32_T k;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (i = 0; i < n1; i++) {
    int32_T ni;
    uint32_T a0;
    uint32_T a1;
    uint32_T cb;
    uint32_T u1i;
    cb = 0U;
    u1i = u1[i];
    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      uint32_T b1;
      uint32_T w01;
      uint32_T w10;
      uint32_T yk;
      u1i = u2[j];
      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (uint32_T)(yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }
}

/* Model step function */
void ChargingAlgorithm_step(void)
{
  int64m_T tmp;
  int64m_T tmp_0;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  int32_T rtb_VoltageDiff_mV;
  uint32_T tmp_1;
  uint32_T tmp_2;
  boolean_T tmp_5;
  boolean_T tmp_6;

  /* Chart: '<S1>/ChargingAlgorithm' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm == 0U) {
    ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm = 1U;
    ChargingAlgorithm_DW.durationCounter_2 = 0;
    ChargingAlgorithm_DW.durationCounter_1_l = 0;
    ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
      ChargingAlgori_IN_ChargingLobby;

    /* Outport: '<Root>/ChargingState' */
    /* :  ChargingState = ChargeState.NoCharge; */
    ChargingAlgorithm_Y.ChargingState = NoCharge;
  } else {
    switch (ChargingAlgorithm_DW.is_c5_ChargingAlgorithm) {
     case ChargingAlgori_IN_ChargingLobby:
      /* Outport: '<Root>/ChargingState' */
      ChargingAlgorithm_Y.ChargingState = NoCharge;

      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.FastCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      tmp_5 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
      if (tmp_5 || (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
        ChargingAlgorithm_DW.durationCounter_1_l = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_l * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.durationCounter_2_f = 0;
        ChargingAlgorithm_DW.durationCounter_1_o = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgorit_IN_FastCharging;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.FastCharge; */
        ChargingAlgorithm_Y.ChargingState = FastCharge;
        ChargingAlgorithm_DW.durationCounter_1_gu = 0;
        ChargingAlgorithm_DW.is_FastCharging = ChargingAlgor_IN_BatteryRevival;

        /* :  Constant_K = Thresholds.FastChargeAlgorithm.BatteryRevival_K; */
        ChargingAlgorithm_B.Constant_K =
          ChargingAlgorithm_U.Thresholds.FastChargeAlgorithm.BatteryRevival_K;
      } else {
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.SlowCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if (tmp_5 || (!ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge)) {
          ChargingAlgorithm_DW.durationCounter_2 = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_2 * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.durationCounter_2_b = 0;
          ChargingAlgorithm_DW.durationCounter_1_f = 0;
          ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
            ChargingAlgorit_IN_SlowCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.SlowCharge; */
          ChargingAlgorithm_Y.ChargingState = SlowCharge;
          ChargingAlgorithm_DW.durationCounter_1 = 0;
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.SlowChargeAlgorithm.BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.BatteryRevival_K;
        } else {
          /* :  Constant_K = Thresholds.SlowChargeAlgorithm.NoCharge_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.NoCharge_K;
        }
      }
      break;

     case ChargingAlgorit_IN_FastCharging:
      /* Outport: '<Root>/ChargingState' */
      ChargingAlgorithm_Y.ChargingState = FastCharge;

      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.FastCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
          ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
        ChargingAlgorithm_DW.durationCounter_1_o = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_o * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.durationCounter_2 = 0;
        ChargingAlgorithm_DW.durationCounter_1_l = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgori_IN_ChargingLobby;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.NoCharge; */
        ChargingAlgorithm_Y.ChargingState = NoCharge;
      } else {
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.SlowCharge == true && DataPipeline.VCU.FastCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if ((!ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) ||
            (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
          ChargingAlgorithm_DW.durationCounter_2_f = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_f * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
          ChargingAlgorithm_DW.durationCounter_2_b = 0;
          ChargingAlgorithm_DW.durationCounter_1_f = 0;
          ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
            ChargingAlgorit_IN_SlowCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.SlowCharge; */
          ChargingAlgorithm_Y.ChargingState = SlowCharge;
          ChargingAlgorithm_DW.durationCounter_1 = 0;
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.SlowChargeAlgorithm.BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.BatteryRevival_K;
        } else {
          switch (ChargingAlgorithm_DW.is_FastCharging) {
           case ChargingAlgor_IN_BatteryRevival:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_gu = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_gu * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_d = 0;
              ChargingAlgorithm_DW.is_FastCharging = ChargingAlgorithm_IN_CC;

              /* :  Constant_K = Thresholds.FastChargeAlgorithm.CC_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastChargeAlgorithm.CC_K;
            }
            break;

           case ChargingAlgorithm_IN_CC:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.Balancing_Min_Thr_mV, msec) >= Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
              ChargingAlgorithm_DW.durationCounter_1_d = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_d * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_e = 0;
              ChargingAlgorithm_DW.is_FastCharging =
                ChargingAlgorit_IN_CC_Balancing;

              /* :  Constant_K = Thresholds.FastChargeAlgorithm.CC_Balancing_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastChargeAlgorithm.CC_Balancing_K;
            }
            break;

           case ChargingAlgorit_IN_CC_Balancing:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_e = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_e * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.is_FastCharging = ChargingAlgorithm_IN_CV;

              /* :  Constant_K = Thresholds.FastChargeAlgorithm.CV_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastChargeAlgorithm.CV_K;
            }
            break;

           default:
            /* case IN_CV: */
            break;
          }
        }
      }
      break;

     default:
      /* Outport: '<Root>/ChargingState' */
      /* case IN_SlowCharging: */
      ChargingAlgorithm_Y.ChargingState = SlowCharge;

      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.SlowCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
          ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
        ChargingAlgorithm_DW.durationCounter_1_f = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_f * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.durationCounter_2 = 0;
        ChargingAlgorithm_DW.durationCounter_1_l = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgori_IN_ChargingLobby;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.NoCharge; */
        ChargingAlgorithm_Y.ChargingState = NoCharge;
      } else {
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.FastCharge == true && DataPipeline.VCU.SlowCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if ((!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) ||
            ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
          ChargingAlgorithm_DW.durationCounter_2_b = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_b * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
          ChargingAlgorithm_DW.durationCounter_2_f = 0;
          ChargingAlgorithm_DW.durationCounter_1_o = 0;
          ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
            ChargingAlgorit_IN_FastCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.FastCharge; */
          ChargingAlgorithm_Y.ChargingState = FastCharge;
          ChargingAlgorithm_DW.durationCounter_1_gu = 0;
          ChargingAlgorithm_DW.is_FastCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.FastChargeAlgorithm.BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.FastChargeAlgorithm.BatteryRevival_K;
        } else {
          switch (ChargingAlgorithm_DW.is_SlowCharging) {
           case ChargingAlgor_IN_BatteryRevival:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1 = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1 * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_g = 0;
              ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgorithm_IN_CC;

              /* :  Constant_K = Thresholds.SlowChargeAlgorithm.CC_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.CC_K;
            }
            break;

           case ChargingAlgorithm_IN_CC:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.Balancing_Min_Thr_mV, msec) >= Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
              ChargingAlgorithm_DW.durationCounter_1_g = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_g * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_k = 0;
              ChargingAlgorithm_DW.is_SlowCharging =
                ChargingAlgorit_IN_CC_Balancing;

              /* :  Constant_K = Thresholds.SlowChargeAlgorithm.CC_Balancing_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.CC_Balancing_K;
            }
            break;

           case ChargingAlgorit_IN_CC_Balancing:
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_k = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_k * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgorithm_IN_CV;

              /* :  Constant_K = Thresholds.SlowChargeAlgorithm.CV_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowChargeAlgorithm.CV_K;
            }
            break;

           default:
            /* case IN_CV: */
            break;
          }
        }
      }
      break;
    }
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1 = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
    ChargingAlgorithm_DW.durationCounter_1_g++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_g = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_k++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_k = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_gu++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_gu = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
    ChargingAlgorithm_DW.durationCounter_1_d++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_d = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_e++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_e = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
      ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    ChargingAlgorithm_DW.durationCounter_1_l++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_l = 0;
  }

  tmp_5 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
  if (tmp_5 || (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
    ChargingAlgorithm_DW.durationCounter_1_o++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_o = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
      ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
    ChargingAlgorithm_DW.durationCounter_2++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2 = 0;
  }

  tmp_6 = !ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge;
  if (tmp_5 || tmp_6) {
    ChargingAlgorithm_DW.durationCounter_1_f++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_f = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.FastCharge && tmp_6) {
    ChargingAlgorithm_DW.durationCounter_2_b++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_b = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge &&
      ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    ChargingAlgorithm_DW.durationCounter_2_f++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_f = 0;
  }

  /* End of Chart: '<S1>/ChargingAlgorithm' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    rtb_VoltageDiff_mV =
      ChargingAlgorithm_U.Thresholds.targetVoltagesFastCharging;
  } else {
    rtb_VoltageDiff_mV =
      ChargingAlgorithm_U.Thresholds.targetVoltageSlowCharging;
  }

  /* Sum: '<S1>/Minus' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Switch: '<S1>/Switch'
   */
  rtb_VoltageDiff_mV -= ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_max;

  /* Gain: '<S1>/Gain2' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  tmp_1 = 1000U;
  tmp_2 = (uint32_T)ChargingAlgorithm_U.DataPipeline.DCL_ChargingCurrent_A;
  sMultiWordMul(&tmp_1, 1, &tmp_2, 1, &tmp_0.chunks[0U], 2);
  MultiWordSignedWrap(&tmp_0.chunks[0U], 2, 16U, &tmp.chunks[0U]);

  /* Gain: '<S1>/Gain3' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  uMultiWordMul(&tmp_1, 1,
                &ChargingAlgorithm_U.DataPipeline.VCU.ChargersCurrentLimit_A, 1,
                &tmp_4.chunks[0U], 2);
  MultiWordUnsignedWrap(&tmp_4.chunks[0U], 2, 16U, &tmp_3.chunks[0U]);

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S1>/Constant'
   *  Product: '<S1>/Product'
   */
  if (rtb_VoltageDiff_mV > 0) {
    rtb_VoltageDiff_mV = (int32_T)floorf(ChargingAlgorithm_B.Constant_K *
      (real32_T)rtb_VoltageDiff_mV);
  } else {
    rtb_VoltageDiff_mV = 0;
  }

  /* Outport: '<Root>/ChargingCurrentReq_mA' incorporates:
   *  MinMax: '<S1>/Min'
   *  Switch: '<S1>/Switch1'
   */
  ChargingAlgorithm_Y.ChargingCurrentReq_mA = (int32_T)floor(fmin(fmin
    (rtb_VoltageDiff_mV, sMultiWord2Double(&tmp.chunks[0U], 2, 0)),
    uMultiWord2Double(&tmp_3.chunks[0U], 2, 0)));
}

/* Model initialize function */
void ChargingAlgorithm_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(ChargingAlgorithm_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &ChargingAlgorithm_B), 0,
                sizeof(B_ChargingAlgorithm_T));

  /* states (dwork) */
  (void) memset((void *)&ChargingAlgorithm_DW, 0,
                sizeof(DW_ChargingAlgorithm_T));

  /* external inputs */
  (void)memset(&ChargingAlgorithm_U, 0, sizeof(ExtU_ChargingAlgorithm_T));

  /* external outputs */
  (void)memset(&ChargingAlgorithm_Y, 0, sizeof(ExtY_ChargingAlgorithm_T));
  ChargingAlgorithm_Y.ChargingState = NoCharge;

  /* SystemInitialize for Chart: '<S1>/ChargingAlgorithm' */
  ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm = 0U;
  ChargingAlgorithm_DW.is_c5_ChargingAlgorithm = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_B.Constant_K = 0.0F;
}

/* Model terminate function */
void ChargingAlgorithm_terminate(void)
{
  /* (no terminate code required) */
}
