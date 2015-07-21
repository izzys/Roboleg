#include <LegMotorsEncoders_Step_closed_loop.ino>
#include "MsTimer2.h"


#define MAX_PWM 125
#define PI 3.14159265359

// First values set to match those that MATLAB expects
/*=========================================*/

float amplitude = 30; //first amplitude
float Kcontroller  =20;// 0.724;
/*=========================================*/

boolean running;//used to test if motor is running

float Loop_Freq = 500;// Set here time period for loop
int Dl = 1000/Loop_Freq;//Delay to move the motor each loop

float Sample_freqency = 100 ;
float Sample_Period  =  (1/Sample_freqency)*1000;
float T ; //Time


float sync_time = 0.0;
float sin_wave = 0;
int first_loop =1;

float enc2;
float comm;
float comm_output;
float enc1;
float freq;
float u;
float e ;

int state = 99; //initial case state==default

void setup()
{
    Serial.begin(115200);
  delay(100);
      
  pin_setup(); //set up the pins on the board
  ResetEncoder(1); //reset the encoders to read zero from their initial position
  ResetEncoder(2);
  ResetEncoder(3);
  ResetEncoder(4);
 
  MsTimer2::set(Sample_Period,Sample_Angle_and_Send);
  MsTimer2::start();
}


void loop(){//Send the motor commands to move each time through this loop

   enc1 =  ReadEncoder(1);//Read the encoders and stardardize positve/negative values (?)
   enc2 = -ReadEncoder(2);
   
  switch (state) {

  case 1: //When the board is in the state of "running", set from a serial command below:

    //while the motor is staring up, the first time through sampling the encoders, realize there will be junk
    if (first_loop){

      Serial.println("starting......");
      T = 0.0;
      first_loop = 0;
      sync_time = millis()*1e-3;

    }
    

    //Set the time for each iteration through the motor commands
    T =  millis()*1e-3 - sync_time ;
    
    //The sin wave to send;
    
    u = Kcontroller*e;
    comm = u; //and set the motor command to that point

    //If the leg gets to high-->emergency stop
    
   /* if((enc2)>75 || (enc2<-75) || abs(enc1-enc2)>90)     

    {
      Serial.println("emergency stop");
      comm_output = 0;
      state = 99;

    }*/
  
    //Whenever you send a command, only send it if it is below the MAX_PWM value set above
    //And actually move the motor, i.e. set comm_output based on comm
    if(comm>0)
    {
      comm_output = min(comm,MAX_PWM);
      M2_FWD(comm_output);
    }
    else
    {
      comm_output = min(abs(comm),MAX_PWM);
      M2_BWD(comm_output);
    }

    break;

  case 0: //When the board is in the state of "stopping", set from a serial command below
    M2_COAST();
    ResetEncoder(1); //reset the encoders to read zero from their initial position
   ResetEncoder(2);
   e = 0;
    break;

  default:
    M2_COAST();
    break;
  }

  delay(Dl);//delay enough to let the leg move between each motor command
}

void Sample_Angle_and_Send() //Based on sample frequency set above, send data to MATLAB
{


  if (!first_loop){
    switch(state){

    case 1://If running
     
     enc1 = ReadEncoder(1);
     e = amplitude - enc1;
     
     
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
         //    Serial.println("0");

             state = 99;
              first_loop = 0;
      break;

    default:
           //Serial.println("99");
    break;
    }
  }
}

void serialEvent(){ //Receive Serial data from MATLAB
  while (Serial.available()){
    char incoming = Serial.read();
    delay(50);
    if(incoming == '1') {
      state = 1;//Run the motors
      //
    }

    else if(incoming == '0'){ //Stop motors and reset

      state = 0;
      first_loop = 0; //reset loop(?)
    }
    else {
     // Serial.println("junk");
      //do nothing
    }
   
  }
}

