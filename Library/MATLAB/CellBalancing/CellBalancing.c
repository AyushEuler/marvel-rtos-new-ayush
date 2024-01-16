/*
 * CellBalancing.c
 *
 * Code generation for model "CellBalancing".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:45:49 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (12), Warning (1), Error (0)
 */

#include "CellBalancing.h"
#include "rtwtypes.h"
#include "CellBalancing_private.h"
#include <math.h>
#include "WriteMosfetsData.h"
#include "CellBalancing_types.h"
#include <string.h>

/* Named constants for Chart: '<S1>/BalancingStateMachine' */
#define CellBalancin_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define CellBalancing_IN_Even_Balance  ((uint8_T)1U)
#define CellBalancing_IN_Lobby         ((uint8_T)2U)
#define CellBalancing_IN_Odd_Balance   ((uint8_T)3U)
#define CellBalancing_IN_Terminate     ((uint8_T)4U)

/* Named constants for Chart: '<S3>/Chart' */
#define Cell_IN_BalancingTimeOn_Default ((uint8_T)2U)
#define IN_BalancingTimeOn_BasedOnTemp_ ((uint8_T)1U)

/* Named constants for Chart: '<S3>/Chart1' */
#define CellBalancing_IN_PCBTemp_Danger ((uint8_T)1U)
#define CellBalancing_IN_PCBTemp_Safe  ((uint8_T)2U)

/* Named constants for Chart: '<S3>/Chart2' */
#define Cell_IN_FastCharge_OVProtectThr ((uint8_T)1U)
#define Cell_IN_SlowCharge_OVProtectThr ((uint8_T)2U)

/* Block signals (default storage) */
B_CellBalancing_T CellBalancing_B;

/* Block states (default storage) */
DW_CellBalancing_T CellBalancing_DW;

/* External inputs (root inport signals with default storage) */
ExtU_CellBalancing_T CellBalancing_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_CellBalancing_T CellBalancing_Y;

/* Real-time model */
static RT_MODEL_CellBalancing_T CellBalancing_M_;
RT_MODEL_CellBalancing_T *const CellBalancing_M = &CellBalancing_M_;

/* Forward declaration for local functions */
static void CellBalancing_CellBalance(void);
const CellBalancingOutputBus CellBalancing_rtZCellBalancingO = { 0,/* MaxImbalance_mV */
  false,                               /* BalanceEn */
  Terminate                            /* CurrentBalancingState */
};

real_T rt_roundd_cb(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * Function for Chart: '<S1>/BalancingStateMachine'
 * function CellBalance(index)
 */
static void CellBalancing_CellBalance(void)
{
  /* :  sf_internal_predicateOutput = BalanceEn == 1 && (Voltage_mV(i)>(mV_min+Thresholds.Balancing_MaxAllowedImb_mV)); */
  if (CellBalancing_B.BalanceEn) {
    int32_T tmp;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    if ((CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min < 0) &&
        (CellBalancing_U.Thresholds.Balancing_MaxAllowedImb_mV < MIN_int32_T
         - CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min)) {
      tmp = MIN_int32_T;
    } else if ((CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min > 0) &&
               (CellBalancing_U.Thresholds.Balancing_MaxAllowedImb_mV >
                MAX_int32_T
                - CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min)) {
      tmp = MAX_int32_T;
    } else {
      tmp = CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min +
        CellBalancing_U.Thresholds.Balancing_MaxAllowedImb_mV;
    }

    /* End of Inport: '<Root>/DataPipeline' */
    if (CellBalancing_U.DataPipeline.VoltageSenseBus.Voltages_mV[(int32_T)
        CellBalancing_DW.i - 1] > tmp) {
      /* :  BalanceCmd(i)=1; */
      CellBalancing_B.BalanceCmd[(int32_T)CellBalancing_DW.i - 1] = 1;

      /* :  BalanceFetCmd(i,1) */
    } else {
      /* :  BalanceCmd(i)=0; */
      CellBalancing_B.BalanceCmd[(int32_T)CellBalancing_DW.i - 1] = 0;

      /* :  BalanceFetCmd(i,0) */
    }
  } else {
    /* :  BalanceCmd(i)=0; */
    CellBalancing_B.BalanceCmd[(int32_T)CellBalancing_DW.i - 1] = 0;

    /* :  BalanceFetCmd(i,0) */
  }
}

/* Model step function */
void CellBalancing_step(void)
{
  int32_T BalanceEn_tmp;
  int32_T i;
  int8_T rtb_BalanceCmd[20];
  boolean_T rtb_PCBTemp;

  /* Chart: '<S3>/Chart1' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (CellBalancing_DW.is_active_c26_CellBalancing == 0U) {
    CellBalancing_DW.is_active_c26_CellBalancing = 1U;
    CellBalancing_DW.is_c26_CellBalancing = CellBalancing_IN_PCBTemp_Safe;

    /* :  PCBTemp = true; */
    rtb_PCBTemp = true;
  } else if (CellBalancing_DW.is_c26_CellBalancing ==
             CellBalancing_IN_PCBTemp_Danger) {
    rtb_PCBTemp = false;

    /* :  sf_internal_predicateOutput = DataPipeline.TemperatureSenseBus(2).T_max < Thresholds.Temperature(2).Protections_OTC_Error_C || DataPipeline.TemperatureSenseBus(2).T_max < Thresholds.Temperature(2).Protections_OTD_Error_C; */
    if ((CellBalancing_U.DataPipeline.TemperatureSenseBus[1].T_max <
         CellBalancing_U.Thresholds.Temperature[1].Protections_OTC_Error_C) ||
        (CellBalancing_U.DataPipeline.TemperatureSenseBus[1].T_max <
         CellBalancing_U.Thresholds.Temperature[1].Protections_OTD_Error_C)) {
      CellBalancing_DW.is_c26_CellBalancing = CellBalancing_IN_PCBTemp_Safe;

      /* :  PCBTemp = true; */
      rtb_PCBTemp = true;
    }
  } else {
    /* case IN_PCBTemp_Safe: */
    rtb_PCBTemp = true;

    /* :  sf_internal_predicateOutput = DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTC_Error_C || DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTD_Error_C; */
    if ((CellBalancing_U.DataPipeline.TemperatureSenseBus[1].T_max >=
         CellBalancing_U.Thresholds.Temperature[1].Protections_OTC_Error_C) ||
        (CellBalancing_U.DataPipeline.TemperatureSenseBus[1].T_max >=
         CellBalancing_U.Thresholds.Temperature[1].Protections_OTD_Error_C)) {
      CellBalancing_DW.is_c26_CellBalancing = CellBalancing_IN_PCBTemp_Danger;

      /* :  PCBTemp = false; */
      rtb_PCBTemp = false;
    }
  }

  /* End of Chart: '<S3>/Chart1' */

  /* Chart: '<S3>/Chart2' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (CellBalancing_DW.is_active_c4_CellBalancing == 0U) {
    CellBalancing_DW.is_active_c4_CellBalancing = 1U;
    CellBalancing_DW.is_c4_CellBalancing = Cell_IN_SlowCharge_OVProtectThr;

    /* :  Balancing_OVThr_mV = Thresholds.Balancing_SlowCh_OVThr_mV; */
    CellBalancing_B.Balancing_OVThr_mV =
      CellBalancing_U.Thresholds.Balancing_SlowCh_OVThr_mV;
  } else if (CellBalancing_DW.is_c4_CellBalancing ==
             Cell_IN_FastCharge_OVProtectThr) {
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.FastCharge == false; */
    if ((!CellBalancing_U.DataPipeline.VCU.isChargerConnected) ||
        (!CellBalancing_U.DataPipeline.VCU.FastCharge)) {
      CellBalancing_DW.is_c4_CellBalancing = Cell_IN_SlowCharge_OVProtectThr;

      /* :  Balancing_OVThr_mV = Thresholds.Balancing_SlowCh_OVThr_mV; */
      CellBalancing_B.Balancing_OVThr_mV =
        CellBalancing_U.Thresholds.Balancing_SlowCh_OVThr_mV;
    }

    /* case IN_SlowCharge_OVProtectThr: */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.FastCharge == true; */
  } else if (CellBalancing_U.DataPipeline.VCU.isChargerConnected &&
             CellBalancing_U.DataPipeline.VCU.FastCharge) {
    CellBalancing_DW.is_c4_CellBalancing = Cell_IN_FastCharge_OVProtectThr;

    /* :  Balancing_OVThr_mV = Thresholds.Balancing_FastCh_OVThr_mV; */
    CellBalancing_B.Balancing_OVThr_mV =
      CellBalancing_U.Thresholds.Balancing_FastCh_OVThr_mV;
  }

  /* End of Chart: '<S3>/Chart2' */

  /* Sum: '<S3>/Subtract' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Sum: '<S1>/Subtract'
   */
  BalanceEn_tmp = CellBalancing_U.DataPipeline.VoltageSenseBus.mV_max -
    CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min;

  /* Switch: '<S3>/Switch' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   *  Logic: '<S3>/AND'
   *  Logic: '<S3>/OR1'
   *  RelationalOperator: '<S3>/GreaterThan'
   *  RelationalOperator: '<S3>/GreaterThan1'
   *  RelationalOperator: '<S3>/GreaterThan2'
   *  RelationalOperator: '<S8>/Compare'
   *  Sum: '<S3>/Subtract'
   */
  CellBalancing_B.BalanceEn = (rtb_PCBTemp &&
    (CellBalancing_U.DataPipeline.VCU.BalanceEn_atRest ||
     CellBalancing_U.DataPipeline.VCU.isChargerConnected) &&
    (CellBalancing_U.DataPipeline.VoltageSenseBus.mV_min >
     CellBalancing_U.Thresholds.Balancing_Min_Thr_mV) &&
    (CellBalancing_U.DataPipeline.VoltageSenseBus.mV_max <
     CellBalancing_B.Balancing_OVThr_mV) && (BalanceEn_tmp >
    CellBalancing_U.Thresholds.Balancing_MaxAllowedImb_mV));

  /* Chart: '<S3>/Chart' incorporates:
   *  Inport: '<Root>/BalancingTempGroup'
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/StLineEq_varA'
   *  Inport: '<Root>/StLineEq_varB'
   *  Inport: '<Root>/Thresholds'
   */
  if (CellBalancing_DW.is_active_c6_CellBalancing == 0U) {
    CellBalancing_DW.is_active_c6_CellBalancing = 1U;
    CellBalancing_DW.durationCounter_1 = 0;
    CellBalancing_DW.is_c6_CellBalancing = Cell_IN_BalancingTimeOn_Default;

    /* :  BalancingTimeOn_msec = double(Balancing_DefaultBalancingTimeOn_msec); */
    CellBalancing_B.BalancingTimeOn_msec =
      CellBalancing_U.Thresholds.Balancing_DefaultBalancingTimeOn_msec;
  } else if (CellBalancing_DW.is_c6_CellBalancing ==
             IN_BalancingTimeOn_BasedOnTemp_) {
    real_T tmp;

    /* :  BalancingTimeOn_msec = double(A*x + B); */
    tmp = rt_roundd_cb((real_T)CellBalancing_U.StLineEq_varA * CellBalancing_DW.x);
    if (tmp < 2.147483648E+9) {
      if (tmp >= -2.147483648E+9) {
        i = (int32_T)tmp;
      } else {
        i = MIN_int32_T;
      }
    } else {
      i = MAX_int32_T;
    }

    if ((i < 0) && (CellBalancing_U.StLineEq_varB < MIN_int32_T - i)) {
      CellBalancing_B.BalancingTimeOn_msec = -2.147483648E+9;
    } else if ((i > 0) && (CellBalancing_U.StLineEq_varB > MAX_int32_T - i)) {
      CellBalancing_B.BalancingTimeOn_msec = 2.147483647E+9;
    } else {
      CellBalancing_B.BalancingTimeOn_msec = i + CellBalancing_U.StLineEq_varB;
    }
  } else {
    /* case IN_BalancingTimeOn_Default: */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(BalancingTempGroup).T_max > BalancingCurrentDerateActivationTemp_C , msec) > BalancingCurrentDerateCheckTime_msec; */
    if (CellBalancing_U.DataPipeline.TemperatureSenseBus[CellBalancing_U.BalancingTempGroup_a
        - 1].T_max <=
        CellBalancing_U.Thresholds.BalancingCurrentDerateActivationTemp_C) {
      CellBalancing_DW.durationCounter_1 = 0;
    }

    if (CellBalancing_DW.durationCounter_1 * 100 >
        CellBalancing_U.Thresholds.BalancingCurrentDerateCheckTime_msec) {
      CellBalancing_DW.is_c6_CellBalancing = IN_BalancingTimeOn_BasedOnTemp_;

      /* :  x = double(DataPipeline.TemperatureSenseBus(BalancingTempGroup).T_max); */
      CellBalancing_DW.x =
        CellBalancing_U.DataPipeline.TemperatureSenseBus[CellBalancing_U.BalancingTempGroup_a
        - 1].T_max;
    }
  }

  if (CellBalancing_U.DataPipeline.TemperatureSenseBus[CellBalancing_U.BalancingTempGroup_a
      - 1].T_max >
      CellBalancing_U.Thresholds.BalancingCurrentDerateActivationTemp_C) {
    CellBalancing_DW.durationCounter_1++;
  } else {
    CellBalancing_DW.durationCounter_1 = 0;
  }

  /* End of Chart: '<S3>/Chart' */

  /* Chart: '<S1>/BalancingStateMachine' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  if (CellBalancing_DW.temporalCounter_i1 < MAX_uint32_T) {
    CellBalancing_DW.temporalCounter_i1++;
  }

  if (CellBalancing_DW.is_active_c3_CellBalancing == 0U) {
    CellBalancing_DW.is_active_c3_CellBalancing = 1U;
    CellBalancing_DW.durationCounter_1_b = 0;
    CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Lobby;
    CellBalancing_DW.temporalCounter_i1 = 0U;

    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  Outport: '<Root>/CellBalancingOutput'
     */
    /* :  CurrentBalancingState = BalancingState.Lobby; */
    CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Lobby;
  } else {
    switch (CellBalancing_DW.is_c3_CellBalancing) {
     case CellBalancing_IN_Even_Balance:
      /* BusCreator: '<S1>/Bus Creator' incorporates:
       *  Outport: '<Root>/CellBalancingOutput'
       */
      CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Even_Balancing;

      /* :  sf_internal_predicateOutput = after(BalancingTimeOn_msec,msec); */
      if ((uint32_T)((int32_T)CellBalancing_DW.temporalCounter_i1 * 100) >=
          (uint32_T)ceil(CellBalancing_B.BalancingTimeOn_msec)) {
        /* :  BalanceCmd=zeros(1,20); */
        for (i = 0; i < 20; i++) {
          CellBalancing_B.BalanceCmd[i] = 0;
        }

        /* :  BalanceFetCmd_AllOff; */
        CellBalancing_DW.durationCounter_1_k3 = 0;
        CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Odd_Balance;
        CellBalancing_DW.temporalCounter_i1 = 0U;

        /* BusCreator: '<S1>/Bus Creator' incorporates:
         *  Outport: '<Root>/CellBalancingOutput'
         */
        /* :  BalanceFetCmd_AllOff; */
        /* :  CurrentBalancingState = BalancingState.Odd_Balancing; */
        CellBalancing_Y.CellBalancingOutput.CurrentBalancingState =
          Odd_Balancing;

        /* :  i = 1; */
        CellBalancing_DW.i = 1.0;

        /* :  sf_internal_predicateOutput = i<=nCells_series; */
        while ((int32_T)CellBalancing_DW.i <=
               CellBalancing_U.DataPipeline.nCells_series) {
          /* :  CellBalance(i); */
          CellBalancing_CellBalance();

          /* :  i=i+2; */
          CellBalancing_DW.i += 2.0;
        }
      } else {
        /* :  sf_internal_predicateOutput = duration(BalanceEn==0,msec)>Thresholds.Balancing_StateRestPeriod_msec; */
        if (CellBalancing_B.BalanceEn) {
          CellBalancing_DW.durationCounter_1_k = 0;
        }

        if (CellBalancing_DW.durationCounter_1_k * 100 >
            CellBalancing_U.Thresholds.Balancing_StateRestPeriod_msec) {
          /* :  BalanceFetCmd_AllOff; */
          CellBalancing_DW.durationCounter_1_e = 0;
          CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Terminate;

          /* BusCreator: '<S1>/Bus Creator' incorporates:
           *  Outport: '<Root>/CellBalancingOutput'
           */
          /* :  BalanceFetCmd_AllOff; */
          /* :  CurrentBalancingState = BalancingState.Terminate; */
          CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Terminate;
        }
      }
      break;

     case CellBalancing_IN_Lobby:
      /* BusCreator: '<S1>/Bus Creator' incorporates:
       *  Outport: '<Root>/CellBalancingOutput'
       */
      CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Lobby;

      /* :  sf_internal_predicateOutput = duration(BalanceEn==0,msec)>Thresholds.Balancing_StateRestPeriod_msec; */
      if (CellBalancing_B.BalanceEn) {
        CellBalancing_DW.durationCounter_1_b = 0;
      }

      if (CellBalancing_DW.durationCounter_1_b * 100 >
          CellBalancing_U.Thresholds.Balancing_StateRestPeriod_msec) {
        CellBalancing_DW.durationCounter_1_e = 0;
        CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Terminate;

        /* BusCreator: '<S1>/Bus Creator' incorporates:
         *  Outport: '<Root>/CellBalancingOutput'
         */
        /* :  BalanceFetCmd_AllOff; */
        /* :  CurrentBalancingState = BalancingState.Terminate; */
        CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Terminate;

        /* :  sf_internal_predicateOutput = BalanceEn==1 && after(Thresholds.Balancing_StateRestPeriod_msec,msec); */
      } else if (CellBalancing_B.BalanceEn && ((uint32_T)((int32_T)
                   CellBalancing_DW.temporalCounter_i1 * 100) >= (uint32_T)
                  CellBalancing_U.Thresholds.Balancing_StateRestPeriod_msec)) {
        CellBalancing_DW.durationCounter_1_k = 0;
        CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Even_Balance;
        CellBalancing_DW.temporalCounter_i1 = 0U;

        /* BusCreator: '<S1>/Bus Creator' incorporates:
         *  Outport: '<Root>/CellBalancingOutput'
         */
        /* :  BalanceFetCmd_AllOff; */
        /* :  CurrentBalancingState = BalancingState.Even_Balancing; */
        CellBalancing_Y.CellBalancingOutput.CurrentBalancingState =
          Even_Balancing;

        /* :  i = 2; */
        CellBalancing_DW.i = 2.0;

        /* :  sf_internal_predicateOutput = i<=nCells_series; */
        while ((int32_T)CellBalancing_DW.i <=
               CellBalancing_U.DataPipeline.nCells_series) {
          /* :  CellBalance(i); */
          CellBalancing_CellBalance();

          /* :  i=i+2; */
          CellBalancing_DW.i += 2.0;
        }
      }
      break;

     case CellBalancing_IN_Odd_Balance:
      /* BusCreator: '<S1>/Bus Creator' incorporates:
       *  Outport: '<Root>/CellBalancingOutput'
       */
      CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Odd_Balancing;

      /* :  sf_internal_predicateOutput = after(BalancingTimeOn_msec,msec); */
      if ((uint32_T)((int32_T)CellBalancing_DW.temporalCounter_i1 * 100) >=
          (uint32_T)ceil(CellBalancing_B.BalancingTimeOn_msec)) {
        /* :  BalanceCmd =zeros(1,20); */
        for (i = 0; i < 20; i++) {
          CellBalancing_B.BalanceCmd[i] = 0;
        }

        /* :  BalanceFetCmd_AllOff; */
        CellBalancing_DW.durationCounter_1_b = 0;
        CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Lobby;
        CellBalancing_DW.temporalCounter_i1 = 0U;

        /* BusCreator: '<S1>/Bus Creator' incorporates:
         *  Outport: '<Root>/CellBalancingOutput'
         */
        /* :  CurrentBalancingState = BalancingState.Lobby; */
        CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Lobby;
      } else {
        /* :  sf_internal_predicateOutput = duration(BalanceEn==0,msec)>Thresholds.Balancing_StateRestPeriod_msec; */
        if (CellBalancing_B.BalanceEn) {
          CellBalancing_DW.durationCounter_1_k3 = 0;
        }

        if (CellBalancing_DW.durationCounter_1_k3 * 100 >
            CellBalancing_U.Thresholds.Balancing_StateRestPeriod_msec) {
          /* :  BalanceFetCmd_AllOff; */
          CellBalancing_DW.durationCounter_1_e = 0;
          CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Terminate;

          /* BusCreator: '<S1>/Bus Creator' incorporates:
           *  Outport: '<Root>/CellBalancingOutput'
           */
          /* :  BalanceFetCmd_AllOff; */
          /* :  CurrentBalancingState = BalancingState.Terminate; */
          CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Terminate;
        }
      }
      break;

     default:
      /* BusCreator: '<S1>/Bus Creator' incorporates:
       *  Outport: '<Root>/CellBalancingOutput'
       */
      /* case IN_Terminate: */
      CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Terminate;

      /* :  sf_internal_predicateOutput = duration(BalanceEn==1,msec)>Thresholds.Balancing_StateRestPeriod_msec; */
      if (!CellBalancing_B.BalanceEn) {
        CellBalancing_DW.durationCounter_1_e = 0;
      }

      if (CellBalancing_DW.durationCounter_1_e * 100 >
          CellBalancing_U.Thresholds.Balancing_StateRestPeriod_msec) {
        CellBalancing_DW.durationCounter_1_b = 0;
        CellBalancing_DW.is_c3_CellBalancing = CellBalancing_IN_Lobby;
        CellBalancing_DW.temporalCounter_i1 = 0U;

        /* BusCreator: '<S1>/Bus Creator' incorporates:
         *  Outport: '<Root>/CellBalancingOutput'
         */
        /* :  CurrentBalancingState = BalancingState.Lobby; */
        CellBalancing_Y.CellBalancingOutput.CurrentBalancingState = Lobby;
      }
      break;
    }
  }

  if (!CellBalancing_B.BalanceEn) {
    CellBalancing_DW.durationCounter_1_b++;
    CellBalancing_DW.durationCounter_1_e = 0;
    CellBalancing_DW.durationCounter_1_k++;
    CellBalancing_DW.durationCounter_1_k3++;
  } else {
    CellBalancing_DW.durationCounter_1_b = 0;
    CellBalancing_DW.durationCounter_1_e++;
    CellBalancing_DW.durationCounter_1_k = 0;
    CellBalancing_DW.durationCounter_1_k3 = 0;
  }

  /* End of Chart: '<S1>/BalancingStateMachine' */

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[0] = CellBalancing_B.BalanceCmd[0];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[1] = CellBalancing_B.BalanceCmd[1];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[2] = CellBalancing_B.BalanceCmd[2];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[3] = CellBalancing_B.BalanceCmd[3];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[4] = CellBalancing_B.BalanceCmd[4];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[5] = CellBalancing_B.BalanceCmd[5];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[6] = CellBalancing_B.BalanceCmd[6];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[7] = CellBalancing_B.BalanceCmd[7];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[8] = CellBalancing_B.BalanceCmd[8];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[9] = CellBalancing_B.BalanceCmd[9];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[10] = CellBalancing_B.BalanceCmd[10];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[11] = CellBalancing_B.BalanceCmd[11];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[12] = CellBalancing_B.BalanceCmd[12];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[13] = CellBalancing_B.BalanceCmd[13];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[14] = CellBalancing_B.BalanceCmd[14];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[15] = CellBalancing_B.BalanceCmd[15];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[16] = CellBalancing_B.BalanceCmd[16];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[17] = CellBalancing_B.BalanceCmd[17];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[18] = CellBalancing_B.BalanceCmd[18];

  /* SignalConversion generated from: '<S1>/Vector Concatenate' */
  rtb_BalanceCmd[19] = CellBalancing_B.BalanceCmd[19];

  /* BusCreator: '<S1>/Bus Creator' incorporates:
   *  Outport: '<Root>/CellBalancingOutput'
   */
  CellBalancing_Y.CellBalancingOutput.MaxImbalance_mV = BalanceEn_tmp;
  CellBalancing_Y.CellBalancingOutput.BalanceEn = CellBalancing_B.BalanceEn;

  /* Outport: '<Root>/BalanceCmd' */
  for (i = 0; i < 20; i++) {
    CellBalancing_Y.BalanceCmd[i] = rtb_BalanceCmd[i];
  }

  /* End of Outport: '<Root>/BalanceCmd' */

  /* MATLAB Function: '<S1>/WriteMosfetsData' incorporates:
   *  Inport: '<Root>/NumberOfMosfets'
   */
  CellBalancing_WriteMosfetsData(rtb_BalanceCmd,
    &CellBalancing_B.sf_WriteMosfetsData);

  /* Outport: '<Root>/fetStates' */
  for (i = 0; i < 20; i++) {
    CellBalancing_Y.fetStates[i] =
      CellBalancing_B.sf_WriteMosfetsData.fetStates[i];
  }

  /* End of Outport: '<Root>/fetStates' */
}

/* Model initialize function */
void CellBalancing_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(CellBalancing_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &CellBalancing_B), 0,
                sizeof(B_CellBalancing_T));

  /* states (dwork) */
  (void) memset((void *)&CellBalancing_DW, 0,
                sizeof(DW_CellBalancing_T));

  /* external inputs */
  (void)memset(&CellBalancing_U, 0, sizeof(ExtU_CellBalancing_T));

  /* external outputs */
  (void)memset(&CellBalancing_Y, 0, sizeof(ExtY_CellBalancing_T));
  CellBalancing_Y.CellBalancingOutput = CellBalancing_rtZCellBalancingO;

  {
    int32_T i;

    /* SystemInitialize for Chart: '<S3>/Chart1' */
    CellBalancing_DW.is_active_c26_CellBalancing = 0U;
    CellBalancing_DW.is_c26_CellBalancing = CellBalancin_IN_NO_ACTIVE_CHILD;

    /* SystemInitialize for Chart: '<S3>/Chart2' */
    CellBalancing_DW.is_active_c4_CellBalancing = 0U;
    CellBalancing_DW.is_c4_CellBalancing = CellBalancin_IN_NO_ACTIVE_CHILD;
    CellBalancing_B.Balancing_OVThr_mV = 0;

    /* SystemInitialize for Chart: '<S3>/Chart' */
    CellBalancing_DW.is_active_c6_CellBalancing = 0U;
    CellBalancing_DW.is_c6_CellBalancing = CellBalancin_IN_NO_ACTIVE_CHILD;
    CellBalancing_DW.x = 1.0;
    CellBalancing_B.BalancingTimeOn_msec = 1.0;

    /* SystemInitialize for Chart: '<S1>/BalancingStateMachine' */
    CellBalancing_DW.temporalCounter_i1 = 0U;
    CellBalancing_DW.is_active_c3_CellBalancing = 0U;
    CellBalancing_DW.is_c3_CellBalancing = CellBalancin_IN_NO_ACTIVE_CHILD;
    CellBalancing_DW.i = 0.0;
    for (i = 0; i < 20; i++) {
      CellBalancing_B.BalanceCmd[i] = 0;
    }

    /* End of SystemInitialize for Chart: '<S1>/BalancingStateMachine' */
  }
}

/* Model terminate function */
void CellBalancing_terminate(void)
{
  /* (no terminate code required) */
}
