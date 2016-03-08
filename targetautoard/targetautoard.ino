#include <Servo.h>

/**
  Code adapted from Jaycon Systems tutorial code and comment from
  forum user, Amit Sommer.
  http://www.jayconsystems.com/tutorial/s7sd/
  5/14/2015
**/

const int SERVOPIN = 11;
const int SONARPIN = 9;
const int RELAYPIN = 3;
const int BUTTONPIN = 2;
const int buttonState = 0;

const int ARRAYSIZE = 9;

Servo servo1;

int onDur = 500;
int incomingVal = 1;

int rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
long pulse, inches;
int range;




void setup() {
  
  Serial.begin(9600);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(SONARPIN, INPUT);
  pinMode(BUTTONPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  servo1.attach(SERVOPIN);
  
  delay(500);
  
  servo1.write(0);
  delay(2000);
  servo1.write(155);

}


void loop() {
  
  //collect arraysize sonar measurements
  for(int i = 0; i < ARRAYSIZE; i++){								    
    pulse = pulseIn(SONARPIN,HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  isort(rangevalue,ARRAYSIZE);
  range = mode(rangevalue,ARRAYSIZE);
  
  if(Serial){
    incomingVal = Serial.read();
    if(incomingVal == 200){

      digitWrite(RELAYPIN,LOW);
      delay(500);
      digitalWrite(RELAYPIN,HIGH);
    }
    
    delay(onDur);
  }
}

void isort(int *a, int n){
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
int mode(int *x,int n){

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
