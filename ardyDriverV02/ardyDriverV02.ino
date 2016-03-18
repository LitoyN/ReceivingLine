#include "Arduino.h"
#include <Servo.h>
#include <digitalWriteFast.h>  // library for high performance reads and writes by jrraines
                               // see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1267553811/0
                               // and http://code.google.com/p/digitalwritefast/
 
// It turns out that the regular digitalRead() calls are too slow and bring the arduino down when
// I use them in the interrupt routines while the motor runs at full speed creating more than
// 40000 encoder ticks per second per motor.
 
// encoder
#define ENCODERINTERRUPT 0
#define ENCODERPINA 2
#define ENCODERPINB 3
#define ENCODERREVERSED
const int encoderBeltIncrement = 1336;

volatile bool encoderBSet;
volatile long encoderTicks = 0;

int conveyorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int arrayIndex = 0;
int boxBin = 1;

void updateConveyorArray() {

  for (int i = 9; i >= 0; i--) {
    conveyorArray[i] = conveyorArray[i - 1];
  }
  conveyorArray[0] = boxBin;
  for(int i = 0; i < 10; i++){
    Serial.print(conveyorArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
 
void setup()
{
  Serial.begin(115200);
 
  // Left encoder
  pinMode(ENCODERPINA, INPUT);      // sets pin A as input
  digitalWrite(ENCODERPINA, LOW);  // turn on pullup resistors
  pinMode(ENCODERPINB, INPUT);      // sets pin B as input
  digitalWrite(ENCODERPINB, LOW);  // turn on pullup resistors
  attachInterrupt(ENCODERINTERRUPT, HandleLeftMotorInterruptA, RISING);
 
}
 
void loop()
{
 
  Serial.print(encoderTicks);
  Serial.print("\n");
 
  delay(250);
}
 
// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  encoderBSet = digitalReadFast(ENCODERPINB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef ENCODERREVERSED
    encoderTicks -= encoderBSet ? -1 : +1;
  #else
    encoderTicks += encoderBSet ? -1 : +1;
  #endif

  if(encoderTicks > encoderBeltIncrement){
    updateConveyorArray();
  }
}
 
