/* 
 * File:   ardySerial.h
 * Author: elliot
 *
 * Created on March 23, 2016, 6:42 PM
 */

#ifndef ARDYSERIAL_H
#define	ARDYSERIAL_H

#include "tserial.h"

class ardySerial {
public:
    ardySerial();
    ardySerial(const ardySerial& orig);
    virtual ~ardySerial();
    
    char readData();
    bool startDevice(char *port,int speed);
    void stopDevice();
    void sendData(unsigned char data);
    
private:
    Tserial *com;
    char incoming;

};

#endif	/* ARDYSERIAL_H */

