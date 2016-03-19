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
#include "bot_control.h"


using namespace std;


serial comm;
char data;

int main()
{
    data = '4';
    comm.startDevice("\\\\.\\COM11",9600);
    
    for(int i = 1; i <5; i++){
        comm.send_data((char)i);
        Sleep(2000);
    }
    
    comm.stopDevice();
    
    return 0;
    
}

