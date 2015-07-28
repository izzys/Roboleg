//#include <LegMotorsEncodersV2_1.ino>
#include <MsTimer2.h>
//#include "IEEE754tools.h"
#include "LegMotorsEncoders.h"
#include "TimerThree.h"

#define MAX_PWM 255
#define PI 3.14159265359

// User defined parameters:
/*=========================================*/
float  firstfreq = 1;//first frequency
float  lastfreq = 1;//first frequency
float lasttime = 500;
float amplitude = 255; 
boolean Initial_Linear_Position = 1; // Set here lonear motot inital position
float Loop_freqency = 250;// Set here control cycle frequency (Hz)
float Sample_freqency = 10 ; // Set here sample frequency (Hz)
/*=========================================*/

boolean running;//used to test if motor is running

unsigned int Control_Period = 1000 / Loop_freqency; // Control cycle period (mili-seconds)
unsigned int Sample_Period  = 1000 / Sample_freqency; // Sample period (mili-seconds)
volatile int Control_loop_counter;
volatile int Control_loop_counter_max;

float T ; //Time

float sync_time = 0.0;
float sin_wave = 0;
volatile int first_loop =1;
volatile int first_stop = 0;

float enc1;
float enc2;
float comm;
float comm_output;
float freq;

LegMotorsEncoders LME;

int state = 0; //initial case state==0

void setup()
{
  
  Serial.begin(115200);
  delay(100);
 
  
  // Set timer 3 PWM:
  Timer3.initialize(50); //im micro secons units. (50 micro seconds == 20 Khz)
  
  LME.pin_setup(Initial_Linear_Position); //set up the pins on the board
  
  LME.ResetEncoder(1); //reset the encoders to read zero from their initial position
  LME.ResetEncoder(2);
  LME.ResetEncoder(3);
  LME.ResetEncoder(4);



  MsTimer2::set(Control_Period, LoopAction);
  Control_loop_counter_max = Loop_freqency / Sample_freqency;
  

}


void loop(){//Send the motor commands to move each time through this loop
   

  switch (state) {

  case 1: //When the board is in the state of "running", set from a serial command below:
  
    //while the motor is staring up, the first time through sampling the encoders, realize there will be junk
    noInterrupts();
    
    if (first_loop){

      Serial.println("starting......");
      Control_loop_counter = 0;
      T = 0.0;
      first_loop = 0;
      sync_time = millis()*1e-3;
      MsTimer2::start();
    }
    
    interrupts();

    break;

   case 0: //Stop motors and reset
   
      noInterrupts();
      if (first_stop) {

        MsTimer2::stop();
        
        LME.M1_COAST();
        LME.M2_COAST();

        first_loop = 1; //reset
        first_stop = 0;

        Serial.println("stoping.");
      }
      interrupts();

  default:

    LME.M1_COAST();
    LME.M2_COAST();
    break;
  }

}

void LoopAction() {
  
    if (T>=lasttime){
      state =0;
      return;
    }
    
    enc1 =  LME.ReadEncoder(1);//Read the encoders and stardardize positve/negative values (?)
    enc2 = -LME.ReadEncoder(2);
    
    //Set the time for each iteration through the motor commands
    T =  millis()*1e-3 - sync_time ;
    
    //The sin wave to send;
    freq = 0.5*(lastfreq-firstfreq)*T/lasttime + firstfreq;
    sin_wave = sin(2*PI*freq*T);
    comm = amplitude*sin_wave; //and set the motor command to that point

    //If the leg gets to high-->emergency stop
   
    if((enc2)>75 || (enc2<-75) || abs(enc1-enc2)>90)     
    {
      Serial.println("emergency stop");
      comm_output = 0;
      state = 99;

    }
    
 
    //Whenever you send a command, only send it if it is below the MAX_PWM value set above
    //And actually move the motor, i.e. set comm_output based on comm
    if(comm>0)
    {
      comm_output = min(comm,MAX_PWM);
      LME.M2_FWD(comm_output);
    }
    else
    {
      comm_output = min(abs(comm),MAX_PWM);
      LME.M2_BWD(comm_output);
    }
    
    Control_loop_counter++;
    
    if ( Control_loop_counter >= Control_loop_counter_max) {

      Sample_Angle_and_Send();
      Control_loop_counter = 0;
    }
  
  

}

void Sample_Angle_and_Send() //Based on sample frequency set above, send data to MATLAB
{


  if (!first_loop){
    switch(state){

    case 1://If running

      Serial.print(enc1);
      Serial.print("   ");
      Serial.print(enc2);
      Serial.print("   ");
      Serial.print(comm);
      Serial.print("   ");
      Serial.print(T);
      Serial.print("   ");
      Serial.println(freq);
      
      break;

    case 0://If stopping

      break;

    default:

      break;
    }
  }
}

void serialEvent(){ //Receive Serial data from MATLAB
  while (Serial.available()){
    char incoming = Serial.read();
    delay(50);
    if(incoming == '1') {
      if (!state)  {
        state = 1;
      }
    }

     else if (incoming == '0') { //Stop motors and reset


      if (state) {

        state = 0;
        first_stop = 1;
      }
     }
    else {
      // do nothing
    }
  }
}

