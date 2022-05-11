#include "camera.h"

#include <opencv2/calib3d.hpp>

Camera::~Camera()
{
}

void Camera::acquireNextFrame(Mat &frame, bool removeDistortion)
{
    if (calibrated && removeDistortion)
    {
        Mat rawFrame;
        readNextFrame(rawFrame);
        undistort(rawFrame, frame, cameraMatrix, distortionCoefficients);
    }
    else
    {
        readNextFrame(frame);
    }
}

void Camera::calibrate(Mat &cameraMatrix, Mat &distortionCoefficients)
{
    this->cameraMatrix = cameraMatrix;
    this->distortionCoefficients = distortionCoefficients;
    this->calibrated = true;
}
