#pragma once
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
