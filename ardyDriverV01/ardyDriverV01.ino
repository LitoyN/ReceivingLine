
#define ENCODER_PIN_1 2
#define ENCODER_PIN_2 3
const int moveIncrementTime = 250;
const int encoderBeltIncrement = 1336;

volatile int encoderPos = 0;
volatile boolean PastA = 0;
volatile boolean PastB = 0;

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

/**void interruptEncoder() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(encoderPin2) == LOW) {
      encoderCount++;
    }
    if (encoderCount > conveyorMoveEncoderCount) {
      encoderCount = 0;
      updateConveyorArray();
    }
    lastInterruptTime = interruptTime;
  }
}*/

void doEncoder(){
  if(digitalRead(ENCODER_PIN_1) == digitalRead(ENCODER_PIN_2)){
    encoderPos++;
    Serial.println(encoderPos);
  }
  if(encoderPos > encoderBeltIncrement){
    encoderPos = 0;
    boxBin++;
    updateConveyorArray();
  }
}

void doEncoderA()
{
     PastB ? encoderPos--:  encoderPos++;
     Serial.println(encoderPos);
     if(encoderPos > encoderBeltIncrement){
      encoderPos = 0;
      boxBin++;
      updateConveyorArray();
     }
     if(encoderPos < 0){
      encoderPos = 0;
     }
}

void doEncoderB()
{
     PastB = !PastB; 
}

void setup() {
  Serial.begin(9600);

  pinMode(ENCODER_PIN_1, INPUT);
  pinMode(ENCODER_PIN_2, INPUT);
  digitalWrite(ENCODER_PIN_1, HIGH);
  digitalWrite(ENCODER_PIN_2, HIGH);
  PastA = (boolean)digitalRead(ENCODER_PIN_1);
  PastB = (boolean)digitalRead(ENCODER_PIN_2);
  attachInterrupt(0, doEncoder, CHANGE);
  //attachInterrupt(1, doEncoderB, CHANGE);
  updateConveyorArray();
}

void loop() {

  

}
