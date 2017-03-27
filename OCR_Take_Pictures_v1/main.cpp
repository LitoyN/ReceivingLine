/* 
 * File:   main.cpp
 * Author: elliot
 *
 * Created on March 10, 2016, 6:38 PM
 */

#include <iostream>
#include <fstream>  
#include <cstdlib>  
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/ml/ml.hpp"
#include "SerialClass.h"



using namespace cv;
using namespace std;
#define CLASScount 7

//CV variables
Mat frame_hsv, frame;
int attributeCount = 300;
int imageNumber;
Mat features(1, attributeCount, CV_32F);
Mat classificationResult(1, CLASScount, CV_32F);
char *filename = new char[200];
Mat currentImage;
string fileDirName = "C:\\Users\\elliot\\Google Drive\\UNCA 15-16\\Receiving Line\\Template2\\A\\A%03d.jpg";


Serial* ardyComm; 
char port[] = "COM5";
char incoming[256] ="";
int dataLen = 255;
char bin1 = '1';
char bin2 = '2';
char bin3 = '3';
char binFlush = '0';

int main(int argc, char** argv) {

    ardyComm = new Serial(port);
    imageNumber = 0;

    VideoCapture cam(1);

    
    if(!cam.isOpened()){
        cout << "Camera capture start unsuccessful" << endl;
        return -1;
    }
    
    namedWindow("Conveyor");
    
    while(char(waitKey(1)) != 'q' && cam.isOpened()){
        
        ardyComm->ReadData(incoming, dataLen);
        if(*incoming == '1'){
            cout << "incoming == '1'" << endl;

            cam.retrieve(currentImage,CV_8UC3);
            cout << "taking picture" << endl;
            namedWindow("Current Image");
            imshow("Current Image", currentImage);
            sprintf(filename, fileDirName.c_str(), imageNumber);
            cout << "file name = " << filename << endl;
            imwrite(filename, currentImage);
            incoming[0] = '0';
            imageNumber++;
        }
        cam >> frame;
        if(frame.empty()){
            cout << "Video over" << endl;
        }
        
        imshow("Conveyor", frame);
        
    }
    
    return 0;
}

