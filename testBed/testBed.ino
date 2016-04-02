//serial handling
//https://github.com/abdurrahmanbmf/Serial-Communication/

//encoder handling
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//sonar handling
//Lessels, Jason (2011/June/06), Allen, Bruce (2009/July/23) and Gentles, Bill (2010/Nov/12)
//http://playground.arduino.cc/Main/MaxSonar

#include <AFMotor.h>

//pin constants
//serialRXPin = 0
//serialTXPin = 1
const int encoderPin1 = 2; //encoder pins must be 2 & 3 (interrupt pins)
const int encoderPin2 = 3; //encoder pins must be 2 & 3 (interrupt pins)
//74hc595Pin = 4
const int buttonPin = 5;
//motor3Pin = 6
//74hc595Pin = 7
//74hc595Pin = 8
const int sonarPin = 9;
const int relayPin = 10; 
//motor1Pin = 11
//74hc595Pin = 12
//pin 13 is on board LED but available

//other constants
const int holdBin = -1;
const int emptyBin = 0;
const int conveyorArraySize = 10;

const int bin1 = 4; //index array of bin1 location
const int bin2 = 7; //index array of bin2 location
const int bin3 = 10; //index array of bin3 location
const int encoderBeltIncrement = 20;
const long cycleTime = 250;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;


//system mgmt
int preCycleTime;
int actualCycleTime;
int buttonState = 0;
volatile int incrementCounter = 0;

//conveyor mgmt
//int conveyorArraySize = 10;
int conveyorArray[] = {1, 2, 3, 4, 0, 0, 0, 0, 0, 0};
int arrayIndex = 0;
int currentBin;
bool newBox = true;

//conveyor driver
AF_DCMotor bin1Actuator(1, MOTOR12_1KHZ);
AF_DCMotor bin2Actuator(3, MOTOR12_1KHZ);
int actuatorSpeed = 100; //range from 0 to 255

//sonar
int sonarArraySize = 5;
int rangevalue[] = {0, 0, 0, 0, 0};
long pulse;
int range;
int minRange = 10;

//serial
char input;

const uint8_t LATCHSTATE = 3;
int8_t oldState = 3;
const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
 -1,  0,  0,  1,
  0,  1, -1,  0};
  
int positionInternal = 0;
int positionExternal = 0;
int oldPositionExternal = 0;


void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  pinMode(sonarPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  digitalWrite(relayPin, LOW);

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(digitalPinToInterrupt(3), tick, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(2), tick, CHANGE);

  bin1Actuator.setSpeed(actuatorSpeed);
  bin2Actuator.setSpeed(actuatorSpeed);


}

void tick(){
  int sigA = digitalRead(3);
  int sigB = digitalRead(2);
  int8_t thisState = sigA | (sigB << 1);
  if (oldState != thisState) {
    positionInternal += KNOBDIR[thisState | (oldState<<2)];
    if (thisState == LATCHSTATE)
      positionExternal = positionInternal >> 2;   
    oldState = thisState;
    if(positionExternal >= encoderBeltIncrement){
      positionExternal = 0;
      incrementCounter++;
    }
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  Serial.println(encoderValue);//**********************
  lastEncoded = encoded; //store this value for next time
  if(encoderValue >= encoderBeltIncrement){
    encoderValue = 0;
    incrementCounter++;
  }
}

void sonarCheck(){
  //collect arraysize sonar measurements
  for(int i = 0; i < sonarArraySize; i++){                    
    pulse = pulseIn(sonarPin,HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  sortSonar(rangevalue,sonarArraySize);
  range = modeSonar(rangevalue,sonarArraySize);
  if(range <= minRange && newBox){
    addToConveyorArray(holdBin);
    currentBin = readAndSort();
    addToConveyorArray(currentBin);
    newBox = false;
  }
  else if(range > minRange){
    newBox = true;
    
  }
}

void sortSonar(int *a, int n){
// *a is an array pointer function
  for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

//Mode function, returning the mode or median.
int modeSonar(int *x,int n){

  int i = 0;
  int count = 0;
  int maxCount = 0;
  int mode = 0;
  int bimodal;
  int prevCount = 0;
  while(i<(n-1)){
    prevCount=count;
    count=0;
    while(x[i]==x[i+1]){
      count++;
      i++;
    }
    if(count>prevCount&count>maxCount){
      mode=x[i];
      maxCount=count;
      bimodal=0;
    }
    if(count==0){
      i++;
    }
    if(count==maxCount){//If the dataset has 2 or more modes.
      bimodal=1;
    }
    if(mode==0||bimodal==1){//Return the median if there is no mode.
      mode=x[(n/2)];
    }
    return mode;
  }
}

int readAndSort(){
  int sortToBin = 0;
  bool serialReceived = false;
  Serial.println('1');
  while(!serialReceived){
    sortToBin = checkSerial();
    if(sortToBin > -2){
      serialReceived = true;
    }
  }

  return sortToBin;
}

void addToConveyorArray(int value) {

  if(value == holdBin){
    conveyorArray[0] = value;
  }
  else{
    for (int i = conveyorArraySize; i >= 0; i--) {
      if(conveyorArray[i] == holdBin){
        conveyorArray[i] = value;
        i = 0;
      }
    }
  }
  /**for(int i = 0; i < 10; i++){
    Serial.print(conveyorArray[i]);
    Serial.print(" ");
  }
  Serial.println("");*/
}

void updateConveyorArray() {
Serial.println("updating array");
  for (int i = conveyorArraySize; i > 0; i--) {
    conveyorArray[i] = conveyorArray[i - 1];
    if(conveyorArray[i] == i){
      pushBox(i);
    }
  }
  conveyorArray[0] = emptyBin;
  for(int i = 0; i < 10; i++){
    Serial.print(conveyorArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void microDelay(int x){
  for(int i = 0; i <= x; i++){
    delayMicroseconds(1000);
  }
}

void pushBox(int binArrayIndex){
  int actuatorTime = 200;
  switch(binArrayIndex){
    case bin1:
    //Serial.println("pushing bin1");
      bin1Actuator.run(FORWARD);
      microDelay(actuatorTime);
      bin1Actuator.run(BACKWARD);
      microDelay(actuatorTime);
      bin1Actuator.run(RELEASE);
      break;
    case bin2:
    //Serial.println("pushing bin2");
      bin2Actuator.run(FORWARD);
      microDelay(actuatorTime);
      bin2Actuator.run(BACKWARD);
      microDelay(actuatorTime);
      bin2Actuator.run(RELEASE);
      break;
  }
}

void loopUpdateArray(){
  for (int i = conveyorArraySize; i > 0; i--) {
    conveyorArray[i] = conveyorArray[i - 1];
    if(conveyorArray[i] == i){
      pushBox(i);
    }
  }
}

int checkSerial(){
  
  if(Serial.available()>0){
    input=Serial.read();
  }
  
  switch(input){
    case '1':
      return bin1;
      break;
    case '2':
      return bin2;
      break;
    case '3':
      return bin3;
      break;
  }
  return -2;
}

void checkButton(){
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH){
    digitalWrite(relayPin,HIGH);
  }
  else{
    digitalWrite(relayPin,LOW);
  }
}

void loop(){
  delay(2000);
  pushBox(bin1);
  delay(2000);
  /**actualCycleTime = millis() - preCycleTime;
  if(actualCycleTime < cycleTime){
    delay(cycleTime - actualCycleTime);
  }*/

}



