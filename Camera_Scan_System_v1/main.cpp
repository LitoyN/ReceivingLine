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
int imageNumber = 0;
int attributeCount = 300;
int testFileCounts = 30;
int totalTestFileCount = testFileCounts * CLASScount;
Mat features(1, attributeCount, CV_32F);
Mat classificationResult(1, CLASScount, CV_32F);
char *filename = new char[200];
Mat currentImage;
CvANN_MLP *nnetwork;
Mat dictionary;
FileStorage dictionaryFile("dictionary04.yml", FileStorage::READ);
string annLocation = "C:\\Users\\elliot\\Documents\\GitHub\\ReceivingLine\\cvAnn07.xml";
string labels[16] = {"A", "B", "C", "D", "E", "F", "G", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

Serial* ardyComm; 
char port[] = "COM4";
char incoming[256] ="";
int dataLen = 255;
char bin1 = '1';
char bin2 = '2';
char bin3 = '3';

void sortToBin(int index){
    cout << endl << "sorting to bin: " << index << endl;
    switch(index){
        case 0:
            ardyComm->WriteData(&bin1,1);
            break;
        case 1:
            ardyComm->WriteData(&bin2,1);
            break;
        case 2:
            ardyComm->WriteData(&bin3,1);
            break;
        case 3:
            ardyComm->WriteData(&bin1,1);
            break;
        case 4:
            ardyComm->WriteData(&bin2,1);
            break;
        case 5:
            ardyComm->WriteData(&bin3,1);
            break;
        case 6:
            ardyComm->WriteData(&bin1,1);
            break;
    }
}


void detectFeatures(){
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    vector<KeyPoint> keypoints; //hold keypoints
    BOWImgDescriptorExtractor bowExtractor(extractor, matcher);
    bowExtractor.setVocabulary(dictionary);
    Mat bowDescriptor;             
    cout << "image loaded" << endl;
    //detect feature points
    detector->detect(currentImage, keypoints);
    cout << "keypoints detected" << endl;
    if(keypoints.empty())
        cout << "keypoint vector empty" << endl;
    else
       printf("keypoint vector filled. #keypoints: %d \n",keypoints.size());
    //extract BoW (or BoF) descriptor from given image
    bowExtractor.compute(currentImage, keypoints, bowDescriptor);
    if(bowDescriptor.empty())
        cout << "descriptor empty" << endl;
    else
        cout <<"descriptor filled" << endl;
    cout << "descriptors computed" << endl;
    features.push_back(bowDescriptor);      
}

void annClassify(){
    nnetwork->predict(features.row(imageNumber), classificationResult); //ANN makes a classification for the current test image

    cout << "imageNumber: " << imageNumber << endl;
    cout << "prediction made" << endl;
    int maxIndex = 0;
    float value = 0.0f;
    float maxValue = classificationResult.at<float>(0, 0);
    //cout << "initial maxValue: " << classificationResult.at<float>(0,0) << endl;
    for (int index = 0; index < CLASScount; index++) {
        value = classificationResult.at<float>(0, index);
        cout << value << " ";
        if (value > maxValue) {
            maxValue = value;
            maxIndex = index;

        }
    }
    
    sortToBin(maxIndex);
    cout << endl;
    //cout << "end maxValue: " << maxValue << endl;
    printf("ANN Classification for Current Image: %s \n", labels[maxIndex].c_str());
}

void annLoad(){
    nnetwork = new CvANN_MLP; //creates new artificial neural network
    nnetwork->load(annLocation.c_str());
    cout << "nnework layer count: " << nnetwork->get_layer_count() << endl;
    cout << "ann loaded" << endl;
}

int main(int argc, char** argv) {

    ardyComm = new Serial(port);
    annLoad();
    dictionaryFile["vocabulary"] >> dictionary; //puts the features from the dictionary into the dictionary matrix
    dictionaryFile.release();
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
            imageNumber++;
            cam.retrieve(currentImage,CV_8UC3);
            cout << "taking picture" << endl;
            namedWindow("Current Image");
            imshow("Current Image", currentImage);
            detectFeatures();
            annClassify();
            incoming[0] = '0';
        }
        cam >> frame;
        if(frame.empty()){
            cout << "Video over" << endl;
        }
        
        imshow("Conveyor", frame);
        
    }
    
    return 0;
}

