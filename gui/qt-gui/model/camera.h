#pragma once

#include <opencv2/core/core.hpp>

using namespace cv;

class Camera
{
private:
    bool active = false;
    Mat cameraMatrix;
    Mat distortionCoefficients;

public:
    virtual ~Camera() = 0;

    virtual void acquireNextFrame(Mat& frame) = 0;

    void calibrate(Mat &cameraMatrix, Mat &distortionCoefficients);

};
