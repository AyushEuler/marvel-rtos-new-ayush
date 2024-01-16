/*
 * DataPipeline.c
 *
 * Code generation for model "DataPipeline".
 *
 * Model version              : 4.1075
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Mon Dec 18 14:37:59 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#include "DataPipeline.h"
#include <string.h>
#include "MaximumFunction.h"
#include "MinimumFunction.h"
#include "AverageFunction.h"
#include "DynamicCurrentLimits_perParallelCell.h"
#include "I2t_perParallelCell.h"
#include <math.h>
#include "rtwtypes.h"
#include "multiword_types.h"
#include "DataPipeline_types.h"
#include "DataPipeline_private.h"

/* Block signals (default storage) */
B_DataPipeline_T DataPipeline_B;

/* Block states (default storage) */
DW_DataPipeline_T DataPipeline_DW;

/* External inputs (root inport signals with default storage) */
ExtU_DataPipeline_T DataPipeline_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_DataPipeline_T DataPipeline_Y;

/* Real-time model */
static RT_MODEL_DataPipeline_T DataPipeline_M_;
RT_MODEL_DataPipeline_T *const DataPipeline_M = &DataPipeline_M_;
const DataPipelineBus DataPipeline_rtZDataPipelineBus = { 0,/* Current_mA */
  { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },/* Voltages_mV */
    0,                                 /* mV_min */
    0,                                 /* mV_max */
    0                                  /* VoltageDelta_mV */
  },                                   /* VoltageSenseBus */
  0,                                   /* MaxTemperature_C */
  0,                                   /* DeltaTemperature_C */
  0,                                   /* MinTemperature_C */
  0,                                   /* DeltaTemperatureGroup1_C */
  0,                                   /* DeltaTemperatureGroup2_C */

  { { { 0, 0, 0, 0, 0, 0, 0 },         /* Temperatures_C */
      0,                               /* T_min */
      0,                               /* T_max */
      0                                /* TemperatureDelta_C */
    }, { { 0, 0, 0, 0, 0, 0, 0 },      /* Temperatures_C */
      0,                               /* T_min */
      0,                               /* T_max */
      0                                /* TemperatureDelta_C */
    } },                               /* TemperatureSenseBus */
  0U,                                  /* isChargeEn */
  false,                               /* isDynamicCurrentEn */
  0U,                                  /* nCells_series */
  0,                                   /* nCells_parallel */
  0,                                   /* DCL_ChargingCurrent_A */
  0,                                   /* DCL_DisChargingCurrent_A */
  0,                                   /* i2t_ChargingCurrent_A2sec */
  0,                                   /* i2t_DisChargingCurrent_A2sec */
  0.0F,                                /* LoopTimer_SOC_msec */

  { false,                             /* SlowCharge */
    false,                             /* FastCharge */
    false,                             /* isChargerConnected */
    false,                             /* TransmissionFail */
    false,                             /* VCUPermanentFail */
    false,                             /* ServiceMode */
    false,                             /* BalanceEn_atRest */
    false,                             /* EmergencyDisconnectEn */
    0U                                 /* ChargersCurrentLimit_A */
  }                                    /* VCU */
};

boolean_T sMultiWordGe(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  return sMultiWordCmp(u1, u2, n) >= 0;
}

int32_T sMultiWordCmp(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  int32_T y;
  uint32_T su1;
  su1 = u1[n - 1] & 2147483648U;
  if ((u2[n - 1] & 2147483648U) != su1) {
    y = su1 != 0U ? -1 : 1;
  } else {
    int32_T i;
    y = 0;
    i = n;
    while ((y == 0) && (i > 0)) {
      uint32_T u2i;
      i--;
      su1 = u1[i];
      u2i = u2[i];
      if (su1 != u2i) {
        y = su1 > u2i ? 1 : -1;
      }
    }
  }

  return y;
}

void sMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T i;
  int32_T nb;
  int32_T nc;
  uint32_T u1i;
  uint32_T ys;
  nb = (int32_T)(n2 >> 5);
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  nc = nb > n ? n : nb;
  u1i = 0U;
  for (i = 0; i < nc; i++) {
    y[i] = 0U;
  }

  if (nb < n) {
    uint32_T nl;
    nl = n2 - ((uint32_T)nb << 5);
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      for (nc = 0; nc < nb; nc++) {
        uint32_T yi;
        yi = u1i >> (32U - nl);
        u1i = u1[nc];
        y[i] = u1i << nl | yi;
        i++;
      }

      if (i < n) {
        y[i] = u1i >> (32U - nl) | ys << nl;
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void sLong2MultiWord(int32_T u, uint32_T y[], int32_T n)
{
  int32_T i;
  uint32_T yi;
  y[0] = (uint32_T)u;
  yi = u < 0 ? MAX_uint32_T : 0U;
  for (i = 1; i < n; i++) {
    y[i] = yi;
  }
}

void sMultiWordMul_dp(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
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

real32_T sMultiWord2Single(const uint32_T u1[], int32_T n1, int32_T e1)
{
  int32_T exp_0;
  int32_T i;
  real32_T y;
  y = 0.0F;
  exp_0 = e1;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    uint32_T cb;
    cb = 1U;
    for (i = 0; i < n1; i++) {
      uint32_T u1i;
      u1i = ~u1[i];
      cb += u1i;
      y -= (real32_T)ldexp((real32_T)cb, exp_0);
      cb = (uint32_T)(cb < u1i);
      exp_0 += 32;
    }
  } else {
    for (i = 0; i < n1; i++) {
      y += (real32_T)ldexp((real32_T)u1[i], exp_0);
      exp_0 += 32;
    }
  }

  return y;
}

int32_T MultiWord2sLong(const uint32_T u[])
{
  return (int32_T)u[0];
}

void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T i;
  int32_T i1;
  int32_T nb;
  uint32_T ys;
  nb = (int32_T)(n2 >> 5);
  i = 0;
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  if (nb < n1) {
    int32_T nc;
    uint32_T nr;
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - ((uint32_T)nb << 5);
    if (nr > 0U) {
      uint32_T u1i;
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        uint32_T yi;
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = u1i << (32U - nr) | yi;
        i++;
      }

      y[i] = (nc < n1 ? u1[nc] : ys) << (32U - nr) | u1i >> nr;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void mul_wide_s32_dp(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo)
{
  uint32_T absIn0;
  uint32_T absIn1;
  uint32_T in0Hi;
  uint32_T in0Lo;
  uint32_T in1Hi;
  uint32_T productHiLo;
  uint32_T productLoHi;
  absIn0 = in0 < 0 ? ~(uint32_T)in0 + 1U : (uint32_T)in0;
  absIn1 = in1 < 0 ? ~(uint32_T)in1 + 1U : (uint32_T)in1;
  in0Hi = absIn0 >> 16U;
  in0Lo = absIn0 & 65535U;
  in1Hi = absIn1 >> 16U;
  absIn0 = absIn1 & 65535U;
  productHiLo = in0Hi * absIn0;
  productLoHi = in0Lo * in1Hi;
  absIn0 *= in0Lo;
  absIn1 = 0U;
  in0Lo = (productLoHi << 16U) + absIn0;
  if (in0Lo < absIn0) {
    absIn1 = 1U;
  }

  absIn0 = in0Lo;
  in0Lo += productHiLo << 16U;
  if (in0Lo < absIn0) {
    absIn1++;
  }

  absIn0 = (((productLoHi >> 16U) + (productHiLo >> 16U)) + in0Hi * in1Hi) +
    absIn1;
  if ((in0 != 0) && ((in1 != 0) && ((in0 > 0) != (in1 > 0)))) {
    absIn0 = ~absIn0;
    in0Lo = ~in0Lo;
    in0Lo++;
    if (in0Lo == 0U) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = in0Lo;
}

int32_T mul_s32_sat_dp(int32_T a, int32_T b)
{
  int32_T result;
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32_dp(a, b, &u32_chi, &u32_clo);
  if (((int32_T)u32_chi > 0) || ((u32_chi == 0U) && (u32_clo >= 2147483648U))) {
    result = MAX_int32_T;
  } else if (((int32_T)u32_chi < -1) || (((int32_T)u32_chi == -1) && (u32_clo <
               2147483648U))) {
    result = MIN_int32_T;
  } else {
    result = (int32_T)u32_clo;
  }

  return result;
}

int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator)
{
  return (((numerator < 0) != (denominator < 0)) && (numerator % denominator !=
           0) ? -1 : 0) + numerator / denominator;
}

/* Model step function */
void DataPipeline_step(void)
{
  int64m_T tmp;
  int64m_T tmp_0;
  int64m_T tmp_3;
  int64m_T tmp_4;
  int64m_T tmp_7;
  int96m_T tmp_5;
  int96m_T tmp_6;
  int96m_T tmp_8;
  real_T rtb_Temperature_C;
  int32_T i;
  int32_T maxV;
  int32_T minV;
  uint32_T tmp_1;
  uint32_T tmp_2;
  int16_T DataPipelineb_tmp;
  int16_T maxV_0;
  int16_T maxV_1;
  int16_T minV_0;
  int16_T minV_1;
  int16_T u1;

  /* MinMax: '<S1>/MinMax1' incorporates:
   *  Inport: '<Root>/VoltageSense_mV'
   */
  minV = DataPipeline_U.VoltageSense[0];

  /* MinMax: '<S1>/MinMax' incorporates:
   *  Inport: '<Root>/VoltageSense_mV'
   */
  maxV = DataPipeline_U.VoltageSense[0];
  for (i = 0; i < 19; i++) {
    int32_T u1_tmp;

    /* MinMax: '<S1>/MinMax1' incorporates:
     *  Inport: '<Root>/VoltageSense_mV'
     *  MinMax: '<S1>/MinMax'
     */
    u1_tmp = DataPipeline_U.VoltageSense[i + 1];
    if (minV > u1_tmp) {
      minV = u1_tmp;
    }

    /* MinMax: '<S1>/MinMax' */
    if (maxV < u1_tmp) {
      maxV = u1_tmp;
    }
  }

  /* Sum: '<S1>/Subtract' incorporates:
   *  MinMax: '<S1>/MinMax'
   *  MinMax: '<S1>/MinMax1'
   */
  DataPipeline_Y.DataPipelineb.VoltageSenseBus.VoltageDelta_mV = maxV - minV;

  /* BusCreator: '<S1>/Bus Creator' incorporates:
   *  Inport: '<Root>/VoltageSense_mV'
   *  MinMax: '<S1>/MinMax'
   *  MinMax: '<S1>/MinMax1'
   */
  memcpy(&DataPipeline_Y.DataPipelineb.VoltageSenseBus.Voltages_mV[0],
         &DataPipeline_U.VoltageSense[0], 20U * sizeof(int32_T));
  DataPipeline_Y.DataPipelineb.VoltageSenseBus.mV_min = minV;
  DataPipeline_Y.DataPipelineb.VoltageSenseBus.mV_max = maxV;

  /* MinMax: '<S1>/Min4' incorporates:
   *  Inport: '<Root>/Temperatures2_C'
   */
  maxV_0 = DataPipeline_U.Temperatures2_C[0];

  /* MinMax: '<S1>/Min3' incorporates:
   *  Inport: '<Root>/Temperatures1_C'
   */
  maxV_1 = DataPipeline_U.Temperatures1_C[0];
  for (i = 0; i < 6; i++) {
    /* MinMax: '<S1>/Min4' incorporates:
     *  Inport: '<Root>/Temperatures2_C'
     */
    u1 = DataPipeline_U.Temperatures2_C[i + 1];
    if (maxV_0 < u1) {
      maxV_0 = u1;
    }

    /* MinMax: '<S1>/Min3' incorporates:
     *  Inport: '<Root>/Temperatures1_C'
     */
    u1 = DataPipeline_U.Temperatures1_C[i + 1];
    if (maxV_1 < u1) {
      maxV_1 = u1;
    }
  }

  /* MATLAB Function: '<S1>/MaximumFunction' incorporates:
   *  Inport: '<Root>/nCelltemperatureGroups'
   *  MinMax: '<S1>/Min3'
   *  MinMax: '<S1>/Min4'
   */
  DataPipeline_MaximumFunction(DataPipeline_U.nCelltemperatureGroups, maxV_0,
    maxV_1, &DataPipeline_B.sf_MaximumFunction);

  /* MinMax: '<S1>/Min2' incorporates:
   *  Inport: '<Root>/Temperatures1_C'
   */
  minV_0 = DataPipeline_U.Temperatures1_C[0];

  /* MinMax: '<S1>/Min1' incorporates:
   *  Inport: '<Root>/Temperatures2_C'
   */
  minV_1 = DataPipeline_U.Temperatures2_C[0];
  for (i = 0; i < 6; i++) {
    /* MinMax: '<S1>/Min2' incorporates:
     *  Inport: '<Root>/Temperatures1_C'
     */
    u1 = DataPipeline_U.Temperatures1_C[i + 1];
    if (minV_0 > u1) {
      minV_0 = u1;
    }

    /* MinMax: '<S1>/Min1' incorporates:
     *  Inport: '<Root>/Temperatures2_C'
     */
    u1 = DataPipeline_U.Temperatures2_C[i + 1];
    if (minV_1 > u1) {
      minV_1 = u1;
    }
  }

  /* MATLAB Function: '<S1>/MinimumFunction' incorporates:
   *  Inport: '<Root>/nCelltemperatureGroups'
   *  MinMax: '<S1>/Min1'
   *  MinMax: '<S1>/Min2'
   */
  DataPipeline_MinimumFunction(DataPipeline_U.nCelltemperatureGroups, minV_0,
    minV_1, &DataPipeline_B.sf_MinimumFunction);

  /* Sum: '<S1>/Add' */
  DataPipeline_Y.DataPipelineb.DeltaTemperature_C = (int16_T)
    (DataPipeline_B.sf_MaximumFunction.Max -
     DataPipeline_B.sf_MinimumFunction.Min);

  /* Sum: '<S1>/Add1' incorporates:
   *  MinMax: '<S1>/Min2'
   *  MinMax: '<S1>/Min3'
   *  Sum: '<S1>/Subtract1'
   */
  u1 = (int16_T)(maxV_1 - minV_0);
  DataPipeline_Y.DataPipelineb.DeltaTemperatureGroup1_C = u1;

  /* Sum: '<S1>/Add2' incorporates:
   *  MinMax: '<S1>/Min1'
   *  MinMax: '<S1>/Min4'
   *  Sum: '<S1>/Subtract2'
   */
  DataPipelineb_tmp = (int16_T)(maxV_0 - minV_1);
  DataPipeline_Y.DataPipelineb.DeltaTemperatureGroup2_C = DataPipelineb_tmp;

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  Concatenate: '<S1>/Vector Concatenate'
   *  Inport: '<Root>/Temperatures1_C'
   *  MinMax: '<S1>/Min2'
   *  MinMax: '<S1>/Min3'
   */
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[0].T_min = minV_0;
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[0].T_max = maxV_1;
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[0].TemperatureDelta_C = u1;
  for (i = 0; i < 7; i++) {
    DataPipeline_Y.DataPipelineb.TemperatureSenseBus[0].Temperatures_C[i] =
      DataPipeline_U.Temperatures1_C[i];

    /* BusCreator: '<S1>/Bus Creator2' incorporates:
     *  Inport: '<Root>/Temperatures1_C'
     *  Inport: '<Root>/Temperatures2_C'
     */
    DataPipeline_Y.DataPipelineb.TemperatureSenseBus[1].Temperatures_C[i] =
      DataPipeline_U.Temperatures2_C[i];
  }

  /* End of BusCreator: '<S1>/Bus Creator1' */

  /* BusCreator: '<S1>/Bus Creator2' incorporates:
   *  Concatenate: '<S1>/Vector Concatenate'
   *  MinMax: '<S1>/Min1'
   *  MinMax: '<S1>/Min4'
   */
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[1].T_min = minV_1;
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[1].T_max = maxV_0;
  DataPipeline_Y.DataPipelineb.TemperatureSenseBus[1].TemperatureDelta_C =
    DataPipelineb_tmp;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/CurrentSense_mA'
   *  Inport: '<Root>/Thresholds'
   *  RelationalOperator: '<S1>/GreaterThan'
   *  RelationalOperator: '<S1>/GreaterThan1'
   */
  if (DataPipeline_U.CurrentSense >
      DataPipeline_U.Thresholds.ChargeDetectionThreshold_mA) {
    DataPipeline_Y.DataPipelineb.isChargeEn = 1U;
  } else {
    /* RelationalOperator: '<S1>/GreaterThan1' */
    sLong2MultiWord(DataPipeline_U.CurrentSense, &tmp_0.chunks[0U], 2);
    sMultiWordShl(&tmp_0.chunks[0U], 2, 31U, &tmp.chunks[0U], 2);

    /* Gain: '<S1>/Gain' */
    tmp_1 = 1932735283U;
    tmp_2 = (uint32_T)DataPipeline_U.Thresholds.ChargeDetectionThreshold_mA;
    sMultiWordMul_dp(&tmp_1, 1, &tmp_2, 1, &tmp_0.chunks[0U], 2);
    DataPipeline_Y.DataPipelineb.isChargeEn = sMultiWordGe(&tmp.chunks[0U],
      &tmp_0.chunks[0U], 2);
  }

  /* End of Switch: '<S1>/Switch' */

  /* S-Function (sdspstatfcns): '<S1>/Mean' incorporates:
   *  Inport: '<Root>/Temperatures1_C'
   */
  DataPipeline_DW.Mean_AccVal = DataPipeline_U.Temperatures1_C[0];
  maxV = 1;
  for (minV = 5; minV >= 0; minV--) {
    DataPipeline_DW.Mean_AccVal += DataPipeline_U.Temperatures1_C[maxV];
    maxV++;
  }

  /* S-Function (sdspstatfcns): '<S1>/Mean1' incorporates:
   *  Inport: '<Root>/Temperatures2_C'
   */
  DataPipeline_DW.Mean1_AccVal = DataPipeline_U.Temperatures2_C[0];
  maxV = 1;
  for (minV = 5; minV >= 0; minV--) {
    DataPipeline_DW.Mean1_AccVal += DataPipeline_U.Temperatures2_C[maxV];
    maxV++;
  }

  /* MATLAB Function: '<S1>/AverageFunction' incorporates:
   *  Inport: '<Root>/nCelltemperatureGroups'
   *  S-Function (sdspstatfcns): '<S1>/Mean'
   *  S-Function (sdspstatfcns): '<S1>/Mean1'
   */
  DataPipeline_AverageFunction((int16_T)div_nde_s32_floor
    (DataPipeline_DW.Mean_AccVal, 7), (int16_T)div_nde_s32_floor
    (DataPipeline_DW.Mean1_AccVal, 7), DataPipeline_U.nCelltemperatureGroups,
    &DataPipeline_B.sf_AverageFunction);

  /* Switch: '<S1>/Switch4' incorporates:
   *  Inport: '<Root>/TemperatureSelectionforDynamicCurrent'
   */
  if (DataPipeline_U.TemperatureSelectionforDynamicC) {
    rtb_Temperature_C = DataPipeline_B.sf_AverageFunction.Average;
  } else {
    rtb_Temperature_C = DataPipeline_B.sf_MaximumFunction.Max;
  }

  /* End of Switch: '<S1>/Switch4' */

  /* MATLAB Function: '<S1>/DynamicCurrentLimits_perParallelCell' incorporates:
   *  Inport: '<Root>/SOC'
   */
  DynamicCurrentLimits_perParalle(DataPipeline_U.SOC, rtb_Temperature_C,
    &DataPipeline_B.sf_DynamicCurrentLimits_perPara);

  /* MATLAB Function: '<S1>/I2t_perParallelCell' incorporates:
   *  Inport: '<Root>/SOC'
   */
  DataPipelin_I2t_perParallelCell(DataPipeline_U.SOC, rtb_Temperature_C,
    &DataPipeline_B.sf_I2t_perParallelCell);

  /* Gain: '<S1>/Gain3' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Gain: '<S1>/Gain2'
   *  Gain: '<S1>/Gain5'
   */
  tmp_1 = (uint32_T)DataPipeline_P.nCells_parallel;
  tmp_2 = (uint32_T)DataPipeline_B.sf_I2t_perParallelCell.i2t_Charge_A2sec;
  sMultiWordMul_dp(&tmp_1, 1, &tmp_2, 1, &tmp_3.chunks[0U], 2);

  /* Gain: '<S1>/Gain4' */
  DataPipeline_Y.DataPipelineb.i2t_ChargingCurrent_A2sec = (int32_T)floorf(0.9F *
    sMultiWord2Single(&tmp_3.chunks[0U], 2, 0));

  /* Gain: '<S1>/Gain5' */
  tmp_2 = (uint32_T)DataPipeline_B.sf_I2t_perParallelCell.i2t_Discharge_A2sec;
  sMultiWordMul_dp(&tmp_1, 1, &tmp_2, 1, &tmp_4.chunks[0U], 2);

  /* Gain: '<S1>/Gain8' */
  DataPipeline_Y.DataPipelineb.i2t_DisChargingCurrent_A2sec = (int32_T)floorf
    (0.9F * sMultiWord2Single(&tmp_4.chunks[0U], 2, 0));

  /* BusCreator: '<S1>/Bus Creator3' incorporates:
   *  Inport: '<Root>/BalanceEn_atRest'
   *  Inport: '<Root>/ChargersCurrentLimit_A'
   *  Inport: '<Root>/EmergencyDisconnectEn'
   *  Inport: '<Root>/FastCharge'
   *  Inport: '<Root>/ServiceMode'
   *  Inport: '<Root>/SlowCharge'
   *  Inport: '<Root>/TransmissionFail'
   *  Inport: '<Root>/VCUPermanentFail'
   *  Inport: '<Root>/isChargerConnected'
   */
  DataPipeline_Y.DataPipelineb.VCU.SlowCharge = DataPipeline_U.SlowCharge;
  DataPipeline_Y.DataPipelineb.VCU.FastCharge = DataPipeline_U.FastCharge;
  DataPipeline_Y.DataPipelineb.VCU.isChargerConnected =
    DataPipeline_U.isChargerConnected;
  DataPipeline_Y.DataPipelineb.VCU.TransmissionFail =
    DataPipeline_U.TransmissionFail;
  DataPipeline_Y.DataPipelineb.VCU.VCUPermanentFail =
    DataPipeline_U.VCUPermanentFail;
  DataPipeline_Y.DataPipelineb.VCU.ServiceMode = DataPipeline_U.ServiceMode;
  DataPipeline_Y.DataPipelineb.VCU.BalanceEn_atRest =
    DataPipeline_U.BalanceEn_atRest;
  DataPipeline_Y.DataPipelineb.VCU.EmergencyDisconnectEn =
    DataPipeline_U.EmergencyDisconnectEn;
  DataPipeline_Y.DataPipelineb.VCU.ChargersCurrentLimit_A =
    DataPipeline_U.ChargersCurrentLimit_A;

  /* BusCreator: '<S1>/Bus Creator7' incorporates:
   *  Inport: '<Root>/CurrentSense_mA'
   *  Inport: '<Root>/isDynamicCurrentEn'
   *  Inport: '<Root>/nCells_Parallel'
   *  Inport: '<Root>/nCells_Series'
   *  Outport: '<Root>/DataPipelineb'
   */
  DataPipeline_Y.DataPipelineb.Current_mA = DataPipeline_U.CurrentSense;
  DataPipeline_Y.DataPipelineb.MaxTemperature_C =
    DataPipeline_B.sf_MaximumFunction.Max;
  DataPipeline_Y.DataPipelineb.MinTemperature_C =
    DataPipeline_B.sf_MinimumFunction.Min;
  DataPipeline_Y.DataPipelineb.isDynamicCurrentEn =
    DataPipeline_U.isDynamicCurrentEn;
  DataPipeline_Y.DataPipelineb.nCells_series = DataPipeline_U.nCells_series;
  DataPipeline_Y.DataPipelineb.nCells_parallel = DataPipeline_U.nCells_parallel;

  /* Gain: '<S1>/Gain1' */
  tmp_2 = (uint32_T)
    DataPipeline_B.sf_DynamicCurrentLimits_perPara.DCL_ChargingCurrent_A;
  sMultiWordMul_dp(&tmp_1, 1, &tmp_2, 1, &tmp_7.chunks[0U], 2);

  /* Gain: '<S1>/Gain6' */
  sMultiWordMul_dp(&DataPipeline_ConstP.pooled1.chunks[0], 2, &tmp_7.chunks[0U], 2,
                &tmp_6.chunks[0U], 3);
  sMultiWordShr(&tmp_6.chunks[0U], 3, 39U, &tmp_5.chunks[0U], 3);

  /* BusCreator: '<S1>/Bus Creator7' incorporates:
   *  Gain: '<S1>/Gain6'
   *  Outport: '<Root>/DataPipelineb'
   */
  DataPipeline_Y.DataPipelineb.DCL_ChargingCurrent_A = MultiWord2sLong
    (&tmp_5.chunks[0U]);

  /* Gain: '<S1>/Gain2' */
  tmp_2 = (uint32_T)
    DataPipeline_B.sf_DynamicCurrentLimits_perPara.DCL_DisChargingCurrent_A;
  sMultiWordMul_dp(&tmp_1, 1, &tmp_2, 1, &tmp_7.chunks[0U], 2);

  /* Gain: '<S1>/Gain7' */
  sMultiWordMul_dp(&DataPipeline_ConstP.pooled1.chunks[0], 2, &tmp_7.chunks[0U], 2,
                &tmp_8.chunks[0U], 3);
  sMultiWordShr(&tmp_8.chunks[0U], 3, 39U, &tmp_6.chunks[0U], 3);

  /* BusCreator: '<S1>/Bus Creator7' incorporates:
   *  Gain: '<S1>/Gain7'
   *  Inport: '<Root>/LoopTimer_SOC_msec'
   *  Outport: '<Root>/DataPipelineb'
   */
  DataPipeline_Y.DataPipelineb.DCL_DisChargingCurrent_A = MultiWord2sLong
    (&tmp_6.chunks[0U]);
  DataPipeline_Y.DataPipelineb.LoopTimer_SOC_msec =
    DataPipeline_U.LoopTimer_SOC_msec;
}

/* Model initialize function */
void DataPipeline_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(DataPipeline_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &DataPipeline_B), 0,
                sizeof(B_DataPipeline_T));

  /* states (dwork) */
  (void) memset((void *)&DataPipeline_DW, 0,
                sizeof(DW_DataPipeline_T));

  /* external inputs */
  (void)memset(&DataPipeline_U, 0, sizeof(ExtU_DataPipeline_T));

  /* external outputs */
  DataPipeline_Y.DataPipelineb = DataPipeline_rtZDataPipelineBus;
}

/* Model terminate function */
void DataPipeline_terminate(void)
{
  /* (no terminate code required) */
}
