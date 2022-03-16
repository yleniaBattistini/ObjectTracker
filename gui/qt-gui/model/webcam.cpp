#include "webcam.h"

WebCam::WebCam(int camIndex)
{
    videoCapture = VideoCapture();
    videoCapture.open(camIndex);
    if (!videoCapture.isOpened())
    {
        throw std::runtime_error("Camera could not be opened");
    }
}

WebCam::~WebCam()
{
    videoCapture.release();
}

void WebCam::AcquireNextFrame(Mat &image)
{
    Mat flippedInput;
    videoCapture >> flippedInput;
    flip(flippedInput, image, 1);
}
