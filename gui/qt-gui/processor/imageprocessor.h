#pragma once
#include <opencv2/core.hpp>

using namespace cv;

class ImageProcessor
{
public:
    virtual void ProcessImage(Mat &input, Mat &output) = 0;
};
