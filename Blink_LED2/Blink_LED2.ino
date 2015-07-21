#include <Event.h>
#include <Timer.h>

#define LED 3
#define PWM 9

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  Timer1.initialize(500000);
  //Timer1.pwm(PWM, 512);
  Timer1.attachInterrupt(callback);

}

void loop() {
 
}

void callback()
{
//  digitalWrite(LED, digitalRead(LED)^1);
  static bool flag = false;
  if(flag) // Turn Off
  {
    digitalWrite(LED, LOW);
  }
  else // Turn On
  {
    digitalWrite(LED, HIGH);
  }
//  digitalWrite(LED,HIGH);
//  Serial.print("On");
//  delay(1000);
//  digitalWrite(LED,LOW);
//  Serial.print("Off");
//  delay(1000);
}

