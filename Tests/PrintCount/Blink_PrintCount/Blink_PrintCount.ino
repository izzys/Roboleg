#include <TimerOne.h>

#define LED 3
#define PWM_PORT 9

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  Timer1.initialize(500000); // every half a second
  //Timer1.pwm(PWM_PORT, 1024); // 1024 equals to 100%; 512 equals to 50% of the duty cycle
  Timer1.attachInterrupt(callback);
}

volatile unsigned long blinkCount = 0; // use volatile for shared variables

void loop() {
  //// ALWAYS TURN ON
  //analogWrite(PWM_PORT, 255); // The difference between using analogWrite and Timer, is that on analogWrite the period is already default.
  
  unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  blinkCopy = blinkCount;
  interrupts();

  Serial.print("blinkCount = ");
  Serial.println(blinkCopy);
  delay(1000);
}

void callback()
{
  blinkCount += 1;
  static bool flag = false;
  if(flag) // Turn Off
  {
    digitalWrite(LED, LOW);
  }
  else // Turn On
  {
    digitalWrite(LED, HIGH);
  }
  flag = !flag;
}

