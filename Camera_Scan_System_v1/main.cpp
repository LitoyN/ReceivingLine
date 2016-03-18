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
#define CLASScount 7

Mat frame_hsv, frame;
bool selected = false;
int imageNumber = 0;
int attributeCount = 300;
int testFileCounts = 30;
int totalTestFileCount = testFileCounts * CLASScount; //total number of test files
Mat features(1, attributeCount, CV_32F);
//matrix to hold the test labels.
Mat classificationResult(1, CLASScount, CV_32F);
char *filename = new char[200];
Mat currentImage;
Mat dictionary;
FileStorage dictionaryFile("dictionary04.yml", FileStorage::READ); //loads the feature dictionary that was created in an earlier step

string annLocation = "C:\\Users\\elliot\\Documents\\GitHub\\ReceivingLine\\cvAnn07.xml";
string labels[16] = {"A", "B", "C", "D", "E", "F", "G", "1", "2", "3", "4", "5", "6", "7", "8", "9"};





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
    CvANN_MLP *nnetwork = new CvANN_MLP; //creates new artificial neural network
    nnetwork->load(annLocation.c_str());
    cout << "nnework layer count: " << nnetwork->get_layer_count() << endl;
    cout << "ann loaded" << endl;

    nnetwork->predict(features.row(imageNumber), classificationResult); //ANN makes a classification for the current test image

    
    cout << "prediction made" << endl;
    int maxIndex = 0;
    float value = 0.0f;
    float maxValue = classificationResult.at<float>(0, 0);
    cout << "initial maxValue: " << classificationResult.at<float>(0,0) << endl;
    for (int index = 0; index < CLASScount; index++) {
        value = classificationResult.at<float>(0, index);
        cout << value << " ";
        if (value > maxValue) {
            maxValue = value;
            maxIndex = index;

        }
    }
    cout << endl;
    cout << "end maxValue: " << maxValue << endl;
    printf("ANN Classification for Current Image: %s \n", labels[maxIndex].c_str());
}

int main(int argc, char** argv) {

    dictionaryFile["vocabulary"] >> dictionary; //puts the features from the dictionary into the dictionary matrix
    dictionaryFile.release();
    VideoCapture cam(1);
    
    if(!cam.isOpened()){
        cout << "Camera capture start unsuccessful" << endl;
        return -1;
    }
    
    namedWindow("Conveyor");
    
    while(char(waitKey(1)) != 'q' && cam.isOpened()){
        
        if(waitKey(1) == 'p'){
            imageNumber++;
            cam.retrieve(currentImage,CV_8UC3);
            cout << "taking picture" << endl;
            namedWindow("Current Image");
            imshow("Current Image", currentImage);
            detectFeatures();
            annClassify();            
        }
        cam >> frame;
        if(frame.empty()){
            cout << "Video over" << endl;
        }
        
        imshow("Conveyor", frame);
        
    }
    
    return 0;
}

