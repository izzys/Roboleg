#include <TimerOne.h>

#define LED 3
#define PWM_PORT 9

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
 
 
}

void loop() {
  if (Serial.available())
  {
    char ser = Serial.read();
    if (ser == 'y')
    {
      digitalWrite(LED, HIGH);
    }
    if (ser == 'g')
    {
      digitalWrite(LED, LOW);
    }
  
  } 
}


