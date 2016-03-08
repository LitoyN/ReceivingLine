#include <Servo.h>

int servoPin = 6;
Servo servo1;
int post = 0;


void setup() {

  pinMode(servoPin,OUTPUT);
  servo1.attach(servoPin);
  servo1.write(0);
  delay(2000);
  
}

void loop() {

  delay(2000);
  servo1.write(170);
  delay(4000);
  servo1.write(0);
  
  
}
