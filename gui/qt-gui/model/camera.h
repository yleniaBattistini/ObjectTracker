#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core/core.hpp>

using namespace cv;

class Camera
{
private:
    bool active = false;

public:
    virtual ~Camera() = 0;

    virtual void AcquireNextFrame(Mat& frame) = 0;
};

#endif // CAMERA_H
