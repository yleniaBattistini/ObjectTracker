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
    void detectFace(Mat& img, vector<Rect>& faces, Mat& output);

    CascadeClassifier cascade = cv::CascadeClassifier();
    double scale;
    ComputePose *cp;
public:
    FaceDetection();
    void detection(Mat& image, Mat& output, ComputePose *computePose);

};

#endif // FACEDETECTION_H
