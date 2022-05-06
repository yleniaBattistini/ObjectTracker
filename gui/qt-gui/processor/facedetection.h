#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "utils/drawelement.h"
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class FaceDetection
{
private:
    CascadeClassifier cascade = cv::CascadeClassifier();
    double scale;

public:
    FaceDetection();
    void detection(Mat& image, vector<Rect> &faces);

};

#endif // FACEDETECTION_H
