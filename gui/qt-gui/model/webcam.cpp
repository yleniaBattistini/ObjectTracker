#include "webcam.h"

WebCam::WebCam(int camIndex) : videoCapture(VideoCapture(camIndex))
{
}

WebCam::~WebCam()
{
    videoCapture.release();
}

void WebCam::readNextFrame(Mat &image)
{
    Mat flippedInput;
    videoCapture >> flippedInput;
    flip(flippedInput, image, 1);
}
