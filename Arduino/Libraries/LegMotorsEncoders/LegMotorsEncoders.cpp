/*
  LegMotorsEncoders.ino - Functions and pin definitions for blue box of Roboleg.
  Created by Eric Sidorov and Jonathan Spitz. (2013)
  Last edited by Israel Schallheim: 26/3/2015
*/
#include "Arduino.h"
#include "LegMotorsEncoders.h"

// Define pins for motor control
#define M1_ENABLE 22
#define M2_ENABLE 26
#define M1_IN_A 23
#define M2_IN_A 27
#define M1_IN_B 24
#define M2_IN_B 28
#define M1_PWM 2
#define M2_PWM 3

#define LINEAR_ON 38
#define LINEAR_APHASE 41

#define OE1 37
#define OE2 35
#define OE3 33
#define OE4 31
#define SEL 40
#define RST1 36
#define RST2 34
#define RST3 32
#define RST4 30

#define D0 53
#define D1 52
#define D2 51
#define D3 50
#define D4 10
#define D5 11
#define D6 12
#define D7 13

const int  LegMotorsEncoders::D[] = {D0,D1,D2,D3,D4,D5,D6,D7};
const int  LegMotorsEncoders::OE[] = {OE1,OE2,OE3,OE4} ;
const int  LegMotorsEncoders::CPR[] = {14400,14400,14400,14400};

  LegMotorsEncoders::LegMotorsEncoders(){
  }

void LegMotorsEncoders::pin_setup(boolean Initial_Linear_Position) {
  
  // Set pins modes for motors
  pinMode(M1_ENABLE,OUTPUT);
  pinMode(M2_ENABLE,OUTPUT);
  pinMode(M1_IN_A,OUTPUT);
  pinMode(M2_IN_A,OUTPUT);
  pinMode(M1_IN_B,OUTPUT);
  pinMode(M2_IN_B,OUTPUT);
  pinMode(M1_PWM,OUTPUT);
  pinMode(M2_PWM,OUTPUT);
  
  pinMode(LINEAR_ON,OUTPUT);
  pinMode(LINEAR_APHASE,OUTPUT);

  pinMode(OE1,OUTPUT);
  pinMode(OE2,OUTPUT);
  pinMode(OE3,OUTPUT);
  pinMode(OE4,OUTPUT);
  pinMode(SEL,OUTPUT);
  pinMode(RST1,OUTPUT);
  pinMode(RST2,OUTPUT);
  pinMode(RST3,OUTPUT);
  pinMode(RST4,OUTPUT);
  
  pinMode(D0,INPUT);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(D4,INPUT);
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  pinMode(D7,INPUT);

  // Set pins default value for motors
  digitalWrite(M1_ENABLE, LOW);
  digitalWrite(M2_ENABLE, LOW);
  
  digitalWrite(LINEAR_ON,HIGH);
  digitalWrite(LINEAR_APHASE,Initial_Linear_Position);
  digitalWrite(RST1,HIGH);
  digitalWrite(RST2,HIGH);
  digitalWrite(RST3,HIGH);
  digitalWrite(RST4,HIGH);
  digitalWrite(OE1,HIGH);
  digitalWrite(OE2,HIGH);
  digitalWrite(OE3,HIGH);
  digitalWrite(OE4,HIGH);
  
}

void LegMotorsEncoders::ResetEncoder(int nEncoderIndex)
{
  int nResetPin;
  switch(nEncoderIndex)
  {
    case 1:
      nResetPin = RST1;
      break;
    case 2:
      nResetPin = RST2;
      break;
    case 3:
      nResetPin = RST3;
      break;
    case 4:
      nResetPin = RST4;
      break;
  }
  
  digitalWrite(nResetPin, LOW);
  delay(50);
  digitalWrite(nResetPin, HIGH);
  delay(50);
}

float LegMotorsEncoders::ReadEncoder(int nEncoderIndex)
{
//  unsigned long T0, T1;
//  T0 = micros();
  long output = 0x0;
  float deg;
  byte h_byte = 0x0;
  byte l_byte = 0x0;
  int i = 0;
  byte val = 0;
  
  //select encoder to read
  for (i=0;i<4;i++)
  {
    if (i == nEncoderIndex - 1)
    {
      digitalWrite(this->OE[i],LOW);
    }
    else
    {
      digitalWrite(this->OE[i],HIGH);
    }
  }
  // Read high byte
  digitalWrite(SEL, LOW);

  for(i=0;i<8;i++)
  {
    val = digitalRead(this->D[i]);
    h_byte = h_byte|(val<<i);
  }
  digitalWrite(SEL, HIGH);

  for(i=0;i<8;i++)
  {
    val = digitalRead(this->D[i]);
    l_byte = l_byte|(val<<i);
  }
  // add bytes
  output = (h_byte<<8)|l_byte;
       
  // Reset inhibition on encoder buffer
  digitalWrite(this->OE[nEncoderIndex-1], HIGH);

  deg = float(output)*360.0/float(this->CPR[nEncoderIndex-1]);
  return deg;
}

void LegMotorsEncoders::M1_FWDaccel(float PWM0, float PWM1, float time) {
  float numsteps=10.0;
  float timestep=time/numsteps;
  for (int i=1; i<=numsteps; i++) {
    this->M1_FWD(PWM1+(1-(float)i/numsteps)*(PWM0-PWM1));
    delay(timestep*1000);
  }
}

void LegMotorsEncoders::M1_BWDaccel(float PWM0, float PWM1, float time) {
  float numsteps=10.0;
  float timestep=time/numsteps;
  for (int i=1; i<=numsteps; i++) {
    this->M1_BWD(PWM1+(1-(float)i/numsteps)*(PWM0-PWM1));
    delay(timestep*1000);
  }
}

void LegMotorsEncoders::M1_FWD(float PWM_VALUE) {
  // Set InB to HIGH and InA to LOW
  digitalWrite(M1_IN_A, HIGH);
  digitalWrite(M1_IN_B, LOW);
  analogWrite(M1_PWM,PWM_VALUE);
  digitalWrite(M1_ENABLE, HIGH);
}

void LegMotorsEncoders::M1_BWD(float PWM_VALUE) {
  // Set InB to LOW and InA to HIGH
  digitalWrite(M1_IN_A, LOW);
  digitalWrite(M1_IN_B, HIGH);
  analogWrite(M1_PWM,PWM_VALUE);
  digitalWrite(M1_ENABLE, HIGH);
}

void LegMotorsEncoders::M1_BRAKE() {
  // Set InA to HIGH and InB to HIGH
  digitalWrite(M1_IN_A, HIGH);
  digitalWrite(M1_IN_B, HIGH);
  //analogWrite(M1_PWM,100);
  digitalWrite(M1_ENABLE, HIGH);
}

void LegMotorsEncoders::M1_COAST() {
  digitalWrite(M1_ENABLE, LOW);
}

void LegMotorsEncoders::M2_BWDaccel(float PWM0, float PWM1, float time) {
  float numsteps=10.0;
  float timestep=time/numsteps;
  for (int i=1; i<=numsteps; i++) {
    this->M2_BWD(PWM1+(1-(float)i/numsteps)*(PWM0-PWM1));
    delay(timestep*1000);
  }
}

void LegMotorsEncoders::M2_FWD(float PWM_VALUE) {
  // Set InA to HIGH and InB to LOW
  digitalWrite(M2_IN_B, HIGH);
  digitalWrite(M2_IN_A, LOW);
  analogWrite(M2_PWM,PWM_VALUE);
  digitalWrite(M2_ENABLE, HIGH);
}

void LegMotorsEncoders::M2_BWD(float PWM_VALUE) {
  // Set InA to LOW and InB to HIGH
  digitalWrite(M2_IN_B, LOW);
  digitalWrite(M2_IN_A, HIGH);
  analogWrite(M2_PWM,PWM_VALUE);
  digitalWrite(M2_ENABLE, HIGH);
}

void LegMotorsEncoders::M2_BRAKE() {
  // Set InA to HIGH and InB to HIGH
  digitalWrite(M2_IN_A, HIGH);
  digitalWrite(M2_IN_B, HIGH);
  digitalWrite(M2_ENABLE, HIGH);
}

void LegMotorsEncoders::M2_COAST() {
  digitalWrite(M2_ENABLE, LOW);
}

void LegMotorsEncoders::LINEAR_IN() {
  digitalWrite(LINEAR_APHASE,HIGH);
}

void LegMotorsEncoders::LINEAR_OUT() {
  digitalWrite(LINEAR_APHASE,LOW);
}


