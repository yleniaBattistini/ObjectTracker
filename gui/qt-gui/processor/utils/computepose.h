#ifndef COMPUTEPOSE_H
#define COMPUTEPOSE_H

#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void computePose(
        const Mat &cameraMatrix,
        const Mat &distortionCoefficients,
        const vector<Point3f> &objectPoints,
        const vector<Point2f> &imagePoints,
        Mat &t,
        Mat &rotVec,
        double &thetaX,
        double &thetaY);

#endif // COMPUTEPOSE_H
