/* 
 * File:   UseDictionary.cpp
 * Author: Archeress1966
 * 
 * This file demonstrates how to create feature/response vectors from training set and dictionary of words
 * A modification of code found here:
 * http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
 * 
 * Created on November 5, 2014, 10:00 PM
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
#define TESTCLASScount 7

int attributeCount = 300; //dictionary size
int trainFileCounts = 70;    
int testFileCounts = 30;
int totalTrainFileCount = trainFileCounts * CLASScount; //total number of training files
int totalTestFileCount = testFileCounts * TESTCLASScount; //total number of test files
string dictionaryFileName = "dictionary04.yml";
string toSaveANNFile = "C:\\Users\\elliot\\Documents\\GitHub\\ReceivingLine\\cvAnn08.xml";

int main(int argc, char** argv) {
    string labels[16] = {"A", "B", "C", "D", "E", "F", "G", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    string trainDirPath = "C:\\Users\\elliot\\Documents\\GitHub\\ReceivingLine\\OCR_Make_Sift_Dictionary_v1\\Crop\\TrainingDir\\";
    string testDirPath = "C:\\Users\\elliot\\Documents\\GitHub\\ReceivingLine\\OCR_Make_Sift_Dictionary_v1\\Crop\\TestDir\\";
    string filenameTemplates[16] = {
        "Letters\\A\\A%03d.jpg", //%.03d is like printf
        "Letters\\B\\B%03d.jpg",
        "Letters\\C\\C%03d.jpg",
        "Letters\\D\\D%03d.jpg",
        "Letters\\E\\E%03d.jpg",
        "Letters\\F\\F%03d.jpg",
        "Letters\\G\\G%03d.jpg",
        "Nums\\1\\1%03d.jpg",
        "Nums\\2\\2%03d.jpg",
        "Nums\\3\\3%03d.jpg",
        "Nums\\4\\4%03d.jpg",
        "Nums\\5\\5%03d.jpg",
        "Nums\\6\\6%03d.jpg",
        "Nums\\7\\7%03d.jpg",
        "Nums\\8\\8%03d.jpg",
        "Nums\\9\\9%03d.jpg"
    };
    
    Mat classificationResult(1, CLASScount, CV_32F);
    char *filename = new char[200];
    Mat currentImage;


    //matrix to hold the training sample
    Mat trainFeatures(0, attributeCount, CV_32F);
    //matrix to hold the labels of each taining sample
    Mat trainResponses(totalTrainFileCount, CLASScount, CV_32F, float(0.0));
    //matric to hold the test samples
    Mat testFeatures(0, attributeCount, CV_32F);
    //matrix to hold the test labels.
    Mat testResponses(totalTestFileCount, TESTCLASScount, CV_32F, float(0.0));

    Mat dictionary;
    FileStorage dictionaryFile(dictionaryFileName, FileStorage::READ);
    dictionaryFile["vocabulary"] >> dictionary;
    dictionaryFile.release();

    vector<KeyPoint> keypoints; //hold keypoints  
    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //You can change the type of feature extracter here
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);

    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor, matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);

    int realImageNumber = -1;
    for (int classification = 0; classification < CLASScount; classification++) {
        for (int imageNumber = 1; imageNumber <= trainFileCounts; imageNumber++) {
            realImageNumber++;
            string tempName = trainDirPath + filenameTemplates[classification];
            sprintf(filename, tempName.c_str(), imageNumber);
            cout << "file name = " << filename << endl;
            currentImage = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale                
            cout << "image read" << endl;
            detector->detect(currentImage, keypoints); //detect feature points
            cout << "keypoints detected" << endl;
            Mat bowDescriptor2; //To store the BoW (or BoF) representation of the image
            //extract BoW (or BoF) descriptor from given image
            bowDE.compute(currentImage, keypoints, bowDescriptor2); //compute the descriptors for each keypoint
            
            trainFeatures.push_back(bowDescriptor2); //place descriptors in matrix    
            trainResponses.at<float>(realImageNumber, classification) = 1.0;
        }//for each test image
    }//for each test classification
    cout << "zaboomafoo" << endl;
    realImageNumber = -1;
    for (int classification = 0; classification < TESTCLASScount; classification++) {
        for (int imageNumber = trainFileCounts + 1; imageNumber < trainFileCounts+1+testFileCounts; imageNumber++) {
            cout << "image number: " << imageNumber << endl;
            realImageNumber++;
            cout << "real image number: " << realImageNumber << endl;
            //*****Change to testDirPath*****
            string tempName = trainDirPath + filenameTemplates[classification];
            sprintf(filename, tempName.c_str(), imageNumber);
            cout << "file name = " << filename << endl;
            currentImage = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale                
            cout << "image loaded" << endl;
            //detect feature points
            detector->detect(currentImage, keypoints);
            cout << "keypoints detected" << endl;
            //compute the descriptors for each keypoint
            //To store the BoW (or BoF) representation of the image
            Mat bowDescriptor2;
            //extract BoW (or BoF) descriptor from given image
            bowDE.compute(currentImage, keypoints, bowDescriptor2);
            cout << "descriptors computed" << endl;
            testFeatures.push_back(bowDescriptor2); //place descriptors in matrix    
            cout << "rows in testFeatures: " << testFeatures.rows << endl;
            testResponses.at<float>(realImageNumber, classification) = 1.0;

            //debugging check that responses array is being built correctly
            for (int i = 0; i < TESTCLASScount; i++)
                printf("%1.2f ", testResponses.at<float>(realImageNumber, i));
            cout << endl;
        }//for each test image
    }//for each test classification
    cout << "feature cols = " << trainFeatures.cols << endl;
    cout << "feature rows = " << trainFeatures.rows << endl;
    cout << "responses cols = " << trainResponses.cols << endl;
    cout << "responses rows = " << trainResponses.rows << endl;
    cout << "feature2 cols = " << testFeatures.cols << endl;
    cout << "feature2 rows = " << testFeatures.rows << endl;
    cout << "responses2 cols = " << testResponses.cols << endl;
    cout << "responses2 rows = " << testResponses.rows << endl;
    // define the structure for the neural network (MLP)
    // The neural network has 3 layers.
    // - one input node per attribute in a sample so 256 input nodes
    // - 16 hidden nodes
    // - 10 output node, one for each class.

    cv::Mat layers(3, 1, CV_32S);
    layers.at<int>(0, 0) = attributeCount; //input layer
    layers.at<int>(1, 0) = 80; //hidden layer
    layers.at<int>(2, 0) = CLASScount; //output layer

    cout << "building ann" << endl;
    //create the neural network.
    //for more details check http://docs.opencv.org/modules/ml/doc/neural_networks.html
    CvANN_MLP nnetwork(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

    cout << "ann built" << endl;
    CvANN_MLP_TrainParams params(

            // terminate the training after either 1000
            // iterations or a very small change in the
            // network wieghts below the specified value
            cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.0001),
            // use backpropogation for training
            CvANN_MLP_TrainParams::BACKPROP,
            // co-efficents for backpropogation training
            // recommended values taken from http://docs.opencv.org/modules/ml/doc/neural_networks.html#cvann-mlp-trainparams
            0.1,
            0.1);

    cout << "training parameters set" << endl;
    // train the neural network (using training data)

    printf("\nUsing training dataset\n");
    int iterations = nnetwork.train(trainFeatures, trainResponses, cv::Mat(), cv::Mat(), params);
    printf("Training iterations: %i\n\n", iterations);

    // Save the model generated into an xml file.
    CvFileStorage* storage = cvOpenFileStorage(toSaveANNFile.c_str(), 0, CV_STORAGE_WRITE);
    nnetwork.write(storage, "ANN");
    cvReleaseFileStorage(&storage);

    // Test the generated model with the test samples.
    cv::Mat test_sample;
    //count of correct classifications
    int correct_class = 0;
    //count of wrong classifications
    int wrong_class = 0;

    //classification matrix gives the count of classes to which the samples were classified.
    int classification_matrix[CLASScount][CLASScount] = {
        {}};

    // for each sample in the test set.
    for (int tsample = 0; tsample < totalTestFileCount; tsample++) {

        // extract the sample
        test_sample = testFeatures.row(tsample);

        //try to predict its class
        nnetwork.predict(test_sample, classificationResult);
        /*The classification result matrix holds weightage  of each class.
        we take the class with the highest weightage as the resultant class */

        // find the class with maximum weightage.
        int maxIndex = 0;
        float value = 0.0f;
        float maxValue = classificationResult.at<float>(0, 0);
        for (int index = 0; index < CLASScount; index++) {
            value = classificationResult.at<float>(0, index);
            if (value > maxValue) {
                maxValue = value;
                maxIndex = index;

            }
        }

        printf("Testing Sample %i -> class result: %d (%s)\n", tsample, maxIndex, labels[maxIndex].c_str());

        //Now compare the predicted class to the actural class. if the prediction is correct then\
            //test_set_classifications[tsample][ maxIndex] should be 1.
        //if the classification is wrong, note that.
        if (testResponses.at<float>(tsample, maxIndex) != 1.0f) {
            // if they differ more than floating point error => wrong class

            wrong_class++;

            //find the actual label 'class_index'
            for (int class_index = 0; class_index < CLASScount; class_index++) {
                if (testResponses.at<float>(tsample, class_index) == 1.0f) {

                    classification_matrix[class_index][maxIndex]++; // A class_index sample was wrongly classified as maxindex.
                    break;
                }
            }

        } else {

            // otherwise correct

            correct_class++;
            classification_matrix[maxIndex][maxIndex]++;
        }
    }

    printf("\nResults on the testing dataset\n"
            "\tCorrect classification: %d (%g%%)\n"
            "\tWrong classifications: %d (%g%%)\n",
            correct_class, (double) correct_class * 100 / totalTestFileCount,
            wrong_class, (double) wrong_class * 100 / totalTestFileCount);
    cout << "   ";
    for (int i = 0; i < CLASScount; i++) {
        cout << i << "\t";
    }
    cout << "\n";
    for (int row = 0; row < CLASScount; row++) {
        cout << row << "  ";
        for (int col = 0; col < CLASScount; col++) {
            cout << classification_matrix[row][col] << "\t";
        }
        cout << "\n";
    }

    return 0;

}//main



