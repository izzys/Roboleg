#include <TimerOne.h>
#include "LegMotorsEncoders.h"
#include "TimerThree.h"
#define SIZE 28
#define MAX_PWM 255

// Declarations
int resetVariables(int** vars);
LegMotorsEncoders LME;


// variables
int Duration[SIZE] = {200,200,200,200,200,200,400,100,100,100,100,100,100,400, 50, 50, 50, 50, 50, 50,400,600,100,600,1000,2000,1200,6000};
float Amplitude[SIZE] = {50, -50,75, -75, 100,-75,0,50, -50,75, -75, 100,-100,0,75, -75,75, -75, 75,-75,000,.015,100,-.025,-40,.055,-.02,000};
int Type[SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,000,1,0,1,0,1,1,0};
int LoopRate = 5000; 

// States
int state = 0; //initial case state==0
bool firstLoop = true;
boolean Initial_Linear_Position = 1; // Set here lonear motot inital position
unsigned long sync_time = 0;
//unsigned long lasttime = 500 * 1e6; // seconds * (micro)
bool doChange = false;

void setup() {
  
  Serial.begin(115200);
  delay(100);
  Timer3.initialize(50); //im micro secons units. (50 micro seconds == 20 Khz)
  // Get Ready to Test
  LME.pin_setup(Initial_Linear_Position); //set up the pins on the board
  
  Timer1.initialize(LoopRate); 
}

void LoopAction()
{
  // stop if Not started
  if(state != 1)
    return;
  float comm = 0;

  // Static Variables
  static short pos = 0;
  static short count_inside = 0; 
  static int totalCount = 0;
  static int last_value = 0;
  
  // Sampling Variables
  long enc1;
  long enc2;
  unsigned long T;
    
  // Reset Static variables
  if(firstLoop == true)
  {
    firstLoop = false;
    pos = 0;
    count_inside = 0;
    totalCount = 0;
    last_value = 0;
  }
  // ==================== //
  
  if(Type[pos] == 0) // Block
  {
      comm = Amplitude[pos]; 
  }
  else
  {
      comm = Amplitude[pos]*count_inside + last_value; 
  }

  if(count_inside < Duration[pos] - 1)  
  {
    count_inside++;
  }
  else
  {
    count_inside = 0;
    pos++;
    last_value = comm; // last input
  }
  
  
  // Sending Commands:
  enc1 =  LME.ReadEncoder(1);
  enc2 = -LME.ReadEncoder(2);
  T = micros() - sync_time;
  if(IsEmergencyStop(enc1, enc2))
  {
    noInterrupts(); // {
    state = 99;
    Serial.println("emergency stop");
    interrupts();  // }
  }
  else
  {
      SendCommand(comm);
      PrintSamples(enc1, enc2, comm, T);
  }
  if(pos == SIZE) // || T >= lasttime
  {
    noInterrupts();
    Timer1.detachInterrupt();
    interrupts();
    Serial.println("done.");
  }
}

bool IsEmergencyStop(long enc1, long enc2)
{
  //If the leg gets to high-->emergency stop
//  return ((enc2)>3000 || (enc2<-3000) || abs(enc1-enc2)>3600);     // 3600 = 90*; 3000 = 75*
    return ((enc2)>1200 || (enc2<-1800) || abs(enc1-enc2)>3000);     // 3600 = 90*; 3000 = 75*
}

void SendCommand(float comm)
{ 
  float comm_output = 0;
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
}


void PrintSamples(long enc1, long enc2, short comm, unsigned long T)
{ 
      Serial.print(enc1);
      Serial.print("   ");
      Serial.print(enc2);
      Serial.print("   ");
      Serial.print(comm);
      Serial.print("   ");
      Serial.println(T);
}

/*
//template<typename T>
void DebugPrint(var)
{
  Serial.print("===========");
  Serial.println(var);
}*/


void loop() {
  if(doChange)
  {

    if(state == 1)
    {
          // Reset Encoders
          LME.ResetEncoder(1); //reset the encoders to read zero from their initial position
          LME.ResetEncoder(2);
          LME.ResetEncoder(3);
          LME.ResetEncoder(4);
          
          sync_time = micros();
          firstLoop = true;
          Serial.println("starting...");
          Timer1.attachInterrupt(LoopAction);
    }
    else //if(state == 0)
    {
          Serial.println("stop.");
          Timer1.detachInterrupt();
          // Stop motors
          LME.M1_COAST();
          LME.M2_COAST();
    }
    doChange = false;
  }
}

void serialEvent(){ //Receive Serial data from MATLAB
    while (Serial.available())
    {
      char incoming = (char)Serial.read();
      delay(50);
      if(incoming == '0')
      {
        if(state != 0)
          state = 0;
          doChange = true;
      }
      else if(incoming == '1')
      {       
        if(state != 1)
          state = 1;
          doChange = true;
      }
    }
}
 




