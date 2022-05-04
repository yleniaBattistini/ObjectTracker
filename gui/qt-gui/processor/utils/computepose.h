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
    int width, height;
    Mat cameraMatrix, distortionCoefficients, undistortedFrame;
    vector<Mat> rvecs, tvecs;
    vector<Point2f> imagePoints;
    vector<Point3f> objectPoints;
    Mat Himg2scene, Hscene2img;
    double squareSize;

public:
    ComputePose();
    void setComponent(Mat& K, Mat& distCoeff, vector<Mat> rvec, vector<Mat> tvec, double squareSize, Mat& frame)
    {
        this->cameraMatrix = K;
        this->distortionCoefficients = distCoeff;
        this->rvecs = rvec;
        this->tvecs = tvec;
        this->objectPoints = {Point3f(1.0, 0.0, 0.0), Point3f(0.0, 1.0, 0.0), Point3f(0.0, 0.0, 1.0), Point3f(1.0, 1.0, 1.0)}; //TODO vettore punti 3d 010 ecc.
        this->squareSize = squareSize;
        this->undistortedFrame = frame;
    }
    void computePose(vector<Point2f> imagePointsVec, int imageWidth, int imageHeight);

};

#endif // COMPUTEPOSE_H
