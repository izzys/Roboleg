/*******************************************************************************
  FILE:
    ds1104_enc_sw_index_s.c

  DESCRIPTION:

    Level-2 S-function for polling one index line of a DS1104 DSP Controller Board.
    When the index is found the corresponding counter is set to the specified
    value if desired.

    The DS1104ENC_INDEX block refers to the S-function file ds1104enc_index_s.c
    which is available from the directory $(DSPACE_ROOT)\matlab\rti1104\sfcn.
    By default, this S-function searches for the index signal of the specified channel.
    If the index is found, the position counter is set to the value specified
    and the block output is set to 1. If a different functionality is required
    for your application, the source code of the S-function can be customized accordingly.

  REMARKS:
    Syntax: [sys, x0] = ds1104_enc_sw_index_s(t, x, u, ...
                                         ChannelNo, ...
                                         SearchType, ...
                                         SetPosMode, ...
                                         PosValue, ...
                                         SetOutMode,
                                         SampleTime);

    input variables
    (run/hold control):
        u(1)    enable index search

    output variables
    (index found flag)
        sys(1)  index found

    parameters:

        ChannelNo               Channel Number of the Incremental Encoder
                                Interface (1-2)

        SearchType              Indicates whether the index search should be
                                performed in normal mode (index is searched
                                once) or in speedup mode (index is searched
                                twice). Values:
                                        0: index is searched once (default)
                                        1: index is searched twice

        SetPosMode              Indicates whether the position count should be set to
                                the PosValue if the index search is finished or if it
                                should be kept unaffected. Values:
                                        0: position should not be set
                                        1: position should be set (default)

        PosValue                The value the position count is to if the index search is
                                finished and SetPosMode is set to 1.

        SetOutMode              Indicates whether the block outputs keeps high after the
                                index has been found or if it follows the current value
                                of the index.
                                        1: keep high
                                        2: current index

        SampleTime              Block's sample time, inherits from system if
                                less or equal than 0.0

  IMPORTANT NOTES:
    * You may modify this S-function to satisfy your needs.
      ! Be sure to copy this file to your local working directory !
      ! before modifying. At the origin location the file will    !
      ! be overwritten with each (re-)installation of the RTI.    !
      Please refer to the RTI1104 User's Guide for notes on S-functions
      and user-code.
    * As long as you are not introducing changes to the number of in- and
      outputs, states, I/R/PWork-size you don't need to recompile the
      MEX/DLL function coming with this S-function.
    * The dSPACE Software-Environment functions are using data types
      as 'long' or 'float', defined in DSPTYPES.H.
    * The MATLAB/Simulink S-functions are refering the to the new data types
      'real_T' and 'int_T' as defined for the RTW.

  AUTHOR(S):
    G. Aakolk

  Copyright (c) 2000 dSPACE GmbH, GERMANY

  $RCSfile: ds1104_enc_sw_index_s.c $ $Revision: 2.0 $ $Date: 2004/01/12 11:34:28MEZ $
  $ProjectName: e:/ARC/Products/ImplSW/RTISW/RTI/Components/RTI1xxxBlocks/DS1104/Master/ENC/project.pj $
 ******************************************************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME ds1104_enc_sw_index_s

#include "simstruc.h"

#include <math.h>

#ifndef MATLAB_MEX_FILE
# include <ds1104.h>
# include <io1104.h>
# include <rti_msg_access.h>
# include <rti_common_msg.h>
#endif

#ifdef MATLAB_MEX_FILE
  static char *RCSfile = "$RCSfile: ds1104_enc_sw_index_s.c $";
  static char *RCSrev  = "$Revision: 2.0 $";
  static char *RCSdate = "$Date: 2004/01/12 11:34:28MEZ $";
#endif

/* SL/RTI specific defines */
#define INPUTPORTSNUMOF       (1)
#define INPUTPORTSWIDTH       (1)
#define OUTPUTPORTSNUMOF      (1)
#define OUTPUTPORTSWIDTH      (1)

/* input argument access macros */
#define NUM_PARAM                   (6)
#define CHANNEL_NO_IDX              (0)
#define SEARCH_TYPE_IDX             (1)
#define SET_POS_MODE_IDX            (2)
#define POS_VALUE_IDX               (3)
#define SET_OUT_MODE_IDX            (4)
#define SAMPLE_TIME_IDX             (5)

#define CHANNEL_NO              (mxGetPr(ssGetSFcnParam(S, CHANNEL_NO_IDX))[0])
#define SEARCH_TYPE             (mxGetPr(ssGetSFcnParam(S, SEARCH_TYPE_IDX))[0])
#define SET_POS_MODE            (mxGetPr(ssGetSFcnParam(S, SET_POS_MODE_IDX))[0])
#define POS_VALUE               (mxGetPr(ssGetSFcnParam(S, POS_VALUE_IDX))[0])
#define SET_OUT_MODE            (mxGetPr(ssGetSFcnParam(S, SET_OUT_MODE_IDX))[0])
#define SAMPLE_TIME             (mxGetPr(ssGetSFcnParam(S, SAMPLE_TIME_IDX))[0])



/* macro constants */
#define IDX_NOT_FOUND            (-1)
#define IDX_FOUND_LEVEL1         ( 0)
#define IDX_FOUND_LEVEL2         (+1)
#define IDX_NOT_FOUND_SEARCH_DIS (0)
#define IDX_FOUND_SEARCH_DIS     (+1)
#define IDX_SEARCH_ONCE          ( 0)
#define IDX_SEARCH_TWICE         (+1)
#define OUT_KEEP                 (+1)
#define OUT_CURRENT_IDX          (+2)
#define FIRST_ENTRY              (+1)
#define RESET_FIRST_ENTRY        (0)





/* IWork access */
#define IWORK_SIZE               (3)
#define IWORK_IDX_STATUS         (0)
#define IWORK_INIT_STATUS        (1)
#define IWORK_DIS_IDX_STATUS     (2)



#define NUM_DIRECT_FEEDTHROUGH  (1)
#define NUM_SAMPLE_TIMES        (1)


static void write_Y(SimStruct *S, int_T value)
{
  int8_T *pointer_Y0 =  (int8_T *)ssGetOutputPortSignal(S,0);
  pointer_Y0[0] = (int8_T)value;
}


static void mdlInitializeSizes(SimStruct *S)
{

  ssSetNumSFcnParams(S, NUM_PARAM);
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
  {
#   ifndef MATLAB_MEX_FILE
       rti_msg_error_set(RTI_SFUNCTION_PARAM_ERROR);
#   endif

    return;
  }

  ssSetNumContStates(              S, 0);
  ssSetNumDiscStates(              S, 0);
  ssSetNumInputPorts(              S, INPUTPORTSNUMOF);
  ssSetNumOutputPorts(             S, OUTPUTPORTSNUMOF);
  ssSetInputPortWidth(             S, 0, INPUTPORTSWIDTH);
  ssSetOutputPortWidth(            S, 0, OUTPUTPORTSWIDTH);
  ssSetInputPortDirectFeedThrough( S, 0, NUM_DIRECT_FEEDTHROUGH);
  ssSetNumSampleTimes(             S, NUM_SAMPLE_TIMES);
  ssSetNumIWork(                   S, IWORK_SIZE);
  ssSetNumRWork(                   S, 0);
  ssSetNumPWork(                   S, 0);
  ssSetNumModes(                   S, 0);
  ssSetNumNonsampledZCs(           S, 0);
  ssSetOptions(                    S, SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME);

  ssSetOutputPortDataType(S, 0, SS_INT8);
  ssSetInputPortDataType(S, 0, SS_BOOLEAN);
}


static void mdlInitializeSampleTimes(SimStruct *S)
{
  real_T  sampleTime = (real_T) SAMPLE_TIME;

  /* set sample time from parameter list */
  if (sampleTime == -1.0)          /* inherited */
  {
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
  }
  else if ((sampleTime == 0.0))    /* continuous */
  {
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
  }
  else                           /* discrete */
  {
    ssSetSampleTime(S, 0, sampleTime);
    ssSetOffsetTime(S, 0, 0.0);
  }
}

#define MDL_START
#if defined(MDL_START)
static void mdlStart(SimStruct *S)
{
  int_T     first_entry;
  int_T     index_found;
#ifndef MATLAB_MEX_FILE
  UInt16    channelNo  = (UInt16)CHANNEL_NO;

  /* clear index register */
  ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET);
#endif

  /* perform initialization for this S-function only once */
  first_entry = !ssGetIWorkValue(S, IWORK_INIT_STATUS);
  if (first_entry)
  {
    ssSetIWorkValue(S, IWORK_INIT_STATUS, FIRST_ENTRY);
    index_found = (SEARCH_TYPE == IDX_SEARCH_TWICE ? IDX_NOT_FOUND : IDX_FOUND_LEVEL1);
    ssSetIWorkValue(S, IWORK_IDX_STATUS, index_found);
    /* clear marker for premature index detection */
    ssSetIWorkValue(S, IWORK_DIS_IDX_STATUS, (int_T)IDX_NOT_FOUND_SEARCH_DIS);
    /* write initial index found level to the output */
    write_Y(S, index_found);
  }
}
#endif


#undef MDL_INITIALIZE_CONDITIONS
#if defined(MDL_INITIALIZE_CONDITIONS)
static void mdlInitializeConditions(SimStruct *S)
{
}
#endif


static void mdlOutputs(SimStruct *S, int_T tid)
{
  int_T         index_found;
  boolean_T     indexSearchEnabled;
  InputPtrsType u0Pointer           = (InputPtrsType)ssGetInputPortSignalPtrs(S,0);
  boolean_T     value_U0            = *((boolean_T *)u0Pointer[0]);

#ifndef MATLAB_MEX_FILE
  UInt16        channelNo           = (UInt16)CHANNEL_NO;

  indexSearchEnabled = (value_U0 > 0);
#endif

#ifdef MATLAB_MEX_FILE
  write_Y(S, IDX_FOUND_LEVEL2);
#else

  if (indexSearchEnabled)
   {
    if (ssGetIWorkValue(S, IWORK_DIS_IDX_STATUS) == (int_T)IDX_FOUND_SEARCH_DIS)
     {
      /* Index flag was set while search was disabled, clear
         corresponding marker, index flag and abort search
	 for this sample hit */
      ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET);
      ssSetIWorkValue(S, IWORK_DIS_IDX_STATUS, (int_T)IDX_NOT_FOUND_SEARCH_DIS);
      return;
     }
    /* search enabled & no previous index detection */
    index_found = ssGetIWorkValue(S, IWORK_IDX_STATUS);

    if (SET_OUT_MODE == OUT_KEEP)
     {
      if (index_found != IDX_FOUND_LEVEL2)
       {
        /* poll index line of encoder channel CHANNEL_NO */
        if (ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET))
         {
          index_found++;
          if (index_found == IDX_FOUND_LEVEL2)
           {
            /* index search is completed when reaching level 2,
               set position count to the specified value if setting
               of position is selected
             */
            if (SET_POS_MODE)
             {
              ds1104_inc_position_write(channelNo, (Float64)POS_VALUE, (Int32)DS1104_INC_LINE_SUBDIV_4);
             }
           }
         }
         /* store current index found level */
         ssSetIWorkValue(S, IWORK_IDX_STATUS, index_found);
       } /* if (index_found!=IDX_FOUND_LEVEL2)) */

      /* write current index found level to the output */
      write_Y(S, index_found);

     } /* if (SET_OUT_MODE == OUT_KEEP) */

    else /* current index */
     {
      if (index_found != IDX_FOUND_LEVEL2)
       {
        /* poll index line of encoder channel CHANNEL_NO */
        if (ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET))
         {
          index_found++;
          if (index_found == IDX_FOUND_LEVEL2)
           {
            /* index search is completed when reaching level 2,
               set position count to the specified value if setting
               of position is selected
             */
            if (SET_POS_MODE)
             {
              ds1104_inc_position_write(channelNo, (Float64)POS_VALUE, (Int32)DS1104_INC_LINE_SUBDIV_4);
             }
           }
         }

        /* store current index found level */
        ssSetIWorkValue(S, IWORK_IDX_STATUS, index_found);

        /* write current index found level to the output */
        write_Y(S, index_found);

       } /* if (index_found != IDX_FOUND_LEVEL2) */

      else /* search is finished, write current index to the output */
       {
        int_T value;

        value = ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET);
        write_Y(S, value);
       }
     } /* current index */
   } /* indexSearchEnabled */
  else if (ssGetIWorkValue(S, IWORK_DIS_IDX_STATUS) == (int_T)IDX_NOT_FOUND_SEARCH_DIS)
   {
    /* search disabled & no previous index detection */
    if (ds1104_inc_index_read(channelNo, (UInt32)DS1104_INC_IDX_NOT_SET))
     {
      ssSetIWorkValue(S, IWORK_DIS_IDX_STATUS, (int_T)IDX_FOUND_SEARCH_DIS);
     }
   }

#endif
} /* mdlOutputs */


#undef MDL_UPDATE
#if defined(MDL_UPDATE)
static void mdlUpdate(SimStruct *S, int_T tid)
{
}
#endif


#undef MDL_DERIVATIVES
#if defined(MDL_DERIVATIVES)
static void mdlDerivatives(SimStruct *S, int_T tid)
{
}
#endif


static void mdlTerminate(SimStruct *S)
{
  /* reset first entry value */
  ssSetIWorkValue(S, IWORK_INIT_STATUS, RESET_FIRST_ENTRY);
}


#undef MDL_RTW
#if defined(MDL_RTW) && (defined(MATLAB_MEX_FILE) || defined(NRT))
static void mdlRTW(SimStruct *S)
{
}
#endif


#ifdef  MATLAB_MEX_FILE
# include "simulink.c"
#else
# include "cg_sfun.h"
#endif
