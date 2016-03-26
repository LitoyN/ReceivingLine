int ledPin = 13;
int state=0;
void setup() {
    pinMode(ledPin, OUTPUT); // pin will be used to for output
    Serial.begin(9600); // same as in your c++ script
    //digitalWrite(ledPin,HIGH);
}

void loop() {
  if (Serial.available() > 0)
  {
    state = Serial.read(); // used to read incoming data
    
    switch(state)// see what was sent to the board
    {
      case '1': // if the the one was sent
        digitalWrite(ledPin,HIGH);
        Serial.println('n');
        Serial.flush();
      break;
      case '0': // if 0 was sent
        digitalWrite(ledPin,LOW);
        Serial.println('f');
        Serial.flush();
      break;
      default:
      break; 
    }
  }
  else{
    state = 0;
  }
}
