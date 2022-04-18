#pragma once

#include "imageprocessor.h"

using namespace cv;

class TestProcessor : public ImageProcessor
{
public:
    void processImage(Mat &input, Mat &output);
};
