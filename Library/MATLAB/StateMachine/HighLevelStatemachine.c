/*
 * HighLevelStatemachine.c
 *
 * Code generation for model "HighLevelStatemachine".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:55:14 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: All passed
 */

#include "HighLevelStatemachine.h"
#include "rtwtypes.h"
#include "HighLevelStatemachine_types.h"
#include <string.h>

/* Named constants for Chart: '<S1>/HighLevelStatemachine' */
#define HighLevelSt_IN_PermanentFailure ((uint8_T)2U)
#define HighLevelSta_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define HighLevelState_IN_DeepSleepMode ((uint8_T)9U)
#define HighLevelStatem_IN_FastCharging ((uint8_T)5U)
#define HighLevelStatem_IN_LowPowerMode ((uint8_T)10U)
#define HighLevelStatem_IN_Regeneration ((uint8_T)7U)
#define HighLevelStatem_IN_SlowCharging ((uint8_T)8U)
#define HighLevelStatema_IN_Discharging ((uint8_T)4U)
#define HighLevelStatema_IN_ServiceMode ((uint8_T)11U)
#define HighLevelStatemac_IN_Protection ((uint8_T)3U)
#define HighLevelStatemachi_IN_LimpHome ((uint8_T)1U)
#define HighLevelStatemachine_IN_Idle  ((uint8_T)6U)

/* Block states (default storage) */
DW_HighLevelStatemachine_T HighLevelStatemachine_DW;

/* External inputs (root inport signals with default storage) */
ExtU_HighLevelStatemachine_T HighLevelStatemachine_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_HighLevelStatemachine_T HighLevelStatemachine_Y;

/* Real-time model */
static RT_MODEL_HighLevelStatemachin_T HighLevelStatemachine_M_;
RT_MODEL_HighLevelStatemachin_T *const HighLevelStatemachine_M =
  &HighLevelStatemachine_M_;

/* Forward declaration for local functions */
static void HighLevelStatemachi_Discharging(void);
static void HighLevelStatemach_FastCharging(void);
static void HighLevelStatemachine_Idle(void);
static void HighLevelStatemach_Regeneration(void);
static void HighLevelStatemach_SlowCharging(void);
const ProtectionOutput HighLevelStatemachine_rtZProtec = { NoError,/* ThermalRunaway */
  NoError,                             /* TemperatureGradient */
  NoError,                             /* HighImbalanceFlag */
  NoError,                             /* ShortCircuitDetect */
  NoError,                             /* SuddenVoltageDrop */
  NoError,                             /* OV */
  NoError,                             /* UV */
  NoError,                             /* OCC */
  NoError,                             /* OCD */
  0.0F,                                /* i2t_Calculated_A2sec */
  NoError,                             /* eFuseChargeFlag */
  NoError,                             /* eFuseDischargeFlag */
  NoErr,                               /* TempState1 */
  NoErr,                               /* TempState2 */
  NoError,                             /* FlagGroup1 */
  NoError,                             /* FlagGroup2 */
  NoError,                             /* TempOverallState */
  NoError                              /* ErrorDetect */
};

/* Function for Chart: '<S1>/HighLevelStatemachine' */
static void HighLevelStatemachi_Discharging(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==1, msec)>Thresholds.NormalModeTimeout_msec; */
  if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1) {
    HighLevelStatemachine_DW.durationCounter_2_g = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (HighLevelStatemachine_DW.durationCounter_2_g * 100 >
      HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4_e = 0;
    HighLevelStatemachine_DW.durationCounter_3_j = 0;
    HighLevelStatemachine_DW.durationCounter_2_i = 0;
    HighLevelStatemachine_DW.durationCounter_1_m = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatem_IN_SlowCharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelState = HighLevelMode.SlowCharging; */
    HighLevelStatemachine_Y.HighLevelStateMode = SlowCharging;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
        HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
      HighLevelStatemachine_DW.durationCounter_1_n = 0;
    }

    if (HighLevelStatemachine_DW.durationCounter_1_n * 100 >
        HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_o = 0;
      HighLevelStatemachine_DW.durationCounter_3_i = 0;
      HighLevelStatemachine_DW.durationCounter_2_n = 0;
      HighLevelStatemachine_DW.durationCounter_1 = 0;
      HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
        HighLevelStatemachine_IN_Idle;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelState = HighLevelMode.Idle; */
      HighLevelStatemachine_Y.HighLevelStateMode = Idle;
      HighLevelStatemachine_DW.durationCounter_1_c = 0;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == 0 && DataPipeline.isChargeEn ==1), msec)>Thresholds.NormalModeTimeout_msec; */
      if (HighLevelStatemachine_U.DataPipeline.VCU.isChargerConnected ||
          (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1)) {
        HighLevelStatemachine_DW.durationCounter_3 = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_3 * 100 >
          HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_2_n4 = 0;
        HighLevelStatemachine_DW.durationCounter_1_h = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatem_IN_Regeneration;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelState = HighLevelMode.Regeneration; */
        HighLevelStatemachine_Y.HighLevelStateMode = Regeneration;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* Inport: '<Root>/ProtectionOutput' */
        /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
        if ((int32_T)HighLevelStatemachine_U.ProtectionOutput_a.ErrorDetect != 1)
        {
          HighLevelStatemachine_DW.durationCounter_4 = 0;
        }

        /* End of Inport: '<Root>/ProtectionOutput' */
        if (HighLevelStatemachine_DW.durationCounter_4 * 100 >
            HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_2_ns = 0;
          HighLevelStatemachine_DW.durationCounter_1_b = 0;
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatemac_IN_Protection;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelState = HighLevelMode.ProtectionFault; */
          HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/HighLevelStatemachine' */
static void HighLevelStatemach_FastCharging(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.SlowCharge == 1, msec)>Thresholds.NormalModeTimeout_msec; */
  if (!HighLevelStatemachine_U.DataPipeline.VCU.SlowCharge) {
    HighLevelStatemachine_DW.durationCounter_3_a = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (HighLevelStatemachine_DW.durationCounter_3_a * 100 >
      HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4_e = 0;
    HighLevelStatemachine_DW.durationCounter_3_j = 0;
    HighLevelStatemachine_DW.durationCounter_2_i = 0;
    HighLevelStatemachine_DW.durationCounter_1_m = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatem_IN_SlowCharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelState = HighLevelMode.SlowCharging; */
    HighLevelStatemachine_Y.HighLevelStateMode = SlowCharging;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
        HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
      HighLevelStatemachine_DW.durationCounter_2 = 0;
    }

    if (HighLevelStatemachine_DW.durationCounter_2 * 100 >
        HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_o = 0;
      HighLevelStatemachine_DW.durationCounter_3_i = 0;
      HighLevelStatemachine_DW.durationCounter_2_n = 0;
      HighLevelStatemachine_DW.durationCounter_1 = 0;
      HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
        HighLevelStatemachine_IN_Idle;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelState = HighLevelMode.Idle; */
      HighLevelStatemachine_Y.HighLevelStateMode = Idle;
      HighLevelStatemachine_DW.durationCounter_1_c = 0;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
      if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
        HighLevelStatemachine_DW.durationCounter_1_f = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_f * 100 >
          HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_4 = 0;
        HighLevelStatemachine_DW.durationCounter_3 = 0;
        HighLevelStatemachine_DW.durationCounter_2_g = 0;
        HighLevelStatemachine_DW.durationCounter_1_n = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatema_IN_Discharging;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelState = HighLevelMode.Discharging; */
        HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* Inport: '<Root>/ProtectionOutput' */
        /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
        if ((int32_T)HighLevelStatemachine_U.ProtectionOutput_a.ErrorDetect != 1)
        {
          HighLevelStatemachine_DW.durationCounter_4_a = 0;
        }

        /* End of Inport: '<Root>/ProtectionOutput' */
        if (HighLevelStatemachine_DW.durationCounter_4_a * 100 >
            HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_2_ns = 0;
          HighLevelStatemachine_DW.durationCounter_1_b = 0;
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatemac_IN_Protection;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelState = HighLevelMode.ProtectionFault; */
          HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/HighLevelStatemachine' */
static void HighLevelStatemachine_Idle(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
  if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
    HighLevelStatemachine_DW.durationCounter_1 = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (HighLevelStatemachine_DW.durationCounter_1 * 100 >
      HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4 = 0;
    HighLevelStatemachine_DW.durationCounter_3 = 0;
    HighLevelStatemachine_DW.durationCounter_2_g = 0;
    HighLevelStatemachine_DW.durationCounter_1_n = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatema_IN_Discharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelState = HighLevelMode.Discharging; */
    HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==1, msec)>Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1) {
      HighLevelStatemachine_DW.durationCounter_2_n = 0;
    }

    if (HighLevelStatemachine_DW.durationCounter_2_n * 100 >
        HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_e = 0;
      HighLevelStatemachine_DW.durationCounter_3_j = 0;
      HighLevelStatemachine_DW.durationCounter_2_i = 0;
      HighLevelStatemachine_DW.durationCounter_1_m = 0;
      HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
        HighLevelStatem_IN_SlowCharging;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelState = HighLevelMode.SlowCharging; */
      HighLevelStatemachine_Y.HighLevelStateMode = SlowCharging;
      HighLevelStatemachine_DW.durationCounter_1_c = 0;
      HighLevelStatemachine_DW.durationCounter_4_o = 0;
    } else {
      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  sf_internal_predicateOutput = duration(HighLevelState == HighLevelMode.Idle, msec) > Thresholds.PowerSaverModeTimeout_msec; */
      if (HighLevelStatemachine_Y.HighLevelStateMode != Idle) {
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_4_o * 100 >
          HighLevelStatemachine_U.Thresholds.PowerSaverModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatem_IN_LowPowerMode;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelState = HighLevelMode.LowPowerMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = LowPowerMode;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
        if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
          HighLevelStatemachine_DW.durationCounter_3_i = 0;
        }

        if (HighLevelStatemachine_DW.durationCounter_3_i * 100 >
            HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatema_IN_ServiceMode;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelState = HighLevelMode.ServiceMode; */
          HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/HighLevelStatemachine' */
static void HighLevelStatemach_Regeneration(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn == 0, msec)> Thresholds.NormalModeTimeout_msec; */
  if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
    HighLevelStatemachine_DW.durationCounter_1_h = 0;
  }

  /* End of Inport: '<Root>/DataPipeline' */

  /* Inport: '<Root>/Thresholds' */
  if (HighLevelStatemachine_DW.durationCounter_1_h * 100 >
      HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4 = 0;
    HighLevelStatemachine_DW.durationCounter_3 = 0;
    HighLevelStatemachine_DW.durationCounter_2_g = 0;
    HighLevelStatemachine_DW.durationCounter_1_n = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatema_IN_Discharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelState = HighLevelMode.Discharging; */
    HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  } else {
    /* Inport: '<Root>/ProtectionOutput' */
    /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
    if ((int32_T)HighLevelStatemachine_U.ProtectionOutput_a.ErrorDetect != 1) {
      HighLevelStatemachine_DW.durationCounter_2_n4 = 0;
    }

    /* End of Inport: '<Root>/ProtectionOutput' */
    if (HighLevelStatemachine_DW.durationCounter_2_n4 * 100 >
        HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_2_ns = 0;
      HighLevelStatemachine_DW.durationCounter_1_b = 0;
      HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
        HighLevelStatemac_IN_Protection;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelState = HighLevelMode.ProtectionFault; */
      HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
      HighLevelStatemachine_DW.durationCounter_1_c = 0;
      HighLevelStatemachine_DW.durationCounter_4_o = 0;
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/HighLevelStatemachine' */
static void HighLevelStatemach_SlowCharging(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.FastCharge == 1, msec)>Thresholds.NormalModeTimeout_msec; */
  if (!HighLevelStatemachine_U.DataPipeline.VCU.FastCharge) {
    HighLevelStatemachine_DW.durationCounter_3_j = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (HighLevelStatemachine_DW.durationCounter_3_j * 100 >
      HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4_a = 0;
    HighLevelStatemachine_DW.durationCounter_3_a = 0;
    HighLevelStatemachine_DW.durationCounter_2 = 0;
    HighLevelStatemachine_DW.durationCounter_1_f = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatem_IN_FastCharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelState = HighLevelMode.FastCharging; */
    HighLevelStatemachine_Y.HighLevelStateMode = FastCharging;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
        HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
      HighLevelStatemachine_DW.durationCounter_2_i = 0;
    }

    if (HighLevelStatemachine_DW.durationCounter_2_i * 100 >
        HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_o = 0;
      HighLevelStatemachine_DW.durationCounter_3_i = 0;
      HighLevelStatemachine_DW.durationCounter_2_n = 0;
      HighLevelStatemachine_DW.durationCounter_1 = 0;
      HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
        HighLevelStatemachine_IN_Idle;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelState = HighLevelMode.Idle; */
      HighLevelStatemachine_Y.HighLevelStateMode = Idle;
      HighLevelStatemachine_DW.durationCounter_1_c = 0;
    } else {
      /* Inport: '<Root>/ProtectionOutput' */
      /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
      if ((int32_T)HighLevelStatemachine_U.ProtectionOutput_a.ErrorDetect != 1)
      {
        HighLevelStatemachine_DW.durationCounter_4_e = 0;
      }

      /* End of Inport: '<Root>/ProtectionOutput' */
      if (HighLevelStatemachine_DW.durationCounter_4_e * 100 >
          HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_2_ns = 0;
        HighLevelStatemachine_DW.durationCounter_1_b = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatemac_IN_Protection;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelState = HighLevelMode.ProtectionFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
        if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
          HighLevelStatemachine_DW.durationCounter_1_m = 0;
        }

        if (HighLevelStatemachine_DW.durationCounter_1_m * 100 >
            HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_4 = 0;
          HighLevelStatemachine_DW.durationCounter_3 = 0;
          HighLevelStatemachine_DW.durationCounter_2_g = 0;
          HighLevelStatemachine_DW.durationCounter_1_n = 0;
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatema_IN_Discharging;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelState = HighLevelMode.Discharging; */
          HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Model step function */
void HighLevelStatemachine_step(void)
{
  /* Chart: '<S1>/HighLevelStatemachine' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   *  Outport: '<Root>/HighLevelStateMode'
   */
  if (HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach == 0U) {
    HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach = 1U;
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
    HighLevelStatemachine_DW.durationCounter_3_i = 0;
    HighLevelStatemachine_DW.durationCounter_2_n = 0;
    HighLevelStatemachine_DW.durationCounter_1 = 0;
    HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
      HighLevelStatemachine_IN_Idle;

    /* :  HighLevelState = HighLevelMode.Idle; */
    HighLevelStatemachine_Y.HighLevelStateMode = Idle;
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
  } else {
    switch (HighLevelStatemachine_DW.is_c8_HighLevelStatemachine) {
     case HighLevelStatemachi_IN_LimpHome:
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.VCUPermanentFail ==1, msec)> Thresholds.FaultsModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.VCUPermanentFail) {
        HighLevelStatemachine_DW.durationCounter_1_cq = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_cq * 100 >
          HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_1_g = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelSt_IN_PermanentFailure;

        /* :  HighLevelState = HighLevelMode.PermanentFailFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = PermanentFailFault;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
        if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
          HighLevelStatemachine_DW.durationCounter_2_d = 0;
        }

        if (HighLevelStatemachine_DW.durationCounter_2_d * 100 >
            HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatema_IN_ServiceMode;

          /* :  HighLevelState = HighLevelMode.ServiceMode; */
          HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        } else {
          /* :  HighLevelState = HighLevelMode.LimpHomeFault; */
          HighLevelStatemachine_Y.HighLevelStateMode = LimpHomeFault;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
      break;

     case HighLevelSt_IN_PermanentFailure:
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
        HighLevelStatemachine_DW.durationCounter_1_g = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_g * 100 >
          HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatema_IN_ServiceMode;

        /* :  HighLevelState = HighLevelMode.ServiceMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* :  HighLevelState = HighLevelMode.PermanentFailFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = PermanentFailFault;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      }
      break;

     case HighLevelStatemac_IN_Protection:
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.TransmissionFail ==1 , msec)> Thresholds.FaultsModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.TransmissionFail) {
        HighLevelStatemachine_DW.durationCounter_1_b = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_b * 100 >
          HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_2_d = 0;
        HighLevelStatemachine_DW.durationCounter_1_cq = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatemachi_IN_LimpHome;

        /* :  HighLevelState = HighLevelMode.LimpHomeFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = LimpHomeFault;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      } else {
        /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
        if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
          HighLevelStatemachine_DW.durationCounter_2_ns = 0;
        }

        if (HighLevelStatemachine_DW.durationCounter_2_ns * 100 >
            HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
          HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
            HighLevelStatema_IN_ServiceMode;

          /* :  HighLevelState = HighLevelMode.ServiceMode; */
          HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        } else {
          /* :  HighLevelState = HighLevelMode.ProtectionFault; */
          HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
          HighLevelStatemachine_DW.durationCounter_1_c = 0;
          HighLevelStatemachine_DW.durationCounter_4_o = 0;
        }
      }
      break;

     case HighLevelStatema_IN_Discharging:
      HighLevelStatemachi_Discharging();
      break;

     case HighLevelStatem_IN_FastCharging:
      HighLevelStatemach_FastCharging();
      break;

     case HighLevelStatemachine_IN_Idle:
      HighLevelStatemachine_Idle();
      break;

     case HighLevelStatem_IN_Regeneration:
      HighLevelStatemach_Regeneration();
      break;

     case HighLevelStatem_IN_SlowCharging:
      HighLevelStatemach_SlowCharging();
      break;

     case HighLevelState_IN_DeepSleepMode:
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
        HighLevelStatemachine_DW.durationCounter_1_go = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_go * 100 >
          HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelStatema_IN_ServiceMode;

        /* :  HighLevelState = HighLevelMode.ServiceMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      }
      break;

     case HighLevelStatem_IN_LowPowerMode:
      /* :  sf_internal_predicateOutput = duration(HighLevelState == HighLevelMode.LowPowerMode, msec) > Thresholds.PowerSaverModeTimeout_msec; */
      if (HighLevelStatemachine_Y.HighLevelStateMode != LowPowerMode) {
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
      }

      if (HighLevelStatemachine_DW.durationCounter_1_c * 100 >
          HighLevelStatemachine_U.Thresholds.PowerSaverModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_1_go = 0;
        HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
          HighLevelState_IN_DeepSleepMode;

        /* :  HighLevelState = HighLevelMode.DeepSleepMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = DeepSleepMode;
        HighLevelStatemachine_DW.durationCounter_1_c = 0;
        HighLevelStatemachine_DW.durationCounter_4_o = 0;
      }
      break;

     default:
      /* case IN_ServiceMode: */
      break;
    }
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_1++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1 = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_1_n++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_n = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_1_f++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_f = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_2++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2 = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1) {
    HighLevelStatemachine_DW.durationCounter_2_g++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_g = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_1_m++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_m = 0;
  }

  if ((!HighLevelStatemachine_U.DataPipeline.VCU.isChargerConnected) &&
      (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1)) {
    HighLevelStatemachine_DW.durationCounter_3++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3 = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_1_h++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_h = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1) {
    HighLevelStatemachine_DW.durationCounter_2_n++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_n = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_2_i++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_i = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.FastCharge) {
    HighLevelStatemachine_DW.durationCounter_3_j++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_j = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.SlowCharge) {
    HighLevelStatemachine_DW.durationCounter_3_a++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_a = 0;
  }

  if (HighLevelStatemachine_Y.HighLevelStateMode == LowPowerMode) {
    HighLevelStatemachine_DW.durationCounter_1_c++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_c = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.VCUPermanentFail) {
    HighLevelStatemachine_DW.durationCounter_1_cq++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_cq = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.TransmissionFail) {
    HighLevelStatemachine_DW.durationCounter_1_b++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_b = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
    HighLevelStatemachine_DW.durationCounter_3_i++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_i = 0;
  }

  if ((int32_T)HighLevelStatemachine_U.ProtectionOutput_a.ErrorDetect == 1) {
    HighLevelStatemachine_DW.durationCounter_4++;
    HighLevelStatemachine_DW.durationCounter_4_a++;
    HighLevelStatemachine_DW.durationCounter_4_e++;
    HighLevelStatemachine_DW.durationCounter_2_n4++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4 = 0;
    HighLevelStatemachine_DW.durationCounter_4_a = 0;
    HighLevelStatemachine_DW.durationCounter_4_e = 0;
    HighLevelStatemachine_DW.durationCounter_2_n4 = 0;
  }

  if (HighLevelStatemachine_Y.HighLevelStateMode == Idle) {
    HighLevelStatemachine_DW.durationCounter_4_o++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4_o = 0;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
    HighLevelStatemachine_DW.durationCounter_1_g++;
    HighLevelStatemachine_DW.durationCounter_1_go++;
    HighLevelStatemachine_DW.durationCounter_2_ns++;
    HighLevelStatemachine_DW.durationCounter_2_d++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_g = 0;
    HighLevelStatemachine_DW.durationCounter_1_go = 0;
    HighLevelStatemachine_DW.durationCounter_2_ns = 0;
    HighLevelStatemachine_DW.durationCounter_2_d = 0;
  }

  /* End of Chart: '<S1>/HighLevelStatemachine' */
}

/* Model initialize function */
void HighLevelStatemachine_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(HighLevelStatemachine_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&HighLevelStatemachine_DW, 0,
                sizeof(DW_HighLevelStatemachine_T));

  /* external inputs */
  (void)memset(&HighLevelStatemachine_U, 0, sizeof(ExtU_HighLevelStatemachine_T));
  HighLevelStatemachine_U.ProtectionOutput_a = HighLevelStatemachine_rtZProtec;

  /* external outputs */
  HighLevelStatemachine_Y.HighLevelStateMode = Idle;

  /* SystemInitialize for Chart: '<S1>/HighLevelStatemachine' */
  HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach = 0U;
  HighLevelStatemachine_DW.is_c8_HighLevelStatemachine =
    HighLevelSta_IN_NO_ACTIVE_CHILD;

  /* SystemInitialize for Outport: '<Root>/HighLevelStateMode' incorporates:
   *  Chart: '<S1>/HighLevelStatemachine'
   */
  HighLevelStatemachine_Y.HighLevelStateMode = Idle;
}

/* Model terminate function */
void HighLevelStatemachine_terminate(void)
{
  /* (no terminate code required) */
}
