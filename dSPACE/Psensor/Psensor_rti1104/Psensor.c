/*
 * Psensor.c
 *
 * Code generation for model "Psensor".
 *
 * Model version              : 1.4
 * Simulink Coder version : 8.7 (R2014b) 08-Sep-2014
 * C source code generated on : Mon Nov 30 11:37:19 2015
 *
 * Target selection: rti1104.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->Custom
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "Psensor_trc_ptr.h"
#include "Psensor.h"
#include "Psensor_private.h"

/* Block signals (auto storage) */
B_Psensor_T Psensor_B;

/* Real-time model */
RT_MODEL_Psensor_T Psensor_M_;
RT_MODEL_Psensor_T *const Psensor_M = &Psensor_M_;

/* Model output function */
static void Psensor_output(void)
{
  /* S-Function (rti_commonblock): '<S1>/S-Function' */
  /* This comment workarounds a code generation problem */

  /* Gain: '<Root>/P1' */
  Psensor_B.P1 = Psensor_P.P1_Gain * Psensor_B.SFunction[0];

  /* Gain: '<Root>/P2' */
  Psensor_B.P2 = Psensor_P.P2_Gain * Psensor_B.SFunction[1];
}

/* Model update function */
static void Psensor_update(void)
{
  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++Psensor_M->Timing.clockTick0)) {
    ++Psensor_M->Timing.clockTickH0;
  }

  Psensor_M->Timing.t[0] = Psensor_M->Timing.clockTick0 *
    Psensor_M->Timing.stepSize0 + Psensor_M->Timing.clockTickH0 *
    Psensor_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
static void Psensor_initialize(void)
{
}

/* Model terminate function */
static void Psensor_terminate(void)
{
  /* (no terminate code required) */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  Psensor_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  Psensor_update();
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
  Psensor_initialize();
}

void MdlTerminate(void)
{
  Psensor_terminate();
}

/* Registration function */
RT_MODEL_Psensor_T *Psensor(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)Psensor_M, 0,
                sizeof(RT_MODEL_Psensor_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = Psensor_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    Psensor_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    Psensor_M->Timing.sampleTimes = (&Psensor_M->Timing.sampleTimesArray[0]);
    Psensor_M->Timing.offsetTimes = (&Psensor_M->Timing.offsetTimesArray[0]);

    /* task periods */
    Psensor_M->Timing.sampleTimes[0] = (0.001);

    /* task offsets */
    Psensor_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(Psensor_M, &Psensor_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = Psensor_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    Psensor_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(Psensor_M, -1);
  Psensor_M->Timing.stepSize0 = 0.001;
  Psensor_M->solverInfoPtr = (&Psensor_M->solverInfo);
  Psensor_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&Psensor_M->solverInfo, 0.001);
  rtsiSetSolverMode(&Psensor_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  Psensor_M->ModelData.blockIO = ((void *) &Psensor_B);
  (void) memset(((void *) &Psensor_B), 0,
                sizeof(B_Psensor_T));

  /* parameters */
  Psensor_M->ModelData.defaultParam = ((real_T *)&Psensor_P);

  {
    /* user code (registration function declaration) */
    /*Call the macro that initializes the global TRC pointers
       inside the model initialization/registration function. */
    RTI_INIT_TRC_POINTERS();
  }

  /* Initialize Sizes */
  Psensor_M->Sizes.numContStates = (0);/* Number of continuous states */
  Psensor_M->Sizes.numY = (0);         /* Number of model outputs */
  Psensor_M->Sizes.numU = (0);         /* Number of model inputs */
  Psensor_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  Psensor_M->Sizes.numSampTimes = (1); /* Number of sample times */
  Psensor_M->Sizes.numBlocks = (3);    /* Number of blocks */
  Psensor_M->Sizes.numBlockIO = (3);   /* Number of block outputs */
  Psensor_M->Sizes.numBlockPrms = (2); /* Sum of parameter "widths" */
  return Psensor_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
