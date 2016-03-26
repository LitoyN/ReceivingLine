/* 
 * File:   ardySerial.cpp
 * Author: elliot
 * 
 * Created on March 23, 2016, 6:42 PM
 */

#include "ardySerial.h"
#include "tserial.h"

ardySerial::ardySerial() {
    Tserial *com;
    char incoming;
}

ardySerial::ardySerial(const ardySerial& orig) {
}

ardySerial::~ardySerial() {
}

char ardySerial::readData(){
    //fprintf("Hello again \n");
    incoming = com->getChar();
    return incoming;
    //return 'a';
}
void ardySerial::sendData(unsigned char data){
    com->sendChar(data);
    //fprintf("%c",data);
}

bool ardySerial::startDevice(char *port, int speed){
    com = new Tserial();
    if (com!=0)
    {
            //if(com->connect(port, speed, spNONE))
                    //fprintf("Not Connected...\n");
            //else
                    //fprintf("Connected..\n");
            return TRUE;
    }
    else
            return FALSE; 
}

void ardySerial::stopDevice(){
    com->disconnect();
    // ------------------
    //fprintf("Hello \n");
    delete com;
    com = 0;
}

