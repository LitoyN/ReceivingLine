int ledPin = 13;
int state=0;
void setup() {
    pinMode(ledPin, OUTPUT); // pin will be used to for output
    Serial.begin(9600); // same as in your c++ script
    //digitalWrite(ledPin,HIGH);
}

void loop() {

  int testInt;
  if (Serial.available() > 0)
  {Serial.flush();
    state = Serial.read(); // used to read incoming data
    testInt = state;

    switch(state)// see what was sent to the board
    {
      case '1': // if the the one was sent
        Serial.println('n');
        Serial.flush();
        digitalWrite(ledPin,HIGH);

      break;
      case '2': // if 0 was sent
        Serial.println('f');
        Serial.flush();
        digitalWrite(ledPin,LOW);

      break;
      case '3':
        Serial.println(state);
        Serial.flush();
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        Serial.println('b');
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
