//serial handling
//https://github.com/abdurrahmanbmf/Serial-Communication/

//encoder handling
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//sonar handling
//Lessels, Jason (2011/June/06), Allen, Bruce (2009/July/23) and Gentles, Bill (2010/Nov/12)
//http://playground.arduino.cc/Main/MaxSonar

//serialRXPin = 0
//serialTXPin = 1
const int encoderPin1 = 2; //encoder pins must be 2 & 3 (interrupt pins)
const int encoderPin2 = 3; //encoder pins must be 2 & 3 (interrupt pins)
//74hc595Pin = 4
//pin 5 is available
//motor3Pin = 6
//74hc595Pin = 7
//74hc595Pin = 8
const int sonarPin = 9;
const int relayPin = 10; 
//motor1Pin = 11
//74hc595Pin = 12
//pin 13 is on board LED but available

const int yPin = 11; //used pin 11 in led serial test program. will control motor driver board for actuator
const int rPin = 10; //used pin 10 in led serial test program. will control motor driver board for actuator
const int gPin = 9; //used 3 led in serial test program. will only have 2 actuators. this pin not needed.

const int brightness = 100;
const int encoderBeltIncrement = 80;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int conveyorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int arrayIndex = 0;
int boxBin = 1;

int rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
long pulse;
int range;

int input;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

  digitalWrite(yPin, LOW);
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);

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

void sonarCheck(){
  //collect arraysize sonar measurements
  for(int i = 0; i < ARRAYSIZE; i++){                    
    pulse = pulseIn(SONARPIN,HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  sortSonar(rangevalue,ARRAYSIZE);
  range = modeSonar(rangevalue,ARRAYSIZE);
  
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
void updateConveyorArray() {

  for (int i = 9; i >= 0; i--) {
    conveyorArray[i] = conveyorArray[i - 1];
  }
  conveyorArray[0] = boxBin;
  boxBin++;
  for(int i = 0; i < 10; i++){
    Serial.print(conveyorArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void checkSerial(){
  if(Serial.available()>0){
    input=Serial.read();

  }


  switch(input){
    case 4:
      digitalWrite(yPin, LOW);
      digitalWrite(rPin, LOW);
      digitalWrite(gPin, LOW);
      break;
    case 1:
      analogWrite(yPin, brightness);
      digitalWrite(rPin, LOW);
      digitalWrite(gPin, LOW);
      break;
    case 2:
      analogWrite(rPin, brightness);
      digitalWrite(yPin, LOW);
      digitalWrite(gPin, LOW);
      break;
    case 3:
      analogWrite(gPin, brightness);
      digitalWrite(yPin, LOW);
      digitalWrite(rPin, LOW);
      break;
  }
}



void loop(){ 
  //Do stuff here

  Serial.println(encoderValue);
  delay(1000); //just here to slow down the output, and show it will work  even during a delay
}



