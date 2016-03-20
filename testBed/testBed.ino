#include <AFMotor.h>

const int encoderPin1 = 2; //encoder pins must be 2 & 3 (interrupt pins)
const int encoderPin2 = 3; 

const int emptyBin = 0;
int conveyorArraySize = 10;
int conveyorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int count = 1;
const int encoderBeltIncrement = 80;

//conveyor driver
AF_DCMotor bin1Actuator(1, MOTOR12_1KHZ);
AF_DCMotor bin2Actuator(3, MOTOR12_1KHZ);
int actuatorSpeed = 100; //range from 0 to 255
const int bin1 = 5; //index array of bin1 location
const int bin2 = 4; //index array of bin2 location
const int bin3 = 10; //index array of bin3 location

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

void setup() {

  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH);

    attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

    bin1Actuator.setSpeed(actuatorSpeed);
  bin2Actuator.setSpeed(actuatorSpeed);
}
void pushBox(int binArrayIndex){
  switch(binArrayIndex){
    case bin1:
    Serial.println("pushing bin1");
      bin1Actuator.run(FORWARD);
      delay(250);
      bin1Actuator.run(BACKWARD);
      delay(250);
      bin2Actuator.run(RELEASE);
      break;
    case bin2:
    Serial.println("pushing bin2");
      bin2Actuator.run(FORWARD);
      delay(500);
      bin2Actuator.run(BACKWARD);
      delay(500);
      bin2Actuator.run(RELEASE);
      break;
  }
}
void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
  if(encoderValue >= encoderBeltIncrement){
    encoderValue = 0;
    updateConveyorArray();
  }
  
}

void updateConveyorArray() {

  for (int i = conveyorArraySize; i >= 0; i--) {
    conveyorArray[i] = conveyorArray[i - 1];
    if(conveyorArray[i] == i){
      Serial.println("Pushing Box");
    }
  }
  conveyorArray[0] = count;
  count++;
  if(count > 9)
  count = 1;
  for(int i = 0; i < 10; i++){
    Serial.print(conveyorArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
void loop() {

bin2Actuator.run(FORWARD);
delay(250);
bin2Actuator.run(BACKWARD);
delay(250);
bin2Actuator.run(RELEASE);
delay(1000);
}
