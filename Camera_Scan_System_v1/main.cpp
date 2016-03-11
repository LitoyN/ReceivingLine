/* 
 * File:   main.cpp
 * Author: elliot
 *
 * Created on March 10, 2016, 6:38 PM
 */

#include <cstdlib>
#include <iostream> //require for opening and reading files
#include <fstream>  //required for opening and reading files
#include <cstdlib>  //C++ std library
#include "opencv2/highgui/highgui.hpp" //must have for core openCV matrices
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/ml/ml.hpp"      //must have for machine learning

using namespace cv;
using namespace std;

Mat frame_hsv, frame;
bool selected = false;

int main(int argc, char** argv) {

    VideoCapture cam(1);
    
    if(!cam.isOpened()){
        cout << "Camera capture start unsuccessful" << endl;
        return -1;
    }
    
    namedWindow("Conveyor");
    //namedWindow("Features");
    
    
    while(char(waitKey(1)) != 'q' && cam.isOpened()){
        
        if(waitKey(1) == 'p'){
            Mat currentImage;
            cam.retrieve(currentImage,CV_8UC3);
            namedWindow("Current Image");
            imshow("Current Image", currentImage);
            
        }
        cam >> frame;
        if(frame.empty()){
            cout << "Video over" << endl;
        }
        //cvtColor(frame, frame_hsv, CV_BGR2HSV);
        
        imshow("Conveyor", frame);
        
    }
    
    return 0;
}

