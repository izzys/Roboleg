/*
  Controller.h - Library for Controlling RoboLeg.
  Created by Israel Schallheim, 10.2.15.
  Last edited: 11.2.15
*/
#ifndef Controller_h
#define Controller_h

#include "Arduino.h"

#include <stlport.h>
#include <Eigen30.h>
using namespace Eigen; 


class Controller
{
  public:
 
    //sample frequency:
    float fs;
    
    //Control gains:
    RowVector4f K_hip;
    float K_ankle;
    
    // Observer gains:
    MatrixXf L_obs;
    
    // Model for observer:
    Matrix4f Ao;
    Vector4f Bo;
    MatrixXf Co;
    
    // reference signal:
    float Hip_Ref_Amplitude;
    float Ankle_Ref_Amplitude;

    // states vector:
    Vector4f Hip_state;
    Vector4f Ankle_state;
    Vector4f Hip_observed;
    Vector4f Ankle_observed; 
    
    // Ref signal:
    float Ref_Period;
    RowVectorXf Hip_Ref_Block; 
    RowVectorXf Ankle_Ref_Block; 
    
 // Methods:   
    Controller(float hip_amplitude, float ankle_amplitude,float ref_period);
    void Reset(float enc1,float enc2,float enc3,float enc4);
    float GetHipCommand(float enc1, float enc2);
    float GetAnkleCommand(float enc2, float enc3);  
    void SetHipState(float enc1, float enc2, float ref ) ;
    void SetHipObserved(float u , float enc1 , float enc2 ) ;
    void SetAnkleState(float enc3, float enc4 , float ref);  
    float SetHipRef(); 
    float SetAnkleRef();  
    float Get_Ref(float time);
    int Set_Internal_Time(float time);
      
  private:
  
 // Variables: 
    int _rst_flag;
    int _nStates;
    int _nInputs;
    int _nOutputs;
    float _Internal_Time; 
    float _Last_Sync_Time;  
    float  _Ref_Block_interval;
    int  _Ref_Block_index;
};

#endif

