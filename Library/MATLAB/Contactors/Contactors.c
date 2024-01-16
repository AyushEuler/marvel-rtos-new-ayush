/*
 * Contactors.c
 *
 * Code generation for model "Contactors".
 *
 * Model version              : 4.1122
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jan  4 18:51:18 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: All passed
 */

#include "Contactors.h"
#include "rtwtypes.h"
#include "Contactors_types.h"
#include <string.h>

/* Named constants for Chart: '<S1>/Contactor_StateMachine' */
#define Con_IN_ChangeState01_to_Opening ((uint8_T)8U)
#define Contacto_IN_NegContact_Check_01 ((uint8_T)6U)
#define Contacto_IN_NegContact_Check_02 ((uint8_T)7U)
#define Contactor_IN_ContWeldErrorCheck ((uint8_T)2U)
#define Contactors_IN_Closed_Substate  ((uint8_T)1U)
#define Contactors_IN_NO_ACTIVE_CHILD  ((uint8_T)0U)
#define Contactors_IN_NegContact_Check ((uint8_T)9U)
#define Contactors_IN_Open_Substate    ((uint8_T)5U)
#define Contactors_IN_PermanentFail    ((uint8_T)4U)
#define Contactors_IN_PosContact_Check ((uint8_T)10U)
#define Contactors_IN_PreCharge_Check01 ((uint8_T)11U)
#define Contactors_IN_PreCharge_Check02 ((uint8_T)12U)
#define Contactors_IN_Time_to_Open     ((uint8_T)13U)
#define IN_ExternalShortCircuit_Handler ((uint8_T)3U)

/* Block signals (default storage) */
B_Contactors_T Contactors_B;

/* Block states (default storage) */
DW_Contactors_T Contactors_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Contactors_T Contactors_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Contactors_T Contactors_Y;

/* Real-time model */
static RT_MODEL_Contactors_T Contactors_M_;
RT_MODEL_Contactors_T *const Contactors_M = &Contactors_M_;

/* Forward declaration for local functions */
static void Contactors_Open_Substate(void);
static void Contactors_PreCharge_Check01(void);
static void Contactors_PreCharge_Check02(void);
static void Contactors_ContactorOperations(void);
const ProtectionOutput Contactors_rtZProtectionOutput = { NoError,/* ThermalRunaway */
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

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contactors_Open_Substate(void)
{
  boolean_T guard1 = false;

  /* Update for Outport: '<Root>/NegContactors' */
  Contactors_Y.NegContactors = Open;

  /* Update for Outport: '<Root>/PosContactors' */
  Contactors_Y.PosContactors = Open;

  /* Update for Outport: '<Root>/PreChargeContactors' */
  Contactors_Y.PreChargeContactors = Open;

  /* Outport: '<Root>/ContactorsWeldWarning' */
  Contactors_Y.ContactorsWeldWarning = None;

  /* Outport: '<Root>/ContactorsState' */
  Contactors_Y.ContactorsState = OpenSubstate;

  /* Inport: '<Root>/Thresholds' */
  /* :  sf_internal_predicateOutput = ContactorWarningCounter >= Thresholds.ContactorWarningCounterLimit || duration(abs(DataPipeline.Current_mA) > Thresholds.ContWeldCheckCurrent_mA) > Thresholds.ContactorWeldTimeout_msec; */
  guard1 = false;
  if (Contactors_DW.ContactorWarningCounter >=
      Contactors_U.Thresholds.ContactorWarningCounterLimit) {
    guard1 = true;
  } else {
    int32_T tmp;

    /* Inport: '<Root>/DataPipeline' */
    if (Contactors_U.DataPipeline.Current_mA < 0) {
      if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Contactors_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Contactors_U.DataPipeline.Current_mA;
    }

    if (tmp <= Contactors_U.Thresholds.ContWeldCheckCurrent_mA) {
      Contactors_DW.durationCounter_1 = 0;
    }

    if (Contactors_DW.durationCounter_1 >
        Contactors_U.Thresholds.ContactorWeldTimeout_msec * 10) {
      guard1 = true;
    } else {
      /* :  sf_internal_predicateOutput = duration(Flags_detection == 1, msec) >= Thresholds.OpenToPreChargeStateTime_msec; */
      if (!Contactors_B.Flags_Detection) {
        Contactors_DW.durationCounter_2 = 0;
      }

      if (Contactors_DW.durationCounter_2 * 100 >=
          Contactors_U.Thresholds.OpenToPreChargeStateTime_msec) {
        Contactors_DW.durationCounter_2_d = 0;
        Contactors_DW.durationCounter_1_p = 0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_NegContact_Check;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

        /* Inport: '<Root>/ProtectionOutput' incorporates:
         *  Inport: '<Root>/DataPipeline'
         */
        /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
      } else if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
                 (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
                 Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;

        /* End of Inport: '<Root>/ProtectionOutput' */
      }
    }
  }

  if (guard1) {
    Contactors_DW.is_ContactorOperations = Contactor_IN_ContWeldErrorCheck;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    /* :  ContactorsWeldWarning = ContactorsFlags.Open; */
    Contactors_Y.ContactorsWeldWarning = Open;
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contactors_PreCharge_Check01(void)
{
  /* Update for Outport: '<Root>/PreChargeContactors' */
  Contactors_Y.PreChargeContactors = Closed;

  /* Outport: '<Root>/ContactorsState' */
  Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

  /* Inport: '<Root>/ProtectionOutput' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
  if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
      (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
      Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
    Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
    /* :  PermanentFail = ProtectionFlags.Error; */
    Contactors_Y.PermanentFaiil = Error;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.PermanentFailure; */
    Contactors_Y.ContactorsState = PermanentFailure;
  } else {
    int32_T tmp;

    /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) <= Thresholds.ContCurrent_mA, msec) > Thresholds.FlagDebounceTime_msec; */
    if (Contactors_U.DataPipeline.Current_mA < 0) {
      if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Contactors_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Contactors_U.DataPipeline.Current_mA;
    }

    /* Inport: '<Root>/Thresholds' */
    if (tmp > Contactors_U.Thresholds.ContCurrent_mA) {
      Contactors_DW.durationCounter_2_ev = 0;
    }

    if (Contactors_DW.durationCounter_2_ev * 100 >
        Contactors_U.Thresholds.FlagDebounceTime_msec) {
      Contactors_DW.durationCounter_2_en = 0;
      Contactors_DW.durationCounter_1_m = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_PosContact_Check;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Closed; */
      Contactors_Y.PosContactors = Closed;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
      Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;

      /* :  sf_internal_predicateOutput = after(Thresholds.Max_PreChargeTime_msec, msec); */
    } else if ((uint32_T)((int32_T)Contactors_DW.temporalCounter_i1 * 100) >=
               (uint32_T)Contactors_U.Thresholds.Max_PreChargeTime_msec) {
      /* SystemInitialize for Outport: '<Root>/ExternalShortCheck' */
      /* :  ExternalShortCheck=ExternalShortCheck+1; */
      tmp = (int32_T)(Contactors_Y.ExternalShortCheck + 1U);
      if (Contactors_Y.ExternalShortCheck + 1U > 255U) {
        tmp = 255;
      }

      Contactors_Y.ExternalShortCheck = (uint8_T)tmp;

      /* End of SystemInitialize for Outport: '<Root>/ExternalShortCheck' */
      Contactors_DW.is_ContactorOperations = IN_ExternalShortCircuit_Handler;
      Contactors_DW.temporalCounter_i1 = 0U;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.ExternalShortCircuit; */
      Contactors_Y.ContactorsState = ExternalShortCircuit;
    } else {
      /* Inport: '<Root>/PreChargeContactors_FB' */
      /* :  sf_internal_predicateOutput = duration(PreChargeContactors_FB ==0, msec) >= Thresholds.ContCutoffTime_msec; */
      if ((int32_T)Contactors_U.PreChargeContactors_FB != 0) {
        Contactors_DW.durationCounter_1_d = 0;
      }

      /* End of Inport: '<Root>/PreChargeContactors_FB' */
      if (Contactors_DW.durationCounter_1_d * 100 >=
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        Contactors_DW.is_ContactorOperations = Con_IN_ChangeState01_to_Opening;
      }
    }

    /* End of Inport: '<Root>/Thresholds' */
  }

  /* End of Inport: '<Root>/ProtectionOutput' */
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contactors_PreCharge_Check02(void)
{
  /* Update for Outport: '<Root>/PreChargeContactors' */
  Contactors_Y.PreChargeContactors = Open;

  /* Outport: '<Root>/ContactorsState' */
  Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;

  /* Inport: '<Root>/PreChargeContactors_FB' */
  /* :  sf_internal_predicateOutput = duration(PreChargeContactors_FB == 0, msec) >= Thresholds.ContCutoffTime_msec; */
  if ((int32_T)Contactors_U.PreChargeContactors_FB != 0) {
    Contactors_DW.durationCounter_1_h = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Contactors_DW.durationCounter_1_h * 100 >=
      Contactors_U.Thresholds.ContCutoffTime_msec) {
    Contactors_DW.durationCounter_1_i = 0;
    Contactors_DW.is_ContactorOperations = Contactors_IN_Closed_Substate;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Closed; */
    Contactors_Y.NegContactors = Closed;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Closed; */
    Contactors_Y.PosContactors = Closed;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    /* :  ContactorsWeldWarning = ContactorsFlags.None */
    Contactors_Y.ContactorsWeldWarning = None;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.ClosedSubstate; */
    Contactors_Y.ContactorsState = ClosedSubstate;
  } else {
    /* Inport: '<Root>/PreChargeContactors_FB' */
    /* :  sf_internal_predicateOutput = duration(PreChargeContactors_FB ==1, msec) >= Thresholds.ContCutoffTime_msec; */
    if ((int32_T)Contactors_U.PreChargeContactors_FB != 1) {
      Contactors_DW.durationCounter_2_e = 0;
    }

    if (Contactors_DW.durationCounter_2_e * 100 >=
        Contactors_U.Thresholds.ContCutoffTime_msec) {
      Contactors_DW.is_ContactorOperations = Con_IN_ChangeState01_to_Opening;

      /* Inport: '<Root>/ProtectionOutput' incorporates:
       *  Inport: '<Root>/DataPipeline'
       */
      /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
    } else if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
               (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
               Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
      Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
      /* :  PermanentFail = ProtectionFlags.Error; */
      Contactors_Y.PermanentFaiil = Error;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.PermanentFailure; */
      Contactors_Y.ContactorsState = PermanentFailure;

      /* End of Inport: '<Root>/ProtectionOutput' */
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contactors_ContactorOperations(void)
{
  switch (Contactors_DW.is_ContactorOperations) {
   case Contactors_IN_Closed_Substate:
    /* Update for Outport: '<Root>/NegContactors' */
    Contactors_Y.NegContactors = Closed;

    /* Update for Outport: '<Root>/PosContactors' */
    Contactors_Y.PosContactors = Closed;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    Contactors_Y.ContactorsWeldWarning = None;

    /* Outport: '<Root>/ContactorsState' */
    Contactors_Y.ContactorsState = ClosedSubstate;

    /* Inport: '<Root>/ProtectionOutput' incorporates:
     *  Inport: '<Root>/DataPipeline'
     */
    /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
    if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
        (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
        Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
      Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
      /* :  PermanentFail = ProtectionFlags.Error; */
      Contactors_Y.PermanentFaiil = Error;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.PermanentFailure; */
      Contactors_Y.ContactorsState = PermanentFailure;
    } else {
      /* :  sf_internal_predicateOutput = duration(Flags_detection == 0, msec) > Thresholds.FlagDebounceTime_msec; */
      if (Contactors_B.Flags_Detection) {
        Contactors_DW.durationCounter_1_i = 0;
      }

      /* Inport: '<Root>/Thresholds' */
      if (Contactors_DW.durationCounter_1_i * 100 >
          Contactors_U.Thresholds.FlagDebounceTime_msec) {
        Contactors_DW.durationCounter_1_e = 0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Time_to_Open;
        Contactors_DW.temporalCounter_i1 = 0U;
      }
    }
    break;

   case Contactor_IN_ContWeldErrorCheck:
    /* Outport: '<Root>/ContactorsWeldWarning' */
    Contactors_Y.ContactorsWeldWarning = Open;

    /* Inport: '<Root>/PermanentFailRecoveryTrigger' */
    /* :  sf_internal_predicateOutput = PermanentFailRecoveryTrigger == 1; */
    if (Contactors_U.PermanentFailRecoveryTrigger) {
      Contactors_DW.durationCounter_2 = 0;
      Contactors_DW.durationCounter_1 = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      /* :  ContactorsWeldWarning = ContactorsFlags.None; */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpenSubstate; */
      Contactors_Y.ContactorsState = OpenSubstate;
    }
    break;

   case IN_ExternalShortCircuit_Handler:
    /* Update for Outport: '<Root>/NegContactors' */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsState' */
    Contactors_Y.ContactorsState = ExternalShortCircuit;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = after(Thresholds.ExternalShortCheckTime_msec, msec); */
    if ((uint32_T)((int32_T)Contactors_DW.temporalCounter_i1 * 100) >= (uint32_T)
        Contactors_U.Thresholds.ExternalShortCheckTime_msec) {
      Contactors_DW.durationCounter_2 = 0;
      Contactors_DW.durationCounter_1 = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      /* :  ContactorsWeldWarning = ContactorsFlags.None; */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpenSubstate; */
      Contactors_Y.ContactorsState = OpenSubstate;

      /* SystemInitialize for Outport: '<Root>/ExternalShortCheck' incorporates:
       *  Outport: '<Root>/ContactorsState'
       *  Outport: '<Root>/NegContactors'
       *  Outport: '<Root>/PermanentFaiil'
       *  Outport: '<Root>/PosContactors'
       *  Outport: '<Root>/PreChargeContactors'
       */
      /* :  sf_internal_predicateOutput = ExternalShortCheck >= Thresholds.ExternalShortLimit; */
    } else if (Contactors_Y.ExternalShortCheck >=
               Contactors_U.Thresholds.ExternalShortLimit) {
      Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* :  PermanentFail = ProtectionFlags.Error; */
      Contactors_Y.PermanentFaiil = Error;

      /* :  ContactorsState = ContState.PermanentFailure; */
      Contactors_Y.ContactorsState = PermanentFailure;
    }
    break;

   case Contactors_IN_PermanentFail:
    /* Update for Outport: '<Root>/NegContactors' */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    Contactors_Y.PreChargeContactors = Open;

    /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
    Contactors_Y.PermanentFaiil = Error;

    /* Outport: '<Root>/ContactorsState' */
    Contactors_Y.ContactorsState = PermanentFailure;

    /* Inport: '<Root>/PermanentFailRecoveryTrigger' */
    /* :  sf_internal_predicateOutput = PermanentFailRecoveryTrigger == 1; */
    if (Contactors_U.PermanentFailRecoveryTrigger) {
      /* SystemInitialize for Outport: '<Root>/ExternalShortCheck' */
      /* :  ExternalShortCheck =0; */
      Contactors_Y.ExternalShortCheck = 0U;
      Contactors_DW.durationCounter_2 = 0;
      Contactors_DW.durationCounter_1 = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      /* :  ContactorsWeldWarning = ContactorsFlags.None; */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpenSubstate; */
      Contactors_Y.ContactorsState = OpenSubstate;
    }
    break;

   case Contactors_IN_Open_Substate:
    Contactors_Open_Substate();
    break;

   case Contacto_IN_NegContact_Check_01:
    Contactors_DW.durationCounter_2 = 0;
    Contactors_DW.durationCounter_1 = 0;
    Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    /* :  ContactorsWeldWarning = ContactorsFlags.None; */
    Contactors_Y.ContactorsWeldWarning = None;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.OpenSubstate; */
    Contactors_Y.ContactorsState = OpenSubstate;
    break;

   case Contacto_IN_NegContact_Check_02:
    {
      int32_T tmp;

      /* :  ContactorWarningCounter = ContactorWarningCounter + 1; */
      tmp = (int32_T)(Contactors_DW.ContactorWarningCounter + 1U);
      if (Contactors_DW.ContactorWarningCounter + 1U > 255U) {
        tmp = 255;
      }

      Contactors_DW.ContactorWarningCounter = (uint8_T)tmp;
      Contactors_DW.durationCounter_2 = 0;
      Contactors_DW.durationCounter_1 = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      /* :  ContactorsWeldWarning = ContactorsFlags.None; */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpenSubstate; */
      Contactors_Y.ContactorsState = OpenSubstate;
    }
    break;

   case Con_IN_ChangeState01_to_Opening:
    Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_01;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.OpeningSubstate; */
    Contactors_Y.ContactorsState = OpeningSubstate;
    break;

   case Contactors_IN_NegContact_Check:
    /* Update for Outport: '<Root>/NegContactors' */
    Contactors_Y.NegContactors = Closed;

    /* Outport: '<Root>/ContactorsState' */
    Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

    /* Inport: '<Root>/NegContactors_FB' */
    /* :  sf_internal_predicateOutput = duration(NegContactors_FB == 0, msec) >= Thresholds.ContCutoffTime_msec; */
    if ((int32_T)Contactors_U.NegContactors_FB != 0) {
      Contactors_DW.durationCounter_1_p = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Contactors_DW.durationCounter_1_p * 100 >=
        Contactors_U.Thresholds.ContCutoffTime_msec) {
      Contactors_DW.is_ContactorOperations = Con_IN_ChangeState01_to_Opening;
    } else {
      /* Inport: '<Root>/NegContactors_FB' */
      /* :  sf_internal_predicateOutput = duration(NegContactors_FB == 1, msec) >= Thresholds.ContCutoffTime_msec; */
      if ((int32_T)Contactors_U.NegContactors_FB != 1) {
        Contactors_DW.durationCounter_2_d = 0;
      }

      if (Contactors_DW.durationCounter_2_d * 100 >=
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        Contactors_DW.durationCounter_2_ev = 0;
        Contactors_DW.durationCounter_1_d = 0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_PreCharge_Check01;
        Contactors_DW.temporalCounter_i1 = 0U;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Closed; */
        Contactors_Y.PreChargeContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

        /* Inport: '<Root>/ProtectionOutput' incorporates:
         *  Inport: '<Root>/DataPipeline'
         */
        /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
      } else if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
                 (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
                 Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;
      }
    }
    break;

   case Contactors_IN_PosContact_Check:
    /* Update for Outport: '<Root>/PosContactors' */
    Contactors_Y.PosContactors = Closed;

    /* Outport: '<Root>/ContactorsState' */
    Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;

    /* Inport: '<Root>/PosContactors_FB' */
    /* :  sf_internal_predicateOutput = duration(PosContactors_FB == 1, msec) >= Thresholds.ContCutoffTime_msec; */
    if ((int32_T)Contactors_U.PosContactors_FB != 1) {
      Contactors_DW.durationCounter_2_en = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Contactors_DW.durationCounter_2_en * 100 >=
        Contactors_U.Thresholds.ContCutoffTime_msec) {
      Contactors_DW.durationCounter_2_e = 0;
      Contactors_DW.durationCounter_1_h = 0;
      Contactors_DW.is_ContactorOperations = Contactors_IN_PreCharge_Check02;

      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
      Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;
    } else {
      /* Inport: '<Root>/PosContactors_FB' */
      /* :  sf_internal_predicateOutput = duration(PosContactors_FB == 0, msec) >= Thresholds.ContCutoffTime_msec; */
      if ((int32_T)Contactors_U.PosContactors_FB != 0) {
        Contactors_DW.durationCounter_1_m = 0;
      }

      if (Contactors_DW.durationCounter_1_m * 100 >=
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        Contactors_DW.is_ContactorOperations = Con_IN_ChangeState01_to_Opening;

        /* Inport: '<Root>/ProtectionOutput' incorporates:
         *  Inport: '<Root>/DataPipeline'
         */
        /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
      } else if ((Contactors_U.ProtectionOutput_e.ThermalRunaway == Error) ||
                 (Contactors_U.ProtectionOutput_e.ShortCircuitDetect == Error) ||
                 Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* SystemInitialize for Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;
      }
    }
    break;

   case Contactors_IN_PreCharge_Check01:
    Contactors_PreCharge_Check01();
    break;

   case Contactors_IN_PreCharge_Check02:
    Contactors_PreCharge_Check02();
    break;

   default:
    {
      int32_T tmp;

      /* Inport: '<Root>/DataPipeline' */
      /* case IN_Time_to_Open: */
      /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) < Thresholds.ContOpenCurrent_mA) >= Thresholds.Min_ClosedToOpeningTime_msec; */
      if (Contactors_U.DataPipeline.Current_mA < 0) {
        if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Contactors_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Contactors_U.DataPipeline.Current_mA;
      }

      /* Inport: '<Root>/Thresholds' */
      if (tmp >= Contactors_U.Thresholds.ContOpenCurrent_mA) {
        Contactors_DW.durationCounter_1_e = 0;
      }

      if (Contactors_DW.durationCounter_1_e >=
          Contactors_U.Thresholds.Min_ClosedToOpeningTime_msec * 10) {
        Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_01;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpeningSubstate; */
        Contactors_Y.ContactorsState = OpeningSubstate;

        /* :  sf_internal_predicateOutput = after(Thresholds.Max_ClosedToOpeningTime_msec,msec); */
      } else if ((uint32_T)((int32_T)Contactors_DW.temporalCounter_i1 * 100) >=
                 (uint32_T)Contactors_U.Thresholds.Max_ClosedToOpeningTime_msec)
      {
        Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpeningSubstate; */
        Contactors_Y.ContactorsState = OpeningSubstate;
      } else {
        /* SystemInitialize for Outport: '<Root>/Elapsed_TimeToOpen_msec' */
        /* :  Elapsed_TimeToOpen_msec = elapsed(msec); */
        Contactors_Y.Elapsed_TimeToOpen_msec = (real_T)
          Contactors_DW.temporalCounter_i1 * 100.0;
      }
    }
    break;
  }
}

/* Model step function */
void Contactors_step(void)
{
  int32_T tmp_1;
  ProtectionFlags tmp;
  ProtectionFlags tmp_0;
  ProtectionFlags tmp_2;

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   *  Switch: '<S1>/Switch1'
   */
  if (Contactors_U.Thresholds.AdditionalProtectionEn) {
    tmp = Contactors_U.ProtectionOutput_e.TemperatureGradient;
    tmp_0 = Contactors_U.ProtectionOutput_e.SuddenVoltageDrop;
  } else {
    tmp = Contactors_ConstB.DataTypeConversion9;
    tmp_0 = Contactors_ConstB.DataTypeConversion9;
  }

  /* Switch: '<S1>/Switch2' incorporates:
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   */
  if (Contactors_U.Thresholds.HighImbalanceFlagEn) {
    tmp_2 = Contactors_U.ProtectionOutput_e.HighImbalanceFlag;
  } else {
    tmp_2 = Contactors_ConstB.DataTypeConversion9;
  }

  /* Logic: '<S1>/AND' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S11>/Constant'
   *  Constant: '<S12>/Constant'
   *  Constant: '<S13>/Constant'
   *  Constant: '<S14>/Constant'
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Constant: '<S17>/Constant'
   *  Constant: '<S18>/Constant'
   *  Constant: '<S19>/Constant'
   *  Constant: '<S20>/Constant'
   *  Constant: '<S21>/Constant'
   *  Constant: '<S22>/Constant'
   *  Constant: '<S23>/Constant'
   *  Constant: '<S24>/Constant'
   *  Constant: '<S25>/Constant'
   *  Constant: '<S26>/Constant'
   *  Constant: '<S27>/Constant'
   *  Constant: '<S28>/Constant'
   *  Constant: '<S29>/Constant'
   *  Constant: '<S2>/Constant'
   *  Constant: '<S3>/Constant'
   *  Constant: '<S4>/Constant'
   *  Constant: '<S5>/Constant'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S8>/Constant'
   *  Constant: '<S9>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DataTypeConversion: '<S1>/Data Type Conversion10'
   *  DataTypeConversion: '<S1>/Data Type Conversion11'
   *  DataTypeConversion: '<S1>/Data Type Conversion12'
   *  DataTypeConversion: '<S1>/Data Type Conversion13'
   *  DataTypeConversion: '<S1>/Data Type Conversion14'
   *  DataTypeConversion: '<S1>/Data Type Conversion15'
   *  DataTypeConversion: '<S1>/Data Type Conversion16'
   *  DataTypeConversion: '<S1>/Data Type Conversion17'
   *  DataTypeConversion: '<S1>/Data Type Conversion18'
   *  DataTypeConversion: '<S1>/Data Type Conversion19'
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  DataTypeConversion: '<S1>/Data Type Conversion20'
   *  DataTypeConversion: '<S1>/Data Type Conversion21'
   *  DataTypeConversion: '<S1>/Data Type Conversion23'
   *  DataTypeConversion: '<S1>/Data Type Conversion24'
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  DataTypeConversion: '<S1>/Data Type Conversion4'
   *  DataTypeConversion: '<S1>/Data Type Conversion5'
   *  DataTypeConversion: '<S1>/Data Type Conversion6'
   *  DataTypeConversion: '<S1>/Data Type Conversion7'
   *  DataTypeConversion: '<S1>/Data Type Conversion8'
   *  Inport: '<Root>/Communication_Flag'
   *  Inport: '<Root>/ContactorCommand'
   *  Inport: '<Root>/ContactorError_Flag'
   *  Inport: '<Root>/PermanentFail_FB'
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/SC_Flag'
   *  Logic: '<S1>/AND2'
   *  Logic: '<S1>/NAND'
   *  Logic: '<S1>/NOR'
   *  Logic: '<S1>/OR'
   *  Memory: '<S1>/Memory'
   *  Memory: '<S1>/Memory1'
   *  Memory: '<S1>/Memory2'
   *  RelationalOperator: '<S10>/Compare'
   *  RelationalOperator: '<S11>/Compare'
   *  RelationalOperator: '<S12>/Compare'
   *  RelationalOperator: '<S13>/Compare'
   *  RelationalOperator: '<S14>/Compare'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   *  RelationalOperator: '<S17>/Compare'
   *  RelationalOperator: '<S18>/Compare'
   *  RelationalOperator: '<S19>/Compare'
   *  RelationalOperator: '<S20>/Compare'
   *  RelationalOperator: '<S21>/Compare'
   *  RelationalOperator: '<S22>/Compare'
   *  RelationalOperator: '<S23>/Compare'
   *  RelationalOperator: '<S24>/Compare'
   *  RelationalOperator: '<S25>/Compare'
   *  RelationalOperator: '<S26>/Compare'
   *  RelationalOperator: '<S27>/Compare'
   *  RelationalOperator: '<S28>/Compare'
   *  RelationalOperator: '<S29>/Compare'
   *  RelationalOperator: '<S2>/Compare'
   *  RelationalOperator: '<S3>/Compare'
   *  RelationalOperator: '<S4>/Compare'
   *  RelationalOperator: '<S5>/Compare'
   *  RelationalOperator: '<S6>/Compare'
   *  RelationalOperator: '<S7>/Compare'
   *  RelationalOperator: '<S8>/Compare'
   *  RelationalOperator: '<S9>/Compare'
   *  Switch: '<S1>/Switch'
   *  Switch: '<S1>/Switch1'
   *  Switch: '<S1>/Switch2'
   */
  Contactors_B.Flags_Detection = (((int32_T)Contactors_U.ProtectionOutput_e.OCC
    != 2) && ((int32_T)Contactors_U.ProtectionOutput_e.OCD != 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.UV != 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.OV != 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.eFuseChargeFlag < 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.eFuseDischargeFlag < 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.TempOverallState != 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.ShortCircuitDetect != 2) && ((int32_T)
    Contactors_U.ProtectionOutput_e.ThermalRunaway != 2) && ((int32_T)tmp_2 != 2)
    && ((int32_T)tmp != 2) && ((int32_T)tmp_0 != 2) && (Contactors_U.SC_Flag !=
    2) && (Contactors_U.Communication_Flag != 2) &&
    (Contactors_U.ContactorError_Flag != 2) && ((int32_T)
    Contactors_U.PermanentFail_Flag != 2) && (Contactors_U.ContactorCommand == 1)
    && ((Contactors_U.ContactorCommand != 0) || ((int32_T)
    Contactors_DW.Memory_PreviousInput != 0) || ((int32_T)
    Contactors_DW.Memory1_PreviousInput != 0) || ((int32_T)
    Contactors_DW.Memory2_PreviousInput != 0) || (((int32_T)
    Contactors_U.ProtectionOutput_e.OCC != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.OCD != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.eFuseChargeFlag != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.eFuseDischargeFlag != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.UV != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.OV != 1) && ((int32_T)
    Contactors_U.ProtectionOutput_e.TempOverallState != 1))));

  /* Chart: '<S1>/Contactor_StateMachine' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/NegContactors_FB'
   *  Inport: '<Root>/PosContactors_FB'
   *  Inport: '<Root>/PreChargeContactors_FB'
   *  Inport: '<Root>/Thresholds'
   */
  if (Contactors_DW.temporalCounter_i1 < MAX_uint32_T) {
    Contactors_DW.temporalCounter_i1++;
  }

  if (Contactors_DW.is_active_c1_Contactors == 0U) {
    Contactors_DW.is_active_c1_Contactors = 1U;
    Contactors_DW.durationCounter_2 = 0;
    Contactors_DW.durationCounter_1 = 0;
    Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

    /* Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    /* :  ContactorsWeldWarning = ContactorsFlags.None; */
    Contactors_Y.ContactorsWeldWarning = None;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.OpenSubstate; */
    Contactors_Y.ContactorsState = OpenSubstate;
  } else {
    Contactors_ContactorOperations();
  }

  if (Contactors_U.DataPipeline.Current_mA < 0) {
    if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp_1 = MAX_int32_T;
    } else {
      tmp_1 = -Contactors_U.DataPipeline.Current_mA;
    }
  } else {
    tmp_1 = Contactors_U.DataPipeline.Current_mA;
  }

  if (tmp_1 > Contactors_U.Thresholds.ContWeldCheckCurrent_mA) {
    Contactors_DW.durationCounter_1++;
  } else {
    Contactors_DW.durationCounter_1 = 0;
  }

  if (Contactors_B.Flags_Detection) {
    Contactors_DW.durationCounter_2++;
  } else {
    Contactors_DW.durationCounter_2 = 0;
  }

  if (Contactors_U.DataPipeline.Current_mA < 0) {
    if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp_1 = MAX_int32_T;
    } else {
      tmp_1 = -Contactors_U.DataPipeline.Current_mA;
    }
  } else {
    tmp_1 = Contactors_U.DataPipeline.Current_mA;
  }

  if (tmp_1 < Contactors_U.Thresholds.ContOpenCurrent_mA) {
    Contactors_DW.durationCounter_1_e++;
  } else {
    Contactors_DW.durationCounter_1_e = 0;
  }

  if (!Contactors_B.Flags_Detection) {
    Contactors_DW.durationCounter_1_i++;
  } else {
    Contactors_DW.durationCounter_1_i = 0;
  }

  if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
    Contactors_DW.durationCounter_1_h++;
  } else {
    Contactors_DW.durationCounter_1_h = 0;
  }

  if ((int32_T)Contactors_U.NegContactors_FB == 0) {
    Contactors_DW.durationCounter_1_p++;
  } else {
    Contactors_DW.durationCounter_1_p = 0;
  }

  if ((int32_T)Contactors_U.NegContactors_FB == 1) {
    Contactors_DW.durationCounter_2_d++;
  } else {
    Contactors_DW.durationCounter_2_d = 0;
  }

  if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
    Contactors_DW.durationCounter_1_d++;
  } else {
    Contactors_DW.durationCounter_1_d = 0;
  }

  if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
    Contactors_DW.durationCounter_2_e++;
  } else {
    Contactors_DW.durationCounter_2_e = 0;
  }

  if ((int32_T)Contactors_U.PosContactors_FB == 0) {
    Contactors_DW.durationCounter_1_m++;
  } else {
    Contactors_DW.durationCounter_1_m = 0;
  }

  if (Contactors_U.DataPipeline.Current_mA < 0) {
    if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp_1 = MAX_int32_T;
    } else {
      tmp_1 = -Contactors_U.DataPipeline.Current_mA;
    }
  } else {
    tmp_1 = Contactors_U.DataPipeline.Current_mA;
  }

  if (tmp_1 <= Contactors_U.Thresholds.ContCurrent_mA) {
    Contactors_DW.durationCounter_2_ev++;
  } else {
    Contactors_DW.durationCounter_2_ev = 0;
  }

  if ((int32_T)Contactors_U.PosContactors_FB == 1) {
    Contactors_DW.durationCounter_2_en++;
  } else {
    Contactors_DW.durationCounter_2_en = 0;
  }

  /* End of Chart: '<S1>/Contactor_StateMachine' */

  /* Update for Memory: '<S1>/Memory' incorporates:
   *  Outport: '<Root>/NegContactors'
   */
  Contactors_DW.Memory_PreviousInput = Contactors_Y.NegContactors;

  /* Update for Memory: '<S1>/Memory1' incorporates:
   *  Outport: '<Root>/PosContactors'
   */
  Contactors_DW.Memory1_PreviousInput = Contactors_Y.PosContactors;

  /* Update for Memory: '<S1>/Memory2' incorporates:
   *  Outport: '<Root>/PreChargeContactors'
   */
  Contactors_DW.Memory2_PreviousInput = Contactors_Y.PreChargeContactors;
}

/* Model initialize function */
void Contactors_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Contactors_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &Contactors_B), 0,
                sizeof(B_Contactors_T));

  /* states (dwork) */
  (void) memset((void *)&Contactors_DW, 0,
                sizeof(DW_Contactors_T));

  /* external inputs */
  (void)memset(&Contactors_U, 0, sizeof(ExtU_Contactors_T));
  Contactors_U.ProtectionOutput_e = Contactors_rtZProtectionOutput;
  Contactors_U.PermanentFail_Flag = NoError;
  Contactors_U.PosContactors_FB = Open;
  Contactors_U.PreChargeContactors_FB = Open;
  Contactors_U.NegContactors_FB = Open;

  /* external outputs */
  (void)memset(&Contactors_Y, 0, sizeof(ExtY_Contactors_T));
  Contactors_Y.NegContactors = Open;
  Contactors_Y.PreChargeContactors = Open;
  Contactors_Y.PosContactors = Open;
  Contactors_Y.ContactorsWeldWarning = Open;
  Contactors_Y.PermanentFaiil = NoError;
  Contactors_Y.ContactorsState = OpenSubstate;

  /* InitializeConditions for Memory: '<S1>/Memory' */
  Contactors_DW.Memory_PreviousInput = Open;

  /* InitializeConditions for Memory: '<S1>/Memory1' */
  Contactors_DW.Memory1_PreviousInput = Open;

  /* InitializeConditions for Memory: '<S1>/Memory2' */
  Contactors_DW.Memory2_PreviousInput = Open;

  /* SystemInitialize for Chart: '<S1>/Contactor_StateMachine' */
  Contactors_DW.is_ContactorOperations = Contactors_IN_NO_ACTIVE_CHILD;
  Contactors_DW.temporalCounter_i1 = 0U;
  Contactors_DW.is_active_c1_Contactors = 0U;
  Contactors_DW.ContactorWarningCounter = 0U;

  /* SystemInitialize for Outport: '<Root>/NegContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.NegContactors = Open;

  /* SystemInitialize for Outport: '<Root>/PosContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PosContactors = Open;

  /* SystemInitialize for Outport: '<Root>/PreChargeContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PreChargeContactors = Open;

  /* SystemInitialize for Outport: '<Root>/ContactorsWeldWarning' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.ContactorsWeldWarning = Open;

  /* SystemInitialize for Outport: '<Root>/PermanentFaiil' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PermanentFaiil = NoError;

  /* SystemInitialize for Outport: '<Root>/ContactorsState' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.ContactorsState = OpenSubstate;

  /* SystemInitialize for Outport: '<Root>/ExternalShortCheck' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.ExternalShortCheck = 0U;

  /* SystemInitialize for Outport: '<Root>/Elapsed_TimeToOpen_msec' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.Elapsed_TimeToOpen_msec = 0.0;
}

/* Model terminate function */
void Contactors_terminate(void)
{
  /* (no terminate code required) */
}
