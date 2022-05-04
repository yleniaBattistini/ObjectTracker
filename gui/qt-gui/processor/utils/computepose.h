#ifndef COMPUTEPOSE_H
#define COMPUTEPOSE_H

#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class ComputePose
{
private:
    Mat cameraMatrix, distortionCoefficients, undistortedFrame;
    vector<Point3f> objectPoints;

public:
    ComputePose();
    void setComponent(Mat K, Mat& distCoeff);
    void computePose(Mat &frame, vector<Point2f> imagePointsVec);

};

#endif // COMPUTEPOSE_H
