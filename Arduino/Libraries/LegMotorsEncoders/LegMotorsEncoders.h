#ifndef LegMotorsEncoders_h
#define LegMotorsEncoders_h

#include "Arduino.h"

  class LegMotorsEncoders{
    
    int count=0;  
    static const int  D[8];
    static const int  OE[4];  
    static const int  CPR[4]; 
    
    public:
        

         	
        // methods:
        LegMotorsEncoders();
	void  pin_setup(boolean Initial_Linear_Position) ;
        void ResetEncoder(int nEncoderIndex);
        float ReadEncoder(int nEncoderIndex);
        void M1_FWDaccel(float PWM0, float PWM1, float time);
        void M1_BWDaccel(float PWM0, float PWM1, float time);
        void M1_FWD(float PWM_VALUE);
        void M1_BWD(float PWM_VALUE);
        void M1_BRAKE();
        void M1_COAST();
        void M2_BWDaccel(float PWM0, float PWM1, float time);
        void M2_FWD(float PWM_VALUE);
        void M2_BWD(float PWM_VALUE);
        void M2_BRAKE();
        void M2_COAST();
        void LINEAR_IN();
        void LINEAR_OUT() ;
        
  };


#endif
