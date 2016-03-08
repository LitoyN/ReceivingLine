#include <Servo.h>

const int SERVOPIN = 11;
const int SONARPIN = 9;
const int RELAYPIN = 3;
const int ARRAYSIZE = 9;

Servo servo1;

int rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
long pulse, inches;
int range;

int boxDetect[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void setup() {
  Serial.begin(9600);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(SONARPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  servo1.attach(SERVOPIN);
  digitalWrite(RELAYPIN,HIGH);
  delay(500);
  
  servo1.write(20);
  delay(1000);
  servo1.write(140);
}

void loop() {
  
  updateDetectArray();
  Serial.println("updatedetectarray");
  
  if(boxDetect[3] == 1){
  servo1.write(20);
  delay(1000);
  servo1.write(140);
  }
  Serial.println("checked for box at position 9");

  for(int i = 0; i < ARRAYSIZE; i++){								    
    pulse = pulseIn(SONARPIN,HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  
  Serial.println("pulsed 9 times, now checking mode range");
  
  isort(rangevalue,ARRAYSIZE);
  range = mode(rangevalue,ARRAYSIZE);
  Serial.print("range: ");
  Serial.println(range);
  
  if(range < 6){
    boxDetect[0] = 1;
    Serial.println("box detected");
  }
  else{
    boxDetect[0] = 0;
    Serial.println("no box");
  }
  advanceBeltOne();
  Serial.println("advancing belt");
  Serial.println("new box detect array: ");

  for(int i = 0; i<19;i++){
  Serial.print(boxDetect[i]);  
  }
  Serial.println("");
  delay(500);
}

void advanceBeltOne(){
  digitalWrite(RELAYPIN,LOW);
  delay(250);
  digitalWrite(RELAYPIN,HIGH); 
}

void updateDetectArray(){
  
  for(int i = 0; i<19; i++){
    boxDetect[18-i] = boxDetect[18-(i+1)];
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
