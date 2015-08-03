/*
 * encoders.c
 *
 * Code generation for model "encoders".
 *
 * Model version              : 1.4
 * Simulink Coder version : 8.7 (R2014b) 08-Sep-2014
 * C source code generated on : Wed Jul 29 19:29:40 2015
 *
 * Target selection: rti1104.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->Custom
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "encoders_trc_ptr.h"
#include "encoders.h"
#include "encoders_private.h"

/* Block signals (auto storage) */
B_encoders_T encoders_B;

/* Block states (auto storage) */
DW_encoders_T encoders_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_encoders_T encoders_PrevZCX;

/* Real-time model */
RT_MODEL_encoders_T encoders_M_;
RT_MODEL_encoders_T *const encoders_M = &encoders_M_;

/* Model output function */
static void encoders_output(void)
{
  real_T currentTime;
  int8_T rtTempZCSig;
  ZCEventType zcEvent;
  int8_T rtTempZCSig_0;

  /* Sin: '<Root>/Sine Wave' */
  encoders_B.SineWave = sin(encoders_P.SineWave_Freq * encoders_M->Timing.t[0] +
    encoders_P.SineWave_Phase) * encoders_P.SineWave_Amp +
    encoders_P.SineWave_Bias;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   */
  encoders_B.Compare = (encoders_B.SineWave >= encoders_P.Constant_Value);

  /* S-Function (rti_commonblock): '<S2>/S-Function1' */
  /* This comment workarounds a code generation problem */

  /* dSPACE I/O Board DS1104 #1 Unit:BIT_IO Group:BIT_OUT */
  if (encoders_B.Compare > 0) {
    ds1104_bit_io_set(DS1104_DIO0);
  } else {
    ds1104_bit_io_clear(DS1104_DIO0);
  }

  /* S-Function (rti_commonblock): '<S3>/S-Function1' */
  /* This comment workarounds a code generation problem */

  /* S-Function (rti_commonblock): '<S3>/S-Function2' */
  /* This comment workarounds a code generation problem */

  /* S-Function (rti_commonblock): '<S4>/S-Function1' */
  /* This comment workarounds a code generation problem */

  /* S-Function (rti_commonblock): '<S4>/S-Function2' */
  /* This comment workarounds a code generation problem */

  /* Step: '<Root>/Step' */
  currentTime = encoders_M->Timing.t[0];
  if (currentTime < encoders_P.Step_Time) {
    encoders_B.Step = encoders_P.Step_Y0;
  } else {
    encoders_B.Step = encoders_P.Step_YFinal;
  }

  /* End of Step: '<Root>/Step' */

  /* UnitDelay: '<S8>/Delay Input1' */
  encoders_B.Uk1 = encoders_DW.DelayInput1_DSTATE;

  /* RelationalOperator: '<S8>/FixPt Relational Operator' */
  encoders_B.FixPtRelationalOperator = (encoders_B.Step > encoders_B.Uk1);

  /* Gain: '<Root>/Gain' */
  encoders_B.Gain = (uint8_T)(encoders_B.FixPtRelationalOperator ? (int32_T)
    encoders_P.Gain_Gain : 0);

  /* Outputs for Triggered SubSystem: '<Root>/DS1104ENC_SET_POS_C1' incorporates:
   *  TriggerPort: '<S6>/Trigger'
   */
  rtTempZCSig = ( (encoders_B.Gain) > (0) );
  zcEvent = rt_I32ZCFcn(RISING_ZERO_CROSSING,
                        &encoders_PrevZCX.DS1104ENC_SET_POS_C1_Trig_ZCE,
                        (rtTempZCSig));
  if (zcEvent != NO_ZCEVENT) {
    /* S-Function (rti_commonblock): '<S6>/S-Function1' */
    /* This comment workarounds a code generation problem */

    /* dSPACE I/O Board DS1104 Unit:ENC_SET */
    ds1104_inc_position_write(1, 0, DS1104_INC_LINE_SUBDIV_4);
  }

  /* End of Outputs for SubSystem: '<Root>/DS1104ENC_SET_POS_C1' */

  /* Outputs for Triggered SubSystem: '<Root>/DS1104ENC_SET_POS_C2' incorporates:
   *  TriggerPort: '<S7>/Trigger'
   */
  rtTempZCSig_0 = ( (encoders_B.Gain) > (0) );
  zcEvent = rt_I32ZCFcn(RISING_ZERO_CROSSING,
                        &encoders_PrevZCX.DS1104ENC_SET_POS_C2_Trig_ZCE,
                        (rtTempZCSig_0));
  if (zcEvent != NO_ZCEVENT) {
    /* S-Function (rti_commonblock): '<S7>/S-Function1' */
    /* This comment workarounds a code generation problem */

    /* dSPACE I/O Board DS1104 Unit:ENC_SET */
    ds1104_inc_position_write(2, 0, DS1104_INC_LINE_SUBDIV_4);
  }

  /* End of Outputs for SubSystem: '<Root>/DS1104ENC_SET_POS_C2' */

  /* S-Function (rti_commonblock): '<S5>/S-Function1' */
  /* This comment workarounds a code generation problem */

  /* S-Function (rti_commonblock): '<S5>/S-Function2' */
  /* This comment workarounds a code generation problem */
}

/* Model update function */
static void encoders_update(void)
{
  /* Update for UnitDelay: '<S8>/Delay Input1' */
  encoders_DW.DelayInput1_DSTATE = encoders_B.Step;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++encoders_M->Timing.clockTick0)) {
    ++encoders_M->Timing.clockTickH0;
  }

  encoders_M->Timing.t[0] = encoders_M->Timing.clockTick0 *
    encoders_M->Timing.stepSize0 + encoders_M->Timing.clockTickH0 *
    encoders_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++encoders_M->Timing.clockTick1)) {
      ++encoders_M->Timing.clockTickH1;
    }

    encoders_M->Timing.t[1] = encoders_M->Timing.clockTick1 *
      encoders_M->Timing.stepSize1 + encoders_M->Timing.clockTickH1 *
      encoders_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Model initialize function */
static void encoders_initialize(void)
{
  encoders_PrevZCX.DS1104ENC_SET_POS_C1_Trig_ZCE = POS_ZCSIG;
  encoders_PrevZCX.DS1104ENC_SET_POS_C2_Trig_ZCE = POS_ZCSIG;

  /* InitializeConditions for UnitDelay: '<S8>/Delay Input1' */
  encoders_DW.DelayInput1_DSTATE = encoders_P.DelayInput1_InitialCondition;
}

/* Model terminate function */
static void encoders_terminate(void)
{
  /* (no terminate code required) */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  encoders_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  encoders_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  encoders_initialize();
}

void MdlTerminate(void)
{
  encoders_terminate();
}

/* Registration function */
RT_MODEL_encoders_T *encoders(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)encoders_M, 0,
                sizeof(RT_MODEL_encoders_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&encoders_M->solverInfo,
                          &encoders_M->Timing.simTimeStep);
    rtsiSetTPtr(&encoders_M->solverInfo, &rtmGetTPtr(encoders_M));
    rtsiSetStepSizePtr(&encoders_M->solverInfo, &encoders_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&encoders_M->solverInfo, (&rtmGetErrorStatus
      (encoders_M)));
    rtsiSetRTModelPtr(&encoders_M->solverInfo, encoders_M);
  }

  rtsiSetSimTimeStep(&encoders_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&encoders_M->solverInfo,"FixedStepDiscrete");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = encoders_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    encoders_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    encoders_M->Timing.sampleTimes = (&encoders_M->Timing.sampleTimesArray[0]);
    encoders_M->Timing.offsetTimes = (&encoders_M->Timing.offsetTimesArray[0]);

    /* task periods */
    encoders_M->Timing.sampleTimes[0] = (0.0);
    encoders_M->Timing.sampleTimes[1] = (0.001);

    /* task offsets */
    encoders_M->Timing.offsetTimes[0] = (0.0);
    encoders_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(encoders_M, &encoders_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = encoders_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    encoders_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(encoders_M, -1);
  encoders_M->Timing.stepSize0 = 0.001;
  encoders_M->Timing.stepSize1 = 0.001;
  encoders_M->solverInfoPtr = (&encoders_M->solverInfo);
  encoders_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&encoders_M->solverInfo, 0.001);
  rtsiSetSolverMode(&encoders_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  encoders_M->ModelData.blockIO = ((void *) &encoders_B);
  (void) memset(((void *) &encoders_B), 0,
                sizeof(B_encoders_T));

  /* parameters */
  encoders_M->ModelData.defaultParam = ((real_T *)&encoders_P);

  /* states (dwork) */
  encoders_M->ModelData.dwork = ((void *) &encoders_DW);
  (void) memset((void *)&encoders_DW, 0,
                sizeof(DW_encoders_T));

  {
    /* user code (registration function declaration) */
    /*Call the macro that initializes the global TRC pointers
       inside the model initialization/registration function. */
    RTI_INIT_TRC_POINTERS();
  }

  /* Initialize Sizes */
  encoders_M->Sizes.numContStates = (0);/* Number of continuous states */
  encoders_M->Sizes.numY = (0);        /* Number of model outputs */
  encoders_M->Sizes.numU = (0);        /* Number of model inputs */
  encoders_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  encoders_M->Sizes.numSampTimes = (2);/* Number of sample times */
  encoders_M->Sizes.numBlocks = (18);  /* Number of blocks */
  encoders_M->Sizes.numBlockIO = (10); /* Number of block outputs */
  encoders_M->Sizes.numBlockPrms = (10);/* Sum of parameter "widths" */
  return encoders_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
