#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class ImageProcessor
{
public:
    virtual void processImage(Mat &input, Mat &output) = 0;
};
