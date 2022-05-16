#include "facedetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<QtCore/QDir>
#include <QDir>

#include <QProcessEnvironment>

using namespace cv;
using namespace std;

FaceDetection::FaceDetection()
{
    QDir dir = QDir(QProcessEnvironment::systemEnvironment().value("OPENCV_DIR"));
    dir.cd("etc");
    dir.cd("haarcascades");
    QString filePath = dir.filePath("haarcascade_frontalface_default.xml");
    if (!cascade.load(filePath.toStdString()))
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
