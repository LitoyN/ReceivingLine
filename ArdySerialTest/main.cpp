/* 
 * File:   main.cpp
 * Author: elliot
 *
 * Created on March 18, 2016, 2:52 PM
 */



#include <iostream> //require for opening and reading files
#include <fstream>  //required for opening and reading files
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "tserial.h"
#include "ardySerial.h"
#include "bot_control.h"


using namespace std;

serial comm2;
ardySerial comm;
char incoming;
char lastIncoming;
char port[] = "\\\\.\\COM11";
bool runProg = true;
int binToSort = -2;

void readAndSort(){
    
    
}

int main()
{
    
    incoming = '2';
    comm2.startDevice(port,9600);
    //comm.sendData(1);
    Sleep(2000);
    comm2.send_data('1');
    cout << "on" << endl;
    incoming = comm2.read_data();
    cout << incoming << endl;
    Sleep(2000);
    comm2.send_data('0');
    incoming = comm2.read_data();
    cout << incoming << endl;
    cout << "off" << endl;
    Sleep(2000);
    comm2.stopDevice();
    
    return 0;
    
}

