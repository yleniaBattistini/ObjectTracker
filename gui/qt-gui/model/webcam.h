#ifndef WEBCAM_H
#define WEBCAM_H

#include "camera.h"

#include <opencv2/videoio.hpp>

using namespace cv;

class WebCam : public Camera
{
private:
    VideoCapture videoCapture;

public:
    WebCam(int camIndex);
    ~WebCam();

    void AcquireNextFrame(Mat& frame);
};

#endif // WEBCAM_H
