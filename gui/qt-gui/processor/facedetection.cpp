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
    //QString path = QDir().filePath("haarcascade_frontalface_default.xml");
    //TODO: mettergli il path relativo di qt-- non va non so perché
    string my_path = "C:/Users/dente/OneDrive/Desktop/Immagini/ObjectTracker/gui/qt-gui/haarcascade_frontalface_default.xml";
    //cout << path.toStdString() << endl;
    //QString path = QDir::current().filePath("haarcascade_frontalface_default.xml");

    if (!cascade.load(my_path)) {
        throw std::runtime_error("File not loaded");
    }
}

void FaceDetection::detection(Mat& image, vector<Rect> &faces)
{
    cout << "Face Detection Started.." << endl;

    if (image.empty())
    {
        throw std::runtime_error("Empty image");
    }

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    cascade.detectMultiScale(gray, faces);
}
