#pragma once

#include <opencv2/core/core.hpp>

using namespace cv;

class Camera
{
private:
    bool calibrated = false;
    Mat cameraMatrix;
    Mat distortionCoefficients;

protected:
    virtual void readNextFrame(Mat& frame) = 0;

public:
    virtual ~Camera() = 0;

    void acquireNextFrame(Mat& frame, bool ignoreCalibration = false);
    void calibrate(Mat &cameraMatrix, Mat &distortionCoefficients);

};
