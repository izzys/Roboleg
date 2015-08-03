//#include <LegMotorsEncodersV2_1.ino>
#include <MsTimer2.h>   /// TODO: timer2 -> timer1
#include <TimerOne.h>
//#include "IEEE754tools.h"
#include "LegMotorsEncoders.h"
#include "TimerThree.h"
#include <math.h> 

#define MAX_PWM 255
#define PI 3.14159265359

// User defined parameters:
/*=========================================*/
float  firstfreq = 0.01;//first frequency
float  lastfreq = 10;//last frequenc 
unsigned long lasttime = 60*2e6;  // in millis seconds
short amplitude = 70; 
boolean Initial_Linear_Position = 1; // Set here lonear motot inital position
short Loop_freqency = 200;// Set here control cycle frequency (Hz)
//short Sample_freqency = 10 ; // Set here sample frequency (Hz)

// Scitech defenition
bool isLogaritmic = false; // default:: linner
/*=========================================*/


boolean running;//used to test if motor is running

unsigned int Loop_Period = 1e6 / Loop_freqency; // Control cycle period (micro-seconds) 
//unsigned int Sample_Period  = 1e6 / Sample_freqency; // Sample period (micro-seconds)      
volatile int Loop_counter;
volatile int Loop_counter_max;

unsigned long T ; //Time

unsigned long sync_time = 0;
float sin_wave = 0;
volatile int first_loop =1;
volatile int first_stop = 0;

long enc1;
long enc2;
short comm;
short comm_output;
float freq = 0;

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

  Timer1.initialize(Loop_Period);
//  MsTimer2::set(Loop_Period, LoopAction);
 // Loop_counter_max = (short) ( Loop_freqency / Sample_freqency );
  

}


void loop(){//Send the motor commands to move each time through this loop
   

  switch (state) {

  case 1: //When the board is in the state of "running", set from a serial command below:
  
    //while the motor is staring up, the first time through sampling the encoders, realize there will be junk
    noInterrupts();
    
    if (first_loop){

      Serial.println("starting......");
      Loop_counter = 0;
      T = 0;
      first_loop = 0; 
      sync_time = micros(); 
      

   
     Timer1.attachInterrupt(LoopAction);
    //  MsTimer2::start();
    }
    
    interrupts();
                
    break;

   case 0: //Stop motors and reset
   
      noInterrupts();
      if (first_stop) {
        
        Timer1.detachInterrupt();
      //  MsTimer2::stop();

        
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
    
    enc1 =  LME.ReadEncoder(1);
    enc2 = -LME.ReadEncoder(2);
      
    //Set the time for each iteration through the motor commands
    T =  micros() - sync_time ;   
    
    // Sending sine signal
    if(isLogaritmic)
    {
      float N=log(lastfreq/firstfreq)/log(2);
      float R=N/(lasttime*1e-6);
      sin_wave=sin(2*PI*( firstfreq*(-1+pow(2,R*T*1e-6))/(R*log(2)) )    );
    }else{
      freq = 0.5*(lastfreq-firstfreq)*T/lasttime + firstfreq;
      sin_wave = sin(2*PI*freq*T*1e-6); // linner wave
    }

    comm = (short) (amplitude*sin_wave); //and set the motor command to that point

    //If the leg gets to high-->emergency stop
    if((enc2)>3000 || (enc2<-3000) || abs(enc1-enc2)>3600)     // 3600 = 90*; 3000 = 75*
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
    
  //  Loop_counter++;
 //   if ( Loop_counter >= Loop_counter_max) {

      Sample_Angle_and_Send();
      
 //     Loop_counter = 0;
  //  }
  
  

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
      Serial.println(T);
      
//      Serial.print("   ");
//      Serial.println(freq);
      
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
 
