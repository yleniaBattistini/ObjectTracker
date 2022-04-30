#include "facedetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>
#include<QtCore/QDir>

using namespace cv;
using namespace std;

FaceDetection::FaceDetection(): scale(1)
{
    cascade = cv::CascadeClassifier();
    QString path = QDir().filePath("haarcascade_frontalface_default.xml");
    //TODO: mettergli il path relativo di qt-- non va non so perché
    string my_path = "C:/Users/ylenia/Documents/ObjectTracker-old/ObjectTracker/src/ObjectTracker/haarcascade_frontalface_default.xml";
    cout << path.toStdString() << endl;
    if (cascade.load(my_path) == true) {
        cout << "Face Detection Started.." << endl;
    }
    else
        throw std::runtime_error("File not loaded");
}

void FaceDetection::detection(Mat& image, Mat& output, ComputePose *computePose)
{
    cp = computePose;
    cout << "Face Detection Started.." << endl;

    if (image.empty())
        throw std::runtime_error("Empty image");
    vector<Rect> faces;
    detectFace(image, faces, output);
}


void FaceDetection::detectFace(Mat& img, vector<Rect>& faces, Mat& output)
{
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    cascade.detectMultiScale(gray, faces);
    //gray.copyTo(output);
    cvtColor(gray, output, COLOR_GRAY2BGR);
    DrawElement::drawRectangle(cp, output, faces);
}
