int relayPin = 3;
int buttonPin = 2;
int buttonState = 0;

void setup() {

  pinMode(relayPin,OUTPUT);
  
  pinMode(buttonPin,INPUT);

}

void loop() {

  buttonState = digitalRead(buttonPin);

 
  if(buttonState == HIGH){
    digitalWrite(relayPin,LOW);
    delay(1000);
    digitalWrite(relayPin,HIGH);
  }
  
  //delay(500);
    
}
