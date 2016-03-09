/* 
 * File:   main.cpp
 * Author: Archeress1966
 *
 * Created on November 5, 2014, 5:52 AM
 * Based on code found here:  http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
 */
#include <iostream> //require for opening and reading files
#include <fstream>  //required for opening and reading files
#include <cstdlib>  //C++ std library
#include "opencv2/highgui/highgui.hpp" //must have for core openCV matrices
#include "opencv2/ml/ml.hpp"      //must have for machine learning
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;
#define CLASScount 16

int main(int argc, char** argv) {

    string labels[CLASScount] = {"A", "B", "C", "D", "E", "F", "G",
    "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    //the number of words
    int dictionarySize = 300;
    string toSaveDictionary = "dictionary04.yml";
    string filenameTemplates[CLASScount] = {
        "TemplateDir\\Letters\\A\\A%03d.jpg", //%.03d is like printf
        "TemplateDir\\Letters\\B\\B%03d.jpg",
        "TemplateDir\\Letters\\C\\C%03d.jpg",
        "TemplateDir\\Letters\\D\\D%03d.jpg",
        "TemplateDir\\Letters\\E\\E%03d.jpg",
        "TemplateDir\\Letters\\F\\F%03d.jpg",
        "TemplateDir\\Letters\\G\\G%03d.jpg",
        "TemplateDir\\Nums\\1\\1%03d.jpg",
        "TemplateDir\\Nums\\2\\2%03d.jpg",
        "TemplateDir\\Nums\\3\\3%03d.jpg",
        "TemplateDir\\Nums\\4\\4%03d.jpg",
        "TemplateDir\\Nums\\5\\5%03d.jpg",
        "TemplateDir\\Nums\\6\\6%03d.jpg",
        "TemplateDir\\Nums\\7\\7%03d.jpg",
        "TemplateDir\\Nums\\8\\8%03d.jpg",
        "TemplateDir\\Nums\\9\\9%03d.jpg"
    };
    int fileCounts = 1;
    //int fileCounts[] = {50, 50, 50, 50, 50};  //using 40 training images

    char *filename = new char[100];
    Mat currentImage;

    vector<KeyPoint> keypoints; //hold keypoints   
    Mat descriptor; //hold descriptors

    Mat features;
    //The SIFT feature extractor and descriptor
    SiftDescriptorExtractor detector;  //SIFT descriptor will give 128 values. descriptor represents region around the keypoint which is 16 x 16. 8 values representing gradient direction in each cell of 4 x 4 subgrid.

    for (int classification = 0; classification < CLASScount; classification++) {
        for (int imageNumber = 1; imageNumber <= fileCounts; imageNumber++) {
            sprintf(filename, filenameTemplates[classification].c_str(), imageNumber);
            cout << "file name = "  << filename << endl;
            currentImage = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale                
            cout << "image loaded" << endl;
            //detect feature points
            detector.detect(currentImage, keypoints);
            cout << "keypoints detected" << endl;
            //compute the descriptors for each keypoint
            detector.compute(currentImage, keypoints, descriptor);
            features.push_back(descriptor); //place descriptors in matrix          
        }//for each image
    }//for each classification

    //Construct BOWKMeansTrainer

    //define Term Criteria
    TermCriteria criteriaForLooping(CV_TERMCRIT_ITER, 100, 0.001);
 
    int retries = 1;  
    int flags = KMEANS_PP_CENTERS;
    
    cout << "BOW Trainer Started" << endl;
    BOWKMeansTrainer bowTrainer(dictionarySize, criteriaForLooping, retries, flags);  //BOW means bag of words. This is the constructor for the trainer
    
    //cluster the feature vectors
    cout << "BOW Clustering Started" << endl;
    Mat dictionary = bowTrainer.cluster(features);  ///this is the actual BOW method that you constructed above
    //store the vocabulary
    FileStorage siftDictionary(toSaveDictionary.c_str(), FileStorage::WRITE);  //writes the dictionary file to be used in another program
    siftDictionary << "vocabulary" << dictionary;
    siftDictionary.release();  //close the file when you are done with it


}//main
