  /*********************** dSPACE target specific file *************************

   Header file encoders_trc_ptr.h:

   Declaration of function that initializes the global TRC pointers

   RTI1104 7.3 (02-Nov-2014)
   Wed Jul 29 19:29:40 2015

   (c) Copyright 2008, dSPACE GmbH. All rights reserved.

  *****************************************************************************/
  #ifndef RTI_HEADER_encoders_trc_ptr_h_
  #define RTI_HEADER_encoders_trc_ptr_h_
  /* Include the model header file. */
  #include "encoders.h"
  #include "encoders_private.h"

  #ifdef EXTERN_C
  #undef EXTERN_C
  #endif

  #ifdef __cplusplus
  #define EXTERN_C                       extern "C"
  #else
  #define EXTERN_C                       extern
  #endif

  /*
   *  Declare the global TRC pointers
   */
              EXTERN_C volatile  real_T *p_0_encoders_real_T_0;
              EXTERN_C volatile  boolean_T *p_0_encoders_boolean_T_1;
              EXTERN_C volatile  uint8_T *p_0_encoders_uint8_T_2;
              EXTERN_C volatile  real_T *p_1_encoders_real_T_0;
              EXTERN_C volatile  uint8_T *p_1_encoders_uint8_T_1;
              EXTERN_C volatile  real_T *p_2_encoders_real_T_0;

   #define RTI_INIT_TRC_POINTERS() \
              p_0_encoders_real_T_0 = &encoders_B.SineWave;\
              p_0_encoders_boolean_T_1 = &encoders_B.Compare;\
              p_0_encoders_uint8_T_2 = &encoders_B.Gain;\
              p_1_encoders_real_T_0 = &encoders_P.SineWave_Amp;\
              p_1_encoders_uint8_T_1 = &encoders_P.Gain_Gain;\
              p_2_encoders_real_T_0 = &encoders_DW.DelayInput1_DSTATE;\

   #endif                       /* RTI_HEADER_encoders_trc_ptr_h_ */
