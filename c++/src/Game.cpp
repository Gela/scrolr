#include "Game.h"

#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using std::cout;
using std::endl;

void Game::run()
{

    /**************************************************************************
    ******************** Window and Detector Initialization *******************
    **************************************************************************/

    //create the cascade classifier object used for the face detection
    CascadeClassifier face_cascade;

    //use the haarcascade_frontalface_alt.xml library
    face_cascade.load("dataSets/haarcascade_frontalface_alt.xml");
    //face_cascade.load("dataSets/haarcascade_mcs_eyepair_small.xml");
 
    //setup video capture device and link it to the first capture device
    VideoCapture capturedevice;
    capturedevice.open(0);
 
    //setup image files used in the capture process
    Mat captureframe;
    Mat grayscaleframe;
 
    //create a window to present the results
    namedWindow("outputcapture", 1);



    /**************************************************************************
    *********************** User Position Initialization **********************
    **************************************************************************/

    // IMPORTANT VARIABLE
    int xInit = 0;
    int xMin = 150;
    int xMax = 500;
    int columns = 50;
    int rows = 40;
    int n = 100; // number of initialization frames
 
    cout << "Initialization Started" << endl;
    for(int i = 0; i < n; i++)
    {
        capturedevice >> captureframe;
        cvtColor(captureframe, grayscaleframe, CV_BGR2GRAY);
        equalizeHist(grayscaleframe, grayscaleframe);
        std::vector<Rect> faces;
        face_cascade.detectMultiScale(grayscaleframe, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, Size(30,30));
        for(size_t i = 0; i < faces.size(); i++)
        {
            Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            Point pt2(faces[i].x, faces[i].y);
            rectangle(captureframe, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
        }
        if (faces.size() > 0){
            xInit += (2*faces[0].x + faces[0].width)/2.0;
        }
        imshow("outputcapture", captureframe);
        waitKey(33);
    }
    xInit /= double(n);
    cout << "Initialization Complete" << endl;


    /**************************************************************************
    ********************** Facial Tracking and Game Logic *********************
    **************************************************************************/

    std::string display[rows]; // The display for the game
    for (int k = 0; k < rows; k++){ // Initialize the display to empty values
        std::string row("");
        for (int j = 0; j < columns; j++){
            row += "-";
        }
        display[k] = row;
    }
    int initCounter = 0; //  This counter variable will ensure the player stays centered

    while(true)
    {
        capturedevice>>captureframe;
        cvtColor(captureframe, grayscaleframe, CV_BGR2GRAY);
        equalizeHist(grayscaleframe, grayscaleframe);
        std::vector<Rect> faces;
        face_cascade.detectMultiScale(grayscaleframe, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, Size(30,30));
        for(size_t i = 0; i < faces.size(); i++)
        {
            Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            Point pt2(faces[i].x, faces[i].y);
            rectangle(captureframe, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
        }

        int x;
        if (faces.size() > 0){
            x = (2*faces[0].x + faces[0].width)/2.0;
        }
        else{
            x = xInit;
        }

        string row;
        int column = columns - (x-xMin)/double(xMax-xMin) * columns;
        for (int i = 0; i < columns; i++){
            if (i == column){
                row += "0";
            }
            else{
                row += "-";
            }
        }

        if (initCounter < rows/2) {
            display[initCounter] = row;
            initCounter++;
        }
        else{
            std::string tempDisplay[rows];
            for (int i = 1; i < rows; i++){
                tempDisplay[i-1] = display[i];
            }

            // Generate new obstables
            std::string obRow("");
            for (int j = 0; j < columns; j++){
                if (j < 5 || j > (columns-1) - 5) {
                    obRow += "X"; 
                }
                else{
                    obRow += "-";
                }
            }
            tempDisplay[rows-1] = obRow;
        
            // Overlay the current layer ON rows/2
            for (int i = 0; i < columns; i++){
                if (row[i] == '0'){
                    tempDisplay[rows/2][i] = '0';
                }
            }

            for (int i = 0; i < rows; i++){
                display[i] = tempDisplay[i];
            }
        }


        // Print out all rows at the end
        for (int k = 0; k < rows; k++){
            cout << display[k] << endl;
        }

        imshow("outputcapture", captureframe);
        waitKey(33);
    }
}
