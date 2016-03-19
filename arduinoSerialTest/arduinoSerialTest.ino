const int yPin = 11;
const int rPin = 10;
const int gPin = 9;
const int brightness = 100;

int input,num;
int buff[5];
int j=-1;

void setup()
{
    Serial.begin(9600);
    pinMode(yPin, OUTPUT);
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
}
 
int calc()
{
    int num=0,x=0;
 
    for(x;x<=j;x++)
          num=num+(buff[x]-48)*pow(10,j-x);
 
    return num;
}
 

 
void loop()
{
    if(Serial.available()>0)
    {
        input=Serial.read();
 
        if(input==',')
        {
            num=calc();
            j=-1;
            Serial.println(num);
        }
        else
        {
            j++;
            buff[j]=input;
        }
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
