


/*
  RoboLeg_SkatingMode.ino - Main file for controlling RoboLeg in skating mode.
 Created by Israel Schallheim, 10.2.15.
 Last edited: 11.2.15
 */
#include "LegMotorsEncoders.h"
#include "Controller.h"
#include "MsTimer2.h"
#include "TimerThree.h"
//#include "TimerOne.h"
//1#include "FlexiTimer2.h"

#define MAX_PWM 255
#define PI 3.14159265359
#define ON_PIN 18
#define PressurePin1 A0
#define PressurePin2 A1
#define START_BUTTON 5 // interupt 5 on pin 18
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Global variables:  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// Set here some stuff:
/*=========================================*/
float hip_amplitude =7;//2.5; // Set here ref amplitude
float ankle_amplitude = 0; //; // Set here ref amplitude
float ref_period = 0.8; // set here ref block period
boolean Initial_Linear_Position = 1; // Set here lonear motot inital position
int linear_on = 0; // Set here if to shorten leg
float Loop_freqency = 500;// Set here control cycle frequency (Hz)
float Sample_freqency = 10 ; // Set here sample frequency (Hz)
/*=========================================*/

// Initialize globals:
unsigned int Control_Period = 1000 / Loop_freqency; // Control cycle period (micro-seconds)
unsigned int Sample_Period  = 1000 / Sample_freqency; // Sample period (micro-seconds)
volatile float T ; //Time
volatile int Control_loop_counter;
volatile int Control_loop_counter_max;

volatile float sync_time = 0.0;
volatile int first_loop = 1;
volatile int first_stop = 0;

volatile float enc1 = 0.0;
volatile float enc2 = 0.0;
volatile float enc3 = 0.0;
volatile float enc4 = 0.0;
volatile float comm_hip = 0.0;
volatile float comm_ankle = 0.0;
volatile float comm_hip_output = 0.0;
volatile float comm_ankle_output = 0.0;

volatile float p1 = 0.0;
volatile float p2 = 0.0;

volatile int state = 0; //initial case state = 0
volatile int timing_ind = 0; // index of time interval within each period

// Controllr class constructor:
Controller Con(hip_amplitude, ankle_amplitude, ref_period);
LegMotorsEncoders LME;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Setup:  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Set timer 3 PWM:
  Timer3.initialize(50); //im micro secons units. (50 micro seconds == 20 Khz)
  
  // Set Control loop period:
//  Timer1.initialize(Control_Period);
//  FlexiTimer2::set(Control_Period, 1/1e6, ControlLoopAction);
  
  //set up the pins on the board:
  LME.pin_setup(Initial_Linear_Position);

  //Set On/Off button:
  attachInterrupt(START_BUTTON, start_walking , CHANGE  );
  //pressure sensors:
  pinMode(PressurePin1,INPUT);
  pinMode(PressurePin2,INPUT);

  //reset the encoders:
  LME.ResetEncoder(1);
  LME.ResetEncoder(2);
  LME.ResetEncoder(3);
  LME.ResetEncoder(4);

  //Set sampling interupt:
  // MsTimer2::set(Sample_Period,Sample_Angle_and_Send);

  // Set interupt for timing control loop:
  MsTimer2::set(Control_Period, ControlLoopAction);

  Control_loop_counter_max = Loop_freqency / Sample_freqency;

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Main loop:  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void loop() {


  switch (state) {

    case 1: //When the board is in the state of "running", set from a serial command below:
     
      noInterrupts(); 
      if (first_loop) {
        
         enc1 =  LME.ReadEncoder(1);
         enc2 =  -LME.ReadEncoder(2);
         enc3 = -LME.ReadEncoder(3);
         enc4 =  LME.ReadEncoder(4);
         
        Con.Reset(enc1,enc2,enc3,enc4);
   Serial.println("magic......");
        Serial.println("starting......");
        Control_loop_counter = 0;
        T = 0.0;
        first_loop = 0;
            
        sync_time = millis() * 1e-3;
        MsTimer2::start();
     // FlexiTimer2::start();
     //   Timer1.start();  
     //   Timer1.attachInterrupt(ControlLoopAction);
      
      }
      interrupts();

      break;

    case 0: //Stop motors and reset

      noInterrupts();
      if (first_stop) {

        MsTimer2::stop();
    //     Timer1.stop(); 
    //FlexiTimer2::stop();
        LME.M1_COAST();
        LME.M2_COAST();

        first_loop = 1; //reset
        first_stop = 0;

        Serial.println("stoping.");
      }
      interrupts();


      break;

    default:
      LME.M1_COAST();
      LME.M2_COAST();
      break;
  }

}

void ControlLoopAction() {
      
  //Read the encoders and stardardize positve/negative values:
   enc1 = LME.ReadEncoder(1);
   enc2 =  -LME.ReadEncoder(2);
   enc3 = -LME.ReadEncoder(3);
   enc4 =  LME.ReadEncoder(4);

  //Set the time for each iteration:
  T =  millis() * 1e-3 - sync_time ;


  timing_ind = Con.Set_Internal_Time(T);

  //    float r;
  //    r  = Con.SetHipRef();
  //     Serial.println(r);

  // Send Command to Hip:
  comm_hip = Con.GetHipCommand(enc1, enc2);
  Move_Hip(comm_hip);


  //pressure sensors:
  p1 = analogRead(PressurePin1);
  p2 = analogRead(PressurePin2);
  
  
  // Send Command to Ankle:
 // comm_ankle = Con.GetAnkleCommand(enc2, enc3);
//  Move_Ankle(comm_ankle);
    //Move_Ankle(150);
  // Command to linear motor:
  if (linear_on) {

    if (timing_ind < 8) {
      LME.LINEAR_IN();
    }
    if (timing_ind >= 8) {
      LME.LINEAR_OUT();
    }
  }


  //If the leg gets to high-->emergency stop
  Emergency_chek();


  Control_loop_counter++;

  if ( Control_loop_counter >= Control_loop_counter_max) {

    Sample_Angle_and_Send();
    Control_loop_counter = 0;
  }

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   End of Main loop  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


//                                                   :)                                                                   //


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~      Functions     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void start_walking() {

  int pin_value;

  pin_value = digitalRead(ON_PIN);

  if (pin_value){
   state = 1;
   Serial.println("ON");
  }
 else{
    state = 0;
     Serial.println("OFF");
    first_stop = 1;
  }

}

void Emergency_chek() {

  if (  (enc2 > 75) || (enc2 < -75) || (abs(enc1 - enc2) > 135)   ) //|| (abs(enc3-enc4)>135)    )
  {
    Serial.println("emergency stop");
    comm_hip_output = 0;
    state = 99;

  }
}

void Move_Hip(float comm_hip) {

  if (comm_hip > 0)
  {
    comm_hip_output = min(comm_hip, MAX_PWM);
    LME.M2_FWD(comm_hip_output);
  }
  else
  {
    comm_hip_output = min(abs(comm_hip), MAX_PWM);
    LME.M2_BWD(comm_hip_output);
  }

}

void Move_Ankle(float comm_ankle) {

  if (comm_ankle > 0)
  {
    comm_ankle_output = min(comm_ankle, MAX_PWM);
    LME.M1_FWD(comm_ankle_output);
  }
  else
  {
    comm_ankle_output = min(abs(comm_ankle), MAX_PWM);
    LME.M1_BWD(comm_ankle_output);
  }

}


//Based on sample frequency set above, send data via serial
void Sample_Angle_and_Send() {

  if (!first_loop) {

    switch (state) {

      case 1://If running

//        Serial.print(p1);
//        Serial.print("   ");
//        Serial.print(p2);
//        Serial.print("   ");



        Serial.print(enc1);
        Serial.print("   ");
        Serial.print(enc2);
        Serial.print("   ");
        Serial.print(comm_hip_output*sign(comm_hip));
        Serial.print("   ");

      //  Serial.print(enc3);
    //    Serial.print("   ");
   //     Serial.print(enc4);
   //     Serial.print("   ");
    //    Serial.print(comm_ankle_output*sign(comm_ankle));
  //      Serial.print("   ");
        Serial.println(T);
   


        //
        //     float x2;
        //     float x4;
        //
        //     x2 = Con.Hip_state(1)*180/PI ;
        //     x4 = Con.Hip_state(3)*180/PI ;
        //
        //     // State:
        //     Serial.print("   ");
        //     Serial.print(x2);
        //     Serial.print("   ");
        //     Serial.print(x4);
        //
        //     float x2h;
        //     float x4h;
        //
        //     x2h = Con.Hip_observed(1)*180/PI;
        //     x4h = Con.Hip_observed(3)*180/PI;
        //
        //     // Observed:
        //     Serial.print("   ");
        //     Serial.print(x2h);
        //     Serial.print("   ");
        //     Serial.println(x4h);



        break;

      case 0://If stopping

        // do nothing

        break;

      default:

        break;
    }
  }
}

void serialEvent() { //Receive Serial data from MATLAB
  while (Serial.available()) {
    char incoming = Serial.read();
    delay(50);

    if (incoming == '1') { //Start motors

      if (!state) {
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

int sign( float val) {
  int s;
  if (val >= 0) {
    s = 1;
  }
  else {
    s = -1;
  }
  return s;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    End of Functions     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


