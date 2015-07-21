/*
  Controller.h - Library for Controlling RoboLeg.
  Created by Israel Schallheim, 10.2.15.
  Last edited: 11.2.15
*/

#include "Arduino.h"
#include "Controller.h"

#include <stlport.h>
#include <Eigen30.h>
using namespace Eigen; 

#define PI 3.14159265359
#define REF_BLOCK_LENGTH  20
  
    
  Controller::Controller(float hip_amplitude , float ankle_amplitude , float ref_period){

    // Set here # of states:    
    this->_nStates = 4;
    this-> _nInputs = 2;
    this-> _nOutputs = 1; 
    
    
    // Set here desired swing amplitude:  
    this->Hip_Ref_Amplitude = hip_amplitude;
    this->Ankle_Ref_Amplitude = ankle_amplitude;
    
    // Set here hip controler gains:
    this->K_hip(0) = -2.2559297e+01 ; this->K_hip(1) = -4.3910763e+01  ; this->K_hip(2) =  -8.3525615e+03  ; this->K_hip(3) = 7.4417963e+01;
  //  this->K_hip(0) = 0 ; this->K_hip(1) = 0 ; this->K_hip(2) =  -2000 ; this->K_hip(3) = 0;

    // Set here ankle controler gains:    
    this->K_ankle = -75;
    
    // Set here hip observer gains:  
    this->L_obs.resize(4,2);
    
    // Pole-Placement observer:
    this->L_obs(0,0) = 2.5874613e+00 ; this->L_obs(0,1) = 1.8402175e-04 ;
    this->L_obs(1,0) = 8.8251956e+02 ; this->L_obs(1,1) = 1.8341572e-01 ;      
    this->L_obs(2,0) = 9.2949984e-05 ; this->L_obs(2,1) =  2.7986000e+00 ;      
    this->L_obs(3,0) = 9.3216291e-02 ; this->L_obs(3,1) =  1.0587311e+03 ;  

    // Kalman observer:
//    this->L_obs(0,0) = 3.5036644e-02 ; this->L_obs(0,1) = 1.7991478e-03 ;
//    this->L_obs(1,0) = 2.3734253e-01 ; this->L_obs(1,1) = -1.2798249e-03 ;      
//    this->L_obs(2,0) = 1.8767465e-03 ; this->L_obs(2,1) =  3.7403338e-03 ;      
//    this->L_obs(3,0) = 3.7897852e-02 ; this->L_obs(3,1) =  3.7575881e-03 ;  

    // Set here model A matrix for observer:     
    this->Ao(0,0) =  9.9990618e-01      ;  this->Ao(0,1) =  1.7791410e-03      ;  this->Ao(0,2) =  9.3813118e-05    ;  this->Ao(0,3) = 6.3750007e-08   ; 
    this->Ao(1,0) =  -9.0226355e-02     ;  this->Ao(1,1) =  7.8755515e-01     ;  this->Ao(1,2) =  9.0222539e-02     ;  this->Ao(1,3) =  9.3779118e-05  ; 
    this->Ao(2,0) =   4.7314428e-05    ;  this->Ao(2,1) =  2.9750003e-08    ;  this->Ao(2,2) = 9.9983302e-01      ;  this->Ao(2,3) =  1.9988224e-03  ; 
    this->Ao(3,0) =  4.7303955e-02      ;  this->Ao(3,1) =  4.3763588e-05      ;  this->Ao(3,2) = -1.6694163e-01    ;  this->Ao(3,3) = 9.9876698e-01   ;  

    // Set here model B matrix for observer:      
    this->Bo(0)= 6.7298374e-06;
    this->Bo(1) = 6.4724575e-03 ; 
    this->Bo(2) =  5.4751344e-11 ;
    this->Bo(3) = 1.0822955e-07 ; 
    
    // Set here model C matrix for observer: 
    this->Co.resize(2,4);    
    this->Co(0,0) = 1 ; this->Co(0,1) = 0 ; this->Co(0,2) = 0 ; this->Co(0,3) = 0;
    this->Co(1,0) = 0 ; this->Co(1,1) = 0 ; this->Co(1,2) = 1 ; this->Co(1,3) = 0; 

    // Set here ref signal period:           
    this->Ref_Period = ref_period;  // matches natural frequency of leg
    this->Hip_Ref_Block.resize(1,REF_BLOCK_LENGTH);
    this->Ankle_Ref_Block.resize(1,REF_BLOCK_LENGTH);
    
//  this-> Hip_Ref_Block <<     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ; 
   this-> Ankle_Ref_Block <<   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ;

  // this-> Hip_Ref_Block <<     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 ;
//   this-> Ankle_Ref_Block <<     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 ;

    this-> Hip_Ref_Block <<    0 , 0 , 0 , 0.3 ,0.3 ,0.3 , 0.3 , 0 , 0 , 0 , 0 , 0 , 0 , -0.5 , -0.5 , -0.5 , -0.5 , 0 , 0 , 0 ;
    //    this-> Hip_Ref_Block <<    0 , 0 , 0.3 , 0.3 ,0.3 ,0.3 , 0 , 0 , 0 , 0 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , 0 , 0 , 0 ;
   // this-> Ankle_Ref_Block <<  0 , 0 , 0   , 0   , 0  , 0  , 0 , 0 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.8 , 0.8 , 0.8 , 0.8, 0.8 ;
     //  this-> Ankle_Ref_Block <<   0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 ;
     
    this->_Ref_Block_interval = Ref_Period/REF_BLOCK_LENGTH;
  }

  
  void Controller::Reset(float enc1,float enc2,float enc3,float enc4){
     
    this->Hip_state(0) = 0;     this->Hip_state(1) = 0;     this->Hip_state(2) = 0;      this->Hip_state(3) = 0;   
    this->Ankle_state(0)= 0;    this->Ankle_state(1) = 0;   this->Ankle_state(2)  = 0;   this->Ankle_state(3) = 0;  
    this->Hip_observed(0) = enc1;  this->Hip_observed(1) = 0;  this->Hip_observed(2)  = enc2;  this->Hip_observed(3) = 0; 
    this->_Internal_Time = 0;
    this->_Last_Sync_Time = 0;
    this->_Ref_Block_index = 1;
      
  } 
  
  
  void Controller::SetHipState(float enc1, float enc2, float ref ) {
  

   this->Hip_state(0) = enc1*PI/180;
   this->Hip_state(1) = this->Hip_observed(1);
   this->Hip_state(2) = (enc1-enc2)*PI/180-ref*PI/180;
   this->Hip_state(3) = this->Hip_observed(3);
   

  } 
  
  
  void Controller::SetAnkleState(float enc3, float enc4, float ref ){
    
  //empty
  
  } 
   
   
  float Controller::GetHipCommand(float enc1, float enc2){
 
    float ref = this->SetHipRef();
    float comm = 0;
    
    this->SetHipState(enc1,enc2,ref );
              
    comm = this->K_hip*this->Hip_state;

    Controller::SetHipObserved( comm , enc1 , enc2 ) ;

    return comm;
  }
  
  
  float Controller::GetAnkleCommand(float enc2, float enc3){
 

    // get ref signa:
    float ref = this->SetAnkleRef();
    //float err = ref-enc2-enc3;
     float err = ref-enc3;
    //this->SetAnkleState(enc3,enc4 ,ref);
      
    float comm = this->K_ankle*err;  
    
    return comm;
  }
  
  
  float Controller::SetHipRef(){
    
    float ref;
    ref =  this->Hip_Ref_Block(this->_Ref_Block_index - 1)*this->Hip_Ref_Amplitude;
    
    return ref;
        
  }
      
      
  float Controller::SetAnkleRef(){
    
    float ref;
    ref =  this->Ankle_Ref_Block(this->_Ref_Block_index - 1)*this->Ankle_Ref_Amplitude;
    
    return ref;
     
  } 
  
  void Controller::SetHipObserved(float u , float enc1 , float enc2 ){

    // xh is taken from last iteration:
    Vector4f xh;
    xh = this->Hip_observed;
    
    // set here u to be -255<u<255
    u = (int)u;
     if (u>255){
        u=255;
     }
     if (u<-255){
        u=-255;
     }  
    
    // Output vector:
    Vector2f y;  
    y(0) = enc1*PI/180;
    y(1) = enc2*PI/180;
    
    Vector2f yh;  
    yh = this->Co*xh;
  
    Vector2f dy;  
    dy = y-yh; 
    
   // do here: xh =  L*dy+B*u+A*xh
   this->Hip_observed =  this->L_obs*dy + this->Bo*u + this->Ao*xh;
   
  } 
  
  int Controller::Set_Internal_Time(float time){
    
    this->_Internal_Time = time -  this->_Last_Sync_Time;
    
    if (this->_Internal_Time>=this-> Ref_Period) {    
      this->_Last_Sync_Time = time;
      this->_Internal_Time = 0;
      this->_Ref_Block_index = 1;
    }
    
    if ( _Internal_Time>=(_Ref_Block_interval*_Ref_Block_index) ){
       this->_Ref_Block_index++;  
    }
    
   return  this->_Ref_Block_index;
  }


