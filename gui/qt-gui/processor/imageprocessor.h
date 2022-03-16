#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/core.hpp>

using namespace cv;

class ImageProcessor
{
public:
    virtual void ProcessImage(Mat &input, Mat &output) = 0;
};

#endif // IMAGEPROCESSOR_H
