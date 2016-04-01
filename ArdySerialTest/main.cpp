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

#include "SerialClass.h"


using namespace std;

Serial* ardyComm;
//serial comm2;
char incoming[256] = "";
char bin1 = '1';
char bin2 = '2';
char bin3 = '3';
int dataLen = 255;
char port[] = "\\\\.\\COM11";
bool runProg = true;
int binToSort = -2;

void readAndSort(){
    
    
}

int main()
{
 
    ardyComm = new Serial("COM4");
    //incoming = "2";
    if(ardyComm->IsConnected()){
        cout << "connected" << endl;

    Sleep(1000);
    
    ardyComm->WriteData(&bin1,1);
    Sleep(10);
    ardyComm->ReadData(incoming,dataLen);
    if(*incoming == 'n')
        cout << "read the incoming data as == 'n'" << endl;
    else
        cout << "did not read incoming data correctly as 'n'" << endl;
    cout << "on" << endl;
    cout << "received: " << incoming << endl;
    
    Sleep(2000);
    
    ardyComm->WriteData(&bin2,1);
    Sleep(10);
    ardyComm->ReadData(incoming,dataLen);
    if(*incoming == 'f')
        cout << "read the incoming data as == 'f'" << endl;
    else
        cout << "did not read incoming data correctly as 'f'" << endl;
    cout << "off" << endl;
    cout << "received: " << incoming << endl;

    Sleep(2000);
    
    ardyComm->WriteData(&bin3,1);
    Sleep(10);
    ardyComm->ReadData(incoming,dataLen);
    if(*incoming == 'b')
        cout << "read the incoming data as == 'b'" << endl;
    else
        cout << "did not read incoming data correctly as 'b'" << endl;
    cout << *incoming << endl;
    cout << "blink" << endl;
    cout << "received: " << incoming << endl;


    Sleep(2000);

    }
    else
        cout << "could not open com" << endl;
    //comm2.stopDevice();
    
    return 0;
    
}

