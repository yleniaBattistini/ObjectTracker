#include "facedetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>
#include<QtCore/QDir>

using namespace cv;
using namespace std;

FaceDetection::FaceDetection()
{
    string my_path = "C:/Users/dente/OneDrive/Desktop/Immagini/ObjectTracker/gui/qt-gui/haarcascade_frontalface_default.xml";
    if (!cascade.load(my_path))
    {
        throw std::runtime_error("File not loaded");
    }
}

void FaceDetection::detection(Mat& image, vector<Rect> &faces)
{
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(50, 50), Size(300, 300));
}
