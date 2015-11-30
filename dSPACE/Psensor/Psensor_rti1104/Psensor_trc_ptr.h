  /*********************** dSPACE target specific file *************************

   Header file Psensor_trc_ptr.h:

   Declaration of function that initializes the global TRC pointers

   RTI1104 7.3 (02-Nov-2014)
   Mon Nov 30 11:37:19 2015

   (c) Copyright 2008, dSPACE GmbH. All rights reserved.

  *****************************************************************************/
  #ifndef RTI_HEADER_Psensor_trc_ptr_h_
  #define RTI_HEADER_Psensor_trc_ptr_h_
  /* Include the model header file. */
  #include "Psensor.h"
  #include "Psensor_private.h"

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
              EXTERN_C volatile  real_T *p_0_Psensor_real_T_0;
              EXTERN_C volatile  real_T *p_1_Psensor_real_T_0;

   #define RTI_INIT_TRC_POINTERS() \
              p_0_Psensor_real_T_0 = &Psensor_B.SFunction[0];\
              p_1_Psensor_real_T_0 = &Psensor_P.P1_Gain;\

   #endif                       /* RTI_HEADER_Psensor_trc_ptr_h_ */
