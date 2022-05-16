#pragma once

#include "camera.h"

#include <opencv2/videoio.hpp>

using namespace cv;

class WebCam : public Camera
{
private:
    VideoCapture videoCapture;

protected:
    void readNextFrame(Mat& frame);

public:
    WebCam(int camIndex);
    ~WebCam();
};
